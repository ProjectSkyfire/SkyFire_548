"""Check realm identity and world-policy SQL using rolled-back synthetic rows."""
import argparse
import os
from pathlib import Path
import re
import subprocess
import uuid

parser = argparse.ArgumentParser()
parser.add_argument('--hub-config', required=True)
parser.add_argument('--auth-config', required=True)
parser.add_argument('--mysql', default='mysql')
args = parser.parse_args()
root = Path(__file__).resolve().parents[3]

def statement(source, name, values=()):
    match = re.search(r'PrepareStatement\('+name+r',\s*((?:"[^"]*"\s*)+),\s*CONNECTION_SYNCH', source)
    sql = ''.join(re.findall(r'"([^"]*)"', match.group(1)))
    parts = sql.split('?')
    assert len(parts) == len(values)+1
    return ''.join(part+value for part,value in zip(parts,values))+parts[-1]

def run(config, key, sql):
    text = Path(config).read_text(encoding='utf-8-sig')
    connection = re.search(r'^\s*'+key+r'\s*=\s*"([^"\r\n]+)"',text,re.M).group(1)
    host,port,user,password,database = connection.split(';')
    env = os.environ.copy(); env['MYSQL_PWD'] = password
    result = subprocess.run([args.mysql,'--no-defaults','--protocol=TCP','--host='+host,'--port='+port,
                             '--user='+user,'--database='+database,'--batch','--raw','--skip-column-names'],
                            input=';\n'.join(sql)+';',text=True,capture_output=True,env=env)
    if result.returncode:
        raise RuntimeError(result.stderr.replace(password,'[redacted]') if password else result.stderr)
    return result.stdout.strip().splitlines()

name = 'realmtest_'+uuid.uuid4().hex[:16]
source = (root/'src/server/shared/Database/Implementation/HubDatabase.cpp').read_text()
row = statement(source,'HUB_SEL_CLUSTER_POLICY')
sql = ['START TRANSACTION',
       statement(source,'HUB_UPSERT_CLUSTER_POLICY',[f"'{name}'", "'Test world'", '72', '1', "'test'", '2']),
       f"SELECT service_type,admin_state FROM ({row}) AS policies WHERE node_key='{name}'",
       statement(source,'HUB_UPSERT_CLUSTER_POLICY',[f"'{name}'", "'Test world'", '72', '0', "'test'", '2']),
       f"SELECT service_type,admin_state FROM ({row}) AS policies WHERE node_key='{name}'",
       'ROLLBACK',f"SELECT COUNT(*) FROM hub_cluster_policy WHERE node_key='{name}'"]
assert run(args.hub_config,'HubDatabaseInfo',sql) == ['2\t1','2\t0','0']

source = (root/'src/server/shared/Database/Implementation/LoginDatabase.cpp').read_text()
sql = ['START TRANSACTION',f"INSERT INTO realmlist(name,flag,gamebuild) VALUES('{name}',3,18414)"]
for query in ('LOGIN_SEL_CLUSTER_REALMLIST','LOGIN_SEL_AUTH_REALMLIST'):
    sql.append(f"SELECT COUNT(*) FROM ({statement(source,query)}) AS realms WHERE name='{name}'")
sql += ['ROLLBACK',f"SELECT COUNT(*) FROM realmlist WHERE name='{name}'"]
assert run(args.auth_config,'LoginDatabaseInfo',sql) == ['1','0','0']
print('Database checks passed: world policy role/state survive upsert; offline configured identities remain in the cluster directory; standalone filtering preserved; synthetic rows rolled back.')
