# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
"""Run schedule integration tests against a disposable copy of the hub schema only.

Requires CREATE/DROP DATABASE for the test identity. Copies no production data and starts
no managed game processes. Arguments match hub_control.test.py; --mysqldump is optional.
"""
import argparse
import os
from pathlib import Path
import re
import secrets
import subprocess
import sys
import tempfile


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--hub', required=True)
    parser.add_argument('--hub-config', required=True)
    parser.add_argument('--php', required=True)
    parser.add_argument('--mysql', default='mysql')
    parser.add_argument('--mysqldump', default='mysqldump')
    parser.add_argument('--openssl', default='openssl')
    args = parser.parse_args()
    config = Path(args.hub_config).read_text(encoding='utf-8-sig')
    match = re.search(r'^\s*HubDatabaseInfo\s*=\s*"([^"\r\n]+)"', config, re.M)
    host, port, user, password, source = match.group(1).split(';')
    database = 'skyfire_controltest_' + secrets.token_hex(8)
    base = ['--no-defaults', '--protocol=TCP', '--host=' + host, '--port=' + port, '--user=' + user]
    env = os.environ.copy(); env['MYSQL_PWD'] = password; env['PYTHONDONTWRITEBYTECODE'] = '1'
    def run(executable, arguments, sql=None):
        result = subprocess.run([executable, *base, *arguments], input=sql, text=True, capture_output=True, env=env)
        if result.returncode:
            raise RuntimeError(result.stderr.replace(password, '[redacted]') if password else result.stderr)
        return result.stdout
    schema = run(args.mysqldump, ['--no-data', '--skip-lock-tables', '--no-tablespaces', '--column-statistics=0', '--set-gtid-purged=OFF', source])
    run(args.mysql, [], 'CREATE DATABASE `' + database + '`')
    try:
        run(args.mysql, ['--database=' + database], schema)
        migration = Path(__file__).resolve().parents[3] / 'sql/updates/hub/2026_09_18_hub_03.sql'
        run(args.mysql, ['--database=' + database], migration.read_text())
        manual = Path(__file__).resolve().parents[3] / 'sql/updates/hub/2026_09_19_hub_00.sql'
        run(args.mysql, ['--database=' + database], manual.read_text())
        if run(args.mysql, ['--batch','--skip-column-names','--database='+database],
               "SELECT COUNT(*) FROM information_schema.columns WHERE table_schema=DATABASE() AND table_name='hub_backup_jobs' AND column_name='verified_at'").strip() == '0':
            automation = Path(__file__).resolve().parents[3] / 'sql/updates/hub/2026_09_19_hub_01.sql'
            run(args.mysql, ['--database=' + database], automation.read_text())
        new_connection = ';'.join((host, port, user, password, database))
        config = config[:match.start(1)] + new_connection + config[match.end(1):]
        # These tests never need access to game-account data.
        config = re.sub(r'^LoginDatabaseInfo\s*=.*$', 'LoginDatabaseInfo = ""', config, flags=re.M)
        with tempfile.TemporaryDirectory(prefix='skyfire-backup-schedule-') as temporary:
            path = Path(temporary) / 'hub.conf'; path.write_text(config)
            command = [sys.executable, str(Path(__file__).with_name('hub_control.test.py')),
                '--hub', args.hub, '--hub-config', str(path), '--php', args.php,
                '--mysql', args.mysql, '--openssl', args.openssl, '--test-backup-schedules']
            subprocess.run(command, check=True, env=env)
    finally:
        assert re.fullmatch(r'skyfire_controltest_[0-9a-f]{16}', database) and database != source
        run(args.mysql, [], 'DROP DATABASE `' + database + '`')
    print('PASS isolated schedule test database removed; deployment data and processes untouched.')


if __name__ == '__main__':
    main()
