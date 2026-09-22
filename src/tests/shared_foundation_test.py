# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
"""Extraction parity against bf9747e2fa; update baselines only for deliberate ABI changes."""
import hashlib
from pathlib import Path
import re
import unittest

ROOT = Path(__file__).resolve().parents[2]
SHARED = ROOT / 'src/server/shared'

def semantic(text):
    text = re.sub(r'^\s*#include[^\n]*', '', text, flags=re.M)
    text = re.sub(r'"(?:\\.|[^"\\])*"|/\*.*?\*/|//[^\n]*', lambda m: m[0] if m[0].startswith('"') else '', text, flags=re.S)
    return re.sub(r'\s+', '', text)

def digest(text):
    return hashlib.sha256(text.encode()).hexdigest()

BASELINE = {'files': {'DataStores/DBCEnums.h': 'a0a7c77350388f0e0c8705a7cd85b2132151d361131b34d361e397580c4666df',
           'DataStores/DBCStructure.h': 'e9e4be5a5c07dbe7cbd4e64d384e0bbc3815910b00ff14daa3f40c60d0e11a7d',
           'DataStores/DBCfmt.h': '14917510f964842e2c706df4e8f0a0746baf42ef72f86e1e601e07de64401d0d',
           'DataStores/DB2Enums.h': '8a66eec76bcd43fdb159d973bb510884cf9111783aa68cb8d7657a8c7a970596',
           'DataStores/DB2Structure.h': '60878eeb57566242b986d927482ab105edf83c178765184aa2e83db28f30c5a1',
           'DataStores/DB2fmt.h': '845a6e60af3464c47d9d33787496f64c3b609b1e7a0e2d8154c7f33aef82cf0e',
           'Packets/WorldPacket.h': '356b85712a92a33ec65bc1581d5294559703683c3b17fb113d44074d0017cc56',
           'Packets/WorldPacket.cpp': '1ebae2d26a248fe58e5e97b26695c1f80ec6e7d2397962899ff70f8091279ae6',
           'Utilities/Path.h': '390d0e2b9a6f0c85e4572ab83fbfea227f514c58ded321572ff2e86de61cd1d4'},
 'enum': '872e7f7377016c2e0b2710444dcf79af9049eb510132a3d03528429b9e8524fe'}

class SharedFoundationTests(unittest.TestCase):
    def test_schemas_formats_packet_implementation_and_path_preserved(self):
        for name, expected in BASELINE['files'].items():
            with self.subTest(name=name):
                self.assertEqual(digest(semantic((SHARED/name).read_text())), expected)

    def test_symbolic_opcode_enum_preserved(self):
        text = (SHARED/'Protocol/Opcodes.h').read_text()
        enum = text[text.index('enum Opcodes'):text.index('// Wire numbers')]
        self.assertEqual(digest(semantic(enum)), BASELINE['enum'])

    def test_all_wire_mappings_and_world_dispatch_policies_preserved(self):
        maps = {}
        for direction in ('Client','Server'):
            entries = re.findall(r'^SKYFIRE_OPCODE\((\w+), (0x[0-9a-fA-F]+)\)', (SHARED/('Protocol/'+direction+'OpcodeCatalog.inc')).read_text(), re.M)
            self.assertEqual(len(entries),len(dict(entries)))
            maps[direction] = dict(entries)
        source = (ROOT/'src/server/game/Server/Protocol/Opcodes.cpp').read_text()
        records = []
        for name, tail in re.findall(r'^    DEFINE_OPCODE_HANDLER\((\w+), ([^\n]*?)\s*\);', source, re.M):
            direction = 'Client' if ',' in tail else 'Server'
            records.append('|'.join((direction,name,maps[direction].pop(name),','.join(x.strip() for x in tail.split(',')))))
        self.assertEqual(len(records),1062)
        self.assertFalse(maps['Client'] or maps['Server'])
        self.assertEqual(digest('\n'.join(records)), '0812c83233c90c70c10ad212990fe319004505223ad83852002b70606185317f')
        self.assertIn('GetOpcodeMetadata(opcode, false).OpcodeNumber',source)
        self.assertIn('GetOpcodeMetadata(opcode, true).OpcodeNumber',source)

    def test_shared_headers_do_not_reintroduce_game_dependencies(self):
        for name in (*BASELINE['files'], 'Protocol/Opcodes.h', 'DataStores/ItemDataConstants.h'):
            for include in re.findall(r'^#include "([^"]+)"', (SHARED/name).read_text(),re.M):
                self.assertNotIn('game/',include)
                self.assertNotIn(include,('World.h','WorldSession.h','ItemPrototype.h','SpellMgr.h','TransportMgr.h'))
        self.assertFalse((ROOT/'src/server/game/Server/WorldPacket.cpp').exists())

if __name__ == '__main__':
    unittest.main()
