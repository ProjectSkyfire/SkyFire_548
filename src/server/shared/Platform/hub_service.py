# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
"""Inherited-pipe supervision for the bundled Python server daemons."""
import asyncio
import os
import queue
import threading
import time
from pathlib import Path
import re
import sys


class Channel:
    def __init__(self, read_handle, write_handle):
        self.commands = queue.Queue(maxsize=16)
        self.authenticated = threading.Event()
        self.closed = False
        self.ready = False
        self.reader, self.writer = read_handle, write_handle
        if os.name == 'nt':
            import msvcrt
            self.reader = msvcrt.open_osfhandle(read_handle, os.O_RDONLY | os.O_BINARY)
            self.writer = msvcrt.open_osfhandle(write_handle, os.O_WRONLY | os.O_BINARY)
        threading.Thread(target=self.read, daemon=True).start()
        if not self.authenticated.wait(5) or self.closed:
            raise RuntimeError('Hub launch authorization failed')
        self.send('STARTING')

    def read(self):
        try:
            with os.fdopen(self.reader, 'rb', buffering=0) as source:
                if source.readline(64) != b'SKYFIRE-HUB-SERVICE-1\n':
                    return
                self.authenticated.set()
                while True:
                    line = source.readline(128)
                    if not line:
                        return
                    if line != b'STOP\n':
                        raise ValueError('Unsupported hub control command')
                    self.commands.put_nowait('STOP')
        except (OSError, ValueError, queue.Full):
            pass
        finally:
            self.closed = True
            self.authenticated.set()

    def send(self, message):
        if self.closed:
            return
        try:
            data = (message+'\n').encode('ascii')
            while data:
                data = data[os.write(self.writer, data):]
        except OSError:
            self.closed = True

    def set_ready(self, ready):
        if self.ready != ready:
            self.ready = ready
            self.send('READY' if ready else 'NOT_READY')

    async def run(self, stop, can_stop):
        heartbeat = 0.0
        while not stop.is_set():
            # On unexpected hub loss, leave persistence online for worlds already
            # saving. Never turn a broken parent pipe into a forced database stop.
            if self.closed:
                print('Hub supervision pipe closed; daemon remains online until stopped explicitly.', flush=True)
                return
            try:
                command = self.commands.get_nowait()
            except queue.Empty:
                command = None
            if command == 'STOP':
                if can_stop():
                    self.send('STOPPING')
                    stop.set()
                    return
                self.send('STOP_REJECTED')
            if time.monotonic() >= heartbeat:
                self.send('HEARTBEAT')
                heartbeat = time.monotonic()+5
            try:
                await asyncio.wait_for(stop.wait(), .2)
            except asyncio.TimeoutError:
                pass


def arguments(parser):
    parser.add_argument('--hub-control-read', type=int)
    parser.add_argument('--hub-status-write', type=int)
    parser.add_argument('--hub-node-key')


def channel(args, config):
    supplied = [args.hub_control_read is not None, args.hub_status_write is not None, args.hub_node_key is not None]
    if not any(supplied):
        return None
    if not all(supplied) or config['node_key'] != args.hub_node_key or not re.fullmatch(r'[A-Za-z0-9_.-]{1,64}',args.hub_node_key):
        raise ValueError('Hub identity must match the daemon configuration')
    # Managed daemons have no visible console on Windows. Keep diagnostics beside
    # their configured script without putting SQL, tokens or credentials in status.
    log = Path(sys.argv[0]).resolve().parent / ('hub-'+args.hub_node_key+'.log')
    sys.stdout = sys.stderr = log.open('a',encoding='utf-8',buffering=1)
    return Channel(args.hub_control_read, args.hub_status_write)
