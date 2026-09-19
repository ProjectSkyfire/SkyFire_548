# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
"""Run all four recovery domains on an exclusively created local MySQL instance."""
import argparse
import importlib.util
from pathlib import Path
import secrets
import socket
import subprocess
import sys
import tempfile
import time

sys.dont_write_bytecode = True


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--mysqld', required=True)
    parser.add_argument('--mysql', required=True)
    parser.add_argument('--mysqldump', required=True)
    parser.add_argument('--work-directory', required=True, type=Path)
    parser.add_argument('--reports', required=True, type=Path)
    args = parser.parse_args()
    args.work_directory.mkdir(parents=True, exist_ok=True)
    args.reports.mkdir(parents=True, exist_ok=True)
    repo = Path(__file__).resolve().parents[3]
    spec = importlib.util.spec_from_file_location('backupserver', repo/'src/server/hub/backup/backupserver.py')
    module = importlib.util.module_from_spec(spec); spec.loader.exec_module(module)
    with tempfile.TemporaryDirectory(prefix='skyfire-recovery-mysql-', dir=args.work_directory) as temporary:
        root = Path(temporary).resolve(); module.private_directory(root)
        data = root/'data'
        subprocess.run([args.mysqld,'--no-defaults','--initialize-insecure','--datadir='+str(data)],
                       check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, **module.hidden())
        with socket.socket() as reservation:
            reservation.bind(('127.0.0.1',0)); port=reservation.getsockname()[1]
        process = subprocess.Popen([args.mysqld,'--no-defaults','--datadir='+str(data),
            '--port='+str(port),'--bind-address=127.0.0.1','--mysqlx=0','--skip-log-bin',
            '--log-error='+str(root/'mysql.log')],stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL,**module.hidden())
        database = module.Database(['127.0.0.1',str(port),'root','','mysql'],Path(args.mysql))
        try:
            deadline=time.monotonic()+60
            while True:
                if process.poll() is not None: raise RuntimeError('Disposable MySQL failed to start.')
                try:
                    database.query('SELECT 1'); break
                except RuntimeError:
                    if time.monotonic()>deadline: raise
                    time.sleep(.25)
            password=secrets.token_hex(24)
            database.query("ALTER USER 'root'@'localhost' IDENTIFIED BY '"+password+"'")
            database.info[3]=password
            connection=';'.join(database.info)
            config=root/'hub.conf'; config.write_text('HubDatabaseInfo = "'+connection+'"',encoding='utf-8')
            verification=root/'verification.conf'
            verification.write_text('VerificationDatabaseInfo = "'+connection+'"',encoding='utf-8')
            for domain in ('auth','characters','world'):
                subprocess.run([sys.executable,'-X','utf8',str(Path(__file__).with_name('manual_backup.test.py')),
                    '--hub-config',str(config),'--mysql',args.mysql,'--mysqldump',args.mysqldump,
                    '--domain',domain,'--verification-config',str(verification),'--report',str(args.reports/(domain+'.json'))],check=True)
            assert database.query("SELECT COUNT(*) FROM information_schema.schemata WHERE schema_name LIKE 'skyfire_verify_%' OR schema_name LIKE 'skyfire_restore_%' OR schema_name LIKE 'sf_backup_%'")=='0'
            assert database.query("SELECT COUNT(*) FROM mysql.user WHERE User LIKE 'sf_verify_%'")=='0'
            print('PASS all domain drills; no temporary verification users or databases remain.')
        finally:
            if process.poll() is None:
                try: database.query('SHUTDOWN')
                except Exception: pass
                try: process.wait(timeout=30)
                except subprocess.TimeoutExpired:
                    # This child owns only the newly created disposable data directory.
                    process.kill(); process.wait()


if __name__=='__main__': main()
