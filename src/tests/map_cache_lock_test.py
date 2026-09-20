# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
"""Regression: world startup waits for concurrent map cache prewarming."""
from pathlib import Path
import subprocess
import sys
import tempfile
import time
import unittest
sys.dont_write_bytecode = True
MODULE = Path(__file__).resolve().parents[1] / 'server/mapserver'
sys.path.insert(0, str(MODULE))
from fetch_maps import cache_lock

class CacheLockTests(unittest.TestCase):
    def child(self, root, timeout):
        script = ('import sys; from pathlib import Path; sys.path.insert(0,sys.argv[1]); '
                  'from fetch_maps import cache_lock; '
                  'lock=cache_lock(Path(sys.argv[2]),float(sys.argv[3])); '
                  'lock.__enter__(); print("acquired",flush=True); lock.__exit__(None,None,None)')
        return subprocess.Popen([sys.executable,'-B','-c',script,str(MODULE),str(root),str(timeout)],
            stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True,
            creationflags=getattr(subprocess,'CREATE_NO_WINDOW',0))

    def test_wait_then_acquire(self):
        with tempfile.TemporaryDirectory() as directory:
            with cache_lock(Path(directory)):
                child=self.child(directory,5)
                try:
                    self.assertIn('Waiting',child.stdout.readline())
                    time.sleep(.3)
                    self.assertIsNone(child.poll())
                except BaseException:
                    child.kill();child.communicate();raise
            out,err=child.communicate(timeout=10)
            self.assertEqual(child.returncode,0,err)
            self.assertIn('acquired',out)

    def test_bounded_timeout(self):
        with tempfile.TemporaryDirectory() as directory:
            with cache_lock(Path(directory)):
                child=self.child(directory,.3)
                out,err=child.communicate(timeout=10)
                self.assertNotEqual(child.returncode,0)
                self.assertIn('Timed out waiting',err)
            with cache_lock(Path(directory),.3): pass

if __name__ == '__main__': unittest.main()
