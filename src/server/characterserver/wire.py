# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
"""Bounded character database RPC, version 1. Never deserializes executable objects."""
import struct

MAX_FRAME = 64 * 1024 * 1024
MAX_STATEMENTS = 16384


class Reader:
    def __init__(self, data):
        self.data, self.offset = data, 0

    def take(self, count):
        if count < 0 or self.offset + count > len(self.data):
            raise ValueError('Truncated character RPC')
        result = self.data[self.offset:self.offset + count]
        self.offset += count
        return result

    def u8(self):
        return self.take(1)[0]

    def u32(self):
        return struct.unpack('!I', self.take(4))[0]

    def blob(self, limit=MAX_FRAME):
        count = self.u32()
        if count > limit:
            raise ValueError('Character RPC field exceeds limit')
        return self.take(count)

    def text(self, limit=65536):
        return self.blob(limit).decode('utf-8')

    def end(self):
        if self.offset != len(self.data):
            raise ValueError('Trailing character RPC data')


def u32(value):
    return struct.pack('!I', value)


def blob(value):
    value = value.encode() if isinstance(value, str) else value
    return u32(len(value)) + value


def prepared_sql(sql):
    """Translate MySQL question marks outside quoted SQL; preserve literal percent signs."""
    output, quote, escaped = [], None, False
    for char in sql:
        if escaped:
            escaped = False
        elif char == '\\' and quote:
            escaped = True
        elif quote:
            if char == quote:
                quote = None
        elif char in "'\"`":
            quote = char
        elif char == '?':
            output.append('%s')
            continue
        output.append('%%' if char == '%' else char)
    return ''.join(output)
