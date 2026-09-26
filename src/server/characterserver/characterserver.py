# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
"""Mutual-TLS character database daemon; all SQL executes on this process."""
import argparse
import asyncio
import contextlib
from concurrent.futures import ThreadPoolExecutor
import ipaddress
from pathlib import Path
import re
import signal
import ssl
import struct
import sys
import time
import tomllib
from database import CharacterDatabase
from wire import MAX_FRAME, u32
from metrics import Metrics
sys.path.append(str(Path(__file__).resolve().parents[1] / 'shared/Platform'))
import hub_service
from cluster_certificates import Lifecycle


def identity(certificate):
    names = [value for group in certificate.get('subject',()) for key,value in group if key == 'commonName']
    if len(names) != 1 or not re.fullmatch(r'[A-Za-z0-9_.-]{1,64}', names[0]):
        raise ValueError('Invalid cluster certificate identity')
    return names[0]


async def hub_exchange(reader, writer, kind, body):
    writer.write(b'SFHC' + struct.pack('!HHI',1,kind,len(body)) + body)
    await writer.drain()
    magic,version,reply,size = struct.unpack('!4sHHI', await reader.readexactly(12))
    if magic != b'SFHC' or version != 1 or reply != 0x8000 or size != 6:
        raise RuntimeError('Hub rejected character service registration')
    request,lease = struct.unpack('!HI', await reader.readexactly(size))
    if request != kind or not 5 <= lease <= 300:
        raise RuntimeError('Invalid hub lease')
    return lease


async def serve(config_file, stop=None, database_factory=CharacterDatabase, channel=None):
    config_file = Path(config_file).resolve()
    config = tomllib.loads(config_file.read_text(encoding='utf-8-sig'))
    certificates = Lifecycle(config, config_file.parent)
    stop = stop or asyncio.Event()
    def path(name):
        return (config_file.parent / config[name]).resolve()
    node = config['node_key']
    if not re.fullmatch(r'[A-Za-z0-9_.-]{1,64}',node):
        raise ValueError('Invalid node_key')
    if type(config['realm_id']) is not int or not 1 <= config['realm_id'] <= 0xffffffff:
        raise ValueError('Invalid realm_id')
    address = ipaddress.ip_address(config['advertise_address'])
    if address.is_unspecified or address.is_multicast:
        raise ValueError('Advertise a concrete numeric address')
    port = config.get('port',54930)
    maximum = config.get('max_connections',32)
    if type(port) is not int or not 1 <= port <= 65535 or type(maximum) is not int or not 1 <= maximum <= 128:
        raise ValueError('Invalid listener limits')
    allowed = set(config['allowed_world_nodes'])
    if not allowed or any(not isinstance(key,str) or not re.fullmatch(r'[A-Za-z0-9_.-]{1,64}',key) for key in allowed):
        raise ValueError('Configure allowed world identities')
    if not isinstance(config.get('allowed_chat_nodes', []), list):
        raise ValueError('allowed_chat_nodes must be a list')
    social_allowed = set(config.get('allowed_chat_nodes', []))
    if social_allowed & allowed or any(not isinstance(key, str) or not re.fullmatch(r'[A-Za-z0-9_.-]{1,64}', key) for key in social_allowed):
        raise ValueError('Chat and world identities must be distinct valid node keys')
    incoming = ssl.create_default_context(ssl.Purpose.CLIENT_AUTH, cafile=str(path('ca')))
    incoming.minimum_version = ssl.TLSVersion.TLSv1_2
    incoming.verify_mode = ssl.CERT_REQUIRED
    incoming.load_cert_chain(path('certificate'),path('private_key'))
    outgoing = ssl.create_default_context(cafile=str(path('ca')))
    outgoing.minimum_version = ssl.TLSVersion.TLSv1_2
    outgoing.load_cert_chain(path('certificate'),path('private_key'))
    db = database_factory(config,path('catalog'))
    executor = ThreadPoolExecutor(max_workers=1, thread_name_prefix='character-database')
    loop = asyncio.get_running_loop()
    async def sql(call, *args):
        # Cancellation cannot let a transaction outlive its ownership/session accounting.
        future = loop.run_in_executor(executor, call, *args)
        try:
            return await asyncio.shield(future)
        except asyncio.CancelledError:
            await asyncio.shield(future)
            raise
    registered_until = 0.0
    clients, tasks = set(), set()
    metrics = Metrics()
    async def database_health():
        previous = True
        while not stop.is_set():
            healthy = await sql(db.health)
            if not healthy:
                # Force fresh handshakes after ownership is reacquired; existing
                # world epochs must never survive an unobserved database loss.
                for writer in list(clients):
                    writer.close()
                if channel:
                    channel.set_ready(False)
            if healthy != previous:
                print('Character database recovered; fresh world handshakes required.' if healthy else
                      'Character database unavailable; closing clients before ownership recovery.', flush=True)
            previous = healthy
            try:
                await asyncio.wait_for(stop.wait(), 5)
            except asyncio.TimeoutError:
                pass
    health_task = asyncio.create_task(database_health())
    supervision = asyncio.create_task(channel.run(stop,lambda: not clients and not metrics.pending)) if channel else None
    async def client(reader,writer):
        task = asyncio.current_task()
        tasks.add(task)
        instance = None
        social = False
        try:
            peer = identity(writer.get_extra_info('peercert') or {})
            if peer not in allowed | social_allowed or len(clients) >= maximum:
                return
            clients.add(writer)
            while not stop.is_set():
                header = await asyncio.wait_for(reader.readexactly(4),5) if instance is None else await reader.readexactly(4)
                size = struct.unpack('!I',header)[0]
                if not 1 <= size <= 8 * 1024 * 1024:
                    raise ValueError('Invalid character RPC size')
                payload = await asyncio.wait_for(reader.readexactly(size),30)
                if not certificates.allowed(writer.get_extra_info('ssl_object')):
                    raise RuntimeError('Client certificate revoked or CRL unavailable')
                if time.monotonic() >= registered_until or db.failed:
                    raise RuntimeError('Character service has no live hub lease or database')
                if instance is None:
                    social = peer in social_allowed
                    if payload[0] != (16 if social else 0):
                        raise ValueError('Character handshake required')
                    instance,epoch = await sql(db.social.attach if social else db.attach,peer,payload[1:])
                    reply = b'\0'
                else:
                    started = metrics.begin()
                    success = False
                    try:
                        reply = b'\0' + await sql(db.social.execute if social else db.execute,peer,instance,epoch,payload)
                        success = True
                    finally:
                        metrics.finish(started,payload[0],success)
                writer.write(u32(len(reply)) + reply)
                await asyncio.wait_for(writer.drain(),30)
        except (asyncio.IncompleteReadError, ConnectionError, asyncio.TimeoutError):
            pass
        except asyncio.CancelledError:
            raise
        except Exception as error:
            # No SQL, credentials, character values or exception messages in this log.
            code = ' code=' + str(error.args[0]) if error.args and type(error.args[0]) is int else ''
            print('Character request rejected: ' + type(error).__name__ + code,flush=True)
            with contextlib.suppress(Exception):
                writer.write(u32(1) + b'\1')
                await asyncio.wait_for(writer.drain(),2)
        finally:
            if instance is not None:
                await sql(db.social.detach if social else db.detach,instance)
            clients.discard(writer)
            writer.close()
            with contextlib.suppress(Exception):
                await asyncio.wait_for(writer.wait_closed(),2)
            tasks.discard(task)
    server = None
    try:
        server = await asyncio.start_server(client,config.get('bind_address','127.0.0.1'),port,
                                            ssl=incoming,ssl_handshake_timeout=5,limit=MAX_FRAME+4)
        backoff = 1
        while not stop.is_set():
            writer = None
            try:
                incoming.load_cert_chain(path('certificate'),path('private_key'))
                outgoing.load_cert_chain(path('certificate'),path('private_key'))
                reader,writer = await asyncio.wait_for(asyncio.open_connection(config['hub_host'],config.get('hub_port',9100),
                    ssl=outgoing,server_hostname=config['hub_host']),5)
                def text(value):
                    data=value.encode()
                    if not 1 <= len(data) <= 255:
                        raise ValueError('Invalid registration string')
                    return struct.pack('!H',len(data))+data
                registration=text(node)+text(config.get('node_name',node))+b'\4'+text(str(address))+struct.pack('!HIIII',port,0,18414,maximum,1024)
                await asyncio.wait_for(hub_exchange(reader,writer,1,registration),5)
                while not stop.is_set():
                    sent=time.monotonic()
                    lease=await asyncio.wait_for(hub_exchange(reader,writer,2,struct.pack('!BI',int(not db.failed),len(clients))),5)
                    registered_until=sent+lease if not db.failed else 0.0
                    await asyncio.wait_for(hub_exchange(reader,writer,10,metrics.packet(len(clients),not db.failed)),5)
                    await certificates.sync(reader, writer, incoming, outgoing)
                    if channel:
                        channel.set_ready(not db.failed)
                    backoff=1
                    try:
                        await asyncio.wait_for(stop.wait(),min(5,lease/3))
                    except asyncio.TimeoutError:
                        pass
            except (OSError,RuntimeError,asyncio.TimeoutError,asyncio.IncompleteReadError):
                registered_until=0.0
                print('Character service hub registration unavailable; requests are blocked.',flush=True)
                try:
                    await asyncio.wait_for(stop.wait(),backoff)
                except asyncio.TimeoutError:
                    pass
                backoff=min(backoff*2,30)
            finally:
                registered_until=0.0
                if channel:
                    channel.set_ready(False)
                if writer:
                    writer.close()
                    with contextlib.suppress(Exception):
                        await asyncio.wait_for(writer.wait_closed(),2)
    finally:
        stop.set()
        registered_until=0.0
        if server:
            server.close()
            await server.wait_closed()
        for writer in list(clients):
            writer.close()
        if tasks:
            await asyncio.gather(*list(tasks),return_exceptions=True)
        await health_task
        executor.shutdown(wait=True)
        db.close()
        if supervision:
            supervision.cancel()
            with contextlib.suppress(asyncio.CancelledError):
                await supervision


async def main(config, channel=None):
    stop=asyncio.Event()
    loop=asyncio.get_running_loop()
    for sig in (signal.SIGINT,signal.SIGTERM):
        try:
            loop.add_signal_handler(sig,stop.set)
        except NotImplementedError:
            signal.signal(sig,lambda *_:loop.call_soon_threadsafe(stop.set))
    await serve(config,stop,channel=channel)


if __name__ == '__main__':
    parser=argparse.ArgumentParser()
    parser.add_argument('--config',default='characterserver.toml')
    hub_service.arguments(parser)
    args=parser.parse_args()
    channel=hub_service.channel(args,tomllib.loads(Path(args.config).read_text(encoding='utf-8-sig')))
    asyncio.run(main(args.config,channel))
