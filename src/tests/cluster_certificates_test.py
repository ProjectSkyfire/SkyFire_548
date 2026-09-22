# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
import base64
import datetime as dt
from pathlib import Path
import sys
import tempfile
import unittest
import asyncio
import ssl
import struct
from types import SimpleNamespace
from unittest.mock import patch
from cryptography import x509
from cryptography.hazmat.primitives import hashes, serialization
from cryptography.hazmat.primitives.asymmetric import ec
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'server/shared/Platform'))
import cluster_certificates as pki


class Certificates(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.key = ec.generate_private_key(ec.SECP256R1())
        self.ca_key = ec.generate_private_key(ec.SECP256R1())
        self.name = x509.Name([x509.NameAttribute(x509.NameOID.COMMON_NAME, 'test-authority')])
        self.ca = self.cert(self.ca_key, self.name, 1, self.name, self.ca_key, True)
        self.leaf = self.cert(self.key, x509.Name([x509.NameAttribute(x509.NameOID.COMMON_NAME, 'world-1')]), 2, self.name, self.ca_key)
        pki.write_private(self.root/'ca.pem', self.ca.public_bytes(serialization.Encoding.PEM))
        pki.write_private(self.root/'key.pem', self.key.private_bytes(serialization.Encoding.PEM, serialization.PrivateFormat.PKCS8, serialization.NoEncryption()))

    def cert(self, key, subject, serial, issuer, signing_key, ca=False):
        return (x509.CertificateBuilder().subject_name(subject).issuer_name(issuer).public_key(key.public_key())
                .serial_number(serial).not_valid_before(pki.now()-dt.timedelta(minutes=1))
                .not_valid_after(pki.now()+dt.timedelta(days=30))
                .add_extension(x509.BasicConstraints(ca=ca, path_length=None), True).sign(signing_key, hashes.SHA256()))

    def crl(self, number, revoked=False, stale=False, signer=None):
        value = (x509.CertificateRevocationListBuilder().issuer_name(self.name)
                 .last_update(pki.now()-dt.timedelta(days=2))
                 .next_update(pki.now()+dt.timedelta(days=-1 if stale else 1))
                 .add_extension(x509.CRLNumber(number), False))
        if revoked:
            value = value.add_revoked_certificate(x509.RevokedCertificateBuilder().serial_number(2).revocation_date(pki.now()-dt.timedelta(minutes=1)).build())
        return value.sign(signer or self.ca_key, hashes.SHA256())

    def install(self, crl):
        pki.install_crl(base64.b64encode(crl.public_bytes(serialization.Encoding.DER)), self.root/'crl.pem', self.root/'ca.pem')

    def test_revocation_applies_to_existing_connection(self):
        leaf = self.leaf
        class Peer:
            def getpeercert(self, binary_form=False):
                return leaf.public_bytes(serialization.Encoding.DER)
        lifecycle = pki.Lifecycle({'crl':'crl.pem', 'ca':'ca.pem'}, self.root)
        self.assertFalse(lifecycle.allowed(Peer()))
        self.install(self.crl(1))
        self.assertTrue(lifecycle.allowed(Peer()))
        self.install(self.crl(2, revoked=True))
        self.assertFalse(lifecycle.allowed(Peer()))

    def test_rollback_same_number_and_bad_signature_rejected(self):
        self.install(self.crl(2, revoked=True))
        for value in (self.crl(1), self.crl(2), self.crl(3, stale=True), self.crl(3, signer=self.key)):
            with self.assertRaises(ValueError):
                self.install(value)

    def test_certificate_must_match_identity_key_and_pinned_ca(self):
        value = base64.b64encode(self.leaf.public_bytes(serialization.Encoding.DER))
        pki.install_certificate(value, self.root/'leaf.pem', self.root/'key.pem', self.root/'ca.pem', 'world-1')
        with self.assertRaises(ValueError):
            pki.install_certificate(value, self.root/'leaf.pem', self.root/'key.pem', self.root/'ca.pem', 'world-2')
        other = ec.generate_private_key(ec.SECP256R1())
        pki.write_private(self.root/'key.pem', other.private_bytes(serialization.Encoding.PEM, serialization.PrivateFormat.PKCS8, serialization.NoEncryption()))
        with self.assertRaises(ValueError):
            pki.install_certificate(value, self.root/'leaf.pem', self.root/'key.pem', self.root/'ca.pem', 'world-1')

    def test_csr_signed_locally(self):
        request = x509.load_der_x509_csr(base64.b64decode(pki.request(self.root/'key.pem', 'world-1')))
        self.assertTrue(request.is_signature_valid)
        self.assertEqual(request.subject.get_attributes_for_oid(x509.NameOID.COMMON_NAME)[0].value, 'world-1')

    def test_remote_enrollment_over_verified_tls_populates_configuration(self):
        async def run():
            server_key = ec.generate_private_key(ec.SECP256R1())
            server_cert = self.cert(server_key, x509.Name([x509.NameAttribute(x509.NameOID.COMMON_NAME, 'localhost')]), 3, self.name, self.ca_key)
            pki.write_private(self.root/'server.pem', server_cert.public_bytes(serialization.Encoding.PEM))
            pki.write_private(self.root/'server-key.pem', server_key.private_bytes(serialization.Encoding.PEM, serialization.PrivateFormat.PKCS8, serialization.NoEncryption()))
            context = ssl.create_default_context(ssl.Purpose.CLIENT_AUTH, cafile=str(self.root/'ca.pem'))
            context.load_cert_chain(self.root/'server.pem', self.root/'server-key.pem')
            context.verify_mode = ssl.CERT_OPTIONAL
            operations, failures = [], []
            async def connection(reader, writer):
                try:
                    magic, version, kind, size = struct.unpack('!4sHHI', await reader.readexactly(12))
                    self.assertEqual((magic, version), (b'SFHC', 1))
                    self.assertLess(size, 12000)
                    body = await reader.readexactly(size)
                    operations.append(kind)
                    if kind == 12:
                        self.assertIsNone(writer.get_extra_info('peercert'))
                        token_length = struct.unpack('!H', body[:2])[0]
                        self.assertEqual(body[2:2+token_length], b'a'*64)
                        csr_field = body[2+token_length:]
                        self.assertEqual(struct.unpack('!H', csr_field[:2])[0], len(csr_field)-2)
                        csr = x509.load_der_x509_csr(base64.b64decode(csr_field[2:], validate=True))
                        self.assertTrue(csr.is_signature_valid)
                        cert = (x509.CertificateBuilder().subject_name(csr.subject).issuer_name(self.name)
                                .public_key(csr.public_key()).serial_number(4)
                                .not_valid_before(pki.now()-dt.timedelta(minutes=1))
                                .not_valid_after(pki.now()+dt.timedelta(days=30)).sign(self.ca_key, hashes.SHA256()))
                        value = base64.b64encode(cert.public_bytes(serialization.Encoding.DER))
                    else:
                        self.assertEqual(kind, 13)
                        self.assertTrue(writer.get_extra_info('peercert'))
                        self.assertEqual(body, b'')
                        value = base64.b64encode(self.crl(1).public_bytes(serialization.Encoding.DER))
                    packet = pki.wire(value)
                    writer.write(b'SFHC'+struct.pack('!HHI', 1, kind|0x8000, len(packet))+packet)
                    await writer.drain()
                except Exception as error:
                    failures.append(error)
                finally:
                    writer.close()
            server = await asyncio.start_server(connection, '127.0.0.1', 0, ssl=context)
            config = self.root/'world.conf'
            config.write_text('Cluster.NodeKey = "world-1"\n')
            args = SimpleNamespace(host='localhost', port=server.sockets[0].getsockname()[1], ca=str(self.root/'ca.pem'),
                                   node='world-1', output=str(self.root/'remote'), config=str(config), python=False)
            try:
                with patch.object(pki.getpass, 'getpass', return_value='a'*64), patch('builtins.print'):
                    await pki.enroll(args)
                self.assertEqual(failures, [])
                self.assertEqual(operations, [12, 13])
                self.assertIn('Cluster.CRL = "', config.read_text())
                self.assertTrue((self.root/'world.conf.pre-pki').exists())
                self.assertTrue((self.root/'remote/private-key.pem').exists())
            finally:
                server.close()
                await server.wait_closed()
        asyncio.run(run())


if __name__ == '__main__':
    unittest.main()
