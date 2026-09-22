# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
"""Read-only terrain/collision daemon; registration through the hub, bytes over mTLS HTTPS."""
import argparse
import asyncio
import contextlib
import ipaddress
import os
import time
from pathlib import Path
import signal
import ssl
import struct
import subprocess
import sys
import uuid
import tomllib
from aiohttp import web
from map_common import KEY, catalog, certificate_key, client_tls, frame, wire_string
sys.path.append(str(Path(__file__).resolve().parents[1] / 'shared/Platform'))
import hub_service
from cluster_certificates import Lifecycle


def memory_mib():
    if os.name == 'nt':
        import ctypes
        from ctypes import wintypes
        class Counters(ctypes.Structure):
            _fields_ = [('cb', wintypes.DWORD), ('faults', wintypes.DWORD)] + [(name, ctypes.c_size_t) for name in ('peak', 'working', 'peakPaged', 'paged', 'peakNonpaged', 'nonpaged', 'pagefile', 'peakPagefile')]
        info = Counters(); info.cb = ctypes.sizeof(info)
        kernel = ctypes.WinDLL('kernel32', use_last_error=True)
        kernel.GetCurrentProcess.restype = wintypes.HANDLE
        psapi = ctypes.WinDLL('psapi', use_last_error=True)
        psapi.GetProcessMemoryInfo.argtypes = [wintypes.HANDLE, ctypes.POINTER(Counters), wintypes.DWORD]
        if psapi.GetProcessMemoryInfo(kernel.GetCurrentProcess(), ctypes.byref(info), info.cb):
            return info.working // (1024 * 1024)
    else:
        try:
            return int(Path('/proc/self/statm').read_text().split()[1]) * os.sysconf('SC_PAGE_SIZE') // (1024 * 1024)
        except (OSError, ValueError, IndexError):
            pass
    return 0


class RestartRequested(Exception):
    pass


async def exchange(reader, writer, kind, body):
    writer.write(frame(kind, body))
    await writer.drain()
    header = await asyncio.wait_for(reader.readexactly(12), 5)
    magic, version, reply, size = struct.unpack('!4sHHI', header)
    if magic != b'SFHC' or version != 1 or size > 4096:
        raise RuntimeError('Invalid hub response')
    data = await asyncio.wait_for(reader.readexactly(size), 5)
    if reply not in (0x8000,0x8004) or len(data) != 6 or (reply == 0x8004 and kind != 3):
        raise RuntimeError('Hub rejected mapserver registration/lifecycle')
    request, lease = struct.unpack('!HI', data)
    if request != kind or not 5 <= lease <= 300:
        raise RuntimeError('Invalid hub acknowledgement')
    if reply == 0x8004:
        raise RestartRequested()
    return lease


def restart_process(config):
    args = [sys.executable, '-B', str(Path(__file__).resolve()), '--config', str(Path(config).resolve())]
    # Windows CRT exec joins argv without quoting paths that contain spaces.
    if os.name == 'nt':
        args = [subprocess.list2cmdline([arg]) for arg in args]
    os.execv(sys.executable, args)


async def serve(config_path, stop=None, channel=None):
    started = time.monotonic()
    generation = uuid.uuid4().hex
    config_path = Path(config_path).resolve()
    config = tomllib.loads(config_path.read_text(encoding='utf-8-sig'))
    certificates = Lifecycle(config, config_path.parent)
    def path(key):
        value = Path(config[key])
        return value if value.is_absolute() else config_path.parent / value
    node = config['node_key']
    if not KEY.fullmatch(node):
        raise ValueError('Invalid node_key')
    address = str(ipaddress.ip_address(config['advertise_address']))
    if ipaddress.ip_address(address).is_unspecified or ipaddress.ip_address(address).is_multicast:
        raise ValueError('Advertise a concrete reachable IP address')
    port, hub_port = config.get('port', 54910), config.get('hub_port', 9100)
    if not 1 <= port <= 65535 or not 1 <= hub_port <= 65535:
        raise ValueError('Invalid port')
    allowed = set(config['allowed_world_nodes'])
    if not allowed or any(not KEY.fullmatch(key) for key in allowed):
        raise ValueError('Configure allowed_world_nodes')
    maps = config['maps']
    if len(maps) != len(set(maps)):
        raise ValueError('Duplicate map ID')
    full_data = config.get('full_data', False)
    if type(full_data) is not bool:
        raise ValueError('full_data must be true or false')
    manifest, dataset, assets = await asyncio.to_thread(catalog, path('data_root'), set(maps), full_data)
    limit = config.get('max_transfers', 4)
    if not 1 <= limit <= 32:
        raise ValueError('max_transfers must be 1..32')
    previous_wall, previous_cpu = time.monotonic(), time.process_time()
    state = {'active': 0, 'requests': 0, 'failures': 0, 'bytes': 0}
    def metrics():
        nonlocal previous_wall, previous_cpu
        wall, cpu = time.monotonic(), time.process_time()
        load = min(10000, max(0, int((cpu-previous_cpu) / max(.001, wall-previous_wall) / (os.cpu_count() or 1) * 10000)))
        previous_wall, previous_cpu = wall, cpu
        values = [int(wall-started), load, memory_mib(), state['requests'], state['failures'], state['bytes']//1024, len(assets), state['active']]
        return bytes([2]) + struct.pack('!8IH', *(min(0xffffffff,value) for value in values),len(maps)) + b''.join(struct.pack('!I',value) for value in sorted(maps)) + wire_string(generation)
    @web.middleware
    async def authorize(request, handler):
        state['requests'] += 1
        try:
            peer = certificate_key(request.transport.get_extra_info('peercert') or {})
        except ValueError:
            state['failures'] += 1
            raise web.HTTPForbidden()
        if peer not in allowed or not certificates.allowed(request.transport.get_extra_info('ssl_object')):
            state['failures'] += 1
            raise web.HTTPForbidden()
        if state['active'] >= limit:
            state['failures'] += 1
            raise web.HTTPServiceUnavailable()
        state['active'] += 1
        try:
            return await handler(request)
        except Exception:
            state['failures'] += 1
            raise
        finally:
            state['active'] -= 1
    async def get_manifest(request):
        return web.Response(body=manifest, content_type='application/json', headers={'X-Map-Dataset':dataset})
    async def get_asset(request):
        if request.match_info['dataset'] != dataset:
            raise web.HTTPNotFound()
        entry = assets.get(request.match_info['asset'])
        if entry is None:
            raise web.HTTPNotFound()
        asset, size, modified = entry
        if asset.is_symlink() or asset.resolve() != asset or asset.stat().st_size != size or asset.stat().st_mtime_ns != modified:
            raise web.HTTPConflict(text='Dataset changed; restart mapserver to publish a new snapshot.')
        # Stream within the concurrency guard instead of scheduling a deferred FileResponse.
        response = web.StreamResponse(headers={'Content-Length': str(size), 'Content-Type':'application/octet-stream'})
        await response.prepare(request)
        with asset.open('rb') as source:
            while chunk := await asyncio.to_thread(source.read, 256 * 1024):
                await asyncio.wait_for(response.write(chunk), 15)
                state['bytes'] += len(chunk)
        await response.write_eof()
        return response
    app = web.Application(middlewares=[authorize], client_max_size=1024)
    app.router.add_get('/v1/manifest', get_manifest)
    app.router.add_get('/v1/assets/{dataset}/{asset:.+}', get_asset)
    inbound = ssl.create_default_context(ssl.Purpose.CLIENT_AUTH, cafile=str(path('ca')))
    inbound.minimum_version = ssl.TLSVersion.TLSv1_2
    inbound.verify_mode = ssl.CERT_REQUIRED
    inbound.load_cert_chain(str(path('certificate')), str(path('private_key')))
    outbound = client_tls(path('ca'), path('certificate'), path('private_key'))
    stop = stop or asyncio.Event()
    supervision = asyncio.create_task(channel.run(stop,lambda: state['active'] == 0)) if channel else None
    restart = False
    runner = web.AppRunner(app, access_log=None, shutdown_timeout=5)
    await runner.setup()
    try:
        await web.TCPSite(runner, config.get('bind_address','127.0.0.1'), port, ssl_context=inbound).start()
        print(f'Mapserver {node}: {len(assets)} assets ready; dataset {dataset}.', flush=True)
        backoff = 1
        while not stop.is_set():
            writer = None
            try:
                inbound.load_cert_chain(path('certificate'), path('private_key'))
                outbound.load_cert_chain(path('certificate'), path('private_key'))
                reader, writer = await asyncio.wait_for(asyncio.open_connection(config['hub_host'], hub_port,
                    ssl=outbound, server_hostname=config['hub_host']), 5)
                payload = wire_string(node) + wire_string(config.get('node_name', node)) + bytes([3]) + wire_string(address)
                payload += struct.pack('!HIIII', port, 0, 18414, limit, 768)
                lease = await exchange(reader, writer, 1, payload)
                await exchange(reader, writer, 2, struct.pack('!BI', 1, state['active']))
                await exchange(reader, writer, 9, metrics())
                await certificates.sync(reader, writer, inbound, outbound)
                if channel:
                    channel.set_ready(True)
                backoff = 1
                while not stop.is_set():
                    try:
                        await asyncio.wait_for(stop.wait(), min(5, max(1, lease // 3)))
                    except asyncio.TimeoutError:
                        await exchange(reader, writer, 3, struct.pack('!I', state['active']))
                        await exchange(reader, writer, 9, metrics())
                        await certificates.sync(reader, writer, inbound, outbound)
                await exchange(reader, writer, 4, b'')
            except RestartRequested:
                print('Hub requested restart; finishing active transfers and reloading the process.', flush=True)
                restart = True
                stop.set()
            except (OSError, RuntimeError, asyncio.TimeoutError, asyncio.IncompleteReadError):
                if not stop.is_set():
                    print('Hub registration unavailable; mapserver will retry.', flush=True)
                    try:
                        await asyncio.wait_for(stop.wait(), backoff)
                    except asyncio.TimeoutError:
                        pass
                    backoff = min(backoff * 2, 30)
            finally:
                if channel:
                    channel.set_ready(False)
                if writer:
                    writer.close()
                    with contextlib.suppress(Exception):
                        await asyncio.wait_for(writer.wait_closed(), 2)
    finally:
        await runner.cleanup()
        if supervision:
            supervision.cancel()
            with contextlib.suppress(asyncio.CancelledError):
                await supervision
    return restart


async def main(config, channel=None):
    stop = asyncio.Event()
    loop = asyncio.get_running_loop()
    for sig in (signal.SIGINT, signal.SIGTERM):
        try:
            loop.add_signal_handler(sig, stop.set)
        except NotImplementedError:
            signal.signal(sig, lambda *_: loop.call_soon_threadsafe(stop.set))
    return await serve(config, stop, channel)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--config', default='mapserver.toml')
    hub_service.arguments(parser)
    args = parser.parse_args()
    channel = hub_service.channel(args,tomllib.loads(Path(args.config).read_text(encoding='utf-8-sig')))
    if asyncio.run(main(args.config,channel)):
        if channel:
            sys.exit(2)  # Parent owns relaunch and fresh supervision handles.
        # Re-exec only this daemon after listeners and transfers have been closed.
        # Preserve absolute paths and invocation flags, independent of working directory.
        restart_process(args.config)
