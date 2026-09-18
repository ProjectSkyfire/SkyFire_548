#!/usr/bin/env python3
"""Exercise a separately started test hub's cluster listener (no game commands).

Requires Hub.Cluster.Enable=1 and test-only mTLS certificates from a dedicated CA.
The client certificate CN must equal --node. Example:
  python hub_cluster_integration.py --host localhost --port 9100 --ca ca.pem \
      --cert node.pem --key node-key.pem --node synthetic-world
Run after building hubserver. Do not use an identity leased by a real node.
"""
import argparse
import socket
import ssl
import struct
import time

MAGIC = b'SFHC'
REGISTER, READY, HEARTBEAT, DEREGISTER, ACK, ERROR = 1, 2, 3, 4, 0x8000, 0xffff

def string(value):
    data = value.encode('utf-8')
    return struct.pack('!H', len(data)) + data

def registration(node):
    return string(node) + string('Synthetic foundation test') + b'\x02' + string('127.0.0.1') + struct.pack('!HIIII', 8085, 1, 18414, 100, 7)

def frame(kind, body=b'', version=1):
    return struct.pack('!4sHHI', MAGIC, version, kind, len(body)) + body

def receive_exact(peer, count):
    data = b''
    while len(data) < count:
        chunk = peer.recv(count - len(data))
        if not chunk:
            raise EOFError('Connection closed')
        data += chunk
    return data

def response(peer):
    magic, version, kind, length = struct.unpack('!4sHHI', receive_exact(peer, 12))
    assert magic == MAGIC and version == 1 and length <= 4096
    return kind, receive_exact(peer, length)

def ack(peer, request):
    kind, body = response(peer)
    assert kind == ACK and len(body) == 6, (kind, body)
    original, lease = struct.unpack('!HI', body)
    assert original == request and 5 <= lease <= 300
    return lease

def rejected(peer, code):
    kind, body = response(peer)
    assert kind == ERROR and len(body) >= 4
    actual, length = struct.unpack('!HH', body[:4])
    assert actual == code and len(body[4:]) == length, (actual, body)
    # Errors must close the connection, not leave a partially parsed stream.
    try:
        assert peer.recv(1) == b''
    except (ssl.SSLError, ConnectionResetError):
        pass

def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('--host', default='localhost')
    parser.add_argument('--port', type=int, default=9100)
    for name in ['ca', 'cert', 'key', 'node']:
        parser.add_argument('--' + name, required=True)
    args = parser.parse_args()
    context = ssl.create_default_context(cafile=args.ca)
    context.load_cert_chain(args.cert, args.key)
    def connect(ctx=context):
        raw = socket.create_connection((args.host, args.port), timeout=8)
        try:
            peer = ctx.wrap_socket(raw, server_hostname=args.host)
            peer.settimeout(8)
            return peer
        except BaseException:
            raw.close()
            raise
    # Missing client certificate must never reach the registration handler.
    try:
        with connect(ssl.create_default_context(cafile=args.ca)) as peer:
            peer.sendall(frame(REGISTER, registration(args.node)))
            try:
                response(peer)
            except (ssl.SSLError, EOFError, ConnectionResetError):
                pass
            else:
                raise AssertionError('Unauthenticated peer received a protocol response')
    except (ssl.SSLError, ConnectionResetError):
        pass
    for packet, code in [
        (frame(REGISTER, registration(args.node), version=2), 2),
        (frame(REGISTER, registration('unbound-node' if args.node != 'unbound-node' else 'different-node')), 3),
        (frame(HEARTBEAT, struct.pack('!I', 0)), 5),
        (struct.pack('!4sHHI', MAGIC, 1, REGISTER, 4097), 1),
        (frame(REGISTER, registration(args.node)[:-1]), 1),
        (frame(999), 1),
    ]:
        with connect() as peer:
            peer.sendall(packet)
            rejected(peer, code)
    with connect() as peer:
        packet = frame(REGISTER, registration(args.node))
        # Fragment header and body across writes; framing must not assume packet boundaries.
        for offset in range(0, len(packet), 3):
            peer.sendall(packet[offset:offset + 3])
        lease = ack(peer, REGISTER)
        with connect() as duplicate:
            duplicate.sendall(frame(REGISTER, registration(args.node)))
            rejected(duplicate, 4)
        peer.sendall(frame(READY, struct.pack('!BI', 1, 12)))
        ack(peer, READY)
        peer.sendall(frame(HEARTBEAT, struct.pack('!I', 15)))
        ack(peer, HEARTBEAT)
        peer.sendall(frame(READY, struct.pack('!BI', 0, 15)))
        ack(peer, READY)
        peer.sendall(frame(DEREGISTER))
        ack(peer, DEREGISTER)
    with connect() as peer:
        peer.sendall(frame(REGISTER, registration(args.node)))
        lease = ack(peer, REGISTER)
        time.sleep(lease + 1)
        # A replacement must register successfully after expiry even if old socket stayed open.
        with connect() as replacement:
            replacement.sendall(frame(REGISTER, registration(args.node)))
            ack(replacement, REGISTER)
            replacement.sendall(frame(DEREGISTER))
            ack(replacement, DEREGISTER)
    with connect() as slow:
        slow.sendall(frame(REGISTER, registration(args.node))[:6])
        try:
            assert slow.recv(1) == b''
        except (ssl.SSLError, ConnectionResetError):
            pass
    print('Cluster integration passed: mutual TLS, identity/version checks, bounds, fragmented frames, readiness, heartbeat, duplicate identity, deregistration, expiry and slow sender timeout.')

if __name__ == '__main__':
    main()
