# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
"""Disposable MySQL fixture for restart maintenance admission; never edits the configured database."""
import argparse
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
import re
import secrets
import sys
sys.dont_write_bytecode = True
REPO = Path(__file__).resolve().parents[2]
sys.path.insert(0,str(REPO/'src/server/hub/backup'))
from backupserver import Database, connection

def exercise(config,mysql):
    info=connection(config,'HubDatabaseInfo')
    name='sf_restart_test_'+secrets.token_hex(8)
    assert re.fullmatch('sf_restart_test_[0-9a-f]{16}',name)
    db=Database(info[:-1]+[name],mysql)
    db.query('CREATE DATABASE '+name,use_database=False)
    try:
        db.query("CREATE TABLE hub_backup_worker(id int PRIMARY KEY,maintenance int NOT NULL,recovery_safe int NOT NULL,services_stopped int NOT NULL) ENGINE=InnoDB; INSERT INTO hub_backup_worker VALUES(1,0,1,0); CREATE TABLE hub_backup_jobs(state varchar(32)); CREATE TABLE hub_backup_cycles(active_slot int);")
        db.query((REPO/'sql/pending_updates/hub/002_node_restarts.sql').read_text())
        # Extract the production conditional write, retaining exactly its SQL predicates.
        source=(REPO/'src/server/hub/HubNodeRestart.cpp').read_text()
        start=source.index('UPDATE hub_backup_worker SET maintenance=1,services_stopped=0,restart_owner=')
        end=source.index(').c_str());',start)
        fragments=re.findall(r'"([^"\n]*)"', '"'+source[start:end])
        statement=''.join(fragments)
        statement=statement.replace("restart_owner=''","restart_owner='{token}'")
        assert '{token}' in statement and 'NOT EXISTS' in statement
        def claim(token):
            db.query(statement.format(token=token))
            return db.query("SELECT restart_owner='"+token+"' FROM hub_backup_worker WHERE id=1")=='1'
        def reset(): db.query("UPDATE hub_backup_worker SET maintenance=0,restart_owner='',recovery_safe=1; DELETE FROM hub_backup_jobs; DELETE FROM hub_backup_cycles;")
        for state in ('queued','running','deleting'):
            reset();db.query("INSERT INTO hub_backup_jobs VALUES('"+state+"')")
            assert not claim('a'*32),state
        reset();db.query('INSERT INTO hub_backup_cycles VALUES(1)');assert not claim('a'*32)
        reset();db.query('UPDATE hub_backup_worker SET recovery_safe=0');assert not claim('a'*32)
        reset();db.query('UPDATE hub_backup_worker SET maintenance=1');assert not claim('a'*32)
        reset()
        with ThreadPoolExecutor(2) as pool: results=list(pool.map(claim,['a'*32,'b'*32]))
        assert sum(results)==1,'Concurrent restart claims must have one owner'
        assert db.query('SELECT maintenance FROM hub_backup_worker')=='1'
        assert not claim('c'*32),'Interrupted restart must retain maintenance fencing'
        print('PASS: pending migration, queued/running/deleting jobs, active cycle, recovery safety, existing maintenance, concurrent owner claim, interrupted ownership.')
    finally:
        assert re.fullmatch('sf_restart_test_[0-9a-f]{16}',name)
        db.query('DROP DATABASE '+name,use_database=False)

if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--hub-config',type=Path,required=True);parser.add_argument('--mysql',type=Path,required=True)
    args=parser.parse_args();exercise(args.hub_config,args.mysql)
