"""Isolated hub handoff integration checks. Requires built hub/client test and a hub DB config.
The copied config uses temporary TLS identities and ports; no managed service is started.
"""
import argparse
import concurrent.futures
import os
import queue
from pathlib import Path
import re
import socket
import ssl
import struct
import subprocess
import tempfile
import threading
import time


def string(value):
    value = value.encode()
    return struct.pack('!H', len(value)) + value


def read_exact(sock, count):
    result = b''
    while len(result) < count:
        part = sock.recv(count - len(result))
        if not part:
            raise RuntimeError('Unexpected cluster connection closure')
        result += part
    return result


def exchange(sock, message, body):
    sock.sendall(b'SFHC' + struct.pack('!HHI', 1, message, len(body)) + body)
    header = read_exact(sock, 12)
    assert header[:4] == b'SFHC'
    version, kind, size = struct.unpack('!HHI', header[4:])
    assert version == 1 and size <= 4096
    return kind, read_exact(sock, size)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--hub', required=True)
    parser.add_argument('--hub-config', required=True)
    parser.add_argument('--client-probe', required=True)
    parser.add_argument('--openssl', default='openssl')
    parser.add_argument('--agent-probe')
    parser.add_argument('--handoff-store', choices=('memory','database'), default='memory')
    args = parser.parse_args()
    hidden = subprocess.CREATE_NO_WINDOW if os.name == 'nt' else 0
    with tempfile.TemporaryDirectory(prefix='skyfire-handoff-') as temp:
        root = Path(temp)
        (root/'openssl.cnf').write_text('[req]\ndistinguished_name=dn\n[dn]\n')
        openssl_env = os.environ.copy()
        openssl_env['OPENSSL_CONF'] = str(root/'openssl.cnf')
        def openssl(*command):
            result = subprocess.run([args.openssl, *command], cwd=root, capture_output=True, creationflags=hidden,env=openssl_env)
            if result.returncode:
                raise RuntimeError('Test certificate generation failed: '+result.stderr.decode(errors='replace'))
        openssl('req','-x509','-newkey','rsa:2048','-nodes','-keyout','ca.key','-out','ca.pem',
                '-days','1','-subj','/CN=Handoff test CA','-addext','basicConstraints=critical,CA:TRUE',
                '-addext','keyUsage=critical,keyCertSign,cRLSign','-addext','subjectKeyIdentifier=hash')
        for name in ('hub','auth-a','auth-b','world','unknown','auth-dir','world-b','auth-agent'):
            openssl('req','-newkey','rsa:2048','-nodes','-keyout',name+'.key','-out',name+'.csr','-subj','/CN='+name)
            (root/'extensions.txt').write_text('basicConstraints=critical,CA:FALSE\nkeyUsage=critical,digitalSignature,keyEncipherment\nsubjectKeyIdentifier=hash\nauthorityKeyIdentifier=keyid,issuer\nsubjectAltName=DNS:localhost\nextendedKeyUsage='+
                                             ('serverAuth' if name == 'hub' else 'clientAuth')+'\n')
            openssl('x509','-req','-in',name+'.csr','-CA','ca.pem','-CAkey','ca.key','-CAcreateserial',
                    '-out',name+'.pem','-days','1','-extfile','extensions.txt')
        with socket.socket() as free:
            free.bind(('127.0.0.1',0))
            port = free.getsockname()[1]
        config = Path(args.hub_config).read_text(encoding='utf-8-sig')
        values = {'Hub.Handoff.Store':'"'+args.handoff_store+'"','HubDatabase.SqlPath':'""','Hub.Port':str(port),'Hub.BindIP':'"127.0.0.1"','Hub.Cluster.Enable':'1',
                  'Hub.Cluster.Certificate':'"'+str(root/'hub.pem')+'"',
                  'Hub.Cluster.PrivateKey':'"'+str(root/'hub.key')+'"',
                  'Hub.Cluster.CA':'"'+str(root/'ca.pem')+'"',
                  'Hub.AuthnetIngress.Enable':'0','Hub.LegacyIngress.Enable':'0','Hub.Cluster.LeaseSeconds':'5',
                  'Hub.PacketLog.Enable':'0','PacketLogServerControlFile':'""','Web.Enable':'0',
                  'Console.Enable':'1','LogsDir':'"'+str(root)+'"'}
        for key, value in values.items():
            expression = r'(?m)^[ \t]*'+re.escape(key)+r'[ \t]*=.*$'
            if re.search(expression,config):
                config = re.sub(expression,lambda _:key+' = '+value,config)
            else:
                config += '\n'+key+' = '+value+'\n'
        path = root/'hub.conf'
        path.write_text(config,encoding='utf-8')
        log = open(root/'hub-output.log','w',encoding='utf-8')
        process = subprocess.Popen([str(Path(args.hub).resolve()),'-c',str(path)],cwd=root,
                                   stdin=subprocess.PIPE,stdout=log,stderr=subprocess.STDOUT,
                                   text=True,creationflags=hidden)
        registrations = []
        stopping = threading.Event()
        def connect(name):
            context = ssl.create_default_context(cafile=str(root/'ca.pem'))
            context.load_cert_chain(str(root/(name+'.pem')),str(root/(name+'.key')))
            return context.wrap_socket(socket.create_connection(('127.0.0.1',port),timeout=3),server_hostname='localhost')
        def register(name, service, realm=1, capabilities=None):
            sock = connect(name)
            body = string(name)+string(name)+bytes([service])+string('127.0.0.1')
            body += struct.pack('!HIIII',1120 if service == 1 else 8085,realm if service == 2 else 0,18414,100,capabilities if capabilities is not None else (80 if service == 1 else 72))
            assert exchange(sock,1,body)[0] == 0x8000
            if service == 2:
                assert exchange(sock,5,struct.pack('!HI',1,realm))[0] == 0x8000
            assert exchange(sock,2,struct.pack('!BI',1,0))[0] == 0x8000
            registrations.append(sock)
            return sock
        def request(name, action, purpose=1, account=123456789, realm=0, token='-', ttl=0, evidence='-', address='127.0.0.1'):
            body = struct.pack('!BBBIII',1,action,purpose,account,realm,ttl)+string(address)+string(evidence)+string(token)
            with connect(name) as sock:
                kind, reply = exchange(sock,6,body)
                assert kind == 0x8001
                return reply[0],reply[3:].decode()
        try:
            for _ in range(100):
                if process.poll() is not None:
                    raise RuntimeError('Isolated hub exited before listening; check hub database/configuration')
                try:
                    with socket.create_connection(('127.0.0.1',port),timeout=.1):
                        break
                except OSError:
                    time.sleep(.1)
            else:
                raise RuntimeError('Isolated hub startup timed out')
            # Directory protocol: distinct realms, incompatible build, readiness, overlapping
            # ownership, deregistration, lease expiry and stale selected-owner rejection.
            directory = register('auth-dir',1,capabilities=208)
            world_a = register('world',2)
            world_b = register('world-b',2,realm=2)
            def query(pairs):
                kind, body = exchange(directory,7,struct.pack('!BH',1,len(pairs))+b''.join(struct.pack('!II',*p) for p in pairs))
                assert kind == 0x8002 and body[0] == 1
                count = struct.unpack_from('!H',body,1)[0]; pos = 3; routes = []
                def text():
                    nonlocal pos
                    size = struct.unpack_from('!H',body,pos)[0]; pos += 2
                    value = body[pos:pos+size].decode(); pos += size; return value
                for _ in range(count):
                    realm, build, state = struct.unpack_from('!IIB',body,pos); pos += 9
                    address = text(); port_, ttl = struct.unpack_from('!HI',body,pos); pos += 6
                    destination = text(); routes.append((realm,build,state,address,port_,ttl,destination))
                assert pos == len(body) and count == len(pairs)
                return routes
            routes = query([(1,18414),(2,18414),(3,18414)])
            assert [v[2] for v in routes] == [1,1,0]
            assert routes[0][6] != routes[1][6] and all(0 < v[5] <= 3000 for v in routes[:2])
            assert query([(1,12345)])[0][2] == 4
            selected_owner = routes[0][6]
            def issue_selected(owner):
                body = struct.pack('!BBBIII',2,1,2,123456790,1,60)+string('127.0.0.1')+string('a'*64)+string('-')+string(owner)
                with connect('auth-dir') as sock:
                    kind, reply = exchange(sock,6,body); assert kind == 0x8001; return reply[0]
            assert issue_selected(selected_owner) == 0
            assert exchange(world_a,2,struct.pack('!BI',0,0))[0] == 0x8000
            assert query([(1,18414)])[0][2] == 0
            assert exchange(world_a,2,struct.pack('!BI',1,0))[0] == 0x8000
            assert exchange(world_b,5,struct.pack('!HII',2,2,1))[0] == 0x8000
            assert query([(1,18414)])[0][2] == 5
            assert exchange(world_b,5,struct.pack('!HI',1,2))[0] == 0x8000
            assert exchange(world_a,4,b'')[0] == 0x8000
            assert query([(1,18414)])[0][2] == 0
            world_a.close(); registrations.remove(world_a)
            world_a = register('world',2)
            assert query([(1,18414)])[0][6] != selected_owner
            assert issue_selected(selected_owner) == 2
            # Keep the query caller alive but let world leases expire without heartbeats.
            for _ in range(6):
                time.sleep(1)
                assert exchange(directory,3,struct.pack('!I',0))[0] == 0x8000
            assert [v[2] for v in query([(1,18414),(2,18414)])] == [0,0]
            for sock in registrations: sock.close()
            registrations.clear()
            time.sleep(.2)
            print('Live realm TLS checks passed: separate realms, readiness, build, conflict, restart, selection binding, lease expiry.')
            if args.agent_probe:
                world_a = register('world',2); world_b = register('world-b',2,realm=2)
                agent = subprocess.Popen([str(Path(args.agent_probe).resolve()),str(port),str(root/'ca.pem'),
                        str(root/'auth-agent.pem'),str(root/'auth-agent.key')],stdout=subprocess.PIPE,stderr=subprocess.STDOUT,
                        text=True,creationflags=hidden)
                events = queue.Queue()
                def read_agent():
                    for line in agent.stdout: events.put(line.strip())
                reader_thread = threading.Thread(target=read_agent,daemon=True); reader_thread.start()
                def expect_agent(expected, live_worlds):
                    deadline = time.monotonic()+20
                    while time.monotonic() < deadline:
                        try:
                            event = events.get(timeout=.25)
                            if event != expected: raise RuntimeError('Agent probe: '+event)
                            return
                        except queue.Empty:
                            if agent.poll() is not None: raise RuntimeError('Agent probe exited unexpectedly')
                            for peer in live_worlds: assert exchange(peer,3,struct.pack('!I',0))[0] == 0x8000
                    raise RuntimeError('Agent probe timed out waiting for '+expected)
                try:
                    expect_agent('READY',[world_a,world_b])
                    assert exchange(world_a,2,struct.pack('!BI',0,0))[0] == 0x8000
                    assert exchange(world_b,4,b'')[0] == 0x8000
                    world_b.close(); registrations.remove(world_b)
                    expect_agent('OFFLINE',[world_a])
                    assert exchange(world_a,2,struct.pack('!BI',1,0))[0] == 0x8000
                    world_b = register('world-b',2,realm=2)
                    expect_agent('RECOVERED',[world_a,world_b])
                    assert agent.wait(timeout=5) == 0
                finally:
                    if agent.poll() is None: agent.terminate(); agent.wait(timeout=5)
                    reader_thread.join(timeout=2)
                for peer in registrations: peer.close()
                registrations.clear(); time.sleep(.2)
                print('Actual C++ agent passed: background multi-batch refresh, offline withdrawal, world restart recovery and cache clearing on stop.')
            register('auth-a',1); register('auth-b',1); register('world',2)
            def heartbeat():
                while not stopping.wait(1):
                    for sock in list(registrations):
                        try: exchange(sock,3,struct.pack('!I',0))
                        except OSError: return
            thread = threading.Thread(target=heartbeat,daemon=True)
            thread.start()
            result, token = request('auth-a',1,ttl=60)
            assert result == 0 and len(token) == 64
            assert request('unknown',3,token=token)[0] == 3
            assert request('auth-b',3,token=token,address='127.0.0.2')[0] == 3
            assert request('auth-b',3,token=token)[0] == 0
            assert request('auth-a',3,token=token)[0] == 6
            result, token = request('auth-a',1,ttl=1)
            assert result == 0
            time.sleep(1.1)
            assert request('auth-b',3,token=token)[0] in (4,5)
            result, token = request('auth-a',1,ttl=60)
            assert result == 0
            with concurrent.futures.ThreadPoolExecutor(max_workers=2) as pool:
                outcomes = list(pool.map(lambda who:request(who,3,token=token)[0],('auth-a','auth-b')))
            assert sorted(outcomes) == [0,6]
            result, token = request('auth-a',1,ttl=60)
            assert result == 0 and request('auth-a',4,token=token)[0] == 0
            assert request('auth-b',3,token=token)[0] == 6
            result, token = request('auth-a',1,purpose=2,realm=1,ttl=60,evidence='a'*64)
            assert result == 0
            assert request('auth-b',3,purpose=2,realm=1,token=token,evidence='a'*64)[0] == 3
            assert request('world',3,purpose=2,realm=1,token=token,evidence='b'*64)[0] == 3
            assert request('world',3,purpose=2,realm=1,token=token,evidence='a'*64)[0] == 0
            probe = subprocess.run([str(Path(args.client_probe).resolve()),str(port),str(root/'ca.pem'),
                            str(root/'auth-a.pem'),str(root/'auth-a.key'),'--live'],capture_output=True,text=True,creationflags=hidden)
            if probe.returncode: raise RuntimeError('C++ client probe failed: '+probe.stdout+probe.stderr)
            print(probe.stdout.strip())
            stopping.set(); thread.join(timeout=4)
            result, token = request('auth-a',1,ttl=60)
            assert result == 0
            registrations[0].close()
            time.sleep(.2)
            assert request('auth-b',3,token=token)[0] == 0  # Issuer process is gone.
            result, token = request('auth-b',1,purpose=2,realm=1,ttl=60,evidence='c'*64)
            assert result == 0
            registrations[2].close()
            time.sleep(.2)
            register('world',2)
            assert request('world',3,purpose=2,realm=1,token=token,evidence='c'*64)[0] == 3
            # A peer without a client certificate must never receive a handoff response.
            context = ssl.create_default_context(cafile=str(root/'ca.pem'))
            try:
                with context.wrap_socket(socket.create_connection(('127.0.0.1',port),timeout=3),server_hostname='localhost') as sock:
                    kind, _ = exchange(sock,6,b'')
                    assert kind != 0x8001
            except (OSError,RuntimeError):
                pass
            print('Hub TLS checks passed: cross-auth handoff after issuer loss, concurrent one-time consume, expiry, revocation, world restart binding, peer authentication, C++ client.')
        finally:
            stopping.set()
            if 'thread' in locals(): thread.join(timeout=4)
            for sock in registrations: sock.close()
            if process.poll() is None:
                # This isolated hub never starts managed/game services. Windows _getch console
                # input does not read redirected stdin; terminating this test-only process is safe.
                if os.name == 'nt': process.terminate()
                else: process.stdin.write('exit\n'); process.stdin.flush()
                process.wait(timeout=15)
            log.close()


if __name__ == '__main__':
    main()
