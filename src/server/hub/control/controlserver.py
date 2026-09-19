# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
"""HTTPS gateway for hub-owned commands and monitoring. No database or game-node access."""
import argparse
import asyncio
from collections import OrderedDict
from contextlib import suppress
from dataclasses import dataclass, field
import hashlib
from http.cookies import SimpleCookie
import ipaddress
import json
import os
from pathlib import Path
import secrets
import ssl
import time
import tomllib
from urllib.parse import urlsplit

from aiohttp import ClientSession, ClientTimeout, ClientError, DummyCookieJar, TCPConnector, WSMsgType, web
from protocol import ACTIONS, IDENTIFIER, MAX_BODY, MAX_EVENT, TOPICS, Audit, History, Outbox, command, decode_json, envelope, filter_event

COOKIE = '__Host-skyfire_control'
ROOT = Path(__file__).resolve().parent


class Failure(Exception):
    def __init__(self, status, message):
        self.status, self.message = status, message


@dataclass
class Session:
    token: str
    hub_token: str
    csrf: str
    expires: float
    username: str = ''
    hub_csrf: str = ''
    status: dict = field(default_factory=dict)
    history: History = field(default_factory=lambda: History(secrets.token_hex(16)))
    subscribers: dict = field(default_factory=dict)
    jobs: dict = field(default_factory=dict)
    sockets: int = 0
    task: object = None
    refresh_lock: object = field(default_factory=asyncio.Lock)


class RateLimit:
    def __init__(self, burst=30, period=30, limit=2048):
        self.burst, self.period, self.limit, self.buckets = burst, period, limit, OrderedDict()

    def allow(self, key):
        now = time.monotonic()
        tokens, previous = self.buckets.pop(key, (self.burst, now))
        tokens = min(self.burst, tokens + (now - previous) * self.burst / self.period)
        allowed = tokens >= 1
        self.buckets[key] = (tokens - 1 if allowed else tokens, now)
        while len(self.buckets) > self.limit:
            self.buckets.popitem(last=False)
        return allowed


class ControlServer:
    def __init__(self, settings, base):
        self.settings, self.base = settings, Path(base)
        self.origin = settings['public_origin'].rstrip('/')
        public = urlsplit(self.origin)
        if public.scheme != 'https' or not public.hostname or public.username or public.password or public.path or public.query or public.fragment:
            raise ValueError('public_origin must be an HTTPS origin without credentials or path')
        self.authority = public.netloc.lower()
        bind = ipaddress.ip_address(settings.get('bind', '127.0.0.1'))
        if not bind.is_loopback and not settings.get('allow_remote', False):
            raise ValueError('Non-loopback binding requires allow_remote=true')
        hub = urlsplit(settings['hub_url'])
        if hub.scheme != 'http' or not hub.hostname or not ipaddress.ip_address(hub.hostname).is_loopback or hub.username or hub.password or hub.path or hub.query or hub.fragment:
            raise ValueError('hub_url must be a numeric loopback HTTP origin')
        self.hub_url = settings['hub_url']
        self.secret = self.path('token_file').read_text().strip()
        if len(self.secret) != 64 or any(c not in '0123456789abcdef' for c in self.secret):
            raise ValueError('Token file requires 64 lowercase hex characters')
        self.lifetime = int(settings.get('session_seconds', 900))
        if not 60 <= self.lifetime <= 3600:
            raise ValueError('session_seconds must be 60..3600')
        self.audit = Audit(self.path('audit_file'), hashlib.sha256(self.secret.encode()).digest())
        self.sessions = {}
        self.login_lock = asyncio.Lock()
        self.rate, self.login_rate = RateLimit(), RateLimit(5, 30)
        self.client = None
        self.gui = b''
        self.app = web.Application(client_max_size=MAX_BODY, middlewares=[self.boundary])
        self.app.router.add_post('/control/v1/session', self.login)
        self.app.router.add_post('/control/v1/logout', self.logout)
        self.app.router.add_get('/control/v1/status', self.status)
        self.app.router.add_get('/control/v1/capabilities', self.capabilities)
        self.app.router.add_post('/control/v1/commands', self.dispatch)
        self.app.router.add_get('/control/v1/commands/{id}', self.result)
        self.app.router.add_get('/control/v1/events', self.events)
        self.app.router.add_get('/', self.index)
        for asset in ('control.js', 'control.css'):
            self.app.router.add_get('/' + asset, self.asset)
        self.app.on_startup.append(self.start)
        self.app.on_cleanup.append(self.close)

    def path(self, key):
        path = Path(self.settings[key])
        return path if path.is_absolute() else (self.base / path).resolve()

    def tls(self):
        context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
        context.minimum_version = ssl.TLSVersion.TLSv1_2
        context.load_cert_chain(self.path('certificate'), self.path('private_key'))
        return context

    async def start(self, app):
        self.client = ClientSession(connector=TCPConnector(limit=8), cookie_jar=DummyCookieJar(),
                                    timeout=ClientTimeout(total=4), trust_env=False)
        # Render a fixed, read-only PHP shell once. No request parameters or secrets reach PHP.
        env = {key: os.environ[key] for key in ('SystemRoot', 'WINDIR', 'PATH', 'TMP', 'TEMP') if key in os.environ}
        process = await asyncio.create_subprocess_exec(str(self.path('php')), '-n', '-d', 'allow_url_fopen=0',
                    '-d', 'allow_url_include=0', str(ROOT / 'gui/index.php'),
                    stdout=asyncio.subprocess.PIPE, stderr=asyncio.subprocess.PIPE, env=env,
                    **({'creationflags': 0x08000000} if os.name == 'nt' else {}))
        try:
            output, errors = await asyncio.wait_for(process.communicate(), 5)
        except BaseException:
            process.kill(); await process.wait(); raise
        if process.returncode or len(output) > MAX_EVENT or b'<!doctype html>' not in output.lower():
            raise ValueError('PHP interface rendering failed')
        self.gui = output

    async def close(self, app):
        for session in list(self.sessions.values()):
            await self.revoke(session)
        if self.client:
            await self.client.close()
        self.audit.close()

    def reply(self, kind, payload, request, status=200):
        return web.json_response(envelope(kind, payload, request_id=request['id']), status=status)

    @web.middleware
    async def boundary(self, request, handler):
        request['id'] = secrets.token_hex(16)
        request['actor'] = '-'
        request['action'] = 'request'
        request['audited'] = False
        try:
            if not request.secure or request.host.lower() != self.authority:
                raise Failure(403, 'HTTPS and the configured host are required')
            peer = ipaddress.ip_address(request.remote or '0.0.0.0')
            if not peer.is_loopback and not self.settings.get('allow_remote', False):
                raise Failure(403, 'Remote access is disabled')
            if request.method == 'POST' or request.path == '/control/v1/events':
                if request.headers.get('Origin') != self.origin:
                    raise Failure(403, 'Same-origin request required')
            if not self.rate.allow(str(peer)):
                raise Failure(429, 'Request rate limit reached')
            response = await handler(request)
        except Failure as error:
            response = self.reply('error', {'code': error.status, 'message': error.message}, request, error.status)
        except (ValueError, UnicodeError, json.JSONDecodeError, RecursionError):
            response = self.reply('error', {'code': 400, 'message': 'Malformed or unsupported request'}, request, 400)
        except web.HTTPException as error:
            response = self.reply('error', {'code': error.status, 'message': 'Request rejected'}, request, error.status)
        except (ClientError, OSError, asyncio.TimeoutError):
            response = self.reply('error', {'code': 503, 'message': 'Control service unavailable; inspect server state before retrying a command'}, request, 503)
        finally:
            # Even malformed, unauthorized, over-limit and unknown POSTs are audited without bodies.
            if request.method == 'POST' and not request['audited']:
                self.audit.write(request['id'], request['actor'], request['action'], 'rejected')
        if not response.prepared:
            response.headers.update({'Cache-Control': 'no-store', 'X-Content-Type-Options': 'nosniff',
                'Referrer-Policy': 'no-referrer', 'X-Frame-Options': 'DENY',
                'Content-Security-Policy': "default-src 'self'; script-src 'self'; style-src 'self'; connect-src 'self'; object-src 'none'; frame-ancestors 'none'; base-uri 'none'; form-action 'self'",
                'Strict-Transport-Security': 'max-age=31536000'})
        return response

    async def json(self, request):
        if request.content_type != 'application/json':
            raise Failure(415, 'application/json required')
        return decode_json(await request.read())

    async def hub(self, method, path, session=None, form=None, peer=None):
        headers = {'X-Skyfire-Control': self.secret}
        if session:
            headers.update({'Cookie': 'hub_session=' + session.hub_token, 'X-Hub-CSRF': session.hub_csrf})
        if peer:
            headers['X-Skyfire-Client'] = peer
        async with self.client.request(method, self.hub_url + '/internal/control/v1/' + path,
                                       headers=headers, data=form, allow_redirects=False) as response:
            raw = bytearray()
            async for chunk in response.content.iter_chunked(16384):
                raw.extend(chunk)
                if len(raw) > MAX_EVENT:
                    raise Failure(502, 'Hub response exceeds the supported snapshot size')
            if response.status >= 400:
                if response.status in (401, 403):
                    raise Failure(response.status, 'Session expired, revoked, or permission denied')
                if response.status == 409:
                    raise Failure(409, 'Request already used or operation rejected; inspect its result and server state')
                raise Failure(response.status, 'Hub rejected the request')
            value = json.loads(raw) if raw else {}
            if not isinstance(value, dict):
                raise Failure(502, 'Invalid hub response')
            return value, response.headers.get('Set-Cookie', '')

    async def refresh(self, session):
        async with session.refresh_lock:
            value, _ = await self.hub('GET', 'status', session)
            session.hub_csrf = value.pop('csrf')
            session.username = value.pop('username')
            value['stale'] = False
            if len(json.dumps(value).encode()) > MAX_EVENT - 40000:
                raise Failure(502, 'Hub snapshot exceeds the supported monitoring size')
            changed = {key: item for key, item in value.items() if session.status.get(key) != item}
            session.status = value
            if changed:
                self.publish(session, 'delta', changed)
            return value

    async def authenticated(self, request, mutation=False):
        token = request.cookies.get(COOKIE, '')
        session = self.sessions.get(token)
        if not session or time.monotonic() >= session.expires:
            if session:
                await self.revoke(session)
            raise Failure(401, 'Session expired; sign in again')
        request['actor'] = session.username
        if mutation and not secrets.compare_digest(request.headers.get('X-Control-CSRF', ''), session.csrf):
            raise Failure(403, 'Invalid request token')
        try:
            await self.refresh(session)
        except Failure as error:
            if error.status in (401, 403):
                await self.revoke(session)
            raise
        return session

    async def login(self, request):
        request['action'] = 'session.login'
        value = await self.json(request)
        if set(value) != {'username', 'password'} or not all(isinstance(value[key], str) for key in value) or not 1 <= len(value['username']) <= 64 or not 1 <= len(value['password']) <= 1024:
            raise ValueError('Invalid login')
        if not self.login_rate.allow(request.remote):
            raise Failure(429, 'Too many login attempts')
        async with self.login_lock:
            if len(self.sessions) >= 16:
                raise Failure(429, 'Session capacity reached')
            self.audit.write(request['id'], '-', 'session.login', 'attempt')
            _, cookie = await self.hub('POST', 'session', form=value, peer=request.remote)
            parsed = SimpleCookie(); parsed.load(cookie)
            if 'hub_session' not in parsed:
                raise Failure(502, 'Hub did not issue a session')
            token = secrets.token_hex(32)
            session = Session(token, parsed['hub_session'].value, secrets.token_hex(32), time.monotonic() + self.lifetime)
            await self.refresh(session)
            self.sessions[token] = session
        session.task = asyncio.create_task(self.poll(session))
        self.audit.write(request['id'], session.username, 'session.login', 'accepted'); request['audited'] = True
        response = self.reply('session', {'username': session.username, 'csrf': session.csrf, 'expiresIn': self.lifetime,
                                          'permissions': session.status['permissions']}, request)
        response.set_cookie(COOKIE, token, secure=True, httponly=True, samesite='Strict', path='/', max_age=self.lifetime)
        return response

    async def logout(self, request):
        request['action'] = 'session.logout'
        session = self.sessions.get(request.cookies.get(COOKIE, ''))
        if not session:
            raise Failure(401, 'Session already ended')
        if not secrets.compare_digest(request.headers.get('X-Control-CSRF', ''), session.csrf):
            raise Failure(403, 'Invalid request token')
        request['actor'] = session.username
        self.audit.write(request['id'], session.username, 'session.logout', 'attempt')
        await self.revoke(session)
        with suppress(Failure, ClientError, OSError, asyncio.TimeoutError):
            await self.hub('POST', 'logout', session)
        self.audit.write(request['id'], session.username, 'session.logout', 'accepted'); request['audited'] = True
        response = self.reply('session', {'authenticated': False}, request)
        response.del_cookie(COOKIE, path='/', secure=True, httponly=True, samesite='Strict')
        return response

    async def revoke(self, session):
        self.sessions.pop(session.token, None)
        if session.task and session.task != asyncio.current_task():
            session.task.cancel()
            with suppress(asyncio.CancelledError):
                await session.task
        for socket in list(session.subscribers):
            with suppress(Exception):
                await asyncio.wait_for(socket.close(code=1008, message=b'Session expired or revoked'), 1)

    async def status(self, request):
        session = await self.authenticated(request)
        return self.reply('snapshot', {'status': session.status, 'csrf': session.csrf, 'username': session.username}, request)

    async def capabilities(self, request):
        session = await self.authenticated(request)
        return self.reply('capabilities', {'protocol': 1, 'commands': sorted(ACTIONS) if session.status['permissions']['operate'] else [],
                          'topics': sorted(TOPICS), 'maxRequestBytes': MAX_BODY, 'maxEventBytes': MAX_EVENT,
                          'historyEvents': 32, 'pendingEvents': 8, 'backupAvailable': False, 'recoveryActionsAvailable': False}, request)

    async def dispatch(self, request):
        value = await self.json(request)
        request_id, payload = command(value)
        request['id'], request['action'] = request_id, payload['action']
        session = await self.authenticated(request, True)
        self.audit.write(request_id, session.username, payload['action'], 'attempt')
        if not session.status['permissions']['operate']:
            raise Failure(403, 'Operator permission required')
        if request_id in session.jobs:
            raise Failure(409, 'Request already submitted; inspect its existing result')
        if len(session.jobs) >= 256:
            raise Failure(429, 'Command history capacity reached; sign in again after inspecting pending work')
        job = {'requestId': request_id, 'state': 'unknown'}
        session.jobs[request_id] = job
        try:
            job, _ = await self.hub('POST', 'commands', session, {'id': request_id, **{key: str(item) for key, item in payload.items()}})
            if job.get('requestId') != request_id or job.get('state') not in ('queued', 'dispatched', 'rejected', 'unknown'):
                raise Failure(502, 'Invalid command acknowledgement; inspect server state')
            session.jobs[request_id] = job
        finally:
            self.audit.write(request_id, session.username, payload['action'], session.jobs[request_id]['state'])
            request['audited'] = True
            self.publish(session, 'command.result', session.jobs[request_id], request_id)
        return self.reply('response', session.jobs[request_id], request, 202)

    async def result(self, request):
        session = await self.authenticated(request)
        request_id = request.match_info['id']
        if not IDENTIFIER.fullmatch(request_id) or request_id not in session.jobs:
            raise Failure(404, 'Command result not found')
        return self.reply('response', session.jobs[request_id], request)

    def snapshot(self, session):
        return envelope('snapshot', dict(session.status, commands=list(session.jobs.values())), epoch=session.history.epoch, sequence=session.history.sequence)

    def publish(self, session, kind, payload, request_id=None):
        event = session.history.publish(kind, payload, request_id)
        for socket, (outbox, topics) in list(session.subscribers.items()):
            outbox.put(filter_event(event, topics), filter_event(self.snapshot(session), topics))

    async def poll(self, session):
        failures = 0
        try:
            while session.token in self.sessions:
                await asyncio.sleep(1)
                if time.monotonic() >= session.expires:
                    await self.revoke(session); return
                try:
                    await self.refresh(session)
                    pending = [(key, job) for key, job in session.jobs.items() if job['state'] == 'queued'][:8]
                    for request_id, job in pending:
                        if job['state'] == 'queued':
                            try:
                                value, _ = await self.hub('GET', 'commands/' + request_id, session)
                            except Failure as error:
                                if error.status != 404:
                                    raise
                                value = {'requestId': request_id, 'state': 'unknown'}
                            if value != job:
                                session.jobs[request_id] = value
                                self.audit.write(request_id, session.username, 'command.result', value['state'])
                                self.publish(session, 'command.result', value, request_id)
                    failures = 0
                except Failure as error:
                    if error.status in (401, 403):
                        await self.revoke(session); return
                    failures += 1
                except (ClientError, OSError, asyncio.TimeoutError):
                    failures += 1
                if failures:
                    session.status['stale'] = True
                    self.publish(session, 'delta', {'stale': True})
                    if failures >= 3:
                        for socket in list(session.subscribers):
                            await socket.close(code=1013, message=b'Hub unavailable; resynchronize on reconnect')
        except asyncio.CancelledError:
            raise
        except Exception:
            await self.revoke(session)

    async def events(self, request):
        session = await self.authenticated(request)
        if session.sockets >= 2:
            raise Failure(429, 'Two monitoring connections per session are supported')
        socket = web.WebSocketResponse(heartbeat=10, max_msg_size=MAX_BODY, compress=False, timeout=2)
        session.sockets += 1
        writer = None
        try:
            await socket.prepare(request)
            message = await asyncio.wait_for(socket.receive(), 5)
            if message.type != WSMsgType.TEXT:
                raise ValueError('Subscription required')
            value = decode_json(message.data.encode())
            if set(value) - {'version', 'type', 'csrf', 'topics', 'cursor'} or value.get('version') != 1 or type(value.get('version')) is not int or value.get('type') != 'subscribe' or not isinstance(value.get('csrf'), str) or not secrets.compare_digest(value['csrf'], session.csrf):
                raise ValueError('Invalid subscription')
            raw_topics = value.get('topics', sorted(TOPICS))
            if not isinstance(raw_topics, list) or not raw_topics or len(raw_topics) > len(TOPICS) or not all(isinstance(topic, str) for topic in raw_topics):
                raise ValueError('Invalid topics')
            if self.sessions.get(session.token) is not session or time.monotonic() >= session.expires:
                raise ValueError('Session expired during subscription')
            topics = set(raw_topics)
            if not topics <= TOPICS or len(topics) != len(raw_topics):
                raise ValueError('Invalid topics')
            outbox = Outbox()
            # All-topic subscribers can resume deltas. Filter changes always get a fresh snapshot.
            replay = session.history.replay(value.get('cursor')) if topics == TOPICS else None
            if replay is None or len(replay) > 6:
                if value.get('cursor') is not None:
                    outbox.queue.put_nowait(envelope('resync_required', {'reason': 'cursor_unavailable', 'dropped': 0},
                                                      epoch=session.history.epoch, sequence=session.history.sequence))
                outbox.queue.put_nowait(filter_event(self.snapshot(session), topics))
            else:
                for event in replay:
                    outbox.queue.put_nowait(filter_event(event, topics))
            session.subscribers[socket] = (outbox, topics)
            async def send():
                while not socket.closed:
                    try:
                        event = await asyncio.wait_for(outbox.queue.get(), 10)
                    except asyncio.TimeoutError:
                        event = envelope('heartbeat', {}, epoch=session.history.epoch, sequence=session.history.sequence)
                    await asyncio.wait_for(socket.send_json(event), 2)
                    if outbox.overflows >= 3:
                        await socket.close(code=1013, message=b'Slow consumer; reconnect for snapshot'); return
            async def guarded_send():
                try:
                    await send()
                except (ClientError, OSError, asyncio.TimeoutError, RuntimeError):
                    await socket.close(code=1013, message=b'Write deadline exceeded')
            writer = asyncio.create_task(guarded_send())
            async for message in socket:
                # Commands never travel on the monitoring channel. A new subscription uses a new socket.
                if message.type in (WSMsgType.TEXT, WSMsgType.BINARY):
                    await socket.close(code=1008, message=b'Monitoring is read-only'); break
        except (ValueError, asyncio.TimeoutError):
            await socket.close(code=1008, message=b'Invalid or missing subscription')
        finally:
            session.sockets -= 1
            session.subscribers.pop(socket, None)
            if writer:
                writer.cancel()
                with suppress(asyncio.CancelledError, OSError):
                    await writer
        return socket

    async def index(self, request):
        return web.Response(body=self.gui, content_type='text/html', charset='utf-8')

    async def asset(self, request):
        return web.FileResponse(ROOT / 'gui' / request.path[1:])


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--config', default=str(ROOT / 'control.toml'))
    args = parser.parse_args()
    path = Path(args.config).resolve()
    settings = tomllib.loads(path.read_text())
    if not settings.get('enabled', False):
        print('Remote control is disabled.'); return
    server = ControlServer(settings, path.parent)
    web.run_app(server.app, host=settings.get('bind', '127.0.0.1'), port=int(settings.get('port', 54881)),
                ssl_context=server.tls(), access_log=None, handler_cancellation=True,
                shutdown_timeout=5, max_line_size=4096, max_field_size=4096)


if __name__ == '__main__':
    main()
