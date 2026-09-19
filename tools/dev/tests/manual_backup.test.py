# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
"""Isolated MySQL backup/restore drill. Never writes to configured deployment databases."""
import argparse
import ast
import re
import importlib.util
import json
import secrets
import tempfile
import sys
sys.dont_write_bytecode = True
import threading
import time
from pathlib import Path


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--hub-config', required=True)
    parser.add_argument('--mysql', required=True)
    parser.add_argument('--mysqldump', required=True)
    args = parser.parse_args()
    repo = Path(__file__).resolve().parents[3]
    spec = importlib.util.spec_from_file_location('backupserver', repo / 'src/server/hub/backup/backupserver.py')
    module = importlib.util.module_from_spec(spec); spec.loader.exec_module(module)
    credentials = module.connection(args.hub_config, 'HubDatabaseInfo')
    admin = module.Database(credentials, Path(args.mysql))
    token = secrets.token_hex(8)
    hub_name, data_name = 'sf_backup_hub_' + token, 'sf_backup_data_' + token
    created = []
    worker = None
    stop_health = threading.Event()
    try:
        for name in [hub_name, data_name]:
            assert name not in (credentials[4],) and name.startswith('sf_backup_')
            admin.query('CREATE DATABASE `' + name + '`', use_database=False); created.append(name)
        hub_info = credentials[:4] + [hub_name]; data_info = credentials[:4] + [data_name]
        hub = module.Database(hub_info, Path(args.mysql)); data = module.Database(data_info, Path(args.mysql))
        hub.query((repo / 'sql/updates/hub/2026_09_18_hub_02.sql').read_text())
        hub.query((repo / 'sql/updates/hub/2026_09_19_hub_00.sql').read_text())
        statements=(repo/'src/server/shared/Database/Implementation/HubDatabase.cpp').read_text()
        selected={'HUB_SEL_BACKUP_WORKER','HUB_SEL_BACKUP_JOBS','HUB_SEL_BACKUP_JOB','HUB_INS_BACKUP_JOB','HUB_UPD_BACKUP_HUB_HEALTH','HUB_INS_RESTORE_JOB'}
        for name,body in re.findall(r'PrepareStatement\((HUB_\w+),\s*(.*?)CONNECTION_SYNCH\);',statements,re.S):
            if name in selected:
                text=''.join(ast.literal_eval(part) for part in re.findall(r'"(?:[^"\\]|\\.)*"',body))
                hub.query('SET @backup_sql='+module.literal(text)+'; PREPARE backup_check FROM @backup_sql; DEALLOCATE PREPARE backup_check;')
        data.query("CREATE TABLE example(id INT PRIMARY KEY,value VARCHAR(100)) ENGINE=InnoDB; INSERT INTO example VALUES(1,'saved character');")
        with tempfile.TemporaryDirectory(prefix='skyfire-backup-drill-') as temporary:
            root = Path(temporary)
            (root / 'hub.conf').write_text('HubDatabaseInfo = "' + ';'.join(hub_info) + '"', encoding='utf-8')
            (root / 'world.conf').write_text('CharacterDatabaseInfo = "' + ';'.join(data_info) + '"', encoding='utf-8')
            config = {'hub_config': str(root / 'hub.conf'), 'world_config': str(root / 'world.conf'),
                      'mysql': args.mysql, 'mysqldump': args.mysqldump, 'output_directory': str(root / 'archives'),
                      'minimum_free_mb': 0, 'timeout_seconds': 60, 'allow_live_restore': True}
            (root / 'backup.toml').write_text('\n'.join(key+' = '+json.dumps(value) for key,value in config.items()), encoding='utf-8')
            worker = module.Worker(root / 'backup.toml'); worker.heartbeat()
            def health():
                while not stop_health.is_set():
                    hub.query('UPDATE hub_backup_worker SET services_stopped=1,hub_seen=NOW() WHERE id=1')
                    stop_health.wait(1)
            thread = threading.Thread(target=health); thread.start()
            def enqueue(kind='backup', source=None):
                identifier = secrets.token_hex(16)
                hub.query("INSERT INTO hub_backup_jobs(id,target,actor,kind,source_id) VALUES(" +
                          ','.join(module.literal(value) for value in (identifier,'characters','test',kind,source or '')) + ')')
                return worker.claim()
            job = enqueue(); worker.backup(job)
            record = hub.query('SELECT state,bytes,sha256 FROM hub_backup_jobs WHERE id='+module.literal(job['id'])).split('\t')
            assert record[0]=='completed' and int(record[1])>0 and len(record[2])==64
            archive = root/'archives'/job['id']/'database.sql'
            original = archive.read_bytes(); archive.write_bytes(original+b'corrupt')
            try: worker.load_archive(job['id'],'characters'); raise AssertionError('Corrupt archive accepted')
            except RuntimeError: pass
            archive.write_bytes(original)
            # Concurrent admission is rejected by the database even across hub instances.
            pending=enqueue()
            try: enqueue(); raise AssertionError('Concurrent backup accepted')
            except RuntimeError: pass
            worker.finish(pending,'failed','Test concurrency check')
            data.query("UPDATE example SET value='new current character'")
            hub.query('UPDATE hub_backup_worker SET maintenance=1,services_stopped=1,hub_seen=NOW() WHERE id=1')
            restore=enqueue('restore',job['id']); worker.restore(restore)
            assert data.query('SELECT value FROM example')=='saved character'
            assert hub.query('SELECT state FROM hub_backup_jobs WHERE id='+module.literal(restore['id']))=='completed'
            journal=json.loads((root/'archives'/restore['id']/'recovery.json').read_text())
            rollback=journal['rollbackId']
            assert (root/'archives'/rollback/'database.sql').is_file()
            assert hub.query('SELECT state FROM hub_backup_jobs WHERE id='+module.literal(rollback))=='completed'
            assert hub.query('SELECT maintenance,recovery_safe FROM hub_backup_worker WHERE id=1')=='1\t1'
            # Fail after live replacement; ensure automatic rollback returns the prior live data.
            data.query("UPDATE example SET value='keep this after failed restore'")
            restore=enqueue('restore',job['id'])
            original_import=worker.import_archive
            injected=[False]
            def failed_import(database,name,path):
                if name==data_name and not injected[0]:
                    injected[0]=True; raise RuntimeError('Injected live import failure')
                return original_import(database,name,path)
            worker.import_archive=failed_import
            try: worker.restore(restore); raise AssertionError('Expected injected failure')
            except RuntimeError as error: assert 'original database was restored' in str(error)
            worker.finish(restore,'failed','Injected failure rolled back')
            assert data.query('SELECT value FROM example')=='keep this after failed restore'
            assert hub.query('SELECT maintenance,recovery_safe FROM hub_backup_worker WHERE id=1')=='1\t1'
            worker.import_archive=original_import
            # Another worker cannot take an unexpired lease.
            other=module.Worker(root/'backup.toml')
            try: other.heartbeat(); raise AssertionError('Second worker acquired live lease')
            except RuntimeError: pass
            stop_health.set(); thread.join()
            # Exercise the offline hub path against this disposable schema only. Process detection
            # is tested separately; the real deployment hub intentionally remains running.
            hub_job=secrets.token_hex(16)
            hub.query("INSERT INTO hub_backup_jobs(id,target,actor) VALUES("+module.literal(hub_job)+",'hub','test')")
            worker.backup(worker.claim())
            hub.query('CREATE TABLE after_archive(id INT PRIMARY KEY) ENGINE=InnoDB; INSERT INTO after_archive VALUES(1)')
            hub.query("UPDATE hub_backup_worker SET services_stopped=1,hub_seen=DATE_SUB(NOW(),INTERVAL 120 SECOND),lease_until=NULL WHERE id=1")
            def assert_disposable(current):
                assert current.hub.info[4]==hub_name and hub_name.startswith('sf_backup_hub_')
            module.assert_local_services_stopped=assert_disposable
            module.offline_restore_hub(worker,hub_job,hub_name)
            assert hub.query("SELECT COUNT(*) FROM information_schema.tables WHERE table_schema=DATABASE() AND table_name='after_archive'")=='0'
            assert hub.query('SELECT maintenance,recovery_safe FROM hub_backup_worker WHERE id=1')=='1\t1'
            print('PASS dump/checksum, corruption rejection, concurrency, staging/live restore, protected rollback, injected failure recovery, worker fencing and offline hub recovery.')
    finally:
        stop_health.set()
        if 'thread' in locals(): thread.join()
        # Drop only exact test databases created by this invocation.
        for name in reversed(created):
            assert name in (hub_name,data_name) and name != credentials[4]
            admin.query('DROP DATABASE IF EXISTS `'+name+'`', use_database=False)
        print('Disposable test databases removed; deployment data unchanged.')


if __name__=='__main__': main()
