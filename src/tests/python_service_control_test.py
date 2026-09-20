# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
"""Exercise real inherited handles, including Windows paths and launch authorization."""
import os
from pathlib import Path
import queue
import subprocess
import sys
import tempfile
import threading
import unittest

PLATFORM = Path(__file__).resolve().parents[1]/'server/shared/Platform'
FIXTURE = '''import asyncio,sys
from pathlib import Path
sys.path.insert(0,sys.argv[1])
from hub_service import Channel
channel=Channel(int(sys.argv[2]),int(sys.argv[3]))
channel.set_ready(True)
asyncio.run(channel.run(asyncio.Event(),lambda:Path(sys.argv[4]).exists()))
'''


class ControlTests(unittest.TestCase):
    def run_child(self, token, busy):
        control_read,control_write = os.pipe()
        status_read,status_write = os.pipe()
        os.set_inheritable(control_read,True); os.set_inheritable(status_write,True)
        options = {}
        read_arg,write_arg = control_read,status_write
        if os.name == 'nt':
            import msvcrt
            read_arg,write_arg = msvcrt.get_osfhandle(control_read),msvcrt.get_osfhandle(status_write)
            startup = subprocess.STARTUPINFO()
            startup.lpAttributeList = {'handle_list':[read_arg,write_arg]}
            options['startupinfo'] = startup
            options['creationflags'] = subprocess.CREATE_NO_WINDOW
        else:
            options['pass_fds'] = (control_read,status_write)
        with tempfile.TemporaryDirectory(prefix='character control ') as folder:
            ready = Path(folder)/'allow stop'
            process = subprocess.Popen([sys.executable,'-B','-c',FIXTURE,str(PLATFORM),str(read_arg),str(write_arg),str(ready)],
                                       stdout=subprocess.PIPE,stderr=subprocess.PIPE,**options)
            os.close(control_read); os.close(status_write)
            lines=queue.Queue()
            def collect():
                with os.fdopen(status_read,'rb') as source:
                    for line in source: lines.put(line.strip())
            reader=threading.Thread(target=collect,daemon=True); reader.start()
            try:
                os.write(control_write,token)
                if token != b'SKYFIRE-HUB-SERVICE-1\n':
                    self.assertNotEqual(process.wait(timeout=10),0)
                    self.assertTrue(lines.empty())
                    return
                def wait_for(expected):
                    while lines.get(timeout=10) != expected: pass
                wait_for(b'READY')
                if busy:
                    os.write(control_write,b'STOP\n'); wait_for(b'STOP_REJECTED')
                    self.assertIsNone(process.poll())
                ready.touch()
                os.write(control_write,b'STOP\n'); wait_for(b'STOPPING')
                self.assertEqual(process.wait(timeout=10),0)
            finally:
                os.close(control_write)
                if process.poll() is None: process.kill()
                process.communicate(timeout=10)
                reader.join(timeout=2)

    def test_stop_drains_and_restart_gets_fresh_pipes(self):
        self.run_child(b'SKYFIRE-HUB-SERVICE-1\n',True)
        self.run_child(b'SKYFIRE-HUB-SERVICE-1\n',False)

    def test_wrong_launch_token_cannot_report_ready(self):
        self.run_child(b'NOT-A-HUB\n',False)


if __name__ == '__main__': unittest.main()
