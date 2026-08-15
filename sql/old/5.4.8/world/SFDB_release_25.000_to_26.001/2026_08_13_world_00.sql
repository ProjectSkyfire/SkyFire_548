UPDATE creature_template SET ScriptName='npc_sean_dempsey' WHERE entry=35081;
UPDATE creature_template SET ScriptName='npc_lord_darius_crowley_c1' WHERE entry=35077;
UPDATE creature_template SET ScriptName='npc_worgen_runt_c1' WHERE entry=35188;
UPDATE creature_template SET ScriptName='npc_worgen_runt_c2' WHERE entry=35456;
UPDATE creature_template SET ScriptName='npc_worgen_alpha_c1' WHERE entry=35170;
UPDATE creature_template SET ScriptName='npc_worgen_alpha_c2' WHERE entry=35167;

UPDATE creature_template SET faction_A=2179, faction_H=2179 WHERE entry IN (35167,35170,35188);
