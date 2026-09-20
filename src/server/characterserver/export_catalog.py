# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
"""Export the shared character statement ABI; run after changing CharacterDatabase.*."""
import argparse
import ast
import json
from pathlib import Path
import re


def catalog(source):
    root = Path(source) / 'src/server/shared/Database/Implementation'
    header = (root / 'CharacterDatabase.h').read_text()
    header = re.sub(r'/\*.*?\*/|//[^\n]*', '', header, flags=re.S)
    block = re.search(r'enum CharacterDatabaseStatements\s*\{(.*?)\}', header, re.S)[1]
    names = [name.strip() for name in block.split(',') if name.strip()]
    if names[-1] != 'MAX_CHARACTERDATABASE_STATEMENTS' or any(not re.fullmatch(r'CHAR_[A-Z0-9_]+', name) for name in names[:-1]):
        raise ValueError('Character statement enum needs an explicit exporter update')
    indexes = {name:index for index,name in enumerate(names[:-1])}
    source_text = (root / 'CharacterDatabase.cpp').read_text()
    # Remove only actual comments; quoted SQL strings can contain comment-like text.
    source_text = re.sub(r'"(?:\\.|[^"\\])*"|/\*.*?\*/|//[^\n]*',
                         lambda match: match[0] if match[0].startswith('"') else '', source_text, flags=re.S)
    pattern = r'PrepareStatement\(\s*(CHAR_[A-Z0-9_]+)\s*,\s*((?:"(?:\\.|[^"\\])*"\s*)+)\s*,\s*CONNECTION_(?:ASYNC|SYNCH|BOTH)\s*\);'
    result = []
    for name, literals in re.findall(pattern, source_text):
        sql = ''.join(ast.literal_eval(value) for value in re.findall(r'"(?:\\.|[^"\\])*"', literals))
        result.append(dict(id=indexes[name], name=name, sql=sql))
    definitions = len(re.findall(r'\bPrepareStatement\(', source_text))
    if len(result) != definitions or len({entry['id'] for entry in result}) != definitions:
        raise ValueError('Every defined character statement must be exported exactly once')
    return sorted(result, key=lambda entry:entry['id'])


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--source', required=True)
    parser.add_argument('--output', required=True)
    args = parser.parse_args()
    Path(args.output).write_text(json.dumps(catalog(args.source), ensure_ascii=True, indent=2) + '\n', encoding='utf-8')
