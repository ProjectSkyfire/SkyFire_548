# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
"""Character-service ABI and disposable-MySQL integration tests. Never uses a live schema."""
import argparse
import asyncio
import datetime
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import secrets
import ssl
import struct
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
SERVICE = ROOT / 'src/server/characterserver'
sys.path.insert(0,str(SERVICE))
from database import CharacterDatabase
from export_catalog import catalog
from wire import Reader, blob, u32, prepared_sql


def raw(sql):
    return b'\0' + blob(sql)


def prepared(index, parameters):
    return b'\1' + u32(index) + u32(len(parameters)) + b''.join(parameters)


def request(operation, statements, request_id=None):
    return bytes([operation]) + blob(request_id or secrets.token_hex(16)) + (u32(len(statements)) if operation == 3 else b'') + b''.join(statements)


def rows(payload):
    reader = Reader(payload)
    fields = [reader.u32() for _ in range(reader.u32())]
    values = [[reader.blob() if reader.u8() else None for _ in fields] for _ in range(reader.u32())]
    reader.end()
    return values


class ProtocolTests(unittest.TestCase):
    def test_metrics_count_only_acknowledged_writes_and_include_failures(self):
        from metrics import Metrics
        metrics=Metrics()
        metrics.finish(metrics.begin(),2,True)
        metrics.finish(metrics.begin(),3,True)
        metrics.finish(metrics.begin(),3,False)
        packet=metrics.packet(2,True)
        self.assertEqual(len(packet),53)
        values=struct.unpack('!13I',packet[1:])
        self.assertEqual(values[3:10],(3,1,1,1,1,2,0))
        self.assertEqual(values[12],1)

    def test_live_character_restore_rejected_before_database_access(self):
        spec = importlib.util.spec_from_file_location('character_backup_fixture',ROOT/'src/server/hub/backup/backupserver.py')
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        worker = module.Worker.__new__(module.Worker)
        worker.allow_live_restore = True
        worker.character_service = True
        # No hub/target connection exists: rejection must precede any database work.
        with self.assertRaisesRegex(RuntimeError,'offline recovery'):
            worker.restore({'target':'characters'})

    def test_catalog_matches_native_statements(self):
        self.assertEqual(catalog(ROOT),json.loads((SERVICE/'statements.json').read_text()))

    def test_parameter_markers_and_percent_literals(self):
        self.assertEqual(prepared_sql("SELECT '?' AS literal, ? AS value, '100%' AS percent"),
                         "SELECT '?' AS literal, %s AS value, '100%%' AS percent")
        with self.assertRaises(ValueError):
            Reader(u32(100) + b'x').blob()


class DatabaseTests(unittest.TestCase):
    config_path = None

    @classmethod
    def setUpClass(cls):
        if not cls.config_path:
            raise unittest.SkipTest('Pass --database-config for disposable MySQL tests')
        import pymysql
        text=Path(cls.config_path).read_text(encoding='utf-8-sig')
        value=re.search(r'^\s*HubDatabaseInfo\s*=\s*"([^"\r\n]+)"',text,re.M)
        if not value:
            raise RuntimeError('Expected HubDatabaseInfo for fixture server credentials')
        host,port,user,password,_=value[1].split(';')
        cls.schema='skyfire_character_test_'+secrets.token_hex(8)
        cls.admin=pymysql.connect(host=host,port=int(port),user=user,password=password,autocommit=True)
        with cls.admin.cursor() as cursor:
            cursor.execute('CREATE DATABASE `'+cls.schema+'` CHARACTER SET utf8mb4')
            cursor.execute('CREATE TABLE `'+cls.schema+'`.characters (guid INT UNSIGNED PRIMARY KEY,name VARCHAR(64),money BIGINT UNSIGNED NOT NULL DEFAULT 0) ENGINE=InnoDB')
            cursor.execute('INSERT INTO `'+cls.schema+'`.characters VALUES(1,%s,0)',("Tester O'雪",))
            cursor.execute('CREATE TABLE `'+cls.schema+'`.gm_tickets (id INT PRIMARY KEY, flags BIT(8)) ENGINE=InnoDB')
            cursor.execute("INSERT INTO `"+cls.schema+"`.gm_tickets VALUES(1,b'10101010')")
            cursor.execute('CREATE TABLE `'+cls.schema+'`.binary_fixture (payload BLOB) ENGINE=InnoDB')
        cls.config=dict(realm_id=1,mysql_host=host,mysql_port=int(port),mysql_user=user,
                        mysql_password=password,mysql_database=cls.schema)

    @classmethod
    def tearDownClass(cls):
        if hasattr(cls,'admin'):
            if not re.fullmatch(r'skyfire_character_test_[0-9a-f]{16}',cls.schema):
                raise RuntimeError('Refusing to drop an unexpected fixture name')
            with cls.admin.cursor() as cursor:
                cursor.execute('DROP DATABASE `'+cls.schema+'`')
            cls.admin.close()

    def test_transactions_fencing_and_recovery(self):
        db=CharacterDatabase(self.config,SERVICE/'statements.json')
        first,second=secrets.token_hex(16),secrets.token_hex(16)
        def hello(instance):
            return b'\1'+u32(1)+blob(instance)+blob(db.catalog_hash)
        try:
            with self.assertRaises(RuntimeError):
                duplicate=CharacterDatabase(self.config,SERVICE/'statements.json')
                duplicate.close()
            _,epoch=db.attach('world-a',hello(first))
            db.attach('world-a',hello(first))
            with self.assertRaises(ValueError):
                db.attach('world-b',hello(second))
            query=lambda sql: rows(db.execute('world-a',first,epoch,request(2,[raw(sql)])))
            self.assertEqual(query("SELECT guid,name,money,NULL,UNHEX('610062') FROM characters WHERE guid=1"),
                             [[b'1',"Tester O'雪".encode(),b'0',None,b'a\0b']])
            ids={entry['name']:entry['id'] for entry in json.loads((SERVICE/'statements.json').read_text())}
            query_name=prepared(ids['CHAR_SEL_CHECK_NAME'],[b'\2'+blob("Tester O'雪")])
            self.assertEqual(rows(db.execute('world-a',first,epoch,request(2,[query_name]))),[[b'1']])
            save=request(3,[raw('UPDATE characters SET money=money+1 WHERE guid=1')])
            db.execute('world-a',first,epoch,save)
            db.execute('world-a',first,epoch,save)
            self.assertEqual(query('SELECT money FROM characters WHERE guid=1'),[[b'1']])
            clear=prepared(ids['CHAR_DEL_ALL_GM_TICKETS'],[])
            with self.assertRaises(Exception):
                db.execute('world-a',first,epoch,request(3,[clear,raw('UPDATE missing_table SET value=1')]))
            self.assertEqual(query('SELECT id FROM gm_tickets'),[[b'1']])
            self.assertEqual(query('SELECT flags FROM gm_tickets'),[[b'170']])
            # A synthetic prepared entry exercises non-UTF8 binary bind transport.
            db.catalog[10000] = 'INSERT INTO binary_fixture VALUES(?)'
            binary = b'\xff\x00\x80\x27\x5c'
            db.execute('world-a',first,epoch,request(1,[prepared(10000,[b'\3'+blob(binary)])]))
            self.assertEqual(query('SELECT payload FROM binary_fixture'),[[binary]])
            broken=request(3,[raw('UPDATE characters SET money=999 WHERE guid=1'),raw('UPDATE missing_table SET value=1')])
            with self.assertRaises(Exception):
                db.execute('world-a',first,epoch,broken)
            self.assertEqual(query('SELECT money FROM characters WHERE guid=1'),[[b'1']])
            with self.assertRaises(ValueError):
                db.execute('world-a',first,epoch,save.replace(b'money+1',b'money+2'))
            with self.assertRaises(ValueError):
                db.execute('world-a',first,epoch,request(2,[raw('UPDATE characters SET money=999 WHERE guid=1')]))
            db.detach(first)
            with self.assertRaises(ValueError):
                db.attach('world-b',hello(second))
            db.detach(first)
            _,new_epoch=db.attach('world-b',hello(second))
            self.assertGreater(new_epoch,epoch)
            with self.assertRaises(ValueError):
                db.execute('world-a',first,epoch,save)
            with self.assertRaises(ValueError):
                db.attach('world-a',hello(first))
            db.detach(second)
        finally:
            db.close()
        recovered=CharacterDatabase(self.config,SERVICE/'statements.json')
        try:
            with self.assertRaises(ValueError):
                recovered.attach('world-a',b'\1'+u32(1)+blob(first)+blob(recovered.catalog_hash))
            _,epoch=recovered.attach('world-b',b'\1'+u32(1)+blob(second)+blob(recovered.catalog_hash))
            self.assertEqual(rows(recovered.execute('world-b',second,epoch,request(2,[raw('SELECT money FROM characters WHERE guid=1')]))),[[b'1']])
        finally:
            recovered.close()

    def test_managed_service_migration_preserves_existing_records(self):
        from pymysql.constants import CLIENT
        cfg=self.config
        import pymysql
        conn=pymysql.connect(host=cfg['mysql_host'],port=cfg['mysql_port'],user=cfg['mysql_user'],password=cfg['mysql_password'],
                             database=self.schema,autocommit=True,client_flag=CLIENT.MULTI_STATEMENTS)
        try:
            with conn.cursor() as cursor:
                cursor.execute('CREATE TABLE hub_managed_services(service_key VARCHAR(64) PRIMARY KEY,enabled INT NOT NULL) ENGINE=InnoDB')
                cursor.execute("INSERT INTO hub_managed_services VALUES('world',1)")
                migration=(ROOT/'sql/pending_updates/hub/003_managed_data_services.sql').read_text()
                for _ in range(2):
                    cursor.execute(migration)
                    while cursor.nextset(): pass
                cursor.execute('SELECT service_key,enabled,service_kind,cluster_key FROM hub_managed_services')
                self.assertEqual(cursor.fetchall(),(('world',1,0,''),))
        finally:
            conn.close()

    def test_tls_daemon_and_hub_registration(self):
        asyncio.run(self.tls_fixture())

    async def tls_fixture(self):
        from cryptography import x509
        from cryptography.hazmat.primitives import hashes, serialization
        from cryptography.hazmat.primitives.asymmetric import rsa
        from cryptography.x509.oid import NameOID, ExtendedKeyUsageOID
        import ipaddress
        from characterserver import serve
        with tempfile.TemporaryDirectory(prefix='skyfire-character-tls-') as temporary:
            root=Path(temporary)
            now=datetime.datetime.now(datetime.timezone.utc)
            ca_key=rsa.generate_private_key(public_exponent=65537,key_size=2048)
            ca_name=x509.Name([x509.NameAttribute(NameOID.COMMON_NAME,'Fixture CA')])
            ca=(x509.CertificateBuilder().subject_name(ca_name).issuer_name(ca_name).public_key(ca_key.public_key())
                .serial_number(x509.random_serial_number()).not_valid_before(now-datetime.timedelta(minutes=1)).not_valid_after(now+datetime.timedelta(days=1))
                .add_extension(x509.BasicConstraints(ca=True,path_length=None),critical=True).sign(ca_key,hashes.SHA256()))
            (root/'ca.pem').write_bytes(ca.public_bytes(serialization.Encoding.PEM))
            for name in ('hub','characters-1','world-a','outsider'):
                key=rsa.generate_private_key(public_exponent=65537,key_size=2048)
                certificate=(x509.CertificateBuilder().subject_name(x509.Name([x509.NameAttribute(NameOID.COMMON_NAME,name)]))
                    .issuer_name(ca_name).public_key(key.public_key()).serial_number(x509.random_serial_number())
                    .not_valid_before(now-datetime.timedelta(minutes=1)).not_valid_after(now+datetime.timedelta(days=1))
                    .add_extension(x509.SubjectAlternativeName([x509.DNSName('localhost'),x509.IPAddress(ipaddress.ip_address('127.0.0.1'))]),False)
                    .add_extension(x509.ExtendedKeyUsage([ExtendedKeyUsageOID.CLIENT_AUTH,ExtendedKeyUsageOID.SERVER_AUTH]),False)
                    .sign(ca_key,hashes.SHA256()))
                (root/(name+'.pem')).write_bytes(certificate.public_bytes(serialization.Encoding.PEM))
                (root/(name+'.key')).write_bytes(key.private_bytes(serialization.Encoding.PEM,serialization.PrivateFormat.PKCS8,serialization.NoEncryption()))
            hub_tls=ssl.create_default_context(ssl.Purpose.CLIENT_AUTH,cafile=str(root/'ca.pem'))
            hub_tls.load_cert_chain(root/'hub.pem',root/'hub.key'); hub_tls.verify_mode=ssl.CERT_REQUIRED
            registered=asyncio.Event()
            async def hub(reader,writer):
                try:
                    while True:
                        magic,version,kind,size=struct.unpack('!4sHHI',await reader.readexactly(12))
                        self.assertEqual((magic,version),(b'SFHC',1))
                        body=await reader.readexactly(size)
                        if kind==1:
                            offset=0
                            for _ in range(2):
                                length=struct.unpack('!H',body[offset:offset+2])[0]; offset+=2+length
                            self.assertEqual(body[offset],4)
                        writer.write(b'SFHC'+struct.pack('!HHIHI',1,0x8000,6,kind,15)); await writer.drain()
                        if kind==2: registered.set()
                        if kind==10:
                            self.assertEqual(len(body),53)
                            self.assertEqual(body[0],1)
                except (asyncio.IncompleteReadError,ConnectionError):
                    pass
                finally:
                    writer.close()
            hub_server=await asyncio.start_server(hub,'127.0.0.1',0,ssl=hub_tls)
            import socket
            with socket.socket() as reserved:
                reserved.bind(('127.0.0.1',0)); port=reserved.getsockname()[1]
            config=self.config | dict(node_key='characters-1',node_name='Fixture character service',bind_address='127.0.0.1',
                advertise_address='127.0.0.1',port=port,hub_host='localhost',hub_port=hub_server.sockets[0].getsockname()[1],
                ca=str(root/'ca.pem'),certificate=str(root/'characters-1.pem'),private_key=str(root/'characters-1.key'),
                catalog=str(SERVICE/'statements.json'),allowed_world_nodes=['world-a'])
            config_path=root/'character.toml'
            config_path.write_text('\n'.join(key+' = '+json.dumps(value) for key,value in config.items()))
            stop=asyncio.Event(); daemon=asyncio.create_task(serve(config_path,stop))
            try:
                await asyncio.wait_for(registered.wait(),10)
                await asyncio.sleep(.05)
                tls=ssl.create_default_context(cafile=str(root/'ca.pem')); tls.load_cert_chain(root/'world-a.pem',root/'world-a.key')
                reader,writer=await asyncio.open_connection('127.0.0.1',port,ssl=tls)
                entries=json.loads((SERVICE/'statements.json').read_text())
                digest=hashlib.sha256(b''.join(u32(entry['id'])+blob(entry['sql']) for entry in entries)).hexdigest()
                async def exchange(payload):
                    writer.write(u32(len(payload))+payload); await writer.drain()
                    size=struct.unpack('!I',await asyncio.wait_for(reader.readexactly(4),5))[0]
                    reply=await asyncio.wait_for(reader.readexactly(size),5)
                    self.assertEqual(reply[0],0)
                    return reply[1:]
                await exchange(b'\0\1'+u32(1)+blob(secrets.token_hex(16))+blob(digest))
                self.assertEqual(rows(await exchange(request(2,[raw('SELECT guid FROM characters WHERE guid=1')]))),[[b'1']])
                writer.close(); await writer.wait_closed()
                outsider=ssl.create_default_context(cafile=str(root/'ca.pem')); outsider.load_cert_chain(root/'outsider.pem',root/'outsider.key')
                denied,connection=await asyncio.open_connection('127.0.0.1',port,ssl=outsider)
                self.assertEqual(await asyncio.wait_for(denied.read(),5),b'')
                connection.close(); await connection.wait_closed()
            finally:
                stop.set()
                await asyncio.wait_for(daemon,10)
                hub_server.close(); await hub_server.wait_closed()


if __name__ == '__main__':
    parser=argparse.ArgumentParser()
    parser.add_argument('--database-config')
    args,remaining=parser.parse_known_args()
    DatabaseTests.config_path=args.database_config
    unittest.main(argv=[sys.argv[0]]+remaining)
