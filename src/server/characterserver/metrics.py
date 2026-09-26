# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
import os
from pathlib import Path
import struct
import time


def memory_mib():
    if os.name == 'nt':
        import ctypes
        from ctypes import wintypes
        class Counters(ctypes.Structure):
            _fields_ = [('cb',wintypes.DWORD),('faults',wintypes.DWORD)] + [(name,ctypes.c_size_t) for name in ('peak','working','poolpeak','pool','nonpeak','non','page','pagepeak')]
        info = Counters(); info.cb = ctypes.sizeof(info)
        kernel = ctypes.WinDLL('kernel32',use_last_error=True)
        kernel.GetCurrentProcess.restype = wintypes.HANDLE
        psapi = ctypes.WinDLL('psapi',use_last_error=True)
        psapi.GetProcessMemoryInfo.argtypes = [wintypes.HANDLE,ctypes.POINTER(Counters),wintypes.DWORD]
        return info.working//1048576 if psapi.GetProcessMemoryInfo(kernel.GetCurrentProcess(),ctypes.byref(info),info.cb) else 0
    try:
        return int(Path('/proc/self/statm').read_text().split()[1])*os.sysconf('SC_PAGE_SIZE')//1048576
    except (OSError,ValueError,IndexError):
        return 0


class Metrics:
    def __init__(self):
        self.started = self.wall = time.monotonic()
        self.cpu = time.process_time()
        self.requests = self.failures = self.reads = self.writes = self.transactions = self.pending = 0
        self.latency_us = 0
        self.last_commit = None

    def begin(self):
        self.pending += 1
        return time.monotonic()

    def finish(self, started, operation, success):
        self.pending -= 1
        self.requests += 1
        self.latency_us += int((time.monotonic()-started)*1000000)
        if not success:
            self.failures += 1
        elif operation in (2, 4, 17, 19):
            self.reads += 1
        else:
            self.writes += 1
            self.transactions += operation in (3, 5, 18)
            self.last_commit = time.monotonic()

    def packet(self, connections, healthy):
        wall,cpu = time.monotonic(),time.process_time()
        usage = int(10000*(cpu-self.cpu)/max(.001,wall-self.wall)/(os.cpu_count() or 1))
        self.wall,self.cpu = wall,cpu
        values = [int(wall-self.started),min(10000,max(0,usage)),memory_mib(),self.requests,self.failures,
                  self.reads,self.writes,self.transactions,connections,self.pending,
                  self.latency_us//max(1,self.requests),0xffffffff if self.last_commit is None else int(wall-self.last_commit),int(healthy)]
        return b'\1'+struct.pack('!13I',*(min(0xffffffff,max(0,value)) for value in values))
