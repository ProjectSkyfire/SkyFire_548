-- The Nightmare Scar: Naralex seals the scar from his new place, and the summoned Horrors, Stalkers and Mass report back to him.
-- Based on Legends of Azeroth 5.4.8.
DELETE FROM `creature` WHERE `guid` BETWEEN 9257300 AND 9257300;
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseId`,`phaseGroup`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`) VALUES
(9257300,37570,1,1,0,0,0,0,-1705.63,-2216.93,98.8852,3.00472,300,0,0,0,0,0,0,0,0);

DELETE FROM `creature_addon` WHERE `guid` = 9257300;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES
(9257300,0,0,0,1,0,'60921');

DELETE FROM `spell_area` WHERE `spell` = 65497 AND `area` IN (4845, 4888) AND `quest_start` = 24601;
INSERT INTO `spell_area` (`spell`,`area`,`quest_start`,`quest_end`,`aura_spell`,`racemask`,`gender`,`autocast`,`quest_start_status`,`quest_end_status`) VALUES
(65497,4845,24601,0,0,0,2,1,74,11),
(65497,4888,24601,0,0,0,2,1,74,11);

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (37570, 37736, 37753, 37791);

DELETE FROM `creature_template_addon` WHERE `entry` IN (37736, 37753, 37791);
INSERT INTO `creature_template_addon` (`entry`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES
(37736,0,0,0,1,0,'39662'),
(37753,0,0,0,1,0,'39662'),
(37791,0,0,0,1,0,'39662');

DELETE FROM `creature_text` WHERE `entry` = 37570;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(37570,0,0,'I will attempt to seal the scar, $n. Make sure nothing from the other side makes its way through!',12,0,100,0,0,0,'Naralex');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 10941 AND `SourceEntry` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorType`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(15,10941,0,0,0,9,0,24601,0,0,0,0,0,'','Show gossip if player has quest 24601');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (-9257300, 37736, 37753, 37791);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` = 3757000;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-9257300, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Naralex - On Respawn - Set Phase 1'),
(-9257300, 0, 1, 2, 62, 1, 100, 0, 10941, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Naralex - On Gossip Select - Store Target List'),
(-9257300, 0, 2, 3, 61, 1, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Naralex - Linked To Id 1 - Close Gossip'),
(-9257300, 0, 3, 4, 61, 1, 100, 0, 0, 0, 0, 0, 0, 83, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Naralex - Linked To Id 2 - Remove NpcFlag 3'),
(-9257300, 0, 4, 0, 61, 1, 100, 0, 0, 0, 0, 0, 0, 1, 0, 4000, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Naralex - Linked To Id 3 - Say Line 0'),
(-9257300, 0, 5, 6, 52, 1, 100, 0, 0, 37570, 0, 0, 0, 11, 55838, 0, 0, 0, 0, 0, 10, 152984, 37522, 0, 0, 0, 0, 0, 'Naralex - On Text Over 0 - Cast Green Wyrmrest Warden Beam'),
(-9257300, 0, 6, 0, 61, 1, 100, 0, 0, 0, 0, 0, 0, 80, 3757000, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Naralex - Linked To Id 5 - Run Script'),
(-9257300, 0, 7, 0, 38, 1, 100, 0, 0, 1, 0, 0, 0, 116, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Naralex - On Data Set 0 1 - Set Counter 1 1'),
(-9257300, 0, 8, 9, 77, 1, 100, 0, 1, 3, 0, 0, 0, 116, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Naralex - On Counter 1 3 - Counter Reset'),
(-9257300, 0, 9, 10, 61, 1, 100, 0, 0, 0, 0, 0, 0, 12, 37736, 5, 0, 0, 0, 0, 8, 0, 0, 0, -1715.31, -2211.77, 99.7378, 6.24828, 'Naralex - Linked To Id 8 - Summon Creature Nightmare Stalker'),
(-9257300, 0, 10, 0, 61, 1, 100, 0, 0, 0, 0, 0, 0, 12, 37736, 5, 0, 0, 0, 0, 8, 0, 0, 0, -1719.95, -2216.9, 99.4253, 6.12611, 'Naralex - Linked To Id 9 - Summon Creature Nightmare Stalker'),
(-9257300, 0, 11, 0, 38, 1, 100, 0, 0, 2, 0, 0, 0, 116, 2, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Naralex - On Data Set 0 2 - Set Counter 2 1'),
(-9257300, 0, 12, 13, 77, 1, 100, 0, 2, 2, 0, 0, 0, 116, 2, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Naralex - On Counter 2 2 - Counter Reset'),
(-9257300, 0, 13, 0, 61, 1, 100, 0, 0, 0, 0, 0, 0, 12, 37753, 5, 0, 0, 0, 0, 8, 0, 0, 0, -1717.87, -2215.25, 99.3264, 6.0912, 'Naralex - Linked To Id 12 - Summon Creature Nightmare Mass'),
(-9257300, 0, 14, 0, 38, 1, 100, 0, 0, 3, 0, 0, 0, 67, 37570, 2000, 2000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Naralex - On Data Set 0 3 - Create Timed Event'),
(-9257300, 0, 15, 16, 59, 1, 100, 0, 37570, 0, 0, 0, 0, 92, 0, 55838, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Naralex - On Timed Event Triggered - Interrupt Spell Green Wyrmrest Warden Beam'),
(-9257300, 0, 16, 0, 61, 1, 100, 0, 0, 0, 0, 0, 0, 81, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Naralex - Linked To Id 15 - Set NpcFlag 3'),
(3757000, 9, 0, 0, 0, 1, 100, 0, 3000, 3000, 0, 0, 0, 12, 37791, 5, 0, 0, 0, 0, 8, 0, 0, 0, -1718.71, -2212.33, 99.7274, 6.24828, 'Naralex - On Script - Summon Creature Nightmare Horror'),
(3757000, 9, 1, 0, 0, 1, 100, 0, 0, 0, 0, 0, 0, 12, 37791, 5, 0, 0, 0, 0, 8, 0, 0, 0, -1715.07, -2214.93, 99.2413, 6.03884, 'Naralex - On Script - Summon Creature Nightmare Horror'),
(3757000, 9, 2, 0, 0, 1, 100, 0, 0, 0, 0, 0, 0, 12, 37791, 5, 0, 0, 0, 0, 8, 0, 0, 0, -1716.38, -2218.01, 98.7179, 6.07375, 'Naralex - On Script - Summon Creature Nightmare Horror'),
(37736, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Nightmare Stalker - On Summoned - Set React State Aggressive'),
(37736, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 20, 0, 0, 0, 0, 0, 0, 'Nightmare Stalker - Linked To Id 0 - Attack Start'),
(37736, 0, 2, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 2, 0, 0, 0, 0, 10, 9257300, 37570, 0, 0, 0, 0, 0, 'Nightmare Stalker - On Death - Set Data 0 2'),
(37753, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Nightmare Mass - On Summoned - Set React State Aggressive'),
(37753, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 10, 0, 0, 0, 0, 0, 0, 'Nightmare Mass - Linked To Id 0 - Attack Start'),
(37753, 0, 2, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 3, 0, 0, 0, 0, 10, 9257300, 37570, 0, 0, 0, 0, 0, 'Nightmare Mass - On Death - Set Data 0 3'),
(37791, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Nightmare Horror - On Summoned - Set React State Aggressive'),
(37791, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 10, 0, 0, 0, 0, 0, 0, 'Nightmare Horror - Linked To Id 0 - Attack Start'),
(37791, 0, 2, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 10, 9257300, 37570, 0, 0, 0, 0, 0, 'Nightmare Horror - On Death - Set Data 0 1');
