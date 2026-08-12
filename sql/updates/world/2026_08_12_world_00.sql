DELETE FROM smart_scripts WHERE entryorguid=50260 AND source_type=0;

UPDATE creature_template SET AIName='', ScriptName='npc_flee_from_player' WHERE entry=50260;
