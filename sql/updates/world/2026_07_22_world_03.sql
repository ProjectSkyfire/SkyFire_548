UPDATE `creature_template` SET `ScriptName`='npc_transient' WHERE `entry`=42383;
UPDATE `creature_template` SET `ScriptName`='npc_transient' WHERE `entry`=42384;
UPDATE `creature_template` SET `ScriptName`='npc_transient' WHERE `entry`=42386;
UPDATE `creature_template` SET `ScriptName`='npc_transient' WHERE `entry`=42391;

UPDATE `creature` SET spawndist=5, MovementType=1 WHERE guid IN
(171550, 171551, 171941, 171575, 171471, 171940, 171939, 171479, 171481, 171476,
171475, 171475, 171938, 171474, 171778, 171482, 171483, 172007, 171480, 171937,
171478, 171477, 172004);
