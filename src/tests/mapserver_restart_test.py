# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
import asyncio
from pathlib import Path
import struct
import sys
import json
import subprocess
import tempfile
import unittest
from unittest.mock import patch
sys.dont_write_bytecode=True
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'server/mapserver'))
import mapserver
from map_common import frame

class RestartTests(unittest.IsolatedAsyncioTestCase):
    async def reply(self,kind,reply,request,lease):
        reader=asyncio.StreamReader();reader.feed_data(frame(reply,struct.pack('!HI',request,lease)));reader.feed_eof()
        class Writer:
            def write(self,data): pass
            async def drain(self): pass
        return await mapserver.exchange(reader,Writer(),kind,b'')
    async def test_restart_only_on_valid_heartbeat_reply(self):
        with self.assertRaises(mapserver.RestartRequested): await self.reply(3,0x8004,3,15)
        for kind,request,lease in [(1,1,15),(3,2,15),(3,3,0)]:
            with self.assertRaises(RuntimeError): await self.reply(kind,0x8004,request,lease)
        self.assertEqual(await self.reply(3,0x8000,3,15),15)
    def test_reexec_preserves_config_and_uses_no_shell(self):
        config=Path('directory with spaces/provider.toml')
        with patch.object(mapserver.os,'execv') as execute:
            mapserver.restart_process(config)
        executable,argv=execute.call_args.args
        self.assertEqual(executable,sys.executable)
        expected=[sys.executable,'-B',str(Path(mapserver.__file__).resolve()),'--config',str(config.resolve())]
        if mapserver.os.name == 'nt': expected=[subprocess.list2cmdline([arg]) for arg in expected]
        self.assertEqual(argv,expected)

    def test_actual_reexec_with_spaced_paths(self):
        with tempfile.TemporaryDirectory(prefix='map restart ') as directory:
            script=Path(directory)/'replacement daemon.py'
            script.write_text('import json, sys\nprint(json.dumps(sys.argv[1:]))\n',encoding='utf-8')
            config=Path(directory)/'provider config.toml'
            source='import sys; sys.path.insert(0,sys.argv[1]); import mapserver; mapserver.__file__=sys.argv[2]; mapserver.restart_process(sys.argv[3])'
            result=subprocess.run([sys.executable,'-B','-c',source,str(Path(mapserver.__file__).parent),str(script),str(config)],capture_output=True,text=True,timeout=20)
            self.assertEqual(result.returncode,0,result.stderr)
            self.assertEqual(json.loads(result.stdout),['--config',str(config)])

if __name__=='__main__': unittest.main()
