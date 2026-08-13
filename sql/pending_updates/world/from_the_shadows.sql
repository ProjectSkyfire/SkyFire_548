UPDATE creature_template SET ScriptName='npc_lorna_crowley_p4' WHERE entry=35378;
UPDATE creature_template SET ScriptName='npc_gilnean_mastiff' WHERE entry=35631;
UPDATE creature_template SET ScriptName='npc_bloodfang_lurker' WHERE entry=35463;

UPDATE creature_template SET npcflag=0 WHERE entry=35463;
DELETE FROM creature_queststarter WHERE id=35463 AND quest=14204;

UPDATE item_template SET spellid_1=0, spelltrigger_1=0 WHERE entry=48707;
