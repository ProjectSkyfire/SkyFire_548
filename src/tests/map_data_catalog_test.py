# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
from pathlib import Path
import json
import sys
import tempfile
import unittest
sys.dont_write_bytecode = True
sys.path.insert(0, str(Path(__file__).resolve().parents[1]/'server/mapserver'))
from map_common import asset_map, catalog

class FullDataTests(unittest.TestCase):
    def test_paths(self):
        for name in ('dbc/Map.dbc','dbc/enUS/Map.dbc','db2/Item.db2','cameras/FlyBy.m2'):
            self.assertIsNone(asset_map(name))
        for name in ('cameras/../secret.m2','dbc/Map.db2','db2/Item.exe','cameras/private.key','dbc/a/b/Map.dbc','dbc\\Map.dbc'):
            with self.assertRaises(ValueError): asset_map(name)

    def test_full_data_sparse_maps_and_missing_families(self):
        with tempfile.TemporaryDirectory() as temp:
            root=Path(temp)
            for folder in ('maps','vmaps','mmaps','dbc','db2','cameras'): (root/folder).mkdir()
            for name in ('maps/0000_00_00.map','vmaps/0001.vmtree','vmaps/shared.vmo','vmaps/GameObjectModels.dtree','dbc/Map.dbc','db2/Item.db2','cameras/FlyBy.m2'):
                (root/name).write_bytes(b'fixture')
            manifest,_,_=catalog(root,{0,1},True)
            self.assertEqual(json.loads(manifest)['version'],2)
            self.assertEqual(json.loads(manifest)['maps'],[0,1])
            with self.assertRaisesRegex(ValueError,'no extracted assets'):catalog(root,{2},True)
            (root/'cameras/FlyBy.m2').unlink()
            with self.assertRaisesRegex(ValueError,'requires dbc, db2 and cameras'):catalog(root,{0,1},True)

if __name__=='__main__':unittest.main()
