# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
"""Versioned, bounded remote-control contracts. No node or database access."""
from collections import deque
from datetime import datetime, timezone
import hashlib
import hmac
import json
import os
from pathlib import Path
import re

VERSION = 1
MAX_BODY = 4096
MAX_EVENT = 128 * 1024
TOPICS = frozenset(('hub', 'services', 'nodes', 'realms', 'routes', 'backup', 'permissions', 'commands'))
ACTIONS = frozenset(('cluster.drain', 'cluster.disable', 'cluster.enable', 'service.start', 'service.stop',
                     'world.shutdown', 'world.restart', 'world.cancel-shutdown', 'world.cancel-restart'))
IDENTIFIER = re.compile(r'[0-9a-f]{32}\Z')
TARGET = re.compile(r'[a-zA-Z0-9_.-]{1,64}\Z')


def utc_now():
    return datetime.now(timezone.utc).isoformat(timespec='milliseconds').replace('+00:00', 'Z')


def unique_object(pairs):
    result = {}
    for key, value in pairs:
        if key in result:
            raise ValueError('Duplicate JSON key')
        result[key] = value
    return result


def decode_json(raw):
    if len(raw) > MAX_BODY:
        raise ValueError('Request exceeds 4096 bytes')
    value = json.loads(raw, object_pairs_hook=unique_object,
                       parse_constant=lambda _: (_ for _ in ()).throw(ValueError('Invalid JSON number')))
    if not isinstance(value, dict):
        raise ValueError('JSON object required')
    return value


def command(value):
    if set(value) != {'version', 'type', 'requestId', 'timestamp', 'payload'} or value['version'] != VERSION or type(value['version']) is not int or value['type'] != 'command':
        raise ValueError('Unsupported command envelope')
    request_id = value['requestId']
    if not isinstance(request_id, str) or not IDENTIFIER.fullmatch(request_id):
        raise ValueError('Request ID must contain 32 lowercase hex characters')
    stamp = value['timestamp']
    if not isinstance(stamp, str) or len(stamp) > 40:
        raise ValueError('UTC timestamp required')
    when = datetime.fromisoformat(stamp.replace('Z', '+00:00'))
    if when.tzinfo is None or abs((datetime.now(timezone.utc) - when).total_seconds()) > 60:
        raise ValueError('Command timestamp is outside the 60-second window')
    payload = value['payload']
    if not isinstance(payload, dict) or not isinstance(payload.get('action'), str) or payload['action'] not in ACTIONS:
        raise ValueError('Unsupported action')
    target = payload.get('target')
    if not isinstance(target, str) or not TARGET.fullmatch(target):
        raise ValueError('Invalid node or service key')
    schedule = payload['action'] in ('world.shutdown', 'world.restart')
    if set(payload) != ({'action', 'target', 'seconds'} if schedule else {'action', 'target'}):
        raise ValueError('Unexpected command parameters')
    if schedule and (type(payload['seconds']) is not int or not 0 <= payload['seconds'] <= 86400):
        raise ValueError('Countdown must be an integer from 0 to 86400 seconds')
    return request_id, payload


def envelope(kind, payload, *, request_id=None, epoch=None, sequence=None):
    return {'version': VERSION, 'type': kind, 'requestId': request_id, 'source': 'hub',
            'timestamp': utc_now(), 'epoch': epoch, 'sequence': sequence, 'payload': payload}


class History:
    def __init__(self, epoch, limit=32):
        self.epoch, self.sequence = epoch, 0
        self.events = deque(maxlen=limit)

    def publish(self, kind, payload, request_id=None):
        event = envelope(kind, payload, request_id=request_id, epoch=self.epoch, sequence=self.sequence + 1)
        if len(json.dumps(event).encode()) > MAX_EVENT:
            raise ValueError('Event exceeds 128 KiB')
        self.sequence += 1
        self.events.append(event)
        return event

    def replay(self, cursor):
        if not isinstance(cursor, dict) or set(cursor) != {'epoch', 'sequence'}:
            return None
        number = cursor['sequence']
        if cursor['epoch'] != self.epoch or type(number) is not int or number < 0 or number > self.sequence:
            return None
        if self.events and number < self.events[0]['sequence'] - 1:
            return None
        return [event for event in self.events if event['sequence'] > number]


def filter_event(event, topics):
    event = dict(event)
    if event['type'] in ('snapshot', 'delta'):
        event['payload'] = {key: value for key, value in event['payload'].items() if key in topics or key == 'stale'}
    elif event['type'] == 'command.result' and 'commands' not in topics:
        event['type'], event['payload'] = 'checkpoint', {}
    return event


class Outbox:
    """Bounded pending events; overflow explicitly invalidates the subscriber's snapshot."""
    def __init__(self, size=8):
        import asyncio
        self.queue = asyncio.Queue(maxsize=size)
        self.overflows = 0

    def put(self, event, snapshot):
        if self.queue.full():
            dropped = self.queue.qsize()
            while not self.queue.empty():
                self.queue.get_nowait()
            self.overflows += 1
            self.queue.put_nowait(envelope('resync_required', {'reason': 'slow_consumer', 'dropped': dropped},
                                          epoch=event['epoch'], sequence=event['sequence']))
            self.queue.put_nowait(snapshot)
        else:
            self.queue.put_nowait(event)


class Audit:
    """Append-only, HMAC-chained gateway audit. Startup refuses a modified/truncated record.

    An OS administrator can replace files/keys; immutable storage or external archival remains
    an operator responsibility. The application exposes no edit, delete, truncate or restore API.
    """
    def __init__(self, path, key):
        self.path, self.key = Path(path), key
        self.previous, self.sequence = '0' * 64, 0
        self.path.parent.mkdir(parents=True, exist_ok=True)
        if self.path.exists():
            with self.path.open('rb') as stream:
                for raw in stream:
                    if len(raw) > 8192 or not raw.endswith(b'\n'):
                        raise ValueError('Invalid audit record')
                    record = json.loads(raw)
                    signature = record.pop('signature')
                    if record.get('previous') != self.previous or record.get('sequence') != self.sequence + 1 or not hmac.compare_digest(signature, self.sign(record)):
                        raise ValueError('Audit verification failed')
                    self.previous, self.sequence = signature, record['sequence']
        self.fd = os.open(self.path, os.O_CREAT | os.O_APPEND | os.O_WRONLY, 0o600)

    def sign(self, record):
        return hmac.new(self.key, json.dumps(record, sort_keys=True, separators=(',', ':')).encode(), hashlib.sha256).hexdigest()

    def write(self, request_id, actor, action, outcome):
        record = {'sequence': self.sequence + 1, 'previous': self.previous, 'timestamp': utc_now(),
                  'requestId': request_id, 'actor': actor[:64], 'action': action[:32], 'outcome': outcome[:32]}
        signature = self.sign(record)
        raw = (json.dumps(dict(record, signature=signature), sort_keys=True) + '\n').encode()
        if os.write(self.fd, raw) != len(raw):
            raise OSError('Incomplete audit write')
        os.fsync(self.fd)
        self.previous, self.sequence = signature, record['sequence']

    def close(self):
        os.close(self.fd)
