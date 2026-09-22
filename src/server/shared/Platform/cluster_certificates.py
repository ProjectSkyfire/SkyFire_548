# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
"""Pinned-CA enrollment, renewal and signed revocation lists for cluster daemons."""
import argparse
import asyncio
import base64
import contextlib
import datetime
import getpass
import os
from pathlib import Path
import re
import secrets
import ssl
import struct
import time


def crypto():
    # Legacy deployments do not require this dependency until CRL/PKI is enabled.
    from cryptography import x509
    from cryptography.hazmat.primitives import hashes, serialization
    from cryptography.hazmat.primitives.asymmetric import ec
    return x509, hashes, serialization, ec


def write_private(path, value):
    path = Path(path).resolve()
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_name(path.name + '.' + secrets.token_hex(16) + '.tmp')
    try:
        if os.name == 'nt':
            import ctypes
            import msvcrt
            from ctypes import wintypes as w
            class Attributes(ctypes.Structure):
                _fields_ = [('length', w.DWORD), ('descriptor', w.LPVOID), ('inherit', w.BOOL)]
            advapi = ctypes.WinDLL('advapi32', use_last_error=True)
            kernel = ctypes.WinDLL('kernel32', use_last_error=True)
            descriptor = w.LPVOID()
            convert = advapi.ConvertStringSecurityDescriptorToSecurityDescriptorW
            convert.argtypes = [w.LPCWSTR, w.DWORD, ctypes.POINTER(w.LPVOID), ctypes.POINTER(w.DWORD)]
            if not convert('D:P(A;;FA;;;SY)(A;;FA;;;BA)(A;;FA;;;OW)', 1, ctypes.byref(descriptor), None):
                raise OSError('Cannot protect certificate file')
            attributes = Attributes(ctypes.sizeof(Attributes), descriptor, False)
            create = kernel.CreateFileW
            create.restype = w.HANDLE
            create.argtypes = [w.LPCWSTR, w.DWORD, w.DWORD, ctypes.POINTER(Attributes), w.DWORD, w.DWORD, w.HANDLE]
            handle = create(str(temporary), 0x40000000, 0, ctypes.byref(attributes), 1, 0x80, None)
            kernel.LocalFree.argtypes = [w.LPVOID]
            kernel.LocalFree(descriptor)
            if handle == ctypes.c_void_p(-1).value:
                raise OSError('Cannot create protected certificate file')
            fd = msvcrt.open_osfhandle(handle, os.O_WRONLY | os.O_BINARY)
        else:
            fd = os.open(temporary, os.O_WRONLY | os.O_CREAT | os.O_EXCL, 0o600)
        with os.fdopen(fd, 'wb') as output:
            output.write(value)
            output.flush()
            os.fsync(output.fileno())
        os.replace(temporary, path)
    finally:
        with contextlib.suppress(FileNotFoundError):
            temporary.unlink()


def now():
    return datetime.datetime.now(datetime.timezone.utc)


def decode(value):
    if not value or len(value) > 60000:
        raise ValueError('Invalid certificate size')
    return base64.b64decode(value, validate=True)


def install_crl(value, path, ca_path):
    x509, _, serialization, _ = crypto()
    authority = x509.load_pem_x509_certificate(Path(ca_path).read_bytes())
    crl = x509.load_der_x509_crl(decode(value))
    if crl.issuer != authority.subject or not crl.is_signature_valid(authority.public_key()) or not crl.last_update_utc <= now() < crl.next_update_utc:
        raise ValueError('Invalid or stale revocation list')
    number = crl.extensions.get_extension_for_class(x509.CRLNumber).value.crl_number
    encoded = crl.public_bytes(serialization.Encoding.PEM)
    path = Path(path)
    if path.exists():
        previous = x509.load_pem_x509_crl(path.read_bytes())
        old = previous.extensions.get_extension_for_class(x509.CRLNumber).value.crl_number
        if number < old or (number == old and encoded != previous.public_bytes(serialization.Encoding.PEM)):
            raise ValueError('Revocation list rollback rejected')
        if number == old:
            return
    write_private(path, encoded)


def install_certificate(value, certificate, private_key, ca_path, node):
    x509, _, serialization, _ = crypto()
    cert = x509.load_der_x509_certificate(decode(value))
    ca = x509.load_pem_x509_certificate(Path(ca_path).read_bytes())
    key = serialization.load_pem_private_key(Path(private_key).read_bytes(), None)
    cert.verify_directly_issued_by(ca)
    cn = cert.subject.get_attributes_for_oid(x509.NameOID.COMMON_NAME)
    public = lambda item: item.public_key().public_bytes(serialization.Encoding.DER, serialization.PublicFormat.SubjectPublicKeyInfo)
    if len(cn) != 1 or cn[0].value != node or public(cert) != public(key) or not cert.not_valid_before_utc <= now() < cert.not_valid_after_utc:
        raise ValueError('Certificate identity/key/validity mismatch')
    write_private(certificate, cert.public_bytes(serialization.Encoding.PEM))


def request(private_key, node):
    x509, hashes, serialization, _ = crypto()
    key = serialization.load_pem_private_key(Path(private_key).read_bytes(), None)
    csr = x509.CertificateSigningRequestBuilder().subject_name(x509.Name([x509.NameAttribute(x509.NameOID.COMMON_NAME, node)])).sign(key, hashes.SHA256())
    return base64.b64encode(csr.public_bytes(serialization.Encoding.DER))


def wire(value):
    if isinstance(value, str):
        value = value.encode('ascii')
    if not 1 <= len(value) <= 60000:
        raise ValueError('Invalid certificate field')
    return struct.pack('!H', len(value)) + value


async def exchange(reader, writer, kind, body=b''):
    async def run():
        writer.write(b'SFHC' + struct.pack('!HHI', 1, kind, len(body)) + body)
        await writer.drain()
        magic, version, reply, size = struct.unpack('!4sHHI', await reader.readexactly(12))
        if magic != b'SFHC' or version != 1 or reply != kind | 0x8000 or not 2 <= size <= 60002:
            raise RuntimeError('Hub rejected certificate request')
        data = await reader.readexactly(size)
        if struct.unpack('!H', data[:2])[0] != size - 2:
            raise RuntimeError('Malformed certificate reply')
        return data[2:]
    return await asyncio.wait_for(run(), 5)


class Lifecycle:
    def __init__(self, config, directory):
        self.config = config
        self.directory = Path(directory)
        self.enabled = bool(config.get('crl'))
        self.next_renewal = 0
        self.cached = None
        self.stamp = None

    def path(self, key):
        return (self.directory / self.config[key]).resolve()

    def allowed(self, connection):
        if not self.enabled:
            return True
        try:
            x509, _, _, _ = crypto()
            path = self.path('crl')
            stamp = (path.stat().st_mtime_ns, path.stat().st_size)
            if stamp != self.stamp:
                ca = x509.load_pem_x509_certificate(self.path('ca').read_bytes())
                crl = x509.load_pem_x509_crl(path.read_bytes())
                if crl.issuer != ca.subject or not crl.is_signature_valid(ca.public_key()):
                    return False
                self.cached, self.stamp = crl, stamp
            crl = self.cached
            cert = x509.load_der_x509_certificate(connection.getpeercert(binary_form=True))
            return (crl.last_update_utc <= now() < crl.next_update_utc and cert.issuer == crl.issuer and
                    cert.not_valid_before_utc <= now() < cert.not_valid_after_utc and
                    crl.get_revoked_certificate_by_serial_number(cert.serial_number) is None)
        except Exception:
            return False

    async def sync(self, reader, writer, *contexts):
        if not self.enabled:
            return
        try:
            value = await exchange(reader, writer, 13)
            await asyncio.to_thread(install_crl, value, self.path('crl'), self.path('ca'))
            if not self.allowed(writer.get_extra_info('ssl_object')):
                raise ValueError('Hub certificate revoked or expired')
            if time.monotonic() >= self.next_renewal:
                self.next_renewal = time.monotonic() + 3600
                x509, _, _, _ = crypto()
                cert = x509.load_pem_x509_certificate(self.path('certificate').read_bytes())
                if cert.not_valid_after_utc <= now() + datetime.timedelta(days=14):
                    csr = await asyncio.to_thread(request, self.path('private_key'), self.config['node_key'])
                    value = await exchange(reader, writer, 14, wire(csr))
                    await asyncio.to_thread(install_certificate, value, self.path('certificate'), self.path('private_key'), self.path('ca'), self.config['node_key'])
            for context in contexts:
                context.load_cert_chain(self.path('certificate'), self.path('private_key'))
        except Exception as error:
            raise RuntimeError('Certificate synchronization failed') from error


async def enroll(args):
    x509, _, serialization, ec = crypto()
    if not re.fullmatch(r'[A-Za-z0-9][A-Za-z0-9_-]{0,63}', args.node):
        raise ValueError('Invalid node key')
    config = Path(args.config).resolve()
    text = config.read_text(encoding='utf-8-sig')
    if args.python:
        import tomllib
        configured = tomllib.loads(text).get('node_key')
    else:
        matches = re.findall(r'(?m)^[ \t]*Cluster\.NodeKey[ \t]*=[ \t]*"?([^"\r\n#;]+)', text)
        configured = matches[0].strip() if len(matches) == 1 else None
    if configured != args.node:
        raise ValueError('Configuration node key does not match the approved enrollment')
    output = Path(args.output).resolve()
    key_path, cert_path, crl_path = (output / name for name in ('private-key.pem', 'certificate.pem', 'revocations.pem'))
    # Reuse a local key after interrupted enrollment; never export it to the hub.
    if not key_path.exists():
        key = ec.generate_private_key(ec.SECP256R1())
        write_private(key_path, key.private_bytes(serialization.Encoding.PEM, serialization.PrivateFormat.PKCS8, serialization.NoEncryption()))
    token = getpass.getpass('One-time enrollment token: ').strip()
    if not re.fullmatch(r'[0-9a-f]{64}', token):
        raise ValueError('Invalid token format')
    ca = Path(args.ca).resolve()
    context = ssl.create_default_context(cafile=str(ca))
    context.minimum_version = ssl.TLSVersion.TLSv1_2
    reader, writer = await asyncio.wait_for(asyncio.open_connection(args.host, args.port, ssl=context, server_hostname=args.host), 5)
    try:
        value = await exchange(reader, writer, 12, wire(token) + wire(request(key_path, args.node)))
        install_certificate(value, cert_path, key_path, ca, args.node)
    finally:
        writer.close()
    context.load_cert_chain(cert_path, key_path)
    reader, writer = await asyncio.wait_for(asyncio.open_connection(args.host, args.port, ssl=context, server_hostname=args.host), 5)
    try:
        install_crl(await exchange(reader, writer, 13), crl_path, ca)
    finally:
        writer.close()
    # Config replacement happens only after both certificate and CRL validation.
    values = {'certificate':cert_path, 'private_key':key_path, 'ca':ca, 'crl':crl_path} if args.python else {
        'Cluster.Certificate':cert_path, 'Cluster.PrivateKey':key_path, 'Cluster.CA':ca, 'Cluster.CRL':crl_path}
    for name, path in values.items():
        value = path.as_posix()
        if any(c in value for c in '"\r\n'):
            raise ValueError('Unsupported configuration path')
        pattern = re.compile(r'(?m)^\s*' + re.escape(name) + r'\s*=[^\r\n]*')
        if len(pattern.findall(text)) > 1:
            raise ValueError('Duplicate TLS configuration setting')
        line = name + ' = "' + value + '"'
        text = pattern.sub(lambda _: line, text) if pattern.search(text) else text + '\n' + line + '\n'
    backup = config.with_name(config.name + '.pre-pki')
    if not backup.exists():
        write_private(backup, config.read_bytes())
    write_private(config, text.encode())
    print('Enrollment complete. Certificate paths updated; start the daemon to register.')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Enroll a remote SkyFire daemon with a pinned cluster CA.')
    parser.add_argument('--host', required=True)
    parser.add_argument('--port', type=int, default=9100)
    parser.add_argument('--ca', required=True)
    parser.add_argument('--node', required=True)
    parser.add_argument('--output', required=True)
    parser.add_argument('--config', required=True)
    parser.add_argument('--python', action='store_true', help='Map/character TOML configuration')
    try:
        asyncio.run(enroll(parser.parse_args()))
    except Exception as error:
        raise SystemExit('Enrollment failed (' + type(error).__name__ + '). Check the pinned CA, approved node, token and file permissions.')
