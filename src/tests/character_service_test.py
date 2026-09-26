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
import time
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
    def test_create_and_update_share_the_same_character_state_layout(self):
        entries = {entry['name']:entry['sql'] for entry in catalog(ROOT)}
        create = re.search(r'\((.*?)\) VALUES',entries['CHAR_INS_CHARACTER'])[1].replace(' ','').lower().split(',')
        update = re.findall(r'(\w+)\s*=\s*\?',entries['CHAR_UPD_CHARACTER'].lower())
        self.assertEqual(create[:3],['guid','realm','account'])
        self.assertEqual(update[-2:],['online','guid'])
        self.assertEqual(create[3:],update[:-2])
        self.assertEqual(len(update)-1,56)

    def test_service_login_plan_matches_native_legacy_layout(self):
        from character_state import LOGIN_PLAN
        header = (ROOT/'src/server/game/Entities/Player/Player.h').read_text()
        indexes = {name:int(index) for name,index in re.findall(r'(PLAYER_LOGIN_QUERY_\w+) = (\d+)',header)}
        source = (ROOT/'src/server/game/Handlers/CharacterHandler.cpp').read_text().split('bool LoginQueryHolder::Initialize()')[1].split('return res;')[0]
        plan = {}
        for name,body,slot in re.findall(r'stmt = CharacterDatabase.GetPreparedStatement\((CHAR_\w+)\);(.*?)SetPreparedQuery\((PLAYER_LOGIN_QUERY_\w+), stmt\)',source,re.S):
            argument = 'account' if 'm_accountId' in body else 'mail' if 'setUInt64' in body else 'guid'
            plan[indexes[slot]] = (name,argument)
        self.assertEqual(tuple(plan[i] for i in range(len(indexes))),LOGIN_PLAN)

    def test_semantic_operations_are_counted_as_reads_and_save_transactions(self):
        from metrics import Metrics
        metrics = Metrics()
        metrics.finish(metrics.begin(),4,True)
        metrics.finish(metrics.begin(),5,True)
        metrics.finish(metrics.begin(),5,False)
        self.assertEqual((metrics.reads,metrics.writes,metrics.transactions,metrics.failures),(1,1,1,1))

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

    def test_social_document_limits_and_secret_validation(self):
        from social_store import document
        channel = dict(name='Test', team=0, channel_id=0, announce=True, ownership=True, password_verifier='', bans=[])
        self.assertIsInstance(document('channels', channel), bytes)
        for change in ({'password_verifier':'plaintext'}, {'bans':[1,1]}, {'team':True}, {'sql':'DELETE FROM characters'}, {'name':'x'*129}):
            with self.assertRaises(ValueError): document('channels', channel | change)
        guild = dict(id=1,name='Guild',leader=1,motd='',info='',ranks=[dict(name='Leader',rights=0)],
                     members=[dict(guid=1,rank=0,public_note='',officer_note='private')])
        self.assertIsInstance(document('guilds',guild),bytes)
        for change in ({'leader':2}, {'bank_money':10}, {'ranks':[]}, {'members':guild['members']*2}):
            with self.assertRaises(ValueError): document('guilds', guild | change)
        with self.assertRaises(ValueError): document('inventory', {})

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
        value=re.search(r'^\s*(?:Hub|Verification)DatabaseInfo\s*=\s*"([^"\r\n]+)"',text,re.M)
        if not value:
            raise RuntimeError('Expected HubDatabaseInfo or VerificationDatabaseInfo for fixture server credentials')
        host,port,user,password,_=value[1].split(';')
        cls.schema='skyfire_character_test_'+secrets.token_hex(8)
        cls.admin=pymysql.connect(host=host,port=int(port),user=user,password=password,autocommit=True)
        with cls.admin.cursor() as cursor:
            cursor.execute('CREATE DATABASE `'+cls.schema+'` CHARACTER SET utf8mb4')
            cursor.execute('CREATE TABLE `'+cls.schema+'`.characters (guid INT UNSIGNED PRIMARY KEY,name VARCHAR(64),money BIGINT UNSIGNED NOT NULL DEFAULT 0) ENGINE=InnoDB')
            cursor.execute('INSERT INTO `'+cls.schema+'`.characters VALUES(1,%s,0)',("Tester O'é›ª",))
            cursor.execute('CREATE TABLE `'+cls.schema+'`.gm_tickets (id INT PRIMARY KEY, flags BIT(8)) ENGINE=InnoDB')
            cursor.execute("INSERT INTO `"+cls.schema+"`.gm_tickets VALUES(1,b'10101010')")
            cursor.execute('CREATE TABLE `'+cls.schema+'`.binary_fixture (payload BLOB) ENGINE=InnoDB')
        with cls.admin.cursor() as cursor:
            cursor.execute('USE `'+cls.schema+'`')
            # CI may promote the pending migration to a dated release filename.
            migrations = [path.read_text() for folder in ('pending_updates', 'updates')
                          for path in (ROOT/'sql'/folder/'characters').glob('*.sql')
                          if 'CREATE TABLE character_social_owners (' in path.read_text()]
            if len(migrations) != 1:
                raise RuntimeError('Expected one social-domain schema migration')
            migration = migrations[0]
            for statement in migration.split(';'):
                if statement.strip(): cursor.execute(statement)
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

    def test_idle_timeout_recovers_with_fresh_ownership(self):
        db = CharacterDatabase(self.config, SERVICE/'statements.json')
        try:
            self.assertTrue(db.health())
            with db.db.cursor() as cursor:
                cursor.execute('SET SESSION wait_timeout=1')
            time.sleep(2)
            self.assertFalse(db.health())
            self.assertTrue(db.failed)
            self.assertTrue(db.health())
            self.assertFalse(db.failed)
        finally:
            db.close()

    def test_lost_connection_drains_sessions_and_respects_other_owner(self):
        db = CharacterDatabase(self.config, SERVICE/'statements.json')
        other = None
        instance = secrets.token_hex(16)
        hello = b'\2'+u32(1)+blob(instance)+blob(db.catalog_hash)
        try:
            _, epoch = db.attach('world-a', hello)
            with self.admin.cursor() as cursor:
                cursor.execute('KILL CONNECTION %s', (db.db.thread_id(),))
            with self.assertRaises(Exception):
                db.execute('world-a', instance, epoch, request(2, [raw('SELECT 1')]))
            self.assertTrue(db.failed)  # Failed BEGIN is covered too.
            self.assertFalse(db.health())  # Attached session prevents recovery.
            db.detach(instance)
            other = CharacterDatabase(self.config, SERVICE/'statements.json')
            self.assertFalse(db.health())  # Another service owns the advisory lock.
            other.close(); other = None
            self.assertTrue(db.health())
            _, epoch = db.attach('world-a', hello)
            self.assertEqual(rows(db.execute('world-a', instance, epoch, request(2, [raw('SELECT 1')]))), [[b'1']])
            db.detach(instance)
        finally:
            if other: other.close()
            db.close()

    def test_uncertain_commit_receipt_prevents_double_write(self):
        import pymysql
        db = CharacterDatabase(self.config, SERVICE/'statements.json')
        instance = secrets.token_hex(16)
        hello = b'\2'+u32(1)+blob(instance)+blob(db.catalog_hash)
        try:
            _, epoch = db.attach('world-a', hello)
            db.execute('world-a', instance, epoch, request(1, [raw('UPDATE characters SET money=0 WHERE guid=1')]))
            save = request(1, [raw('UPDATE characters SET money=money+1 WHERE guid=1')])
            original = db.db.commit
            def lost_ack():
                original()
                raise pymysql.OperationalError(2013, 'simulated lost commit acknowledgement')
            db.db.commit = lost_ack
            with self.assertRaises(pymysql.OperationalError):
                db.execute('world-a', instance, epoch, save)
            self.assertTrue(db.failed)
            db.detach(instance)
            self.assertTrue(db.health())
            _, epoch = db.attach('world-a', hello)
            db.execute('world-a', instance, epoch, save)
            self.assertEqual(rows(db.execute('world-a', instance, epoch, request(2, [raw('SELECT money FROM characters WHERE guid=1')]))), [[b'1']])
            db.detach(instance)
        finally:
            db.close()

    def test_social_ownership_receipts_outbox_and_world_isolation(self):
        import pymysql
        from social_store import canonical
        cfg = self.config | {'allowed_chat_nodes': ['chat-a', 'chat-b']}
        db = CharacterDatabase(cfg, SERVICE/'statements.json')
        first, second, world = (secrets.token_hex(16) for _ in range(3))
        hello = lambda instance, realm=1, domain='channels': b'\1'+u32(realm)+blob(instance)+blob(domain)
        channel = dict(name='Test', team=0, channel_id=0, announce=True, ownership=True, password_verifier='', bans=[])
        key = 'test-'+secrets.token_hex(8)
        message = dict(id=secrets.token_hex(16),key=key,expected=0,actor=1,document=channel)
        rpc = lambda op, data: bytes([op])+blob(canonical(data))
        try:
            _, world_epoch = db.attach('world-a', b'\2'+u32(1)+blob(world)+blob(db.catalog_hash))
            with self.assertRaises(ValueError): db.social.attach('world-a', hello(first))
            with self.assertRaises(ValueError): db.social.attach('chat-a', hello(first, 2))
            session, epoch = db.social.attach('chat-a', hello(first))
            execute = lambda op, data: json.loads(db.social.execute('chat-a', session, epoch, rpc(op, data)))
            revision = execute(18, message)['revision']
            self.assertEqual(execute(18, message)['revision'], revision)
            # Failure to persist a receipt must roll back both state and outbox.
            with self.admin.cursor() as cursor:
                cursor.execute("CREATE TRIGGER reject_social_receipt BEFORE INSERT ON character_social_receipts FOR EACH ROW SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT='fixture receipt failure'")
            try:
                with self.assertRaises(Exception):
                    execute(18, message | {'id':secrets.token_hex(16),'expected':revision,'document':channel | {'announce':False}})
            finally:
                with self.admin.cursor() as cursor:
                    cursor.execute('DROP TRIGGER reject_social_receipt')
            snapshot = execute(17, {'after':'','limit':32})
            record = next(item for item in snapshot['records'] if item['key']==key)
            self.assertEqual(record['revision'], revision)
            self.assertTrue(record['document']['announce'])
            self.assertEqual(execute(19, {'after':revision,'limit':32})['events'], [])

            with self.assertRaises(ValueError): execute(18, message | {'actor': 2})
            with self.assertRaises(ValueError): execute(18, message | {'id': secrets.token_hex(16)})
            with self.assertRaises(ValueError): db.social.attach('chat-b', hello(second))
            with self.assertRaises(ValueError): db.execute('world-a',world,world_epoch,request(2,[raw('SELECT * FROM character_social_records')]))
            self.assertEqual(rows(db.execute('world-a',world,world_epoch,request(2,[raw('SELECT 1')]))), [[b'1']])
            # A committed mutation whose acknowledgement is lost must have exactly
            # one outbox event and one receipt, even after ownership reconnects.
            original = db.db.commit
            def lost_ack():
                original()
                raise pymysql.OperationalError(2013, 'fixture lost acknowledgement')
            db.db.commit = lost_ack
            update = message | {'id': secrets.token_hex(16), 'expected': revision, 'document': channel | {'announce': False}}
            with self.assertRaises(pymysql.OperationalError): execute(18, update)
            self.assertTrue(db.failed)
            db.detach(world)
            self.assertFalse(db.health())  # Social sessions also block reconnect.
            db.social.detach(session)
            self.assertTrue(db.health())
            session, epoch = db.social.attach('chat-a', hello(first))
            revision = execute(18, update)['revision']
            events = execute(19, {'after': revision-1, 'limit': 32})['events']
            self.assertEqual(len(events), 1)
            self.assertEqual(events[0]['document']['announce'], False)
            deleted = execute(18, message | {'id': secrets.token_hex(16), 'expected': revision, 'document': None})['revision']
            with self.assertRaises(ValueError): execute(18, message | {'id': secrets.token_hex(16)})
            self.assertIsNone(execute(19, {'after': deleted-1, 'limit': 1})['events'][0]['document'])
            db.social.detach(session)
            successor, next_epoch = db.social.attach('chat-b', hello(second))
            self.assertGreater(next_epoch, epoch)
            with self.assertRaises(ValueError): db.social.attach('chat-a', hello(first))
            with self.assertRaises(RuntimeError): execute(17, {'after': '', 'limit': 1})
            db.social.detach(successor)
        finally:
            db.close()

    def test_transactions_fencing_and_recovery(self):
        db=CharacterDatabase(self.config,SERVICE/'statements.json')
        first,second=secrets.token_hex(16),secrets.token_hex(16)
        def hello(instance):
            return b'\2'+u32(1)+blob(instance)+blob(db.catalog_hash)
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
                             [[b'1',"Tester O'é›ª".encode(),b'0',None,b'a\0b']])
            ids={entry['name']:entry['id'] for entry in json.loads((SERVICE/'statements.json').read_text())}
            query_name=prepared(ids['CHAR_SEL_CHECK_NAME'],[b'\2'+blob("Tester O'é›ª")])
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
                recovered.attach('world-a',b'\2'+u32(1)+blob(first)+blob(recovered.catalog_hash))
            _,epoch=recovered.attach('world-b',b'\2'+u32(1)+blob(second)+blob(recovered.catalog_hash))
            self.assertEqual(rows(recovered.execute('world-b',second,epoch,request(2,[raw('SELECT money FROM characters WHERE guid=1')]))),[[b'1']])
        finally:
            recovered.close()

    def test_semantic_load_save_and_fenced_recovery(self):
        import pymysql
        from character_state import LOGIN_PLAN
        schema = 'skyfire_character_test_' + secrets.token_hex(8)
        db = fixture = None
        try:
            with self.admin.cursor() as cursor:
                cursor.execute('CREATE DATABASE `' + schema + '` CHARACTER SET utf8mb4')
            cfg = self.config | dict(mysql_database=schema)
            fixture = pymysql.connect(host=cfg['mysql_host'],port=cfg['mysql_port'],user=cfg['mysql_user'],
                                      password=cfg['mysql_password'],database=schema,autocommit=True)
            # Only CREATE statements, always on the disposable connection. Never execute
            # dump USE/DROP/LOCK directives or import any live character records.
            base = (ROOT/'sql/base/characters_database.sql').read_text()
            tables = re.findall(r'CREATE TABLE `\w+` \(.*?\) ENGINE=InnoDB[^;]*;',base,re.S)
            self.assertGreater(len(tables),50)
            with fixture.cursor() as cursor:
                for table in tables:
                    cursor.execute(table)
            db = CharacterDatabase(cfg,SERVICE/'statements.json')
            first = secrets.token_hex(16)
            hello = lambda instance: b'\2'+u32(1)+blob(instance)+blob(db.catalog_hash)
            with self.assertRaisesRegex(ValueError,'protocol'):
                db.attach('world-a',b'\1'+hello(first)[1:])
            _, epoch = db.attach('world-a',hello(first))
            execute = lambda payload: db.execute('world-a',first,epoch,payload)
            entries = {entry['name']:entry['sql'] for entry in json.loads((SERVICE/'statements.json').read_text())}
            columns = re.search(r'\((.*?)\) VALUES',entries['CHAR_INS_CHARACTER'])[1].replace(' ','').split(',')
            state = dict.fromkeys(columns,0)
            state.update(guid=42,realm=1,account=7,name='SnapshotTest',race=1,**{'class':1},level=10,money=500)
            def parameter(value):
                return (b'\2'+blob(value)) if isinstance(value,str) else b'\1'+blob(str(value))
            def save(create=False, account=7, changes=(), values=None, request_id=None):
                fields = re.findall(r'(\w+)\s*=\s*\?',entries['CHAR_UPD_CHARACTER'])[:-1]
                snapshot = state if values is None else values
                params = [parameter(snapshot.get(field,0)) for field in fields]
                return b'\5'+blob(request_id or secrets.token_hex(16))+u32(42)+u32(account)+bytes([create])+u32(len(params))+b''.join(params)+u32(len(changes))+b''.join(changes)
            def load(account=7, declined=False):
                response = execute(b'\4'+blob(secrets.token_hex(16))+u32(42)+u32(account)+bytes([declined]))
                reader = Reader(response)
                result = [reader.blob() for _ in range(reader.u32())]
                reader.end()
                return result
            self.assertEqual(load(),[b'']*len(LOGIN_PLAN))
            creation = save(create=True,changes=[raw('INSERT INTO character_spell(guid,spell,active,disabled) VALUES(42,133,1,0)')])
            execute(creation)
            execute(creation)  # A lost creation acknowledgement must not create twice.
            snapshot = load()
            self.assertEqual(len(snapshot),42)
            self.assertEqual(rows(snapshot[0])[0][:3],[b'42',b'7',b'SnapshotTest'])
            self.assertEqual(rows(snapshot[0])[0][8],b'500')
            self.assertEqual(rows(snapshot[4])[0][0],b'133')
            self.assertEqual(snapshot[15],b'')
            self.assertEqual(rows(load(declined=True)[15]),[])
            self.assertEqual(load(account=8),[b'']*42)
            # Commit an external change between the primary and spell reads. All
            # slots must still come from the original repeatable-read snapshot.
            from unittest.mock import patch
            original_result = db.result
            calls = 0
            def concurrent_result(cursor, limit):
                nonlocal calls
                result = original_result(cursor,limit)
                calls += 1
                if calls == 1:
                    with fixture.cursor() as other:
                        fixture.begin()
                        other.execute('UPDATE characters SET money=600 WHERE guid=42')
                        other.execute('UPDATE character_spell SET disabled=1 WHERE guid=42')
                        fixture.commit()
                return result
            with patch.object(db,'result',side_effect=concurrent_result):
                consistent = load()
            self.assertEqual(rows(consistent[0])[0][8],b'500')
            self.assertEqual(rows(consistent[4])[0][-1],b'0')
            self.assertEqual(rows(load()[0])[0][8],b'600')
            with self.assertRaisesRegex(ValueError,'owner'):
                execute(save(account=8))
            with self.assertRaisesRegex(ValueError,'online state'):
                execute(save(values=state | dict(online=2)))
            with self.assertRaisesRegex(ValueError,'online state'):
                execute(save(create=True,values=state | dict(online=1)))
            state['money'] = 900
            broken = save(changes=[raw('UPDATE absent_snapshot_fixture SET value=1')])
            with self.assertRaises(Exception):
                execute(broken)
            self.assertEqual(rows(load()[0])[0][8],b'600')
            update = save(changes=[raw('UPDATE character_spell SET disabled=1 WHERE guid=42 AND spell=133')])
            execute(update)
            execute(update)
            self.assertEqual(rows(load()[0])[0][8],b'900')
            with self.assertRaisesRegex(ValueError,'different content'):
                execute(update.replace(b'900',b'901'))
            # Force one result to exceed the aggregate limit: no partial login response.
            with patch('database.MAX_FRAME',8300):
                with self.assertRaisesRegex(ValueError,'transport limit'):
                    load()
            self.assertEqual(rows(load()[0])[0][8],b'900')
            db.detach(first)
            second = secrets.token_hex(16)
            _, new_epoch = db.attach('world-b',hello(second))
            self.assertGreater(new_epoch,epoch)
            with self.assertRaisesRegex(ValueError,'revoked'):
                execute(save())
            db.detach(second)
            db.close()
            db = CharacterDatabase(cfg,SERVICE/'statements.json')
            _, new_epoch = db.attach('world-b',hello(second))
            recovered = db.execute('world-b',second,new_epoch,b'\4'+blob(secrets.token_hex(16))+u32(42)+u32(7)+b'\0')
            reader = Reader(recovered)
            self.assertEqual(reader.u32(),42)
            self.assertEqual(rows(reader.blob())[0][8],b'900')
            db.detach(second)
        finally:
            if db:
                db.close()
            if fixture:
                fixture.close()
            if not re.fullmatch(r'skyfire_character_test_[0-9a-f]{16}',schema):
                raise RuntimeError('Invalid disposable schema name')
            with self.admin.cursor() as cursor:
                cursor.execute('DROP DATABASE IF EXISTS `' + schema + '`')

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
            for name in ('hub','characters-1','world-a','chat-a','outsider'):
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
                catalog=str(SERVICE/'statements.json'),allowed_world_nodes=['world-a'],allowed_chat_nodes=['chat-a'])
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
                await exchange(b'\0\2'+u32(1)+blob(secrets.token_hex(16))+blob(digest))
                self.assertEqual(rows(await exchange(request(2,[raw('SELECT guid FROM characters WHERE guid=1')]))),[[b'1']])
                writer.close(); await writer.wait_closed()
                chat_tls=ssl.create_default_context(cafile=str(root/'ca.pem')); chat_tls.load_cert_chain(root/'chat-a.pem',root/'chat-a.key')
                reader,writer=await asyncio.open_connection('127.0.0.1',port,ssl=chat_tls)
                await exchange(b'\x10\1'+u32(1)+blob(secrets.token_hex(16))+blob('guilds'))
                snapshot=json.loads(await exchange(b'\x11'+blob(json.dumps({'after':'','limit':1}))))
                self.assertIn('records',snapshot)
                # A chat identity cannot switch to the world SQL protocol.
                payload=request(2,[raw('SELECT 1')])
                writer.write(u32(len(payload))+payload); await writer.drain()
                size=struct.unpack('!I',await asyncio.wait_for(reader.readexactly(4),5))[0]
                self.assertEqual(await reader.readexactly(size),b'\1')
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
