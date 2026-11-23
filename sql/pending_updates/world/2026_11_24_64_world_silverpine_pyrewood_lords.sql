-- Silverpine Forest, the Pyrewood quests: Lord Godfrey (45878), Lord Walden (45879) and Baron Ashbury (45880) are not
-- spawned in the world. As in Legends of Azeroth 5.4.8 they are summoned next to a Horde player who enters the Forsaken
-- camp at Pyrewood (areas 204, 5480 and 5481) by the auras Summon Godfrey, Summon Walden and Summon Ashbury (85517 to
-- 85519), and they die when they leave those areas. Without the auras nobody could take Of No Consequence (27547),
-- Lessons in Fear (27548), Pyrewood's Fall (27550) or any later quest of the three lords.
-- Pyrewood's Fall: the Bundle of Torches (Toss Torch, 85600) may only target the three Pyrewood Torch Targets
-- (45937 to 45939), which were not spawned and had no script to give the credit.
-- On Her Majesty's Secret Service (27594): Commander Lorna Crowley (45997) stood nowhere and her template was a level 1
-- friendly placeholder. She is spawned in Pyrewood Village with her script, the lords' conversation and the credit when
-- she falls, and the 9 lines of the scene are added.
-- Rows are as in Legends of Azeroth 5.4.8; the three Pyrewood Torch Target spawns and Crowley's spawn are its spawns.
-- The Lord Godfrey vehicle id of that source is not copied: nothing seats a player on him.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (45878, 45879, 45880, 45937, 45938, 45939, 45997) AND `ScriptName` = '';
UPDATE `creature_template` SET `minlevel` = 20, `maxlevel` = 20, `faction_A` = 2166, `faction_H` = 2166, `unit_flags` = 33280 WHERE `entry` = 45997;

DELETE FROM `spell_area` WHERE `spell` IN (85517, 85518, 85519) AND `area` IN (204, 5480, 5481);
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(85517, 204, 0, 0, 0, 33555378, 2, 1, 64, 11),
(85517, 5480, 0, 0, 0, 33555378, 2, 1, 64, 11),
(85517, 5481, 0, 0, 0, 33555378, 2, 1, 64, 11),
(85518, 204, 0, 0, 0, 33555378, 2, 1, 64, 11),
(85518, 5480, 0, 0, 0, 33555378, 2, 1, 64, 11),
(85518, 5481, 0, 0, 0, 33555378, 2, 1, 64, 11),
(85519, 204, 0, 0, 0, 33555378, 2, 1, 64, 11),
(85519, 5480, 0, 0, 0, 33555378, 2, 1, 64, 11),
(85519, 5481, 0, 0, 0, 33555378, 2, 1, 64, 11);

DELETE FROM `creature` WHERE `guid` BETWEEN 9311533 AND 9311536;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9311533, 45937, 0, 1, 0, 0, 0, 0, -339.882, 1560.44, 35.0319, 0, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9311534, 45938, 0, 1, 0, 0, 0, 0, -389.743, 1510.22, 31.7179, 0, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9311535, 45939, 0, 1, 0, 0, 0, 0, -381.773, 1608.74, 25.9594, 0, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9311536, 45997, 0, 1, 0, 0, 0, 0, -805.618, 1336.45, 33.7617, 0.837758, 300, 0, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (45878,45879,45880,45937,45938,45939,45997) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(45878, 0, 0, 0, 60, 0, 100, 0, 1, 1, 1000, 1000, 0, 37, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Godfrey - IC or OOC - Die'),
(45878, 0, 1, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Godfrey - IC or OOC - Despawn'),
(45878, 0, 2, 3, 54, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Godfrey - On Summon - Store Target'),
(45878, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Godfrey - On Summon - State Defensive'),
(45878, 0, 4, 0, 38, 0, 100, 0, 1, 0, 0, 0, 0, 80, 4587800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Godfrey - On Data Set 1 - Run Script'),
(45878, 0, 5, 0, 38, 0, 100, 0, 5, 0, 0, 0, 0, 33, 46002, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Godfrey - On Data Set 5 - Killcredit'),
(45878, 0, 6, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 11, 79849, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Godfrey - On Aggro - Cast Molten Armor'),
(45879, 0, 0, 0, 60, 0, 100, 0, 1, 1, 1000, 1000, 0, 37, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Walden - IC or OOC - Die'),
(45879, 0, 1, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Walden - IC or OOC - Despawn'),
(45879, 0, 2, 3, 54, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Walden - On Summon - Store Target'),
(45879, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Walden - On Summon - State Defensive'),
(45879, 0, 4, 5, 38, 0, 100, 0, 2, 0, 0, 0, 0, 18, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Walden - On Data Set 2 - Set Immune Flags'),
(45879, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 8, 0, 0, 0, -801.354, 1343.76, 33.9703, 0, 'Walden - On Data Set 2 - Move to Pos'),
(45879, 0, 6, 0, 38, 0, 100, 0, 3, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 19, 45997, 50, 0, 0, 0, 0, 0, 'Walden - On Data Set 2 - Set Orientation'),
(45879, 0, 7, 0, 38, 0, 100, 0, 4, 0, 0, 0, 0, 19, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Walden - On Data Set 4 - Remove Immune Flags'),
(45879, 0, 8, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 11, 48168, 34, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Walden - On Aggro - Cast Inner Fire'),
(45879, 0, 9, 0, 0, 0, 100, 0, 1, 1000, 30000, 35000, 0, 11, 11974, 34, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Walden - IC - Cast Power Word: Shield'),
(45880, 0, 0, 0, 60, 0, 100, 0, 1, 1, 1000, 1000, 0, 37, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ashbury - IC or OOC - Die'),
(45880, 0, 1, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ashbury - IC or OOC - Despawn'),
(45880, 0, 2, 3, 54, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Ashbury - On Summon - Store Target'),
(45880, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ashbury - On Summon - State Defensive'),
(45880, 0, 4, 5, 38, 0, 100, 0, 2, 1, 0, 0, 0, 18, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ashbury - On Data Set 2 1 - Set Immune Flags'),
(45880, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 8, 0, 0, 0, -799.559, 1339.91, 33.9053, 0, 'Ashbury - On Data Set 2 1 - Move to Pos'),
(45880, 0, 6, 0, 38, 0, 100, 0, 3, 1, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 19, 45997, 50, 0, 0, 0, 0, 0, 'Ashbury - On Data Set 2 1 - Set Orientation'),
(45880, 0, 7, 0, 38, 0, 100, 0, 4, 1, 0, 0, 0, 19, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ashbury - On Data Set 4 1 - Remove Immune Flags'),
(45880, 0, 8, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 11, 16592, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ashbury - On Aggro - Cast Shadowform'),
(45937, 0, 0, 1, 8, 0, 100, 0, 85600, 0, 0, 0, 0, 33, 45937, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Pyrewood Torch Target 001'),
(45938, 0, 0, 1, 8, 0, 100, 0, 85600, 0, 0, 0, 0, 33, 45938, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Pyrewood Torch Target 002'),
(45939, 0, 0, 1, 8, 0, 100, 0, 85600, 0, 0, 0, 0, 33, 45939, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Pyrewood Torch Target 003'),
(45997, 0, 0, 0, 10, 0, 100, 1, 0, 10, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 19, 45878, 50, 0, 0, 0, 0, 0, 'Commander Lorna Crowley - OOC LOS - Set Data 1'),
(45997, 0, 1, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 18, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Lorna Crowley - On Respawn - Set Immune Flags'),
(45997, 0, 2, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 19, 45878, 50, 0, 0, 0, 0, 0, 'Commander Lorna Crowley - On Data Set 1 1 - Set Orientation'),
(45997, 0, 3, 0, 38, 0, 100, 0, 3, 2, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 19, 45878, 50, 0, 0, 0, 0, 0, 'Commander Lorna Crowley - On Data Set 3 2 - Set Orientation'),
(45997, 0, 4, 0, 38, 0, 100, 0, 4, 2, 0, 0, 0, 19, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Lorna Crowley - On Data Set 4 2 - Remove Immune Flags'),
(45997, 0, 5, 6, 6, 0, 100, 0, 0, 0, 0, 0, 0, 45, 5, 0, 0, 0, 0, 0, 9, 45878, 0, 50, 0, 0, 0, 0, 'Commander Lorna Crowley - On Death - Set Data 5'),
(45997, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Lorna Crowley - On Death - Say Line 4'),
(45997, 0, 7, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 11, 80983, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Lorna Crowley - On Aggro - Cast Commanding Shout');

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (4587800) AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4587800, 9, 0, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 18, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Godfrey - Script - Set Immune Flags'),
(4587800, 9, 1, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 19, 45997, 50, 0, 0, 0, 0, 0, 'Godfrey - Script - Set Data 1 1'),
(4587800, 9, 2, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 45997, 50, 0, 0, 0, 0, 0, 'Godfrey - Script - Say Line 0'),
(4587800, 9, 3, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 8, 0, 0, 0, -799.384, 1342.61, 33.9407, 0, 'Godfrey - Script - Move to Pos'),
(4587800, 9, 4, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 2, 0, 0, 0, 0, 0, 19, 45879, 50, 0, 0, 0, 0, 0, 'Godfrey - Script - Set Data 2'),
(4587800, 9, 5, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 2, 1, 0, 0, 0, 0, 19, 45880, 50, 0, 0, 0, 0, 0, 'Godfrey - Script - Set Data 2 1'),
(4587800, 9, 6, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 19, 45997, 50, 0, 0, 0, 0, 0, 'Godfrey - Script - Set Orientation'),
(4587800, 9, 7, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 3, 0, 0, 0, 0, 0, 19, 45879, 50, 0, 0, 0, 0, 0, 'Godfrey - Script - Set Data 3'),
(4587800, 9, 8, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 3, 1, 0, 0, 0, 0, 19, 45880, 50, 0, 0, 0, 0, 0, 'Godfrey - Script - Set Data 3 1'),
(4587800, 9, 9, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 3, 2, 0, 0, 0, 0, 19, 45997, 50, 0, 0, 0, 0, 0, 'Godfrey - Script - Set Data 3 2'),
(4587800, 9, 10, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Godfrey - Script - Say Line 0'),
(4587800, 9, 11, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 19, 45997, 50, 0, 0, 0, 0, 0, 'Godfrey - Script - Say Line 1'),
(4587800, 9, 12, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 19, 45997, 50, 0, 0, 0, 0, 0, 'Godfrey - Script - Say Line 2'),
(4587800, 9, 13, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Godfrey - Script - Say Line 1'),
(4587800, 9, 14, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Godfrey - Script - Say Line 2'),
(4587800, 9, 15, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Godfrey - Script - Say Line 3'),
(4587800, 9, 16, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 19, 45997, 50, 0, 0, 0, 0, 0, 'Godfrey - Script - Say Line 3'),
(4587800, 9, 17, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 19, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Godfrey - Script - Remove Immune Flags'),
(4587800, 9, 18, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 4, 0, 0, 0, 0, 0, 19, 45879, 50, 0, 0, 0, 0, 0, 'Godfrey - Script - Set Data 4'),
(4587800, 9, 19, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 4, 1, 0, 0, 0, 0, 19, 45880, 50, 0, 0, 0, 0, 0, 'Godfrey - Script - Set Data 4 1'),
(4587800, 9, 20, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 4, 2, 0, 0, 0, 0, 19, 45997, 50, 0, 0, 0, 0, 0, 'Godfrey - Script - Set Data 4 2');


DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 1 AND `SourceEntry` IN (45878, 45879, 45880) AND `SourceId` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 1, 45878, 0, 0, 23, 1, 204, 0, 0, 1, 0, 0, '', 'The lord dies when he is not in the camp areas'),
(22, 1, 45878, 0, 0, 23, 1, 5480, 0, 0, 1, 0, 0, '', 'The lord dies when he is not in the camp areas'),
(22, 1, 45878, 0, 0, 23, 1, 5481, 0, 0, 1, 0, 0, '', 'The lord dies when he is not in the camp areas'),
(22, 1, 45879, 0, 0, 23, 1, 204, 0, 0, 1, 0, 0, '', 'The lord dies when he is not in the camp areas'),
(22, 1, 45879, 0, 0, 23, 1, 5480, 0, 0, 1, 0, 0, '', 'The lord dies when he is not in the camp areas'),
(22, 1, 45879, 0, 0, 23, 1, 5481, 0, 0, 1, 0, 0, '', 'The lord dies when he is not in the camp areas'),
(22, 1, 45880, 0, 0, 23, 1, 204, 0, 0, 1, 0, 0, '', 'The lord dies when he is not in the camp areas'),
(22, 1, 45880, 0, 0, 23, 1, 5480, 0, 0, 1, 0, 0, '', 'The lord dies when he is not in the camp areas'),
(22, 1, 45880, 0, 0, 23, 1, 5481, 0, 0, 1, 0, 0, '', 'The lord dies when he is not in the camp areas');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 1 AND `SourceEntry` = 85600;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 85600, 0, 0, 31, 0, 3, 45937, 0, 0, 0, 0, '', 'Toss Torch targets the Pyrewood Torch Targets'),
(13, 1, 85600, 0, 1, 31, 0, 3, 45938, 0, 0, 0, 0, '', 'Toss Torch targets the Pyrewood Torch Targets'),
(13, 1, 85600, 0, 2, 31, 0, 3, 45939, 0, 0, 0, 0, '', 'Toss Torch targets the Pyrewood Torch Targets');

DELETE FROM `creature_text` WHERE `entry` IN (45878, 45997);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(45878, 0, 0, 'Lorna, is that how you greet old friends? ', 12, 0, 100, 6, 0, 0, 'Lord Godfrey - line 0'),
(45878, 1, 0, 'My kingdom? My kingdom is no more.', 12, 0, 100, 6, 0, 0, 'Lord Godfrey - line 1'),
(45878, 2, 0, 'Your father and Greymane saw to that... No, Gilneas died when they allowed murderous beasts to reign free. Unchecked.', 12, 0, 100, 25, 0, 0, 'Lord Godfrey - line 2'),
(45878, 3, 0, 'Now lay down your arms and surrender. We are not here to kill you. Not yet.', 12, 0, 100, 1, 0, 0, 'Lord Godfrey - line 3'),
(45997, 0, 0, 'So the Banshee Queen has sent assassins?', 12, 0, 100, 1, 0, 0, 'Commander Lorna Crowley - line 0'),
(45997, 1, 0, 'Godfrey? So the rumors are true.', 12, 0, 100, 397, 0, 0, 'Commander Lorna Crowley - line 1'),
(45997, 2, 0, 'You would betray your kingdom out of spite?', 12, 0, 100, 6, 0, 0, 'Commander Lorna Crowley - line 2'),
(45997, 3, 0, 'HAH! You''ll have to kill me! DIE!', 12, 0, 100, 11, 0, 0, 'Commander Lorna Crowley - line 3'),
(45997, 4, 0, '%s falls unconscious.', 16, 0, 100, 0, 0, 0, 'Commander Lorna Crowley - line 4');

UPDATE `creature_template` SET `lootid` = 45995 WHERE `entry` = 45995 AND `lootid` = 0;
DELETE FROM `creature_loot_template` WHERE `entry` = 45995 AND `item` = 61510;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(45995, 61510, -17.8672, 1, 0, 1, 1);
