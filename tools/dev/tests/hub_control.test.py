# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
"""Phase 7 protocol, TLS and real-hub integration checks. Starts no managed children.

Requires the hub audit migration. Temporary identities and cluster policy are removed;
immutable audit records intentionally remain. Uses an isolated hub and ephemeral ports.
"""
import argparse
import asyncio
from copy import deepcopy
from datetime import datetime, timedelta, timezone
import hashlib
import json
import os
from pathlib import Path
import re
import secrets
import socket
import ssl
import subprocess
import sys
import tempfile
import time

ROOT = Path(__file__).resolve().parents[3]
sys.dont_write_bytecode = True
sys.path.insert(0, str(ROOT / 'src/server/hub/control'))
from protocol import Audit, History, Outbox, command, decode_json, envelope, filter_event, utc_now
from controlserver import COOKIE, ControlServer, RateLimit
from aiohttp import ClientSession, CookieJar, WSMsgType, web


def rejected(callback):
    try:
        callback()
    except (ValueError, TypeError):
        return
    raise AssertionError('Invalid input was accepted')


def pure_tests(root):
    valid = {'version': 1, 'type': 'command', 'requestId': 'a' * 32, 'timestamp': utc_now(),
             'payload': {'action': 'world.restart', 'target': 'world', 'seconds': 300}}
    assert command(valid)[1]['seconds'] == 300
    for seconds in (-1, 86401, True, 1.2, '300', None):
        value = deepcopy(valid); value['payload']['seconds'] = seconds
        rejected(lambda: command(value))
    for action in ('shell', 'sql', 'file.read', 'memory.write', 'process.exec', 'server command'):
        value = deepcopy(valid); value['payload']['action'] = action
        rejected(lambda: command(value))
    for target in ('../world', 'world;shutdown', 'world\nstop', 'x' * 65):
        value = deepcopy(valid); value['payload']['target'] = target
        rejected(lambda: command(value))
    for stamp in ('no-date', '2020-01-01T00:00:00Z', datetime.now().isoformat()):
        value = deepcopy(valid); value['timestamp'] = stamp
        rejected(lambda: command(value))
    for raw in (b'{"x":1,"x":2}', b'[]', b'{"x":NaN}', b'{' + b'x' * 4096):
        rejected(lambda: decode_json(raw))
    history = History('epoch', 4)
    for count in range(7):
        history.publish('delta', {'hub': {'uptimeSeconds': count}})
    assert history.replay({'epoch': 'epoch', 'sequence': 2}) is None
    assert len(history.replay({'epoch': 'epoch', 'sequence': 3})) == 4
    assert history.replay({'epoch': 'other', 'sequence': 7}) is None
    assert history.replay({'epoch': 'epoch', 'sequence': True}) is None
    outbox = Outbox()
    for count in range(1000):
        event = history.publish('delta', {'hub': count})
        snapshot = envelope('snapshot', {'hub': count}, epoch='epoch', sequence=history.sequence)
        outbox.put(event, snapshot)
    assert outbox.queue.qsize() <= 8 and outbox.overflows > 100
    queued = list(outbox.queue._queue)
    assert queued[0]['type'] == 'resync_required' and queued[0]['payload']['dropped'] == 8
    assert queued[1]['type'] == 'snapshot'
    assert filter_event(envelope('command.result', {}), {'hub'})['type'] == 'checkpoint'
    path = root / 'audit-pure.jsonl'
    audit = Audit(path, b'test-key'); audit.write('b' * 32, 'viewer', 'test', 'denied'); audit.close()
    audit = Audit(path, b'test-key'); assert audit.sequence == 1; audit.close()
    path.write_bytes(path.read_bytes().replace(b'denied', b'passed'))
    rejected(lambda: Audit(path, b'test-key'))
    limiter = RateLimit(2, 10000)
    assert limiter.allow('peer') and limiter.allow('peer') and not limiter.allow('peer')
    print('PASS protocol validation, replay gaps, 1000-event bounded flood, audit tampering and rate limits')


def free_port():
    with socket.socket() as sock:
        sock.bind(('127.0.0.1', 0))
        return sock.getsockname()[1]


async def integration(args, root):
    hidden = subprocess.CREATE_NO_WINDOW if os.name == 'nt' else 0
    config = Path(args.hub_config).read_text(encoding='utf-8-sig')
    connection = re.search(r'^\s*HubDatabaseInfo\s*=\s*"([^"\r\n]+)"', config, re.M).group(1)
    host, port, user, password, database = connection.split(';')
    env = os.environ.copy(); env['MYSQL_PWD'] = password
    def sql(statement, fail=False):
        result = subprocess.run([args.mysql, '--no-defaults', '--protocol=TCP', '--host=' + host,
                    '--port=' + port, '--user=' + user, '--database=' + database, '--batch', '--raw',
                    '--skip-column-names'], input=statement, text=True, capture_output=True, env=env)
        if fail:
            assert result.returncode and 'immutable' in result.stderr
        elif result.returncode:
            raise RuntimeError(result.stderr.replace(password, '[redacted]'))
        return result.stdout.strip()
    prefix = 'controltest_' + secrets.token_hex(6)
    identity_password = secrets.token_hex(16)
    salt = secrets.token_bytes(16)
    digest = hashlib.pbkdf2_hmac('sha256', identity_password.encode(), salt, 210000).hex()
    verifier = '$pbkdf2-sha256$210000$' + salt.hex() + '$' + digest
    runner = None
    process = None
    log = None
    clients = []
    try:
        for role, flags in [('viewer', 17), ('operator', 21), ('administrator', 31), ('recovery', 49), ('local', 15)]:
            sql(f"INSERT INTO hub_admins(username,password_hash,access_flags,enabled) VALUES('{prefix}_{role}','{verifier}',{flags},1)")
        node = prefix + '_node'
        sql(f"INSERT INTO hub_cluster_policy(node_key,name,capabilities,admin_state,updated_by,service_type) VALUES('{node}','Control test node',72,0,'test',2)")
        private_port, cluster_port, tls_port = free_port(), free_port(), free_port()
        secret = secrets.token_hex(32); (root / 'token').write_text(secret)
        (root / 'openssl.cnf').write_text('[req]\ndistinguished_name=dn\n[dn]\n')
        cert_env = os.environ.copy(); cert_env['OPENSSL_CONF'] = str(root / 'openssl.cnf')
        cert = subprocess.run([args.openssl, 'req', '-x509', '-newkey', 'rsa:2048', '-nodes', '-keyout',
            str(root / 'key.pem'), '-out', str(root / 'cert.pem'), '-days', '1', '-subj', '/CN=localhost',
            '-addext', 'subjectAltName=DNS:localhost,IP:127.0.0.1', '-addext', 'basicConstraints=critical,CA:TRUE'],
            capture_output=True, env=cert_env, creationflags=hidden)
        assert not cert.returncode, 'Test certificate generation failed'
        settings = {'public_origin': f'https://127.0.0.1:{tls_port}', 'hub_url': f'http://127.0.0.1:{private_port}',
            'token_file': 'token', 'audit_file': 'gateway.jsonl', 'certificate': 'cert.pem', 'private_key': 'key.pem',
            'php': args.php, 'session_seconds': 60}
        values = {'Hub.Port': str(cluster_port), 'Hub.BindIP': '"127.0.0.1"', 'Hub.Cluster.Enable': '1',
            'Hub.Cluster.Certificate': '"' + str(root / 'cert.pem') + '"',
            'Hub.Cluster.PrivateKey': '"' + str(root / 'key.pem') + '"',
            'Hub.Cluster.CA': '"' + str(root / 'cert.pem') + '"',
            'Hub.AuthnetIngress.Enable': '0', 'Hub.LegacyIngress.Enable': '0', 'Hub.PacketLog.Enable': '0',
            'PacketLogServerControlFile': '""', 'Web.Enable': '1', 'Web.Port': str(private_port),
            'Web.BindIP': '"127.0.0.1"', 'Web.AllowRemote': '0', 'Web.ControlTokenFile': '"' + str(root / 'token') + '"',
            'Console.Enable': '0', 'LogsDir': '"' + str(root) + '"'}
        for key, value in values.items():
            pattern = r'(?m)^[ \t]*' + re.escape(key) + r'[ \t]*=.*$'
            if re.search(pattern, config): config = re.sub(pattern, lambda _: key + ' = ' + value, config)
            else: config += '\n' + key + ' = ' + value + '\n'
        (root / 'hub.conf').write_text(config)
        log = (root / 'hub.log').open('w')
        process = subprocess.Popen([args.hub, '-c', str(root / 'hub.conf')], cwd=root, stdout=log,
                                   stderr=subprocess.STDOUT, creationflags=hidden)
        for _ in range(100):
            if process.poll() is not None:
                raise RuntimeError('Isolated hub failed; inspect ' + str(root / 'hub.log'))
            try:
                with socket.create_connection(('127.0.0.1', private_port), timeout=.1): break
            except OSError: await asyncio.sleep(.1)
        else: raise RuntimeError('Isolated hub startup timeout')
        server = ControlServer(settings, root)
        runner = web.AppRunner(server.app); await runner.setup()
        await web.TCPSite(runner, '127.0.0.1', tls_port, ssl_context=server.tls()).start()
        trust = ssl.create_default_context(cafile=str(root / 'cert.pem'))
        origin = settings['public_origin']
        async def client(role):
            result = ClientSession(cookie_jar=CookieJar(unsafe=True), headers={'Origin': origin})
            clients.append(result)
            async with result.post(origin + '/control/v1/session', ssl=trust,
                                   json={'username': prefix + '_' + role, 'password': identity_password}) as response:
                value = await response.json()
                assert response.status == (401 if role == 'local' else 200), (role, response.status, value)
                if role != 'local':
                    assert 'Secure' in response.headers['Set-Cookie'] and 'HttpOnly' in response.headers['Set-Cookie']
                    assert value['payload']['permissions']['role'] == role
                    result.headers['X-Control-CSRF'] = value['payload']['csrf']
            return result
        viewer = await client('viewer'); operator = await client('operator')
        administrator = await client('administrator'); recovery = await client('recovery'); await client('local')
        async def req(client, path, body=None, expected=200, headers=None):
            server.rate.buckets.clear()
            async with client.request('POST' if body is not None else 'GET', origin + '/control/v1/' + path,
                                      json=body, ssl=trust, headers=headers) as response:
                value = await response.json()
                assert response.status == expected, (path, response.status, value)
                assert value['version'] == 1 and value['source'] == 'hub'
                return value
        if args.test_backup_schedules:
            assert database.startswith('skyfire_controltest_'), 'Schedule mutations require an isolated test database'
            initial = (await req(viewer, 'backup/schedules'))['payload']
            assert len(initial['schedules']) == 4 and not initial['canEdit'] and not initial['available']
            assert initial['timezone'] == 'UTC' and initial['executionState'] == 'awaiting_backup_service'
            auth = next(row for row in initial['schedules'] if row['target'] == 'auth')
            value = dict(auth, id=secrets.token_hex(16), enabled=1, mode='interval', intervalMinutes=30, minuteOfDay=0, weekday=0)
            for client in (viewer, operator, recovery):
                await req(client, 'backup/schedules', value, 403)
            await req(administrator, 'backup/schedules', value, 403, {'X-Control-CSRF': 'invalid'})
            for patch in ({'enabled': True}, {'intervalMinutes': 14}, {'minuteOfDay': 1440}, {'weekday': 7}, {'target': '../auth'}, {'cron': '* * * * *'}):
                await req(administrator, 'backup/schedules', dict(value, **patch), 400)
            saved = (await req(administrator, 'backup/schedules', value))['payload']
            assert next(row for row in saved['schedules'] if row['target']=='auth')['revision'] == auth['revision']+1
            await req(administrator, 'backup/schedules', value, 409)
            await req(administrator, 'backup/schedules', dict(value,id=secrets.token_hex(16)), 409)
            async with ClientSession(cookie_jar=CookieJar(unsafe=True)) as local:
                url = settings['hub_url']
                async with local.post(url+'/api/v1/session',data={'username':prefix+'_administrator','password':identity_password}) as response:
                    assert response.status == 200
                async with local.get(url+'/api/v1/status') as response:
                    local_csrf = (await response.json())['csrfToken']
                async with local.get(url+'/api/v1/backup/schedules') as response:
                    row = next(row for row in (await response.json())['schedules'] if row['target']=='auth')
                for mode, minute, day in (('daily', 1325, 0), ('weekly', 190, 6)):
                    data = dict(row,id=secrets.token_hex(16),mode=mode,minuteOfDay=minute,weekday=day,intervalMinutes=60)
                    async with local.post(url+'/api/v1/backup/schedules',data=data,headers={'X-Hub-CSRF':local_csrf}) as response:
                        assert response.status == 200
                        row = next(row for row in (await response.json())['schedules'] if row['target']=='auth')
                    assert row['mode']==mode and row['minuteOfDay']==minute and row['weekday']==day
                async with local.post(url+'/api/v1/backup/schedules',data=dict(data,id=secrets.token_hex(16),target='../bad'),headers={'X-Hub-CSRF':local_csrf}) as response:
                    assert response.status == 400
            remote = (await req(administrator,'backup/schedules'))['payload']
            assert next(item for item in remote['schedules'] if item['target']=='auth') == row
            assert sql("SELECT mode,minute_of_day,weekday FROM hub_backup_schedules WHERE target='auth'") == 'weekly\t190\t6'
            assert int(sql("SELECT COUNT(*) FROM hub_control_audit WHERE action='backup.schedule' AND phase='applied'")) >= 3
            print('PASS persisted schedules, both web interfaces, UTC modes, admin checks, malformed inputs and revision conflicts')
        def cmd(action='cluster.drain', **extra):
            return {'version': 1, 'type': 'command', 'requestId': secrets.token_hex(16), 'timestamp': utc_now(),
                    'payload': {'action': action, 'target': node, **extra}}
        await req(viewer, 'commands', cmd(), 403)
        await req(recovery, 'commands', cmd(), 403)
        await req(operator, 'commands', cmd(), 403, {'Origin': 'https://attacker.invalid'})
        await req(operator, 'commands', cmd(), 403, {'X-Control-CSRF': 'invalid'})
        for action in ('shell', 'sql', 'file.read', 'memory.write', 'process.exec'):
            await req(operator, 'commands', cmd(action), 400)
        await req(operator, 'commands', cmd('world.restart', seconds=True), 400)
        await req(operator, 'commands', cmd(extra='forbidden'), 400)
        await req(operator, 'commands', cmd('service.start'), 404)
        await req(operator, 'commands', {'padding': 'x' * 5000}, 413)
        expired = cmd(); expired['timestamp'] = (datetime.now(timezone.utc) - timedelta(minutes=2)).isoformat()
        await req(operator, 'commands', expired, 400)
        print('PASS verified HTTPS, role matrix, CSRF/origin checks, oversized and malformed requests')
        # Every real hub denial and successful policy change must have correlated SQL records.
        async with ClientSession() as internal:
            url = settings['hub_url'] + '/internal/control/v1/'
            async with internal.get(url + 'status') as response: assert response.status == 403
            async with internal.post(url + 'session', headers={'X-Skyfire-Control': secret,
                    'X-Skyfire-Client': '127.0.0.1'}, data={'username': prefix + '_viewer', 'password': identity_password}) as response:
                assert response.status == 200
                hub_cookie = response.cookies['hub_session'].value
            headers = {'X-Skyfire-Control': secret, 'Cookie': 'hub_session=' + hub_cookie}
            async with internal.get(url + 'status', headers=headers) as response:
                headers['X-Hub-CSRF'] = (await response.json())['csrf']
            denied_id = secrets.token_hex(16)
            async with internal.post(url + 'commands', headers=headers, data={'id': denied_id, 'action': 'cluster.drain', 'target': node}) as response:
                assert response.status == 403
            assert sql(f"SELECT outcome FROM hub_control_audit WHERE request_id='{denied_id}' AND phase='rejected'") == 'permission'
        ws = await operator.ws_connect(origin + '/control/v1/events', ssl=trust)
        await ws.send_json({'version': 1, 'type': 'subscribe', 'csrf': operator.headers['X-Control-CSRF']})
        initial = await ws.receive_json(timeout=5); assert initial['type'] == 'snapshot'
        assert not any('ExecutablePath' in json.dumps(row) for row in initial['payload']['services'])
        drain = cmd(); response = await req(operator, 'commands', drain, 202)
        assert response['requestId'] == drain['requestId'] and response['payload']['state'] == 'dispatched', response
        for _ in range(10):
            event = await ws.receive_json(timeout=5)
            if event['type'] == 'command.result':
                assert event['requestId'] == drain['requestId']; break
        else: raise AssertionError('Correlated WebSocket result missing')
        assert sql(f"SELECT admin_state FROM hub_cluster_policy WHERE node_key='{node}'") == '1'
        await req(operator, 'commands', drain, 409)
        restore = cmd('cluster.enable'); assert (await req(operator, 'commands', restore, 202))['payload']['state'] == 'dispatched'
        assert sql(f"SELECT admin_state FROM hub_cluster_policy WHERE node_key='{node}'") == '0'
        audit_id = drain['requestId']
        assert sql(f"SELECT COUNT(*) FROM hub_control_audit WHERE request_id='{audit_id}'") == '3'
        sql(f"UPDATE hub_control_audit SET outcome='tampered' WHERE request_id='{audit_id}'", fail=True)
        sql(f"DELETE FROM hub_control_audit WHERE request_id='{audit_id}'", fail=True)
        print('PASS real hub drain/restore, correlated acknowledgement, duplicate rejection and immutable SQL audit')
        await ws.close()
        server.rate.buckets.clear()
        ws = await operator.ws_connect(origin + '/control/v1/events', ssl=trust)
        await ws.send_json({'version': 1, 'type': 'subscribe', 'csrf': operator.headers['X-Control-CSRF'],
                           'cursor': {'epoch': 'expired', 'sequence': 0}})
        assert (await ws.receive_json(timeout=5))['type'] == 'resync_required'
        snapshot = await ws.receive_json(timeout=5); assert snapshot['type'] == 'snapshot'
        cursor = {'epoch': snapshot['epoch'], 'sequence': snapshot['sequence']}
        await ws.close()
        session = next(value for value in server.sessions.values() if value.username == prefix + '_operator')
        server.publish(session, 'delta', {'hub': {'uptimeSeconds': 123}})
        ws = await operator.ws_connect(origin + '/control/v1/events', ssl=trust)
        await ws.send_json({'version': 1, 'type': 'subscribe', 'csrf': session.csrf, 'cursor': cursor})
        replay = await ws.receive_json(timeout=5)
        assert replay['sequence'] == cursor['sequence'] + 1 and replay['type'] == 'delta'
        await ws.send_json(cmd())
        while (await ws.receive(timeout=5)).type not in (WSMsgType.CLOSE, WSMsgType.CLOSED): pass
        assert ws.close_code == 1008
        await ws.close()
        # Revocation is checked on an already-authenticated session, not only at login.
        sql(f"UPDATE hub_admins SET access_flags=1 WHERE username='{prefix}_viewer'")
        await req(viewer, 'status', expected=401)
        sql(f"UPDATE hub_admins SET password_hash='revoked-for-test' WHERE username='{prefix}_administrator'")
        await req(administrator, 'status', expected=401)
        session.expires = time.monotonic() - 1
        await req(operator, 'status', expected=401)
        assert session.token not in server.sessions
        server.rate = RateLimit(1, 10000)
        async with recovery.get(origin + '/control/v1/capabilities', ssl=trust) as response: assert response.status == 200
        async with recovery.get(origin + '/control/v1/capabilities', ssl=trust) as response: assert response.status == 429
        print('PASS reconnect replay/resync, read-only WebSocket, live permission revocation, expiry and flood rejection')
        server.rate.buckets.clear()
        async with recovery.get(origin + '/', ssl=trust) as response:
            assert response.status == 200 and 'Your cluster' in await response.text()
        process.terminate(); process.wait(timeout=10)
        await req(recovery, 'logout', {}, 200)
        assert not any(value.username == prefix + '_recovery' for value in server.sessions.values())
        print('PASS local logout revokes access even while the hub is unavailable')
    finally:
        for client in clients: await client.close()
        if runner: await runner.cleanup()
        # This process is an isolated hub with no children; never terminate the deployment hub.
        if process and process.poll() is None:
            process.terminate(); process.wait(timeout=10)
        if log: log.close()
        names = ','.join("'" + prefix + '_' + role + "'" for role in ('viewer', 'operator', 'administrator', 'recovery', 'local'))
        sql('DELETE FROM hub_admins WHERE username IN (' + names + ')')
        sql(f"DELETE FROM hub_cluster_policy WHERE node_key='{prefix}_node'")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--hub', required=True)
    parser.add_argument('--hub-config', required=True)
    parser.add_argument('--php', required=True)
    parser.add_argument('--mysql', default='mysql')
    parser.add_argument('--openssl', default='openssl')
    parser.add_argument('--test-backup-schedules', action='store_true')
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='skyfire-control-') as directory:
        root = Path(directory)
        pure_tests(root)
        asyncio.run(integration(args, root))
    print('All Phase 7 integration checks passed; isolated processes stopped and synthetic identities removed.')


if __name__ == '__main__':
    main()
