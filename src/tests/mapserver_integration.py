# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
"""Actual mTLS mapserver/download fixture; mock only hub discovery/lifecycle.
Run: python src/tests/mapserver_integration.py (aiohttp and openssl required).
"""
import asyncio
import contextlib
import json
import os
from pathlib import Path
import socket
import ssl
import struct
import subprocess
import sys
sys.dont_write_bytecode = True
import tempfile
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'server' / 'mapserver'))
from mapserver import serve
from fetch_maps import fetch
from map_common import frame, wire_string


def openssl(root, *args):
    subprocess.run(['openssl', *args], cwd=root, check=True, stdout=subprocess.DEVNULL,
                   stderr=subprocess.PIPE, env=dict(os.environ, OPENSSL_CONF=str(root/'openssl.cnf')), creationflags=getattr(subprocess, 'CREATE_NO_WINDOW', 0))


def port():
    with socket.socket() as sock:
        sock.bind(('127.0.0.1', 0))
        return sock.getsockname()[1]


async def exercise(root):
    (root/'openssl.cnf').write_text('[req]\ndistinguished_name=dn\n[dn]\n')
    openssl(root, 'req', '-x509', '-newkey', 'rsa:2048', '-nodes', '-keyout', 'ca.key', '-out', 'ca.crt', '-days', '1', '-subj', '/CN=test-ca')
    (root/'extensions').write_text('subjectAltName=DNS:localhost,IP:127.0.0.1\nextendedKeyUsage=serverAuth,clientAuth\n')
    for key in ('hub', 'world', 'east', 'west', 'outsider'):
        openssl(root, 'req', '-newkey', 'rsa:2048', '-nodes', '-keyout', key+'.key', '-out', key+'.csr', '-subj', '/CN='+key)
        openssl(root, 'x509', '-req', '-in', key+'.csr', '-CA', 'ca.crt', '-CAkey', 'ca.key', '-CAcreateserial', '-out', key+'.crt', '-days', '1', '-extfile', 'extensions')
    routes, ready = {}, set()
    hub_port = port()
    async def hub(reader, writer):
        registered = None
        try:
            while True:
                header = await reader.readexactly(12)
                magic, version, kind, size = struct.unpack('!4sHHI', header)
                assert magic == b'SFHC' and version == 1 and size <= 4096
                body = await reader.readexactly(size)
                if kind == 8:
                    key = body[3:].decode()
                    online = key in ready
                    response = bytes([1, int(online)]) + wire_string('127.0.0.1' if online else '-')
                    response += struct.pack('!H', routes[key] if online else 0) + wire_string(key if online else '-') + struct.pack('!I', 3000 if online else 0)
                    writer.write(frame(0x8003, response)); await writer.drain(); break
                if kind == 1:
                    length = struct.unpack('!H', body[:2])[0]; registered = body[2:2+length].decode()
                if kind == 2: ready.add(registered)
                writer.write(frame(0x8000, struct.pack('!HI', kind, 15))); await writer.drain()
                if kind == 4: break
        except (asyncio.IncompleteReadError, ConnectionError):
            pass
        finally:
            if registered: ready.discard(registered)
            writer.close()
    context = ssl.create_default_context(ssl.Purpose.CLIENT_AUTH, cafile=str(root/'ca.crt'))
    context.verify_mode = ssl.CERT_REQUIRED
    context.load_cert_chain(root/'hub.crt', root/'hub.key')
    server = await asyncio.start_server(hub, '127.0.0.1', hub_port, ssl=context)
    tasks, stops = [], []
    try:
        for node, map_id in [('east',0), ('west',1)]:
            data = root/node
            for folder in ('maps','vmaps','mmaps'): (data/folder).mkdir(parents=True)
            for name in (f'maps/{map_id:04d}_00_00.map', f'vmaps/{map_id:04d}.vmtree', f'vmaps/{map_id:04d}_00_00.vmtile', f'mmaps/{map_id:04d}.mmap', f'mmaps/{map_id:04d}_00_00.mmtile', 'vmaps/shared.vmo', 'vmaps/GameObjectModels.dtree'):
                (data/name).write_bytes(('fixture:'+name).encode())
            routes[node] = port()
            settings = dict(node_key=node, node_name=node, maps=[map_id], data_root=node,
                advertise_address='127.0.0.1', port=routes[node], hub_host='localhost', hub_port=hub_port,
                ca='ca.crt', certificate=node+'.crt', private_key=node+'.key', allowed_world_nodes=['world'])
            config = root/(node+'.toml')
            config.write_text('\n'.join(key+' = '+json.dumps(value) for key,value in settings.items()))
            stop = asyncio.Event(); stops.append(stop); tasks.append(asyncio.create_task(serve(config, stop)))
        for _ in range(100):
            if len(ready) == 2: break
            for task in tasks:
                if task.done(): task.result()
            await asyncio.sleep(.05)
        assert len(ready) == 2, 'providers never registered'
        config = root/'world.conf'
        text = f'MapData.Enable = 1\nMapData.Sources = "east=0 west=1"\nCluster.HubHost = "localhost"\nCluster.HubPort = {hub_port}\nCluster.CA = "ca.crt"\nCluster.Certificate = "world.crt"\nCluster.PrivateKey = "world.key"\n'
        config.write_text(text)
        snapshot = await asyncio.to_thread(fetch, config, 'receipt-' + '0'*32)
        assert (root/('map-cache/receipt-'+'0'*32)).read_text().strip() == snapshot.name
        assert (snapshot/'maps/0000_00_00.map').is_file() and (snapshot/'maps/0001_00_00.map').is_file()
        assert snapshot == await asyncio.to_thread(fetch, config), 'cache reuse failed'
        original = (root/'map-cache/active').read_bytes()
        async def rejected(settings, expected):
            config.write_text(settings)
            try: await asyncio.to_thread(fetch, config)
            except Exception as error:
                assert expected in str(error), str(error)
            else: raise AssertionError('Expected rejection: '+expected)
            assert (root/'map-cache/active').read_bytes() == original, 'failed bootstrap changed active snapshot'
        await rejected(text.replace('west=1','west=0'), 'only one')
        ready.remove('west')
        await rejected(text, 'unavailable')
        ready.add('west')
        await rejected(text.replace('world.crt','outsider.crt').replace('world.key','outsider.key'), 'rejected')
        (snapshot/'maps/0000_00_00.map').write_bytes(b'corrupt')
        await rejected(text, 'cache is corrupt')
        print('PASS: two providers, TLS authentication, hub lifecycle/discovery, verified downloads, cache reuse, duplicate assignment, unavailable provider, denied identity, corrupt cache, atomic marker preservation.')
    finally:
        for stop in stops: stop.set()
        await asyncio.gather(*tasks, return_exceptions=True)
        server.close(); await server.wait_closed()


if __name__ == '__main__':
    with tempfile.TemporaryDirectory(prefix='skyfire-map-test-') as directory:
        asyncio.run(exercise(Path(directory).resolve()))
