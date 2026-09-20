# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
"""Read-only map asset catalog and bounded cluster framing."""
import hashlib
import json
from pathlib import Path
import re
import ssl
import struct

MAX_FILES = 100000
MAX_FILE = 64 * 1024 * 1024
MAX_MANIFEST = 32 * 1024 * 1024
MAX_TOTAL = 16 * 1024 * 1024 * 1024
MAX_MAPS = 512
KEY = re.compile(r'[A-Za-z0-9_.-]{1,64}\Z')
TILE = re.compile(r'(maps|vmaps|mmaps)/(\d{4})_(\d{2})_(\d{2})\.(map|vmtile|mmtile)\Z')
ROOT_FILE = re.compile(r'(vmaps|mmaps)/(\d{4})\.(vmtree|mmap)\Z')
MODEL = re.compile(r'vmaps/[A-Za-z0-9_. ()-]{1,220}\.vmo\Z')
CLIENT_DATA = re.compile(r'(dbc|db2|cameras)/(?:[a-z]{2}[A-Z]{2}/)?[A-Za-z0-9_. ()-]{1,220}\.(dbc|db2|m2)\Z')


def asset_map(name):
    if not isinstance(name, str) or '..' in name or '\\' in name:
        raise ValueError('Invalid asset name')
    match = TILE.fullmatch(name)
    if match and (match[1], match[5]) in {('maps','map'),('vmaps','vmtile'),('mmaps','mmtile')} and int(match[3]) < 64 and int(match[4]) < 64:
        return int(match[2])
    match = ROOT_FILE.fullmatch(name)
    if match and (match[1], match[3]) in {('vmaps','vmtree'),('mmaps','mmap')}:
        return int(match[2])
    if MODEL.fullmatch(name) or name == 'vmaps/GameObjectModels.dtree':
        return None
    match = CLIENT_DATA.fullmatch(name)
    if match and (match[1], match[2]) in {('dbc','dbc'),('db2','db2'),('cameras','m2')}:
        return None
    raise ValueError('Unsupported asset name')


def digest_file(path):
    hashed = hashlib.sha256()
    with path.open('rb') as source:
        for chunk in iter(lambda: source.read(1024 * 1024), b''):
            hashed.update(chunk)
    return hashed.hexdigest()


def valid_asset_size(name, size):
    # Extractors leave empty placeholders for some unused client tables.
    minimum = 0 if name.startswith(('dbc/', 'db2/')) else 1
    return type(size) is int and minimum <= size <= MAX_FILE


def encoded(value):
    return json.dumps(value, sort_keys=True, separators=(',', ':')).encode()


def certificate_key(certificate):
    names = [value for group in certificate.get('subject', ()) for key, value in group if key == 'commonName']
    if len(names) != 1 or not KEY.fullmatch(names[0]):
        raise ValueError('Certificate requires one node-key common name')
    return names[0]


def client_tls(ca, certificate, key):
    context = ssl.create_default_context(cafile=str(ca))
    context.minimum_version = ssl.TLSVersion.TLSv1_2
    context.load_cert_chain(str(certificate), str(key))
    return context


def wire_string(value):
    data = value.encode()
    if not 1 <= len(data) <= 255:
        raise ValueError('Invalid protocol string')
    return struct.pack('!H', len(data)) + data


def frame(kind, body):
    if len(body) > 4096:
        raise ValueError('Frame too large')
    return b'SFHC' + struct.pack('!HHI', 1, kind, len(body)) + body


def catalog(data_root, maps, full_data=False):
    root = Path(data_root).resolve(strict=True)
    if not maps or len(maps) > MAX_MAPS or any(type(m) is not int or not 0 <= m <= 9999 for m in maps):
        raise ValueError('Configure 1..512 unique map IDs in 0..9999')
    files, paths, folded, total = [], {}, set(), 0
    for folder in ('maps', 'vmaps', 'mmaps') + (('dbc', 'db2', 'cameras') if full_data else ()):
        directory = root / folder
        if directory.is_symlink() or not directory.is_dir() or directory.resolve() != directory:
            raise ValueError('Asset directories must be real directories under data_root')
        for path in sorted(directory.rglob('*')):
            if path.is_symlink() or path.resolve() != path:
                raise ValueError('Asset links are forbidden')
            if path.is_dir():
                continue
            name = path.relative_to(root).as_posix()
            try:
                map_id = asset_map(name)
            except ValueError:
                if path.suffix.lower() == '.vmo':
                    raise ValueError('Unsupported collision model filename: ' + path.name)
                continue
            if map_id is not None and map_id not in maps:
                continue
            if path.is_symlink() or not path.is_file() or path.resolve() != path:
                raise ValueError('Asset links and non-files are forbidden')
            stat = path.stat()
            if not valid_asset_size(name, stat.st_size) or name.casefold() in folded:
                raise ValueError('Asset is empty, oversized or case-ambiguous')
            folded.add(name.casefold())
            total += stat.st_size
            if total > MAX_TOTAL or len(files) >= MAX_FILES:
                raise ValueError('Dataset exceeds proof-of-concept limits')
            files.append({'path': name, 'size': stat.st_size, 'sha256': digest_file(path)})
            paths[name] = (path, stat.st_size, stat.st_mtime_ns)
    for map_id in maps:
        prefix = f'{map_id:04d}'
        if full_data:
            if not any(asset_map(f['path']) == map_id for f in files):
                raise ValueError('Selected map has no extracted assets')
            continue
        if not any(f['path'].startswith('maps/' + prefix + '_') for f in files):
            raise ValueError('Selected map has no terrain tiles')
        if 'vmaps/' + prefix + '.vmtree' not in paths or 'mmaps/' + prefix + '.mmap' not in paths:
            raise ValueError('Selected map needs its vmtree and mmap roots')
    if not any(name.endswith('.vmo') for name in paths) or 'vmaps/GameObjectModels.dtree' not in paths:
        raise ValueError('Collision model files and GameObjectModels.dtree are required')
    if full_data and any(not any(f['path'].startswith(folder + '/') for f in files) for folder in ('dbc','db2','cameras')):
        raise ValueError('Full data requires dbc, db2 and cameras assets')
    document = {'version': 2 if full_data else 1, 'build': 18414, 'maps': sorted(maps), 'files': files}
    payload = encoded(document)
    if len(payload) > MAX_MANIFEST:
        raise ValueError('Manifest too large')
    return payload, hashlib.sha256(payload).hexdigest(), paths
