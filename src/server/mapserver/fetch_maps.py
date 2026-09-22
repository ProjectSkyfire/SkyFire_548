# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
"""Startup-only worldserver client: discover through hub, verify and stage an immutable cache."""
import argparse
import contextlib
import errno
import hashlib
import http.client
import ipaddress
import json
import os
from pathlib import Path
import re
import shutil
import socket
import stat
import struct
import time
import sys
from urllib.parse import quote
from map_common import (KEY, MAX_FILE, MAX_FILES, MAX_MANIFEST, MAX_TOTAL, MAX_MAPS, asset_map,
                        certificate_key, client_tls, digest_file, encoded, frame, wire_string, valid_asset_size)
sys.path.append(str(Path(__file__).resolve().parents[1] / 'shared/Platform'))
from cluster_certificates import Lifecycle


def read_world_config(path):
    settings = {}
    for line in path.read_text(encoding='utf-8-sig').splitlines():
        quoted = None
        for index, char in enumerate(line):
            if char in ('"', "'") and (index == 0 or line[index-1] != '\\'):
                if quoted is None: quoted = char
                elif quoted == char: quoted = None
            elif quoted is None and char in '#;':
                line = line[:index]
                break
        match = re.fullmatch(r'\s*([A-Za-z0-9_.]+)\s*=\s*(.*?)\s*', line)
        if match:
            key, value = match.groups()
            if key.startswith(('MapData.', 'Cluster.')):
                if key in settings:
                    raise ValueError('Duplicate map/cluster configuration key')
                if len(value) >= 2 and value[0] in ('"', "'") and value[-1] == value[0]:
                    value = value[1:-1]
                settings[key] = value
    return settings


def receive(sock, length):
    out = bytearray()
    while len(out) < length:
        part = sock.recv(length - len(out))
        if not part:
            raise RuntimeError('Incomplete hub reply')
        out.extend(part)
    return bytes(out)


def discover(host, port, context, node, certificates=None):
    with socket.create_connection((host, port), timeout=5) as plain:
        with context.wrap_socket(plain, server_hostname=host) as sock:
            if certificates and not certificates.allowed(sock):
                raise ValueError('Hub certificate revoked or CRL unavailable')
            sock.sendall(frame(8, bytes([1]) + wire_string(node)))
            magic, version, kind, size = struct.unpack('!4sHHI', receive(sock, 12))
            if magic != b'SFHC' or version != 1 or kind != 0x8003 or not 1 <= size <= 4096:
                raise RuntimeError('Hub does not support map data discovery or denied the request')
            data = receive(sock, size)
    position = 0
    def number(fmt):
        nonlocal position
        size = struct.calcsize(fmt)
        if position + size > len(data):
            raise ValueError('Truncated map route')
        result = struct.unpack_from(fmt, data, position)[0]
        position += size
        return result
    def text():
        nonlocal position
        size = number('!H')
        if not 1 <= size <= 255 or position + size > len(data):
            raise ValueError('Invalid route text')
        value = data[position:position+size].decode()
        position += size
        return value
    protocol, ready = number('!B'), number('!B')
    address, endpoint, identity, ttl = text(), number('!H'), text(), number('!I')
    if protocol != 1 or ready != 1 or identity != node or not 1 <= endpoint <= 65535 or not 0 < ttl <= 3000 or position != len(data):
        raise RuntimeError('Requested mapserver is unavailable, not ready, or administratively drained')
    addr = ipaddress.ip_address(address)
    if addr.is_unspecified or addr.is_multicast:
        raise ValueError('Invalid mapserver endpoint')
    return str(addr), endpoint


@contextlib.contextmanager
def cache_lock(root, timeout=1800):
    lock_path = root / '.lock'
    if lock_path.is_symlink():
        raise ValueError('Cache lock cannot be a link')
    with lock_path.open('a+b') as lock:
        if os.name == 'nt':
            import msvcrt
            if lock_path.stat().st_size == 0:
                lock.write(b'0'); lock.flush()
        else:
            import fcntl
        deadline = time.monotonic() + timeout
        waiting = False
        while True:
            try:
                lock.seek(0)
                if os.name == 'nt':
                    msvcrt.locking(lock.fileno(), msvcrt.LK_NBLCK, 1)
                else:
                    fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
                break
            except OSError as error:
                if error.errno not in (errno.EACCES, errno.EAGAIN, errno.EDEADLK):
                    raise
                if time.monotonic() >= deadline:
                    raise TimeoutError('Timed out waiting for another map cache preparation') from error
                if not waiting:
                    print('Waiting for another map cache preparation to finish...', flush=True)
                    waiting = True
                time.sleep(min(0.25, max(0, deadline - time.monotonic())))
        try:
            yield
        finally:
            lock.seek(0)
            if os.name == 'nt':
                msvcrt.locking(lock.fileno(), msvcrt.LK_UNLCK, 1)
            else:
                fcntl.flock(lock, fcntl.LOCK_UN)


def file_stamp(path):
    info = path.stat()
    if not stat.S_ISREG(info.st_mode):
        raise ValueError('Cache asset must be a regular file')
    return [info.st_dev, info.st_ino, info.st_size, info.st_mtime_ns, info.st_ctime_ns]


def verification_records(path, snapshot):
    try:
        if path.is_symlink() or path.stat().st_size > MAX_MANIFEST:
            return {}
        data = json.loads(path.read_text(encoding='utf-8'))
        if data.get('version') == 1 and data.get('snapshot') == snapshot and isinstance(data.get('files'), dict):
            return data['files']
    except (OSError, ValueError, AttributeError):
        pass
    return {}


def fetch(config_path, receipt=None):
    if receipt is not None and not re.fullmatch(r"receipt-[0-9a-f]{32}", receipt):
        raise ValueError("Invalid bootstrap receipt")
    config_path = Path(config_path).resolve(strict=True)
    config = read_world_config(config_path)
    certificates = Lifecycle({'crl':config.get('Cluster.CRL', ''), 'ca':config.get('Cluster.CA', '')}, config_path.parent)
    def path(key, default=None):
        value = Path(config.get(key, default) or '')
        return Path(os.path.abspath(value if value.is_absolute() else config_path.parent / value))
    if config.get('MapData.Enable') not in ('1', 'true', 'TRUE', 'yes', 'YES'):
        raise ValueError('MapData.Enable is required')
    sources, assigned = {}, set()
    for assignment in config.get('MapData.Sources', '').split():
        node, separator, values = assignment.partition('=')
        if not separator or not KEY.fullmatch(node) or node in sources or not re.fullmatch(r'\d{1,4}(?:,\d{1,4}){0,511}', values):
            raise ValueError('Use MapData.Sources = "maps-east=0 maps-west=1"')
        maps = [int(value) for value in values.split(',')]
        if len(maps) != len(set(maps)) or assigned.intersection(maps):
            raise ValueError('A map can have only one configured data provider')
        assigned.update(maps); sources[node] = maps
    if not sources or len(sources) > 16 or len(assigned) > MAX_MAPS:
        raise ValueError('Configure 1..16 providers and at most 512 maps')
    full_data = config.get('MapData.FullData', '0')
    if full_data not in ('0', '1'):
        raise ValueError('MapData.FullData must be 0 or 1')
    context = client_tls(path('Cluster.CA'), path('Cluster.Certificate'), path('Cluster.PrivateKey'))
    root = path('MapData.CachePath', 'map-cache')
    root.mkdir(parents=True, exist_ok=True, mode=0o700)
    if root.resolve() != root:
        raise ValueError('Cache path must not contain links')
    timeout = int(config.get('MapData.StartupTimeout', '1800'))
    if not 1 <= timeout <= 86400:
        raise ValueError('Invalid map startup timeout')
    verify = config.get('MapData.VerifyCache', '0')
    if verify not in ('0', '1'):
        raise ValueError('MapData.VerifyCache must be 0 or 1')
    with cache_lock(root, timeout):
        connections, datasets, selected, seen, manifests = {}, {}, {}, {}, {}
        def request(node, url):
            connection = connections[node]
            if connection.sock is None:
                connection.connect()
                if certificate_key(connection.sock.getpeercert()) != node:
                    connection.close()
                    raise ValueError('Mapserver TLS identity does not match hub route')
            if not certificates.allowed(connection.sock):
                connection.close()
                raise ValueError('Mapserver certificate revoked or CRL unavailable')
            connection.request('GET', url, headers={'Accept-Encoding':'identity'})
            response = connection.getresponse()
            if response.status != 200 or response.getheader('Content-Encoding'):
                connection.close()
                raise RuntimeError('Mapserver rejected asset request')
            return response
        try:
            for node, maps in sorted(sources.items()):
                host, port = discover(config.get('Cluster.HubHost', 'localhost'), int(config.get('Cluster.HubPort', '9100')), context, node, certificates)
                connections[node] = http.client.HTTPSConnection(host, port, context=context, timeout=30)
                response = request(node, '/v1/manifest')
                payload = response.read(MAX_MANIFEST + 1)
                if len(payload) > MAX_MANIFEST:
                    raise ValueError('Manifest exceeds limit')
                dataset = hashlib.sha256(payload).hexdigest()
                if response.getheader('X-Map-Dataset') != dataset:
                    raise ValueError('Manifest hash mismatch')
                datasets[node] = dataset
                manifest = json.loads(payload)
                if manifest.get('version') not in (1, 2) or manifest.get('build') != 18414 or not set(maps).issubset(manifest.get('maps', [])):
                    raise ValueError('Dataset build/maps do not match the requested maps')
                if full_data == '1' and (manifest['version'] != 2 or set(maps) != set(manifest['maps'])):
                    raise ValueError('Full data requires complete assignments and full-data providers')
                entries = manifest.get('files')
                if not isinstance(entries, list) or not 1 <= len(entries) <= MAX_FILES:
                    raise ValueError('Invalid manifest file count')
                names, total = set(), 0
                for entry in entries:
                    name = entry['path']
                    map_id = asset_map(name)
                    if name.casefold() in names or not valid_asset_size(name, entry['size']) or not re.fullmatch('[0-9a-f]{64}', entry['sha256']):
                        raise ValueError('Invalid/duplicate manifest entry')
                    names.add(name.casefold()); total += entry['size']
                    if total > MAX_TOTAL:
                        raise ValueError('Dataset exceeds size limit')
                    if map_id is None or map_id in maps:
                        old = seen.get(name.casefold())
                        if old and (old['path'], old['size'], old['sha256']) != (name, entry['size'], entry['sha256']):
                            raise ValueError('Map providers publish conflicting shared collision assets')
                        if not old:
                            seen[name.casefold()] = entry
                            selected[name] = dict(entry, provider=node)
                for map_id in maps:
                    if full_data == '1':
                        if not any(asset_map(e['path']) == map_id for e in entries):
                            raise ValueError('Requested map has no extracted assets')
                        continue
                    if not any(e['path'].startswith(f'maps/{map_id:04d}_') for e in entries) or f'vmaps/{map_id:04d}.vmtree'.casefold() not in names or f'mmaps/{map_id:04d}.mmap'.casefold() not in names:
                        raise ValueError('Requested map is incomplete')
                if 'vmaps/gameobjectmodels.dtree' not in names or not any(e['path'].endswith('.vmo') for e in entries):
                    raise ValueError('Missing shared collision models')
                if full_data == '1' and any(not any(e['path'].startswith(folder + '/') for e in entries) for folder in ('dbc','db2','cameras')):
                    raise ValueError('Missing dbc, db2 or cameras assets')
                manifests[node] = {'dataset':dataset, 'maps':sorted(maps)}
            selected = list(selected.values())
            if len(selected) > MAX_FILES or sum(e['size'] for e in selected) > MAX_TOTAL:
                raise ValueError('Combined cache exceeds proof-of-concept limits')
            snapshot = hashlib.sha256(encoded(manifests)).hexdigest()
            destination = root / snapshot
            stage = destination if destination.exists() else root / (snapshot + '.partial')
            if stage.is_symlink() or stage.resolve() != stage:
                raise ValueError('Cache snapshot cannot be a link')
            stage.mkdir(exist_ok=True)
            remaining = 0 if stage == destination else sum(e['size'] for e in selected)
            if shutil.disk_usage(root).free < remaining + 64 * 1024 * 1024:
                raise RuntimeError('Insufficient free space for map cache')
            downloaded = 0
            checked = reused = 0
            record_path = root / (snapshot + '.verified.json')
            records = verification_records(record_path, snapshot) if verify == '0' and stage == destination else {}
            verified = {}
            progress_at = time.monotonic()
            print(f'Preparing map cache: {len(selected)} assets; previously verified metadata available for {len(records)}.', flush=True)
            for entry in selected:
                target = stage / entry['path']
                if target.parent.is_symlink() or target.is_symlink() or target.resolve() != target:
                    raise ValueError('Cache asset cannot be a link')
                target.parent.mkdir(parents=True, exist_ok=True)
                valid = False
                if target.is_file():
                    before = file_stamp(target)
                    if before[2] == entry['size']:
                        if records.get(entry['path']) == [entry['sha256'], before]:
                            valid = True
                            reused += 1
                        else:
                            checked += 1
                            valid = digest_file(target) == entry['sha256']
                            if before != file_stamp(target):
                                raise RuntimeError('Cache asset changed during verification')
                if valid:
                    verified[entry['path']] = [entry['sha256'], before]
                    if time.monotonic() - progress_at >= 5:
                        print(f'Map cache: {len(verified)}/{len(selected)} ready; {reused} reused, {checked} hashed, {downloaded} downloaded.', flush=True)
                        progress_at = time.monotonic()
                    continue
                if stage == destination:
                    raise RuntimeError('Published cache is corrupt; use a new empty cache directory')
                provider = entry['provider']
                response = request(provider, '/v1/assets/' + datasets[provider] + '/' + quote(entry['path'], safe='/'))
                if response.getheader('Content-Length') != str(entry['size']):
                    raise ValueError('Asset length mismatch')
                temporary = target.with_name(target.name + '.part')
                if temporary.is_symlink():
                    raise ValueError('Temporary cache file cannot be a link')
                hashed, count = hashlib.sha256(), 0
                with temporary.open('wb') as output:
                    while chunk := response.read(256 * 1024):
                        count += len(chunk)
                        if count > entry['size']:
                            raise ValueError('Oversized asset response')
                        output.write(chunk); hashed.update(chunk)
                    output.flush(); os.fsync(output.fileno())
                if count != entry['size'] or hashed.hexdigest() != entry['sha256']:
                    raise ValueError('Asset checksum mismatch')
                os.replace(temporary, target)
                downloaded += 1
                verified[entry['path']] = [entry['sha256'], file_stamp(target)]
                if time.monotonic() - progress_at >= 5:
                    print(f'Map cache: {len(verified)}/{len(selected)} ready; {reused} reused, {checked} hashed, {downloaded} downloaded.', flush=True)
                    progress_at = time.monotonic()
            if stage != destination:
                os.replace(stage, destination)
            record_tmp = root / (snapshot + '.verified.tmp')
            if record_path.is_symlink() or record_tmp.is_symlink():
                raise ValueError('Verification record cannot be a link')
            record_tmp.write_text(json.dumps({'version':1, 'snapshot':snapshot, 'files':verified}, separators=(',', ':')), encoding='utf-8')
            os.replace(record_tmp, record_path)
            active = root / 'active.tmp'
            if active.is_symlink() or (root / 'active').is_symlink():
                raise ValueError('Cache marker cannot be a link')
            active.write_text(snapshot + '\n', encoding='ascii')
            os.replace(active, root / 'active')
            if receipt is not None:
                receipt_path = root / receipt
                with receipt_path.open('x', encoding='ascii') as output:
                    output.write(snapshot + '\n')
                    if full_data == '1':
                        output.write('full-data\n')
            print(f'Map data ready: {len(selected)} verified assets, {downloaded} downloaded, {reused} reused without hashing, {checked} hashed, maps {sorted(assigned)} from {len(sources)} providers.', flush=True)
            return destination
        finally:
            for connection in connections.values():
                connection.close()


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--world-config', type=Path, required=True)
    parser.add_argument('--receipt')
    args = parser.parse_args()
    try:
        fetch(args.world_config, args.receipt)
    except Exception as error:
        # Configuration contains paths to keys, never log its contents or credentials.
        print('Map data bootstrap failed: ' + str(error), flush=True)
        raise SystemExit(1)
