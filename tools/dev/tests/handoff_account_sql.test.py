"""Verify session-key provenance using a temporary, rolled-back account. No real account changes."""
import argparse
import os
from pathlib import Path
import re
import subprocess
import uuid

parser = argparse.ArgumentParser()
parser.add_argument('--auth-config',required=True)
parser.add_argument('--mysql',default='mysql')
args = parser.parse_args()
config = Path(args.auth_config).read_text(encoding='utf-8-sig')
connection = re.search(r'^\s*LoginDatabaseInfo\s*=\s*"([^"\r\n]+)"',config,re.M).group(1)
host,port,user,password,database = connection.split(';')
source = (Path(__file__).resolve().parents[3]/'src/server/shared/Database/Implementation/LoginDatabase.cpp').read_text()
def statement(name,values):
    sql = re.search(r'PrepareStatement\('+name+r', "([^"]+)"',source).group(1)
    pieces = sql.split('?')
    assert len(pieces) == len(values)+1
    return ''.join(part+value for part,value in zip(pieces,values))+pieces[-1]+';'
name = 'handofftest_'+uuid.uuid4().hex[:16]
key = "UNHEX(REPEAT('11',40))"
sql = ['START TRANSACTION;',
       "INSERT INTO account(username,salt,verifier) VALUES('"+name+"',UNHEX(REPEAT('00',32)),UNHEX(REPEAT('00',32)));",
       'SET @test_account=LAST_INSERT_ID();',
       statement('LOGIN_UPD_AUTHNET_LOGONPROOF',[key,"'127.0.0.1'",'0',"'Win'",'1','@test_account']),
       statement('LOGIN_SEL_ACCOUNT_INFO_BY_NAME',["'"+name+"'"]),
       statement('LOGIN_REP_AUTHNET_WORLD_SESSION',['@test_account',"'test-alias'",key,"UNHEX(REPEAT('22',64))",'0','1','1',"'127.0.0.1'",'0',"'Win'",'60']),
       statement('LOGIN_SEL_ACCOUNT_INFO_BY_AUTHNET_TOKEN',["'test-alias'","'127.0.0.1'"]),
       statement('LOGIN_UPD_LOGONPROOF_BY_ID',[key,"'127.0.0.1'",'0',"'Win'",'@test_account']),
       statement('LOGIN_SEL_ACCOUNT_INFO_BY_ID',['@test_account']),
       'ROLLBACK;',"SELECT COUNT(*) FROM account WHERE username='"+name+"';"]
env = os.environ.copy(); env['MYSQL_PWD'] = password
result = subprocess.run([args.mysql,'--no-defaults','--protocol=TCP','--host='+host,'--port='+port,'--user='+user,
                         '--database='+database,'--batch','--raw','--skip-column-names'],
                        input='\n'.join(sql),text=True,capture_output=True,env=env)
if result.returncode:
    raise RuntimeError(result.stderr.replace(password,'[redacted]') if password else result.stderr)
rows = result.stdout.strip().splitlines()
assert len(rows) == 4 and [row.split('\t')[-1] for row in rows] == ['1','1','0','0'], 'Incorrect session-key provenance or rollback'
print('Auth database checks passed: native keys require handoff under token and username aliases; fresh legacy login resets the flag; test account rolled back.')
