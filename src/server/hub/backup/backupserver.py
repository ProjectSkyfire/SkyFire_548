# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
"""Manual backup worker. Python 3.11+, MySQL mysql/mysqldump; no third-party modules."""
import argparse
import csv
import datetime as dt
import hashlib
import json
import os
from pathlib import Path
import re
import secrets
import shutil
import signal
import subprocess
import time
import tomllib

TARGETS = ('auth', 'characters', 'world', 'hub')
STOP = False


def literal(value):
    return "CONVERT(X'" + str(value).encode('utf-8').hex() + "' USING utf8mb4)"


def connection(path, key):
    text = Path(path).read_text(encoding='utf-8-sig')
    match = re.search(r'^\s*' + re.escape(key) + r'\s*=\s*"([^"\r\n]*)"', text, re.M)
    if not match or not match[1]:
        return None
    parts = match[1].split(';')
    if len(parts) != 5 or not parts[1].isdigit() or not 1 <= int(parts[1]) <= 65535 or not parts[0] or not parts[4]:
        raise RuntimeError('Database configuration requires a TCP host and port.')
    return parts


class Database:
    def __init__(self, info, mysql):
        self.info, self.mysql = info, mysql

    def command(self, executable):
        host, port, user, password, name = self.info
        env = os.environ.copy()
        env['MYSQL_PWD'] = password
        return [str(executable), '--no-defaults', '--protocol=TCP', '--host=' + host,
                '--port=' + port, '--user=' + user], env

    def query(self, sql, use_database=True):
        args, env = self.command(self.mysql)
        result = subprocess.run(args + ['--connect-timeout=10'] + (['--database=' + self.info[4]] if use_database else []) + ['--batch', '--raw',
                                       '--skip-column-names', '--default-character-set=utf8mb4'],
                                input=sql, text=True, encoding='utf-8', capture_output=True,
                                timeout=15, env=env, **hidden())
        if result.returncode:
            raise RuntimeError('Database operation failed; check worker privileges and hub migration.')
        return result.stdout.strip()


def hidden():
    return {'creationflags': subprocess.CREATE_NO_WINDOW} if os.name == 'nt' else {}


def digest(path):
    result = hashlib.sha256()
    with path.open('rb') as source:
        for chunk in iter(lambda: source.read(1024 * 1024), b''):
            result.update(chunk)
    return result.hexdigest()


def private_directory(path):
    path.mkdir(parents=True, exist_ok=True, mode=0o700)
    if path.is_symlink():
        raise RuntimeError('Backup storage must not be a symbolic link.')
    if os.name == 'nt':
        result = subprocess.run(['whoami', '/user', '/fo', 'csv', '/nh'], capture_output=True,
                                text=True, check=True, **hidden())
        sid = next(csv.reader(result.stdout.splitlines()))[1]
        if not re.fullmatch(r'S-1-[0-9-]+', sid):
            raise RuntimeError('Cannot identify backup storage owner.')
        subprocess.run(['icacls', str(path), '/inheritance:r', '/grant:r', '*' + sid + ':(OI)(CI)F',
                        '*S-1-5-18:(OI)(CI)F'], check=True, stdout=subprocess.DEVNULL,
                       stderr=subprocess.DEVNULL, **hidden())
    else:
        path.chmod(0o700)


class Worker:
    def __init__(self, config_path):
        config_path = Path(config_path).resolve()
        config = tomllib.loads(config_path.read_text(encoding='utf-8-sig'))
        def path(key):
            value = Path(config[key])
            return (config_path.parent / value).resolve() if not value.is_absolute() else value
        self.mysql, self.dump = path('mysql'), path('mysqldump')
        if not self.mysql.is_file() or not self.dump.is_file():
            raise RuntimeError('Configure existing mysql and mysqldump executable paths.')
        self.output = path('output_directory')
        # Backups contain credentials and player data; restrict storage to this worker identity.
        private_directory(self.output)
        hub_config = path('hub_config')
        hub_info = connection(hub_config, 'HubDatabaseInfo')
        if not hub_info:
            raise RuntimeError('Hub database configuration is required.')
        self.hub = Database(hub_info, self.mysql)
        self.targets = {'hub': self.hub}
        auth = connection(hub_config, 'LoginDatabaseInfo')
        if auth:
            self.targets['auth'] = Database(auth, self.mysql)
        # Each world domain is explicitly bound to one configured worldserver, never a browser path.
        if config.get('world_config'):
            for target, key in [('characters', 'CharacterDatabaseInfo'), ('world', 'WorldDatabaseInfo')]:
                info = connection(path('world_config'), key)
                if info:
                    self.targets[target] = Database(info, self.mysql)
        names = [database.info[4] for database in self.targets.values()]
        if len(names) != len(set(names)):
            raise RuntimeError('Backup domains must use distinct database names; target aliases are not allowed.')
        self.allow_live_restore = config.get('allow_live_restore', False) is True
        self.timeout = int(config.get('timeout_seconds', 3600))
        self.free_bytes = int(config.get('minimum_free_mb', 1024)) * 1024 * 1024
        if not 30 <= self.timeout <= 86400 or self.free_bytes < 0:
            raise RuntimeError('Invalid backup timeout or minimum free space.')
        self.owner = secrets.token_hex(16)
        self.owned = False
        self.last_heartbeat = 0
        result = subprocess.run([str(self.dump), '--no-defaults', '--version'], capture_output=True,
                                text=True, timeout=10, check=True, **hidden())
        self.tool_version = result.stdout.strip()[:255]

    def heartbeat(self):
        targets = ','.join(sorted(self.targets))
        usage = shutil.disk_usage(self.output)
        if time.monotonic() - getattr(self, 'inventory_at', 0) > 30:
            self.archive_bytes = self.archive_count = 0
            for directory in self.output.iterdir():
                if not re.fullmatch('[0-9a-f]{32}', directory.name) or directory.is_symlink():
                    continue
                archive = directory / 'database.sql'
                if archive.is_file() and not archive.is_symlink() and (directory / 'manifest.json').is_file():
                    self.archive_bytes += archive.stat().st_size; self.archive_count += 1
            self.inventory_at = time.monotonic()
        owner = literal(self.owner)
        result = self.hub.query(
            f"UPDATE hub_backup_worker SET owner={owner}, targets={literal(targets)}, restore_enabled={int(self.allow_live_restore)}, storage_total={usage.total},storage_free={usage.free},archive_bytes={self.archive_bytes},archive_count={self.archive_count}, "
            "lease_until=DATE_ADD(CURRENT_TIMESTAMP,INTERVAL 60 SECOND) WHERE id=1 AND "
            f"(owner={owner} OR lease_until IS NULL OR lease_until<CURRENT_TIMESTAMP);"
            "SELECT owner FROM hub_backup_worker WHERE id=1;")
        if result != self.owner:
            raise RuntimeError('Another backup worker owns this hub, or the worker lease was lost.')
        self.owned = True
        self.last_heartbeat = time.monotonic()

    def tick(self, disk=True):
        if STOP and not getattr(self, "rolling_back", False):
            raise RuntimeError('Backup interrupted by worker shutdown.')
        if time.monotonic() - self.last_heartbeat >= 5:
            self.heartbeat()
        if disk and shutil.disk_usage(self.output).free < self.free_bytes:
            raise RuntimeError('Backup stopped: minimum free disk space reached.')

    def recover(self):
        # Only a newly fenced owner can recover abandoned work; never replay an interrupted dump.
        owner = literal(self.owner)
        self.hub.query(
            "START TRANSACTION; "
            "INSERT IGNORE INTO hub_control_audit(request_id,phase,actor,action,target,outcome) "
            "SELECT id,'finished',actor,IF(kind='restore','backup.restore','backup.now'),target,'interrupted' FROM hub_backup_jobs "
            f"WHERE (state='running' AND owner<>{owner}) OR (state='queued' AND created_at<DATE_SUB(NOW(),INTERVAL 5 MINUTE));"
            "UPDATE hub_backup_jobs SET state='failed',finished_at=NOW(),message='Interrupted or expired; request a new backup.' "
            f"WHERE (state='running' AND owner<>{owner}) OR (state='queued' AND created_at<DATE_SUB(NOW(),INTERVAL 5 MINUTE));COMMIT;")

    def claim(self):
        owner = literal(self.owner)
        self.hub.query("UPDATE hub_backup_jobs SET state='running',owner=" + owner +
                       " WHERE state='queued' AND EXISTS(SELECT 1 FROM hub_backup_worker WHERE id=1 AND owner=" +
                       owner + " AND lease_until>NOW()) ORDER BY created_at LIMIT 1;")
        raw = self.hub.query("SELECT JSON_OBJECT('id',id,'target',target,'kind',kind,'sourceId',source_id) FROM hub_backup_jobs WHERE state='running' AND owner=" + owner + " LIMIT 1")
        return json.loads(raw) if raw else None

    def finish(self, job, state, message, size=0, checksum=''):
        owner, job_id = literal(self.owner), literal(job['id'])
        self.hub.query(
            "START TRANSACTION; UPDATE hub_backup_jobs SET state=" + literal(state) + ",message=" + literal(message) +
            f",bytes={int(size)},sha256={literal(checksum)},finished_at=NOW() WHERE id={job_id} AND state='running' AND owner={owner} "
            f"AND EXISTS(SELECT 1 FROM hub_backup_worker WHERE id=1 AND owner={owner} AND lease_until>NOW());"
            "INSERT IGNORE INTO hub_control_audit(request_id,phase,actor,action,target,outcome) "
            f"SELECT id,'finished',actor,IF(kind='restore','backup.restore','backup.now'),target,state FROM hub_backup_jobs WHERE id={job_id} AND state IN ('completed','failed');COMMIT;")
        if self.hub.query(f"SELECT state FROM hub_backup_jobs WHERE id={job_id}") != state:
            raise RuntimeError('Cannot verify backup completion; worker lease may have expired.')

    def backup(self, job, publish=True):
        target, job_id = job['target'], job['id']
        if target not in self.targets or not re.fullmatch('[0-9a-f]{32}', job_id):
            raise RuntimeError('Backup target is not configured.')
        database = self.targets[target]
        # Single-transaction cannot give a consistent snapshot for MyISAM or other nontransactional tables.
        nontransactional = database.query("SELECT COUNT(*) FROM information_schema.tables WHERE table_schema=DATABASE() AND table_type='BASE TABLE' AND engine<>'InnoDB'")
        if nontransactional != '0':
            raise RuntimeError('Backup requires InnoDB tables; nontransactional tables need a maintenance backup.')
        version = database.query('SELECT VERSION()')
        events = int(database.query('SELECT COUNT(*) FROM information_schema.events WHERE event_schema=DATABASE()'))
        table_count = int(database.query("SELECT COUNT(*) FROM information_schema.tables WHERE table_schema=DATABASE() AND table_type='BASE TABLE'"))
        directory = self.output / job_id
        directory.mkdir(mode=0o700)  # Exclusive job directory; never overwrite an existing archive.
        partial = directory / 'database.sql.partial'
        args, env = database.command(self.dump)
        args += ['--single-transaction', '--quick', '--skip-lock-tables', '--no-tablespaces',
                 '--column-statistics=0', '--set-gtid-purged=OFF', '--hex-blob', '--routines', '--events',
                 '--triggers', '--default-character-set=utf8mb4', '--', database.info[4]]
        started = dt.datetime.now(dt.timezone.utc).isoformat()
        self.tick()
        if publish:
            self.hub.query("UPDATE hub_backup_jobs SET message='Creating database dump' WHERE id="+literal(job_id)+" AND state='running' AND owner="+literal(self.owner))
        with partial.open('xb') as output:
            process = subprocess.Popen(args, stdout=output, stderr=subprocess.DEVNULL, env=env, **hidden())
            deadline = time.monotonic() + self.timeout
            try:
                while process.poll() is None:
                    self.tick()
                    if time.monotonic() >= deadline:
                        raise RuntimeError('Backup exceeded its configured timeout.')
                    time.sleep(0.25)
                if process.returncode:
                    raise RuntimeError('mysqldump failed; check database access and dump-tool compatibility.')
                output.flush()
                os.fsync(output.fileno())
            finally:
                if process.poll() is None:
                    process.kill()
                process.wait()
        if not partial.stat().st_size:
            raise RuntimeError('Dump produced an empty archive.')
        # Hash in bounded chunks while renewing the worker lease, including for large world databases.
        def hash_archive():
            value = hashlib.sha256()
            with partial.open('rb') as source:
                for chunk in iter(lambda: source.read(1024 * 1024), b''):
                    self.tick()
                    value.update(chunk)
            return value.hexdigest()
        if publish:
            self.hub.query("UPDATE hub_backup_jobs SET message='Verifying archive checksum' WHERE id="+literal(job_id)+" AND state='running' AND owner="+literal(self.owner))
        checksum = hash_archive()
        if hash_archive() != checksum:
            raise RuntimeError('Archive failed checksum verification.')
        size = partial.stat().st_size
        manifest = {'version': 1, 'jobId': job_id, 'target': target, 'database': database.info[4],
                    'host': database.info[0], 'port': int(database.info[1]), 'databaseVersion': version,
                    'toolVersion': self.tool_version, 'startedAt': started,
                    'completedAt': dt.datetime.now(dt.timezone.utc).isoformat(), 'tables': table_count,
                    'bytes': size, 'sha256': checksum, 'archive': 'database.sql',
                    'consistency': 'single-database InnoDB snapshot; no concurrent schema changes permitted',
                    'restoreVerified': False, 'events': events}
        self.heartbeat()
        with (directory / 'manifest.json.partial').open('x', encoding='utf-8') as output:
            json.dump(manifest, output, indent=2)
            output.flush()
            os.fsync(output.fileno())
        partial.rename(directory / 'database.sql')
        (directory / 'manifest.json.partial').rename(directory / 'manifest.json')
        self.inventory_at = 0
        if publish:
            self.finish(job, 'completed', 'Dump and checksum verified; restore verification pending.', size, checksum)
        return manifest

    def load_archive(self, source_id, target):
        if not re.fullmatch('[0-9a-f]{32}', source_id):
            raise RuntimeError('Invalid archive identity.')
        directory = self.output / source_id
        archive = directory / 'database.sql'
        if directory.is_symlink() or archive.is_symlink() or (directory / 'manifest.json').is_symlink():
            raise RuntimeError('Recovery files must not be symbolic links.')
        manifest = json.loads((directory / 'manifest.json').read_text(encoding='utf-8'))
        database = self.targets[target]
        if (manifest.get('version') != 1 or manifest.get('jobId') != source_id or
                manifest.get('target') != target or manifest.get('database') != database.info[4] or
                manifest.get('host') != database.info[0] or manifest.get('port') != int(database.info[1])):
            raise RuntimeError('Archive identity does not match the configured database.')
        if manifest.get('events', 1):
            raise RuntimeError('Automatic restore refuses archives with scheduled SQL events; use isolated offline recovery.')
        if manifest.get('bytes') != archive.stat().st_size:
            raise RuntimeError('Archive size does not match its manifest.')
        checksum = hashlib.sha256()
        with archive.open('rb') as stream:
            for chunk in iter(lambda: stream.read(1024*1024), b''):
                self.tick(disk=False)
                checksum.update(chunk)
        if checksum.hexdigest() != manifest.get('sha256'):
            raise RuntimeError('Archive checksum verification failed.')
        return archive, manifest

    def import_archive(self, database, name, archive):
        args, env = database.command(self.mysql)
        args += ['--database=' + name, '--binary-mode=1', '--local-infile=0', '--default-character-set=utf8mb4']
        with archive.open('rb') as stream:
            process = subprocess.Popen(args, stdin=stream, stdout=subprocess.DEVNULL,
                                       stderr=subprocess.DEVNULL, env=env, **hidden())
            deadline = time.monotonic() + self.timeout
            try:
                while process.poll() is None:
                    self.tick(disk=False)
                    if time.monotonic() >= deadline:
                        raise RuntimeError('Database restore exceeded its configured timeout.')
                    time.sleep(.25)
                if process.returncode:
                    raise RuntimeError('Database import failed; inspect the recovery journal and worker privileges.')
            finally:
                if process.poll() is None:
                    process.kill()
                process.wait()

    def table_counts(self, database, name):
        info = list(database.info); info[4] = name
        target = Database(info, self.mysql)
        names = target.query("SELECT table_name FROM information_schema.tables WHERE table_schema=DATABASE() AND table_type='BASE TABLE' ORDER BY table_name").splitlines()
        counts = {}
        for table in names:
            self.tick(disk=False)
            escaped = '`' + table.replace('`', '``') + '`'
            counts[table] = int(target.query('SELECT COUNT(*) FROM ' + escaped))
        return counts

    def recovery_safe(self, safe):
        self.hub.query('UPDATE hub_backup_worker SET recovery_safe=' + ('1' if safe else '0') +
                       ' WHERE id=1 AND owner=' + literal(self.owner) + ' AND lease_until>NOW()')

    def restore(self, job):
        if not self.allow_live_restore:
            raise RuntimeError('Live restore is disabled in the backup worker configuration.')
        if job['target'] == 'hub':
            raise RuntimeError('Hub recovery requires the offline recovery command.')
        status = self.hub.query("SELECT maintenance=1 AND services_stopped=1 AND hub_seen>DATE_SUB(NOW(),INTERVAL 5 SECOND) FROM hub_backup_worker WHERE id=1")
        if status != '1':
            raise RuntimeError('Hub must be online in recovery maintenance with all game services stopped.')
        database = self.targets[job['target']]
        name = database.info[4]
        if not re.fullmatch('[A-Za-z0-9_$]+', name):
            raise RuntimeError('Live restore requires a simple configured database name.')
        archive, manifest = self.load_archive(job['sourceId'], job['target'])
        # Compare against the hub record as well as the on-disk manifest.
        recorded = self.hub.query('SELECT sha256 FROM hub_backup_jobs WHERE id=' + literal(job['sourceId']) + " AND state='completed' AND kind='backup'")
        if recorded != manifest['sha256']:
            raise RuntimeError('Archive checksum does not match the hub recovery record.')
        stage = 'skyfire_restore_' + job['id']
        journal_dir = self.output / job['id']; journal_dir.mkdir(mode=0o700)
        journal = {'jobId': job['id'], 'target': job['target'], 'sourceId': job['sourceId'], 'stage': stage,
                   'state': 'validating', 'rollbackId': None}
        def save_journal(state):
            journal['state'] = state
            temporary = journal_dir / 'recovery.json.partial'
            with temporary.open('w', encoding='utf-8') as output:
                json.dump(journal, output, indent=2); output.flush(); os.fsync(output.fileno())
            temporary.replace(journal_dir / 'recovery.json')
            self.hub.query('UPDATE hub_backup_jobs SET message='+literal(state.replace('_',' '))+' WHERE id='+literal(job['id'])+" AND state='running' AND owner="+literal(self.owner))
        save_journal('validating')
        charset = database.query("SELECT CONCAT(DEFAULT_CHARACTER_SET_NAME,' ',DEFAULT_COLLATION_NAME) FROM information_schema.schemata WHERE schema_name=DATABASE()").split()
        if len(charset) != 2 or any(not re.fullmatch('[a-zA-Z0-9_]+', value) for value in charset):
            raise RuntimeError('Cannot verify database character set.')
        create = ' CHARACTER SET ' + charset[0] + ' COLLATE ' + charset[1]
        # CREATE without IF NOT EXISTS guarantees that a pre-existing database is never overwritten.
        database.query('CREATE DATABASE `' + stage + '`' + create, use_database=False)
        stage_created = True
        live_touched = False
        rollback_archive = None
        try:
            self.import_archive(database, stage, archive)
            expected = self.table_counts(database, stage)
            if len(expected) != manifest['tables']:
                raise RuntimeError('Staging restore has an unexpected table count.')
            save_journal('taking_rollback_backup')
            rollback_id = secrets.token_hex(16)
            self.backup({'id': rollback_id, 'target': job['target']}, publish=False)
            rollback_archive, rollback_manifest = self.load_archive(rollback_id, job['target'])
            journal['rollbackId'] = rollback_id
            self.hub.query(
                "START TRANSACTION; INSERT INTO hub_backup_jobs(id,target,actor,kind,state,source_id,bytes,sha256,message,finished_at) "
                f"SELECT {literal(rollback_id)},target,actor,'backup','completed',id,{int(rollback_manifest['bytes'])},"
                f"{literal(rollback_manifest['sha256'])},'Protected pre-restore rollback',NOW() FROM hub_backup_jobs WHERE id={literal(job['id'])};"
                "INSERT INTO hub_control_audit(request_id,phase,actor,action,target,outcome) "
                f"SELECT id,'finished',actor,'backup.rollback',target,'completed' FROM hub_backup_jobs WHERE id={literal(rollback_id)}; COMMIT;")
            if self.hub.query('SELECT sha256 FROM hub_backup_jobs WHERE id='+literal(rollback_id)) != rollback_manifest['sha256']:
                raise RuntimeError('Cannot register the protected rollback recovery point.')
            # Check fencing again immediately before the first destructive statement.
            self.heartbeat()
            if self.hub.query("SELECT maintenance=1 AND services_stopped=1 AND hub_seen>DATE_SUB(NOW(),INTERVAL 5 SECOND) FROM hub_backup_worker WHERE id=1") != '1':
                raise RuntimeError('Maintenance health changed before replacement.')
            self.recovery_safe(False)
            save_journal('replacing_live_database')
            live_touched = True
            database.query('DROP DATABASE `' + name + '`; CREATE DATABASE `' + name + '`' + create, use_database=False)
            self.import_archive(database, name, archive)
            if self.table_counts(database, name) != expected:
                raise RuntimeError('Live restore row counts differ from the verified staging database.')
            save_journal('completed')
            self.recovery_safe(True)
            self.finish(job, 'completed', 'Live restore verified. Rollback archive ' + rollback_id + '. End maintenance when ready.')
        except Exception:
            if live_touched:
                save_journal('rolling_back')
                self.rolling_back = True
                try:
                    self.heartbeat()
                    database.query('DROP DATABASE IF EXISTS `' + name + '`; CREATE DATABASE `' + name + '`' + create, use_database=False)
                    self.import_archive(database, name, rollback_archive)
                    # Verify rollback independently by importing it into our staging database.
                    database.query('DROP DATABASE `' + stage + '`; CREATE DATABASE `' + stage + '`' + create, use_database=False)
                    self.import_archive(database, stage, rollback_archive)
                    if self.table_counts(database, name) != self.table_counts(database, stage):
                        raise RuntimeError('Rollback verification failed; keep services stopped and use the recovery journal.')
                    save_journal('rolled_back')
                    self.recovery_safe(True)
                except Exception:
                    save_journal('recovery_required')
                    raise RuntimeError('Restore and automatic rollback failed. Maintenance remains locked; recover using the saved rollback archive.') from None
                finally:
                    self.rolling_back = False
                raise RuntimeError('Restore failed; original database was restored and verified. Review the recovery journal before ending maintenance.') from None
            save_journal('failed_before_replacement')
            raise
        finally:
            if stage_created:
                # This exact stage name was created exclusively above; cleanup never uses a user-supplied name.
                database.query('DROP DATABASE IF EXISTS `' + stage + '`', use_database=False)

    def run(self, once=False):
        self.heartbeat()
        try:
            self.recover()
            while not STOP:
                self.heartbeat()
                job = self.claim()
                if job:
                    try:
                        if job['kind'] == 'restore':
                            self.restore(job)
                        else:
                            self.backup(job)
                    except Exception as error:
                        # Only fixed application messages reach job history, never database/tool stderr or credentials.
                        message = str(error) if type(error) is RuntimeError else 'Backup failed; check worker storage and configuration.'
                        self.finish(job, 'failed', message[:255])
                if once:
                    break
                time.sleep(2)
        finally:
            if self.owned:
                self.hub.query("UPDATE hub_backup_worker SET lease_until=NULL,targets='' WHERE id=1 AND owner=" + literal(self.owner))


def assert_local_services_stopped(worker):
    if any(db.info[0] not in ('localhost', '127.0.0.1', '::1') for db in worker.targets.values()):
        raise RuntimeError('Offline hub recovery only supports locally hosted databases.')
    if os.name == 'nt':
        command = ['powershell', '-NoProfile', '-NonInteractive', '-Command',
                   "@(Get-Process -Name hubserver,worldserver,authserver,authnetserver -ErrorAction SilentlyContinue).Count"]
        result = subprocess.run(command, check=True, capture_output=True, text=True, timeout=15, **hidden())
        stopped = result.stdout.strip() == '0'
    else:
        result = subprocess.run(['ps', '-A', '-o', 'comm='], check=True, capture_output=True, text=True, timeout=15)
        stopped = not any(Path(name.strip()).name in ('hubserver','worldserver','authserver','authnetserver') for name in result.stdout.splitlines())
    if not stopped:
        raise RuntimeError('Gracefully stop hubserver and all game services before offline recovery.')


def offline_restore_hub(worker, source_id, confirmation):
    database = worker.hub
    name = database.info[4]
    if confirmation != name or not re.fullmatch('[A-Za-z0-9_$]+', name):
        raise RuntimeError('Offline recovery requires the exact configured hub database name as confirmation.')
    assert_local_services_stopped(worker)
    if database.query("SELECT services_stopped=1 AND hub_seen<DATE_SUB(NOW(),INTERVAL 60 SECOND) AND (lease_until IS NULL OR lease_until<NOW()) FROM hub_backup_worker WHERE id=1") != '1':
        raise RuntimeError('Stop game services first, then hub and worker; wait 60 seconds before offline recovery.')
    if database.query("SELECT COUNT(*) FROM hub_backup_jobs WHERE state IN ('queued','running')") != '0':
        raise RuntimeError('Resolve pending backup or recovery jobs before offline hub recovery.')
    # Persist a start fence before touching the hub database. Normal hub starts fail closed during recovery.
    database.query('UPDATE hub_backup_worker SET maintenance=1,recovery_safe=1 WHERE id=1')
    # No worker may renew its lease while this command replaces the control database itself.
    worker.heartbeat = lambda: None
    worker.last_heartbeat = time.monotonic()
    archive, manifest = worker.load_archive(source_id, 'hub')
    stage = 'skyfire_restore_' + secrets.token_hex(16)
    journal_dir = worker.output / ('offline_restore_' + secrets.token_hex(16)); journal_dir.mkdir(mode=0o700)
    journal = {'target':'hub','sourceId':source_id,'stage':stage,'rollbackId':None,'state':'validating'}
    def save(state):
        journal['state']=state
        with (journal_dir/'recovery.json.partial').open('w',encoding='utf-8') as stream:
            json.dump(journal,stream,indent=2); stream.flush(); os.fsync(stream.fileno())
        (journal_dir/'recovery.json.partial').replace(journal_dir/'recovery.json')
    charset=database.query("SELECT CONCAT(DEFAULT_CHARACTER_SET_NAME,' ',DEFAULT_COLLATION_NAME) FROM information_schema.schemata WHERE schema_name=DATABASE()").split()
    if len(charset)!=2 or any(not re.fullmatch('[A-Za-z0-9_]+',part) for part in charset):
        raise RuntimeError('Cannot verify hub database character set.')
    create=' CHARACTER SET '+charset[0]+' COLLATE '+charset[1]
    save('validating')
    database.query('CREATE DATABASE `'+stage+'`'+create,use_database=False)
    touched=False
    try:
        worker.import_archive(database,stage,archive)
        expected=worker.table_counts(database,stage)
        if len(expected)!=manifest['tables']:
            raise RuntimeError('Staging hub restore has an unexpected table count.')
        rollback_id=secrets.token_hex(16)
        worker.backup({'id':rollback_id,'target':'hub'},publish=False)
        rollback,_=worker.load_archive(rollback_id,'hub')
        journal['rollbackId']=rollback_id
        save('replacing_live_database')
        assert_local_services_stopped(worker)
        database.query('UPDATE hub_backup_worker SET recovery_safe=0 WHERE id=1')
        touched=True
        database.query('DROP DATABASE `'+name+'`; CREATE DATABASE `'+name+'`'+create,use_database=False)
        worker.import_archive(database,name,archive)
        if worker.table_counts(database,name)!=expected:
            raise RuntimeError('Hub restore row counts differ from the verified stage.')
        # Restored backup may contain a former lease or queued backup. Fence them before the hub starts.
        database.query("UPDATE hub_backup_worker SET maintenance=1,recovery_safe=1,owner='',lease_until=NULL,targets='',services_stopped=1 WHERE id=1;"
                       "UPDATE hub_backup_jobs SET state='failed',finished_at=NOW(),message='Interrupted by offline hub recovery.' WHERE state IN ('queued','running')")
        save('completed')
    except Exception:
        if touched:
            save('rolling_back'); worker.rolling_back=True
            try:
                database.query('DROP DATABASE IF EXISTS `'+name+'`; CREATE DATABASE `'+name+'`'+create,use_database=False)
                worker.import_archive(database,name,rollback)
                database.query('DROP DATABASE `'+stage+'`; CREATE DATABASE `'+stage+'`'+create,use_database=False)
                worker.import_archive(database,stage,rollback)
                if worker.table_counts(database,name)!=worker.table_counts(database,stage):
                    raise RuntimeError('Offline rollback verification failed.')
                database.query('UPDATE hub_backup_worker SET maintenance=1,recovery_safe=1 WHERE id=1')
                save('rolled_back')
            except Exception:
                save('recovery_required')
                raise RuntimeError('Offline hub recovery and rollback failed. Keep services stopped and use the recovery journal.') from None
            finally: worker.rolling_back=False
            raise RuntimeError('Offline restore failed; original hub database restored. Review the journal before releasing maintenance.') from None
        database.query('UPDATE hub_backup_worker SET recovery_safe=1 WHERE id=1')
        save('failed_before_replacement')
        raise
    finally:
        database.query('DROP DATABASE IF EXISTS `'+stage+'`',use_database=False)
    print('Hub restored and checked. Start the hub, review recovery, then end maintenance in the Backup menu.')


def stop(signum, frame):
    global STOP
    STOP = True


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--config', type=Path, default=Path(__file__).with_name('backup.toml'))
    parser.add_argument('--once', action='store_true', help='Process at most one queued job, then exit.')
    parser.add_argument('--offline-restore-hub', metavar='ARCHIVE_ID')
    parser.add_argument('--confirm-database', default='')
    args = parser.parse_args()
    signal.signal(signal.SIGINT, stop)
    signal.signal(signal.SIGTERM, stop)
    try:
        worker = Worker(args.config)
        if args.offline_restore_hub:
            offline_restore_hub(worker,args.offline_restore_hub,args.confirm_database)
        else:
            worker.run(args.once)
    except Exception:
        # Do not leak credentials, paths, SQL, or child process environments to service logs.
        print('Backup worker stopped: check configuration, database access, migration, storage and worker ownership.', flush=True)
        return 1
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
