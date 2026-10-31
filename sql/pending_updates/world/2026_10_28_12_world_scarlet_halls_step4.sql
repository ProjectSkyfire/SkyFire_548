-- Scarlet Halls step 4: remaining LoA spawns and trash bindings. Loot and challenge mode are deferred.
DELETE FROM `creature` WHERE `guid` BETWEEN 8300245 AND 8300254;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8300245, 64738, 1001, 6, 0, 0, 0, 0, 838.312, 619.134, 12.8953, 3.94516, 7200, 0, 0, 1, 0, 0, 0, 0, 0),
(8300246, 58674, 1001, 262, 0, 0, 0, 0, 1030.96, 491.212, 13.4833, 3.08442, 7200, 0, 0, 1, 0, 0, 0, 0, 0),
(8300247, 58674, 1001, 262, 0, 0, 0, 0, 1031.91, 536.458, 13.5719, 2.02729, 7200, 0, 0, 1, 0, 0, 0, 0, 0),
(8300248, 58674, 1001, 262, 0, 0, 0, 0, 1032.18, 491.028, 13.4824, 1.76664, 7200, 0, 0, 1, 0, 0, 0, 0, 0),
(8300249, 58674, 1001, 262, 0, 0, 0, 0, 1032.39, 534.738, 13.4897, 4.98455, 7200, 0, 0, 1, 0, 0, 0, 0, 0),
(8300250, 58674, 1001, 262, 0, 0, 0, 0, 1032.44, 492.928, 13.4828, 3.86584, 7200, 0, 0, 1, 0, 0, 0, 0, 0),
(8300251, 58674, 1001, 262, 0, 0, 0, 0, 1033.54, 535.395, 13.4898, 5.4957, 7200, 0, 0, 1, 0, 0, 0, 0, 0),
(8300252, 58701, 1001, 262, 0, 0, 0, 0, 1034.02, 488.91, 13.5719, 1.53589, 7200, 0, 0, 1, 0, 0, 0, 0, 0),
(8300253, 58701, 1001, 262, 0, 0, 0, 0, 1034.29, 538.342, 13.5719, 1.53589, 7200, 0, 0, 1, 0, 0, 0, 0, 0),
(8300254, 53488, 1001, 4, 0, 0, 0, 0, 836.666, 605.141, 12.8149, 6.21127, 7200, 0, 0, 1, 0, 0, 0, 0, 0);
DELETE FROM `creature_addon` WHERE `guid` BETWEEN 8300245 AND 8300254;
DELETE FROM `waypoint_data` WHERE `id` BETWEEN 83002450 AND 83002540;
-- Source challenge-only object templates, without challenge scripts.
INSERT IGNORE INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`) VALUES
(211674, 10, 11554, 'Challenger\'s Orb', 1.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13872, 1, 0, 0, 0, 0, 0),
(211989, 0, 11638, 'Challenge Mode - Start Door (Line 2.0)', 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
DELETE FROM `gameobject` WHERE `guid` BETWEEN 910029 AND 910033;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(910029, 209349, 1001, 4, 0, 0, 997.115, 501.425, 13.4859, 0.609938, 0, 0, 0, 1, 86400, 255, 1),
(910030, 209349, 1001, 4, 0, 0, 1005.36, 466.512, 13.1043, 4.70776, 0, 0, 0, 1, 86400, 255, 1),
(910031, 2042, 1001, 262, 0, 0, 1005.36, 466.512, 13.1043, 4.70776, 0, 0, 0, 1, 7200, 255, 1),
(910032, 211674, 1001, 256, 0, 0, 837.096, 607.629, 12.814, 0.61747, 0, 0, 0.303853, 0.952719, 7200, 255, 1),
(910033, 211989, 1001, 256, 0, 0, 837.403, 603.343, 12.814, 4.72, 0, 0, -0.704411, 0.709793, 7200, 255, 1);
UPDATE `creature_template` SET `ScriptName` = 'npc_scarlet_scourge_hewer' WHERE `entry` = 58684 AND `ScriptName` IN ('', 'npc_scarlet_scourge_hewer') AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_scarlet_myrmidon' WHERE `entry` = 58683 AND `ScriptName` IN ('', 'npc_scarlet_myrmidon') AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_scarlet_evangelist' WHERE `entry` = 58685 AND `ScriptName` IN ('', 'npc_scarlet_evangelist') AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_scarlet_cannoneer' WHERE `entry` = 59293 AND `ScriptName` IN ('', 'npc_scarlet_cannoneer') AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_cannon_stalker' WHERE `entry` = 58726 AND `ScriptName` IN ('', 'npc_cannon_stalker') AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_scarlet_halls_defender' WHERE `entry` = 58676 AND `ScriptName` IN ('', 'npc_scarlet_halls_defender') AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_scarlet_evoker' WHERE `entry` = 58756 AND `ScriptName` IN ('', 'npc_scarlet_evoker') AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_scarlet_hall_guardian' WHERE `entry` = 59240 AND `ScriptName` IN ('', 'npc_scarlet_hall_guardian') AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_scarlet_treasurer' WHERE `entry` = 59241 AND `ScriptName` IN ('', 'npc_scarlet_treasurer') AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_scarlet_scholar' WHERE `entry` = 59372 AND `ScriptName` IN ('', 'npc_scarlet_scholar') AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_scarlet_pupil' WHERE `entry` = 59373 AND `ScriptName` IN ('', 'npc_scarlet_pupil') AND `AIName` = '';
