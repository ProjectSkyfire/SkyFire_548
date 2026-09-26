# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
"""Character SQL ownership, typed statement execution and atomic save receipts."""
from decimal import Decimal
import hashlib
import json
import re
import threading
import time
from pathlib import Path
from wire import Reader, blob, u32, prepared_sql, MAX_FRAME, MAX_STATEMENTS
from character_state import LOGIN_PLAN

IDENTITY = re.compile(r'[0-9a-f]{32}\Z')


def one(cursor):
    """Consume the EOF packet even for single-row streaming metadata queries."""
    row = cursor.fetchone()
    cursor.fetchall()
    return row


class CharacterDatabase:
    def __init__(self, config, catalog_path, connector=None):
        cursor_options = {}
        if connector is None:
            import pymysql
            connector = pymysql.connect
            cursor_options['cursorclass'] = pymysql.cursors.SSCursor
        self.lock = threading.Lock()
        self.sessions = {}
        self.realm = config['realm_id']
        self.failed = False
        entries = json.loads(Path(catalog_path).read_text())
        self.catalog = {entry['id']: entry['sql'] for entry in entries}
        self.statement_ids = {entry['name']: entry['id'] for entry in entries}
        self.catalog_hash = hashlib.sha256(b''.join(u32(key) + blob(value) for key,value in sorted(self.catalog.items()))).hexdigest()
        self.connector = connector
        self.cursor_options = cursor_options
        self.config = dict(config)
        self.ownership_lock = 'skyfire-character:' + hashlib.sha256(config['mysql_database'].encode()).hexdigest()[:40]
        self.db = self._open()

    def _open(self):
        config = self.config
        db = self.connector(host=config['mysql_host'], port=config.get('mysql_port',3306),
                            user=config['mysql_user'], password=config['mysql_password'],
                            database=config['mysql_database'], charset='utf8mb4', binary_prefix=True, autocommit=True,
                            connect_timeout=5, read_timeout=30, write_timeout=30, **self.cursor_options)
        try:
            with db.cursor() as cursor:
                cursor.execute("SELECT GET_LOCK(%s,0)", ('skyfire-character:' + hashlib.sha256(config['mysql_database'].encode()).hexdigest()[:40],))
                if one(cursor)[0] != 1:
                    raise RuntimeError('Another character service owns this database')
                cursor.execute("SET SESSION sql_mode='STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION'")
                cursor.execute('SET SESSION TRANSACTION ISOLATION LEVEL REPEATABLE READ')
                cursor.execute("SELECT COUNT(*) FROM information_schema.tables WHERE table_schema=DATABASE() AND engine IS NOT NULL AND engine <> 'InnoDB'")
                if one(cursor)[0]:
                    raise RuntimeError('Character service requires transactional InnoDB tables')
                cursor.execute('SELECT guid FROM characters LIMIT 0')
                cursor.fetchall()
                cursor.execute('''CREATE TABLE IF NOT EXISTS character_service_owners (
                    realm INT UNSIGNED PRIMARY KEY, epoch BIGINT UNSIGNED NOT NULL,
                    node VARCHAR(64) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
                    instance CHAR(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL) ENGINE=InnoDB''')
                cursor.execute('''CREATE TABLE IF NOT EXISTS character_service_retired (
                    instance CHAR(32) CHARACTER SET ascii COLLATE ascii_bin PRIMARY KEY) ENGINE=InnoDB''')
                cursor.execute('''CREATE TABLE IF NOT EXISTS character_service_commits (
                    request_id CHAR(32) CHARACTER SET ascii COLLATE ascii_bin PRIMARY KEY,
                    instance CHAR(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
                    digest CHAR(64) CHARACTER SET ascii NOT NULL,
                    committed_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP) ENGINE=InnoDB''')
        except BaseException:
            db.close()
            raise

        return db

    def close(self):
        self.db.close()

    def health(self):
        """Keep the ownership connection alive. Never reconnect inside a transaction."""
        with self.lock:
            if self.failed:
                # Old RPC sessions must drain before a new ownership connection exists.
                if self.sessions:
                    return False
                try:
                    self.db = self._open()
                except Exception:
                    return False
                self.failed = False
            try:
                with self.db.cursor() as cursor:
                    cursor.execute('SELECT IS_USED_LOCK(%s) = CONNECTION_ID()', (self.ownership_lock,))
                    if one(cursor) != (1,):
                        raise RuntimeError('Character database ownership lost')
                return True
            except Exception:
                self._disconnect()
                return False

    def _disconnect(self):
        self.failed = True
        try:
            self.db.close()
        except Exception:
            pass

    def _rollback(self, error):
        # A failed BEGIN/COMMIT can leave the outcome unknown. Never replay writes.
        if error.args and error.args[0] in (0, 2006, 2013, 2055):
            self._disconnect()
            return
        try:
            self.db.rollback()
        except Exception:
            self._disconnect()

    def attach(self, peer, request):
        reader = Reader(request)
        if reader.u8() != 2 or reader.u32() != self.realm:
            raise ValueError('Character protocol or realm mismatch')
        instance, catalog_hash = reader.text(32), reader.text(64)
        reader.end()
        if not IDENTITY.fullmatch(instance) or catalog_hash != self.catalog_hash:
            raise ValueError('World and character server statement catalogs differ')
        with self.lock:
            if self.failed:
                raise RuntimeError('Character database unavailable')
            try:
                self.db.begin()
                with self.db.cursor() as cursor:
                    cursor.execute('SELECT instance FROM character_service_retired WHERE instance=%s', (instance,))
                    if one(cursor):
                        raise ValueError('This world generation has been fenced')
                    cursor.execute('SELECT epoch,node,instance FROM character_service_owners WHERE realm=%s FOR UPDATE', (self.realm,))
                    owner = one(cursor)
                    if owner and (owner[1],owner[2]) != (peer,instance):
                        if any(self.sessions.values()):
                            raise ValueError('Another world still owns active character connections')
                        cursor.execute('INSERT IGNORE INTO character_service_retired VALUES(%s)', (owner[2],))
                        # A permanently fenced incarnation can never replay its receipts.
                        cursor.execute('DELETE FROM character_service_commits WHERE instance=%s', (owner[2],))
                    epoch = owner[0] if owner and (owner[1],owner[2]) == (peer,instance) else (owner[0] + 1 if owner else 1)
                    cursor.execute('REPLACE INTO character_service_owners VALUES(%s,%s,%s,%s)', (self.realm,epoch,peer,instance))
                self.db.commit()
                self.sessions[instance] = self.sessions.get(instance,0) + 1
                return instance, epoch
            except BaseException as error:
                self._rollback(error)
                raise

    def detach(self, instance):
        with self.lock:
            if instance in self.sessions:
                self.sessions[instance] -= 1
                if not self.sessions[instance]:
                    del self.sessions[instance]

    def statement(self, reader):
        kind = reader.u8()
        if kind == 0:
            sql = reader.text(1024 * 1024)
            # Compatibility for existing raw character callers. Database credentials must
            # have access ONLY to this character schema. No DDL/transaction/session escape.
            if not re.match(r'\s*(SELECT|INSERT|REPLACE|UPDATE|DELETE)\b', sql, re.I):
                raise ValueError('Unsupported raw character operation')
            if re.search(r'character_service_|GET_LOCK|RELEASE_LOCK|INTO\s+(?:OUTFILE|DUMPFILE)|FOR\s+UPDATE', sql, re.I):
                raise ValueError('Reserved character operation')
            return sql, None
        if kind != 1:
            raise ValueError('Invalid statement kind')
        sql = self.catalog.get(reader.u32())
        if sql is None:
            raise ValueError('Unknown character statement')
        # Legacy ticket/guild clearing uses TRUNCATE, which implicitly commits in
        # MySQL. Keep those operations inside the fenced transaction instead.
        truncate = re.fullmatch(r'\s*TRUNCATE\s+(?:TABLE\s+)?([A-Za-z0-9_]+)\s*',sql,re.I)
        if truncate:
            sql = 'DELETE FROM ' + truncate[1]
        if not re.match(r'\s*(SELECT|INSERT|REPLACE|UPDATE|DELETE)\b',sql,re.I):
            raise ValueError('Catalog operation is not transactional')
        return prepared_sql(sql), self.parameters(reader)

    @staticmethod
    def parameters(reader):
        count = reader.u32()
        if count > 256:
            raise ValueError('Too many statement parameters')
        args = []
        for _ in range(count):
            tag = reader.u8()
            if tag == 0:
                args.append(None)
            elif tag == 1:
                value = reader.text(128)
                if not re.fullmatch(r'-?\d+(?:\.\d*)?(?:[eE][+-]?\d+)?', value):
                    raise ValueError('Invalid numeric parameter')
                args.append(Decimal(value))
            elif tag == 2:
                args.append(reader.blob().decode('utf-8'))
            elif tag == 3:
                args.append(reader.blob())
            else:
                raise ValueError('Invalid parameter type')
        return tuple(args)

    @staticmethod
    def result(cursor, limit=MAX_FRAME - 8192):
        if not cursor.description:
            raise ValueError('Query must return rows')
        fields = [field[1] for field in cursor.description]
        if not 1 <= len(fields) <= 1024:
            raise ValueError('Invalid result columns')
        rows, size = [], 8 + 4 * len(fields)
        while True:
            row = cursor.fetchone()
            if row is None:
                break
            values = []
            for field_type, value in zip(fields, row):
                if field_type == 16 and isinstance(value, bytes):
                    value = int.from_bytes(value, 'big')
                value = value if isinstance(value, bytes) or value is None else str(value).encode()
                values.append(b'\0' if value is None else b'\1' + blob(value))
            packed = b''.join(values)
            size += len(packed)
            if size > limit or len(rows) >= 1000000:
                raise ValueError('Character result exceeds transport limit')
            rows.append(packed)
        if size > limit:
            raise ValueError('Character result exceeds transport limit')
        return u32(len(fields)) + b''.join(u32(field) for field in fields) + u32(len(rows)) + b''.join(rows)

    def load_character(self, cursor, guid, account, declined):
        # A missing/deleted character or wrong account is an ordinary rejected login.
        # Return empty slots so worldserver follows its existing login failure path.
        cursor.execute('SELECT account FROM characters WHERE guid=%s', (guid,))
        owner = one(cursor)
        if owner != (account,):
            return u32(len(LOGIN_PLAN)) + blob(b'') * len(LOGIN_PLAN)
        answer = bytearray(u32(len(LOGIN_PLAN)))
        now = int(time.time())
        for name, parameter in LOGIN_PLAN:
            if name == 'CHAR_SEL_CHARACTER_DECLINEDNAMES' and not declined:
                answer.extend(blob(b''))
                continue
            args = (account,) if parameter == 'account' else (guid, now) if parameter == 'mail' else (guid,)
            cursor.execute(prepared_sql(self.catalog[self.statement_ids[name]]), args)
            answer.extend(blob(self.result(cursor, MAX_FRAME - 8192 - len(answer) - 4)))
        return bytes(answer)

    def save_state(self, reader):
        guid, account, create = reader.u32(), reader.u32(), reader.u8()
        if not guid or not account or create not in (0, 1):
            raise ValueError('Invalid character save identity')
        args = self.parameters(reader)
        name = 'CHAR_INS_CHARACTER' if create else 'CHAR_UPD_CHARACTER'
        sql = self.catalog[self.statement_ids[name]]
        # Both create and update carry the common character fields plus online.
        # Identity columns and insert/update selection belong to this service.
        if not args or args[-1] not in (0, 1) or (create and args[-1] != 0):
            raise ValueError('Invalid character online state')
        args = (guid, self.realm, account) + args[:-1] if create else args + (guid,)
        if len(args) != sql.count('?'):
            raise ValueError('Character snapshot layout mismatch')
        return (guid, account, create), (prepared_sql(sql), args)

    def execute(self, peer, instance, epoch, payload):
        reader = Reader(payload)
        operation = reader.u8()
        request_id = reader.text(32)
        if not IDENTITY.fullmatch(request_id) or operation not in (1,2,3,4,5):
            raise ValueError('Invalid character request')
        identity = None
        if operation == 4:
            identity = (reader.u32(), reader.u32(), reader.u8())
            if not identity[0] or not identity[1] or identity[2] not in (0, 1):
                raise ValueError('Invalid character load identity')
            statements = []
        else:
            if operation == 5:
                identity, state = self.save_state(reader)
            count = reader.u32() if operation in (3, 5) else 1
            if not (0 if operation == 5 else 1) <= count <= MAX_STATEMENTS - (operation == 5):
                raise ValueError('Invalid transaction size')
            statements = [self.statement(reader) for _ in range(count)]
            if operation == 5:
                primary = {prepared_sql(self.catalog[self.statement_ids[name]]) for name in ('CHAR_INS_CHARACTER', 'CHAR_UPD_CHARACTER')}
                if any(sql in primary for sql, _ in statements):
                    raise ValueError('Duplicate primary character snapshot')
                statements.insert(0, state)
        reader.end()
        digest = hashlib.sha256(payload).hexdigest()
        with self.lock:
            if self.failed:
                raise RuntimeError('Character database unavailable')
            try:
                self.db.begin()
                with self.db.cursor() as cursor:
                    # Read-only calls must not execute a write hidden behind the query opcode.
                    if operation == 2 and any(not re.match(r'\s*SELECT\b', sql, re.I) for sql,_ in statements):
                        raise ValueError('Character query opcode only accepts SELECT')
                    cursor.execute('SELECT epoch,node,instance FROM character_service_owners WHERE realm=%s FOR UPDATE', (self.realm,))
                    if one(cursor) != (epoch,peer,instance):
                        raise ValueError('World writer generation was revoked')
                    if operation not in (2, 4):
                        cursor.execute('SELECT instance,digest FROM character_service_commits WHERE request_id=%s', (request_id,))
                        receipt = one(cursor)
                        if receipt:
                            if receipt != (instance,digest):
                                raise ValueError('Request ID reused with different content')
                            self.db.rollback()
                            return b''
                    answer = b''
                    if operation == 4:
                        answer = self.load_character(cursor, *identity)
                    elif operation == 5:
                        guid, account, create = identity
                        cursor.execute('SELECT account FROM characters WHERE guid=%s FOR UPDATE', (guid,))
                        owner = one(cursor)
                        if (create and owner is not None) or (not create and owner != (account,)):
                            raise ValueError('Character save owner mismatch')
                    for sql,args in statements:
                        cursor.execute(sql, args)
                        if operation == 2:
                            answer = self.result(cursor)
                    if operation not in (2, 4):
                        cursor.execute('INSERT INTO character_service_commits(request_id,instance,digest) VALUES(%s,%s,%s)', (request_id,instance,digest))
                self.db.commit()  # Save and idempotency receipt commit together, before acknowledgement.
                return answer
            except BaseException as error:
                self._rollback(error)
                raise
