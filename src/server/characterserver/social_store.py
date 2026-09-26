# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
"""Fenced social snapshots, receipts and outbox. Never accepts caller SQL."""
import hashlib
import json
import re
from wire import Reader, blob, u32

DOMAINS = ('channels', 'guilds')
MAX_DOCUMENT = 256 * 1024
MAX_REVISION = (1 << 63) - 1


def canonical(value):
    return json.dumps(value, ensure_ascii=False, sort_keys=True, separators=(',', ':'), allow_nan=False).encode()


def integer(value, maximum=MAX_REVISION, minimum=0):
    if type(value) is not int or not minimum <= value <= maximum:
        raise ValueError('Invalid social integer')


def text(value, limit, empty=True):
    if not isinstance(value, str) or len(value.encode()) > limit or (not empty and not value) or any(ord(c) < 32 for c in value):
        raise ValueError('Invalid social text')


def fields(value, names):
    if not isinstance(value, dict) or set(value) != set(names.split()):
        raise ValueError('Invalid social fields')


def document(domain, value):
    """Persistence schema only. Chat authority must validate actor permissions."""
    if value is None:
        return None  # Tombstone retains the version, preventing ABA updates.
    if domain == 'channels':
        fields(value, 'name team channel_id announce ownership password_verifier bans')
        text(value['name'], 128, False)
        integer(value['team'], 0xffffffff)
        integer(value['channel_id'], 0xffffffff)
        for key in ('announce', 'ownership'):
            if type(value[key]) is not bool:
                raise ValueError('Invalid channel setting')
        verifier = value['password_verifier']
        if not isinstance(verifier, str) or (verifier and not re.fullmatch(r'pbkdf2-sha256\$[1-9][0-9]{4,6}\$[0-9a-f]{32}\$[0-9a-f]{64}', verifier)):
            raise ValueError('Password verifier required; plaintext is forbidden')
        bans = value['bans']
        if not isinstance(bans, list) or len(bans) > 4096:
            raise ValueError('Invalid channel bans')
        for guid in bans:
            integer(guid, (1 << 64)-1, 1)
        if len(set(bans)) != len(bans):
            raise ValueError('Duplicate channel ban')
    elif domain == 'guilds':
        fields(value, 'id name leader motd info ranks members')
        integer(value['id'], 0xffffffff, 1)
        integer(value['leader'], (1 << 64)-1, 1)
        text(value['name'], 128, False)
        text(value['motd'], 2048)
        text(value['info'], 4096)
        if not isinstance(value['ranks'], list) or not 1 <= len(value['ranks']) <= 10:
            raise ValueError('Invalid guild ranks')
        for rank in value['ranks']:
            fields(rank, 'name rights')
            text(rank['name'], 128, False)
            integer(rank['rights'], 0xffffffff)
        members = value['members']
        if not isinstance(members, list) or not 1 <= len(members) <= 4096:
            raise ValueError('Invalid guild members')
        guids = set()
        for member in members:
            fields(member, 'guid rank public_note officer_note')
            integer(member['guid'], (1 << 64)-1, 1)
            integer(member['rank'], len(value['ranks'])-1)
            text(member['public_note'], 512)
            text(member['officer_note'], 512)
            if member['guid'] in guids:
                raise ValueError('Duplicate guild member')
            guids.add(member['guid'])
        if value['leader'] not in guids:
            raise ValueError('Guild leader must be a member')
    else:
        raise ValueError('Invalid social domain')
    result = canonical(value)
    if len(result) > MAX_DOCUMENT:
        raise ValueError('Social snapshot too large')
    return result


def one(cursor):
    result = cursor.fetchone()
    cursor.fetchall()
    return result


class SocialStore:
    def __init__(self, database):
        self.database = database
        self.sessions = {}

    def attach(self, peer, payload):
        reader = Reader(payload)
        version, realm = reader.u8(), reader.u32()
        instance, domain = reader.text(32), reader.text(16)
        reader.end()
        db = self.database
        if peer not in db.config.get('allowed_chat_nodes', []) or version != 1 or realm != db.realm or domain not in DOMAINS or not re.fullmatch('[0-9a-f]{32}', instance):
            raise ValueError('Invalid social identity, realm or protocol')
        with db.lock:
            if db.failed:
                raise RuntimeError('Character database unavailable')
            try:
                db.db.begin()
                with db.db.cursor() as cursor:
                    cursor.execute('SELECT instance FROM character_social_retired WHERE realm=%s AND domain=%s AND instance=%s', (realm, domain, instance))
                    if one(cursor):
                        raise ValueError('Retired social incarnation')
                    cursor.execute('SELECT epoch,node,instance FROM character_social_owners WHERE realm=%s AND domain=%s FOR UPDATE', (realm, domain))
                    owner = one(cursor)
                    if owner and owner[2] == instance and owner[1] != peer:
                        raise ValueError('Social incarnation belongs to another identity')
                    if owner and owner[1:] != (peer, instance):
                        if any(key[0] == domain for key in self.sessions):
                            raise ValueError('Social domain still has attached owner')
                        cursor.execute('INSERT IGNORE INTO character_social_retired VALUES(%s,%s,%s)', (realm, domain, owner[2]))
                    epoch = owner[0] if owner and owner[1:] == (peer, instance) else (owner[0]+1 if owner else 1)
                    cursor.execute('INSERT INTO character_social_owners(realm,domain,epoch,node,instance) VALUES(%s,%s,%s,%s,%s) ON DUPLICATE KEY UPDATE epoch=VALUES(epoch),node=VALUES(node),instance=VALUES(instance)', (realm, domain, epoch, peer, instance))
                db.db.commit()
                session = (domain, instance)
                self.sessions[session] = self.sessions.get(session, 0)+1
                return session, epoch
            except BaseException as error:
                db._rollback(error)
                raise

    def detach(self, session):
        with self.database.lock:
            if session in self.sessions:
                self.sessions[session] -= 1
                if not self.sessions[session]:
                    del self.sessions[session]

    def execute(self, peer, session, epoch, payload):
        reader = Reader(payload)
        operation = reader.u8()
        raw = reader.blob(MAX_DOCUMENT+4096)
        reader.end()
        def unique(items):
            result = {}
            for key, value in items:
                if key in result:
                    raise ValueError('Duplicate social field')
                result[key] = value
            return result
        request = json.loads(raw, object_pairs_hook=unique)
        domain, instance = session
        if operation == 17:
            fields(request, 'after limit')
            text(request['after'], 192)
            integer(request['limit'], 32, 1)
        elif operation == 18:
            fields(request, 'id key expected actor document')
            if not isinstance(request['id'], str) or not re.fullmatch('[0-9a-f]{32}', request['id']):
                raise ValueError('Invalid social request identity')
            text(request['key'], 192, False)
            integer(request['expected'])
            integer(request['actor'], (1 << 64)-1, 1)
            encoded = document(domain, request['document'])
        elif operation == 19:
            fields(request, 'after limit')
            integer(request['after'])
            integer(request['limit'], 32, 1)
        else:
            raise ValueError('Unsupported social operation')
        db = self.database
        with db.lock:
            if db.failed or not self.sessions.get(session):
                raise RuntimeError('Social session unavailable')
            try:
                db.db.begin()
                with db.db.cursor() as cursor:
                    cursor.execute('SELECT epoch,node,instance,revision FROM character_social_owners WHERE realm=%s AND domain=%s FOR UPDATE', (db.realm, domain))
                    owner = one(cursor)
                    if not owner or owner[:3] != (epoch, peer, instance):
                        raise ValueError('Social owner fenced')
                    if operation == 17:
                        cursor.execute('SELECT record_key,revision,document FROM character_social_records WHERE realm=%s AND domain=%s AND record_key>%s ORDER BY record_key LIMIT %s', (db.realm, domain, request['after'].encode(), request['limit']))
                        items = cursor.fetchall()
                        result = {'head': owner[3], 'records': [{'key': key.decode(), 'revision': revision, 'document': json.loads(data) if data is not None else None} for key, revision, data in items]}
                    elif operation == 19:
                        cursor.execute('SELECT revision,record_key,document,actor FROM character_social_outbox WHERE realm=%s AND domain=%s AND revision>%s ORDER BY revision LIMIT %s', (db.realm, domain, request['after'], request['limit']))
                        result = {'head': owner[3], 'events': [{'revision': revision, 'key': key.decode(), 'document': json.loads(data) if data is not None else None, 'actor': actor} for revision, key, data, actor in cursor.fetchall()]}
                    else:
                        digest = hashlib.sha256(canonical(request)).hexdigest()
                        cursor.execute('SELECT instance,digest,revision FROM character_social_receipts WHERE realm=%s AND domain=%s AND request_id=%s', (db.realm, domain, request['id']))
                        receipt = one(cursor)
                        if receipt:
                            if receipt[:2] != (instance, digest):
                                raise ValueError('Social receipt identity reused')
                            result = {'revision': receipt[2]}
                        else:
                            cursor.execute('SELECT revision FROM character_social_records WHERE realm=%s AND domain=%s AND record_key=%s', (db.realm, domain, request['key'].encode()))
                            old = one(cursor)
                            if (old[0] if old else 0) != request['expected']:
                                raise ValueError('Social record revision conflict')
                            revision = owner[3]+1
                            integer(revision)
                            cursor.execute('INSERT INTO character_social_records VALUES(%s,%s,%s,%s,%s) ON DUPLICATE KEY UPDATE revision=VALUES(revision),document=VALUES(document)', (db.realm, domain, request['key'].encode(), revision, encoded))
                            cursor.execute('INSERT INTO character_social_outbox VALUES(%s,%s,%s,%s,%s,%s)', (db.realm, domain, revision, request['key'].encode(), encoded, request['actor']))
                            cursor.execute('INSERT INTO character_social_receipts VALUES(%s,%s,%s,%s,%s,%s)', (db.realm, domain, request['id'], instance, digest, revision))
                            cursor.execute('UPDATE character_social_owners SET revision=%s WHERE realm=%s AND domain=%s', (revision, db.realm, domain))
                            result = {'revision': revision}
                answer = canonical(result)
                db.db.commit()
                return answer
            except BaseException as error:
                db._rollback(error)
                raise
