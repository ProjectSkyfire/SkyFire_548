DELETE FROM `creature` WHERE `guid` IN (167936, 167937, 167938, 167939, 167940);

UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_hogger_elwynn' WHERE `entry`=448;
