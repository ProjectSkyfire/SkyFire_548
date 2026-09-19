# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
"""Isolated MySQL backup/restore drill. Never writes to configured deployment databases."""
import argparse
from concurrent.futures import ThreadPoolExecutor
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
    parser.add_argument('--domain', choices=('auth','characters','world'), default='characters')
    parser.add_argument('--report', type=Path)
    parser.add_argument('--verification-config')
    args = parser.parse_args()
    timings = {'domain':args.domain, 'fixture':'Disposable InnoDB and MyISAM tables; not production-scale RTO', 'startedAt':time.time()}
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
        hub.query((repo / 'sql/updates/hub/2026_09_18_hub_03.sql').read_text())
        hub.query((repo / 'sql/updates/hub/2026_09_19_hub_01.sql').read_text())
        statements=(repo/'src/server/shared/Database/Implementation/HubDatabase.cpp').read_text()
        selected={'HUB_SEL_BACKUP_WORKER','HUB_SEL_BACKUP_JOBS','HUB_SEL_BACKUP_JOB','HUB_INS_BACKUP_JOB','HUB_UPD_BACKUP_HUB_HEALTH','HUB_INS_RESTORE_JOB','HUB_UPD_BACKUP_PIN'}
        for name,body in re.findall(r'PrepareStatement\((HUB_\w+),\s*(.*?)CONNECTION_SYNCH\);',statements,re.S):
            if name in selected:
                text=''.join(ast.literal_eval(part) for part in re.findall(r'"(?:[^"\\]|\\.)*"',body))
                hub.query('SET @backup_sql='+module.literal(text)+'; PREPARE backup_check FROM @backup_sql; DEALLOCATE PREPARE backup_check;')
        data.query("CREATE TABLE example(id INT PRIMARY KEY,value VARCHAR(100)) ENGINE=InnoDB; INSERT INTO example VALUES(1,'saved character'); CREATE TABLE legacy(id INT PRIMARY KEY,value VARCHAR(100)) ENGINE=MyISAM; INSERT INTO legacy VALUES(1,'world data');")
        with tempfile.TemporaryDirectory(prefix='skyfire-backup-drill-') as temporary:
            root = Path(temporary)
            (root / 'hub.conf').write_text('HubDatabaseInfo = "' + ';'.join(hub_info) + '"\n' + ('LoginDatabaseInfo = "' + ';'.join(data_info) + '"' if args.domain=='auth' else ''), encoding='utf-8')
            (root / 'world.conf').write_text(('WorldDatabaseInfo' if args.domain=='world' else 'CharacterDatabaseInfo') + ' = "' + ';'.join(data_info) + '"' if args.domain!='auth' else '', encoding='utf-8')
            config = {'hub_config': str(root / 'hub.conf'), 'world_config': str(root / 'world.conf'),
                      'mysql': args.mysql, 'mysqldump': args.mysqldump, 'output_directory': str(root / 'archives'),
                      'minimum_free_mb': 0, 'timeout_seconds': 60, 'allow_live_restore': True}
            if args.verification_config: config['verification_config']=args.verification_config
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
                          ','.join(module.literal(value) for value in (identifier,args.domain,'test',kind,source or '')) + ')')
                return worker.claim()
            hub.query('UPDATE hub_backup_worker SET services_stopped=1,hub_seen=NOW() WHERE id=1')
            # READ (not READ LOCAL) must block concurrent MyISAM inserts.
            with ThreadPoolExecutor(max_workers=1) as executor:
                with worker.locked_snapshot(data,True):
                    blocked=executor.submit(data.query,"INSERT INTO legacy VALUES(2,'concurrent insert')")
                    time.sleep(.5)
                    assert not blocked.done(), 'MyISAM inserts must wait for the snapshot lock'
                blocked.result(timeout=5)
            data.query('DELETE FROM legacy WHERE id=2')
            stop_health.set(); thread.join()
            hub.query('UPDATE hub_backup_worker SET services_stopped=0,hub_seen=NOW() WHERE id=1')
            try:
                worker.backup({'id':secrets.token_hex(16),'target':args.domain})
                raise AssertionError('MyISAM backup accepted while services were active')
            except RuntimeError as error:
                assert 'all game services stopped' in str(error)
            assert hub.query('SELECT maintenance FROM hub_backup_worker WHERE id=1')=='0'
            hub.query('UPDATE hub_backup_worker SET services_stopped=1,hub_seen=NOW() WHERE id=1')
            stop_health.clear(); thread=threading.Thread(target=health); thread.start()
            started=time.monotonic()
            job = enqueue(); worker.backup(job)
            timings['backupAndVerificationSeconds']=round(time.monotonic()-started,3)
            assert hub.query('SELECT maintenance FROM hub_backup_worker WHERE id=1')=='0'
            assert json.loads((root/'archives'/job['id']/'manifest.json').read_text())['consistency']=='maintenance table READ locks'
            record = hub.query('SELECT state,bytes,sha256 FROM hub_backup_jobs WHERE id='+module.literal(job['id'])).split('\t')
            assert record[0]=='completed' and int(record[1])>0 and len(record[2])==64
            assert hub.query('SELECT verified_at IS NOT NULL FROM hub_backup_jobs WHERE id='+module.literal(job['id']))=='1', hub.query('SELECT message FROM hub_backup_jobs WHERE id='+module.literal(job['id']))
            archive = root/'archives'/job['id']/'database.sql'
            original = archive.read_bytes(); archive.write_bytes(original+b'corrupt')
            try: worker.load_archive(job['id'],args.domain); raise AssertionError('Corrupt archive accepted')
            except RuntimeError: pass
            archive.write_bytes(original)
            # Even a self-consistent archive cannot use verification to write outside its stage.
            malicious=enqueue(); malicious_directory=root/'archives'/malicious['id']; malicious_directory.mkdir()
            malicious_archive=malicious_directory/'database.sql'
            malicious_archive.write_bytes(original+('\nINSERT INTO `'+data_name+"`.example VALUES(99,'forbidden');\n").encode())
            manifest=json.loads((archive.parent/'manifest.json').read_text())
            manifest.update(jobId=malicious['id'],bytes=malicious_archive.stat().st_size,sha256=module.digest(malicious_archive))
            (malicious_directory/'manifest.json').write_text(json.dumps(manifest))
            try: worker.verify_archive(malicious); raise AssertionError('Cross-database verification write accepted')
            except RuntimeError: pass
            assert data.query('SELECT COUNT(*) FROM example WHERE id=99')=='0'
            worker.finish(malicious,'failed','Isolated cross-database write rejected')
            # UTC daily/weekly boundaries and interval scheduling.
            schedule={'mode':'daily','minute':180,'weekday':0,'interval':15}
            monday=module.dt.datetime(2026,9,21,3,tzinfo=module.dt.timezone.utc).timestamp()
            assert worker.next_run(schedule,monday)==monday+86400
            schedule['mode']='weekly'; assert worker.next_run(schedule,monday)==monday+7*86400
            schedule['mode']='interval'; assert worker.next_run(schedule,monday)==monday+900
            # A persisted overdue occurrence survives restarts without duplicate admission.
            hub.query("UPDATE hub_backup_schedules SET enabled=1,mode='interval',interval_minutes=15 WHERE target="+module.literal(args.domain))
            worker.schedule()
            hub.query('UPDATE hub_backup_schedule_runs SET next_run=UNIX_TIMESTAMP()-60 WHERE target='+module.literal(args.domain))
            stop_health.set(); thread.join()
            hub.query('UPDATE hub_backup_worker SET services_stopped=0,hub_seen=NOW() WHERE id=1')
            worker.schedule(); assert worker.claim() is None
            assert 'Waiting for gracefully stopped' in hub.query('SELECT automation_status FROM hub_backup_worker WHERE id=1')
            hub.query('UPDATE hub_backup_worker SET services_stopped=1,hub_seen=NOW() WHERE id=1')
            stop_health.clear(); thread=threading.Thread(target=health); thread.start()
            worker.schedule(); scheduled=worker.claim(); assert scheduled and scheduled['target']==args.domain
            worker.schedule(); assert hub.query("SELECT COUNT(*) FROM hub_backup_jobs WHERE actor='scheduler'")=='1'
            worker.backup(scheduled)
            assert hub.query('SELECT verified_at IS NOT NULL FROM hub_backup_jobs WHERE id='+module.literal(scheduled['id']))=='1'
            # Simulate a worker restart with the same leased owner and the persisted schedule cursor.
            restarted=module.Worker(root/'backup.toml'); restarted.owner=worker.owner; restarted.schedule()
            assert hub.query("SELECT COUNT(*) FROM hub_backup_jobs WHERE actor='scheduler'")=='1'
            # No backlog storm; changing a schedule revision establishes a future run.
            hub.query('UPDATE hub_backup_schedule_runs SET next_run=1 WHERE target='+module.literal(args.domain))
            hub.query('UPDATE hub_backup_schedules SET revision=revision+1 WHERE target='+module.literal(args.domain))
            worker.schedule(); assert worker.claim() is None
            hub.query('UPDATE hub_backup_schedules SET enabled=0')
            # Pinning and the last verified recovery point prevent expiration.
            hub.query('UPDATE hub_backup_jobs SET finished_at=DATE_SUB(NOW(),INTERVAL 40 DAY),pinned=1 WHERE id='+module.literal(job['id']))
            hub.query('UPDATE hub_backup_jobs SET finished_at=DATE_SUB(NOW(),INTERVAL 35 DAY) WHERE id='+module.literal(scheduled['id']))
            worker.retention()
            assert archive.is_file() and (root/'archives'/scheduled['id']/'database.sql').is_file()
            # A separate older recovery point expires and the audit/history remain available.
            expired=enqueue(); worker.backup(expired)
            hub.query('UPDATE hub_backup_jobs SET finished_at=DATE_SUB(NOW(),INTERVAL 45 DAY) WHERE id='+module.literal(expired['id']))
            worker.retention()
            assert hub.query('SELECT state FROM hub_backup_jobs WHERE id='+module.literal(expired['id']))=='expired'
            assert not (root/'archives'/expired['id']/'database.sql').exists()
            assert hub.query("SELECT COUNT(*) FROM hub_control_audit WHERE phase='retention'")=='1'
            # Resume after a crash between unlinking the dump and recording expiration.
            interrupted=enqueue(); worker.backup(interrupted)
            hub.query("UPDATE hub_backup_jobs SET state='deleting',finished_at=DATE_SUB(NOW(),INTERVAL 45 DAY) WHERE id="+module.literal(interrupted['id']))
            (root/'archives'/interrupted['id']/'database.sql').unlink()
            worker.retention()
            assert hub.query('SELECT state FROM hub_backup_jobs WHERE id='+module.literal(interrupted['id']))=='expired'
            failed_verification=enqueue(); verify=worker.verify_archive
            def unavailable_verifier(current): raise RuntimeError('Test verifier unavailable')
            worker.verify_archive=unavailable_verifier
            worker.backup(failed_verification); worker.verify_archive=verify
            assert hub.query('SELECT state,verified_at IS NULL FROM hub_backup_jobs WHERE id='+module.literal(failed_verification['id']))=='completed\t1'
            assert (root/'archives'/failed_verification['id']/'database.sql').is_file()
            # Concurrent admission is rejected by the database even across hub instances.
            pending=enqueue()
            try: enqueue(); raise AssertionError('Concurrent backup accepted')
            except RuntimeError: pass
            worker.finish(pending,'failed','Test concurrency check')
            data.query("UPDATE example SET value='new current character'")
            hub.query('UPDATE hub_backup_worker SET maintenance=1,services_stopped=1,hub_seen=NOW() WHERE id=1')
            started=time.monotonic()
            restore=enqueue('restore',job['id']); worker.restore(restore)
            timings['restoreSeconds']=round(time.monotonic()-started,3)
            assert data.query('SELECT value FROM example')=='saved character'
            assert hub.query('SELECT state FROM hub_backup_jobs WHERE id='+module.literal(restore['id']))=='completed'
            journal=json.loads((root/'archives'/restore['id']/'recovery.json').read_text())
            rollback=journal['rollbackId']
            assert (root/'archives'/rollback/'database.sql').is_file()
            assert hub.query('SELECT state FROM hub_backup_jobs WHERE id='+module.literal(rollback))=='completed'
            assert hub.query('SELECT maintenance,recovery_safe FROM hub_backup_worker WHERE id=1')=='1\t1'
            hub.query('UPDATE hub_backup_jobs SET finished_at=DATE_SUB(NOW(),INTERVAL 45 DAY) WHERE id='+module.literal(rollback))
            hub.query('UPDATE hub_backup_worker SET maintenance=0 WHERE id=1')
            worker.retention()
            assert (root/'archives'/rollback/'database.sql').is_file()
            hub.query('UPDATE hub_backup_worker SET maintenance=1 WHERE id=1')
            # Fail after live replacement; ensure automatic rollback returns the prior live data.
            data.query("UPDATE example SET value='keep this after failed restore'")
            restore=enqueue('restore',job['id'])
            started=time.monotonic()
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
            timings['failedRestoreAndRollbackSeconds']=round(time.monotonic()-started,3)
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
            assert hub.query('SELECT verified_at IS NOT NULL FROM hub_backup_jobs WHERE id='+module.literal(hub_job))=='1', hub.query('SELECT message FROM hub_backup_jobs WHERE id='+module.literal(hub_job))
            hub.query('CREATE TABLE after_archive(id INT PRIMARY KEY) ENGINE=InnoDB; INSERT INTO after_archive VALUES(1)')
            hub.query("UPDATE hub_backup_worker SET services_stopped=1,hub_seen=DATE_SUB(NOW(),INTERVAL 120 SECOND),lease_until=NULL WHERE id=1")
            def assert_disposable(current):
                assert current.hub.info[4]==hub_name and hub_name.startswith('sf_backup_hub_')
            module.assert_local_services_stopped=assert_disposable
            started=time.monotonic()
            module.offline_restore_hub(worker,hub_job,hub_name)
            timings['offlineHubRestoreSeconds']=round(time.monotonic()-started,3)
            assert hub.query("SELECT COUNT(*) FROM information_schema.tables WHERE table_schema=DATABASE() AND table_name='after_archive'")=='0'
            assert hub.query('SELECT maintenance,recovery_safe FROM hub_backup_worker WHERE id=1')=='1\t1'
            print('PASS dump/checksum, corruption rejection, concurrency, staging/live restore, protected rollback, injected failure recovery, worker fencing and offline hub recovery.')
            print('PASS UTC schedules, restart deduplication, revision reset, automatic isolated verification, pinning and verified recovery retention.')
            if args.report: args.report.write_text(json.dumps(timings,indent=2),encoding='utf-8')
    finally:
        stop_health.set()
        if 'thread' in locals(): thread.join()
        # Drop only exact test databases created by this invocation.
        for name in reversed(created):
            assert name in (hub_name,data_name) and name != credentials[4]
            admin.query('DROP DATABASE IF EXISTS `'+name+'`', use_database=False)
        print('Disposable test databases removed; deployment data unchanged.')


if __name__=='__main__': main()
