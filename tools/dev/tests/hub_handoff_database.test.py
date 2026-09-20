"""Disposable MySQL checks for durable handoffs; optionally run the compiled store probe.
Credentials come from VerificationDatabaseInfo in a private config, never command arguments.
"""
import argparse
import concurrent.futures
import importlib.util
import os
from pathlib import Path
import secrets
import subprocess
import sys


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--verification-config', type=Path, required=True)
    parser.add_argument('--mysql', type=Path, required=True)
    parser.add_argument('--probe', type=Path)
    args = parser.parse_args()
    repo = Path(__file__).resolve().parents[3]
    sys.dont_write_bytecode = True
    spec = importlib.util.spec_from_file_location('backupserver', repo/'src/server/hub/backup/backupserver.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    info = module.connection(args.verification_config, 'VerificationDatabaseInfo')
    admin = module.Database(info, args.mysql)
    name = 'sf_handoff_test_' + secrets.token_hex(8)
    admin.query('CREATE DATABASE `' + name + '`', use_database=False)
    try:
        info = info[:4] + [name]
        db = module.Database(info, args.mysql)
        db.query((repo/'sql/pending_updates/hub/001_durable_handoffs.sql').read_text())
        token = secrets.token_hex(32)
        db.query("INSERT INTO hub_handoff_tokens(token,account,purpose,realm,address,destination,evidence,expires_at) VALUES(" +
                 module.literal(token) + ",9001,1,0,'127.0.0.1','authnet','-',DATE_ADD(UTC_TIMESTAMP(6),INTERVAL 60 SECOND))")
        def consume(_):
            # Separate client connections serialize on the same singleton lock.
            result = db.query("START TRANSACTION; SELECT id FROM hub_handoff_lock WHERE id=1 FOR UPDATE; "
                              "UPDATE hub_handoff_tokens SET state=1 WHERE token=" + module.literal(token) +
                              " AND state=0 AND expires_at>UTC_TIMESTAMP(6); SELECT ROW_COUNT(); COMMIT;")
            return int(result.splitlines()[-1])
        with concurrent.futures.ThreadPoolExecutor(max_workers=2) as pool:
            assert sorted(pool.map(consume, range(2))) == [0, 1]
        assert db.query('SELECT state FROM hub_handoff_tokens WHERE token=' + module.literal(token)) == '1'
        db.query("START TRANSACTION; SELECT id FROM hub_handoff_lock WHERE id=1 FOR UPDATE; "
                 "UPDATE hub_handoff_tokens SET state=2; ROLLBACK;")
        assert db.query('SELECT state FROM hub_handoff_tokens WHERE token=' + module.literal(token)) == '1'
        if args.probe:
            env = os.environ.copy()
            env['SKYFIRE_HANDOFF_TEST_DB'] = ';'.join(info)
            result = subprocess.run([str(args.probe.resolve())], env=env, capture_output=True,
                                    text=True, **module.hidden())
            if result.returncode:
                raise RuntimeError('Compiled durable-store probe failed: ' + result.stdout + result.stderr)
            print(result.stdout.strip())
        module.invalidate_restored_handoffs(db)
        assert db.query('SELECT COUNT(*) FROM hub_handoff_tokens') == '0'
        print('PASS pending schema, cross-connection consume serialization, rollback and restored-token invalidation.')
        if not args.probe:
            print('C++ store probe not supplied; runtime implementation testing remains pending.')
    finally:
        admin.query('DROP DATABASE `' + name + '`', use_database=False)
        print('Disposable handoff database removed.')


if __name__ == '__main__':
    main()
