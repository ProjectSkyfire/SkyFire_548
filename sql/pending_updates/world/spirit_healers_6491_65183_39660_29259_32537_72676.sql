-- SkyFireParser NPC spawn import
-- Entries: 6491, 65183, 39660, 29259, 32537, 72676
-- Generated: 2026-07-26T13:22:19.5342690+00:00
-- Review @CGUID before applying; source creature.guid values are left in comments.
-- Reserved free creature guid range in sf_test_world at generation time: 16473-16990
-- Targeted replacement: deletes only these creature entries and the reserved generated guid range.

-- Source spawn counts:
-- entry	name	source_spawn_count
-- 6491	    Spirit Healer	401
-- 65183	Spirit Healer	9
-- 39660	Spirit Healer	35
-- 29259	Spirit Healer	63
-- 32537	Spirit Healer	0
-- 72676	Spirit Healer	10

DELETE FROM `creature` WHERE `id` IN (6491, 65183, 39660, 29259, 32537, 72676);
DELETE FROM `creature` WHERE `guid` BETWEEN 16473 AND 16990;
SET @CGUID := 16472;

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -1497.46, 2648.77, -52.0841, 1.66914, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 12841.1, -6850.52, 12.9348, 5.63261, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 571, 1, 0, 0, 0, 0, 6443.65, 2064.7, 563.455, 5.5162, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 555.058, 1999.04, 105.219, 3.75663, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 3441.65, 4799.2, 260.232, 0.863293, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 6669.36, -6301.85, 29.2549, 3.00984, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -4326.48, -12436.5, 19.9495, 5.46006, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -4115.04, -13666.3, 77.6747, 2.51401, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 2213.91, 6019.99, 136.558, 5.63741, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 3054.01, 5438.67, 149.32, 5.46288, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -1787.52, 4936.03, -22.3513, 3.76991, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -1648.81, 7926.99, -43.2942, 2.95921, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 7012.19, -7312.99, 46.6976, 1.06491, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -1748.33, -11061.2, 80.795, 4.28905, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -3327.42, -12094.4, 28.3907, 1.55194, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -2008.4, -11975.8, 33.2209, 3.16193, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 7700.75, -6731.16, 48.408, 2.82743, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 9417.34, -6853.94, 14.9242, 2.74177, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 8704.18, -6671.09, 70.3358, 6.00507, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 8940.13, -7443.68, 82.8267, 2.29799, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 10463.9, -6370.87, 39.7914, 2.34834, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 2991.62, 1860.71, 143.507, 2.51749, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -815.271, 2725.87, 111.596, 5.01686, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -871.072, 6619.03, 180.867, 5.40214, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -2030.79, 8480.91, -0.261882, 0.558505, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -1087.59, 8066.38, -41.415, 0.017453, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -2505.99, 6801.13, 22.698, 6.24828, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -3561.56, 4672.69, -21.8671, 4.55531, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 694.175, 2220.07, 288.296, 5.13254, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 177.377, 4363.37, 116.79, 5.95802, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -1133.53, 5896.92, 191.047, 3.13937, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -3131.03, 723.833, -20.3191, 4.08407, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -4010.41, 2054.34, 97.9906, 3.4383, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 3520.16, 6827.02, 141.114, 5.14214, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -3010.49, 483.827, -13.553, 0.892296, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -700.959, 4110.22, 63.3266, 1.46686, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -1509.23, 3280.63, -15.874, 5.84475, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 889.877, 7285.43, 21.6225, 3.1813, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 4374.86, 3104.45, 132.979, 3.91101, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -11219.1, 1703.79, 39.0478, 1.62455, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -2996.54, 2456.49, 62.6057, 4.50108, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 2299.27, 7019.98, 364.912, 2.49162, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -4126.66, 1063.25, 31.8112, 5.8113, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 2018.33, 5571.27, 263.043, 2.654, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -3528.89, 610.907, 5.00913, 1.53999, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 1949.96, 6758.89, 164.578, 1.01273, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -3948.7, 3679.95, 286.822, 0.591757, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 3872.78, 5492.54, 272.181, 1.23656, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 6725.31, -7948.79, 170.1, 1.21568, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -248.839, 1014.88, 54.3281, 1.61946, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 165.612, 2571.6, 79.3147, 3.91696, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -1264.2, 2442.64, 61.479, 3.99418, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -212.748, 5595.01, 20.7616, 4.97419, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 228.258, 6200.58, 22.2169, 3.24355, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 253.195, 7050.25, 35.1273, 3.75296, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 348.013, 7628.03, 23.1301, 3.12589, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 319.032, 8622.73, 23.1847, 5.60295, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 2314.33, 5082.34, 272.349, 2.4597, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 3015.99, 3597.32, 145.761, 5.9987, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, 2358.43, 2319.9, 106.134, 1.37673, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 530, 1, 0, 0, 0, 0, -2540.6, 3870.18, 10.0669, 5.39263, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -10350.1, -2574.27, 23.8792, 5.14836, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -7986.89, -2355.5, 124.949, 4.57919, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -3347.18, -3414.44, 64.4871, 5.0871, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 5233, 0, 575.198, -3826.77, 120.303, 0.017453, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 646, 1, 0, 0, 5233, 0, 1978.73, 531.137, 36.5403, 4.66003, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 5233, 0, 2442.74, -2952.63, 123.782, 3.19395, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 5233, 0, 3364.35, -4426.29, 282.186, 1.20428, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 5233, 0, 4798.39, -6845.41, 90.3777, 3.14159, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 5233, 0, -3321.35, 1845.88, 60.2404, 2.33874, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 5233, 0, -3330.8, 2283.64, 29.2127, 4.7822, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 5233, 0, 277.846, -3315.37, 56.6778, 2.86234, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 5233, 0, -3331.61, -2231.73, 91.7834, 2.60054, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 5233, 0, 778.646, 384.097, 71.6102, 3.22886, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 5233, 0, 1019.44, 1620.33, 25.9867, 4.79965, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 571, 1, 0, 0, 0, 0, 1206.44, -4114.87, 146.592, 3.24631, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 571, 1, 0, 0, 0, 0, 2002.74, 5938.1, 37.1899, 5.93412, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 571, 1, 0, 0, 0, 0, 2844.86, 6662.14, 36.3519, 4.62512, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 571, 1, 0, 0, 0, 0, 3041.62, 4322.89, 29.7413, 4.67748, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 571, 1, 0, 0, 0, 0, 3041.89, 3848.5, 5.06485, 4.72984, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 571, 1, 0, 0, 0, 0, 3214.61, -701.304, 167.723, 0.069813, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 571, 1, 0, 0, 0, 0, 3503.54, 2101.82, 69.6656, 3.36848, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 571, 1, 0, 0, 0, 0, 3546.87, 273.161, 45.6924, 3.24631, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 571, 1, 0, 0, 0, 0, 3613.33, 6804.92, 173.035, 0.139626, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 571, 1, 0, 0, 0, 0, 4106.77, -4816.89, 76.1592, 0, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 571, 1, 0, 0, 0, 0, 5557.67, -1607.75, 242.33, 4.79965, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 571, 1, 0, 0, 0, 0, 5707.56, -2468.35, 287.644, 3.15905, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 571, 1, 0, 0, 0, 0, 5791.17, -3293.93, 365.633, 0.680678, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 571, 1, 0, 0, 0, 0, 8466.53, 442.54, 596.155, 1.5708, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 571, 1, 0, 0, 0, 0, 9021.95, -1161.56, 1059.42, 4.60767, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 861, 1, 0, 0, 5233, 0, 1404.19, 234.313, 61.6853, 2.25148, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 5233, 0, 9683.94, 953.23, 1291.72, 5.67232, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 571, 1, 0, 0, 5233, 0, 6612.81, -4800.43, 450.605, 0.750492, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 5233, 0, -8562.7, -4477.47, -213.757, 0.680678, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 5233, 0, 8304.64, 950.09, 14.0221, 2.07694, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 5233, 0, 8691.74, 949.535, 2.23055, 5.67232, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 5233, 0, 1183.11, -4458.11, 21.3981, 4.15388, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 5233, 0, 1916.72, -4342.61, 24.2224, 5.81195, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 1339.99, -4477.61, 25.5879, 2.69681, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -12650.8, -2749.29, 1.40498, 4.75072, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, 1753.08, 1585.8, 112.278, 1.52353, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -9340.94, 164.66, 61.5587, 1.07398, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -9559.79, -604.057, 58.4964, 3.17099, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -9553.59, -1368.61, 51.276, 5.07477, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -10513.3, 1180.91, 31.7867, 3.96346, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -9974.21, 1757.88, 37.5107, 3.35085, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -10985.9, 1625.19, 45.4717, 5.1675, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -11224.9, 1615.64, 32.6436, 4.64323, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -10837.5, -486.576, 42.8429, 1.76123, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -10581.4, 294.679, 30.7886, 3.11682, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -10780.8, -1195.55, 35.7693, 0.632591, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 4023.6, -5443.89, 115.798, 0.348319, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -9392.39, -2019.38, 58.4465, 4.27841, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -9188.79, -2306.29, 90.1269, 3.77418, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -9476.21, -3008.95, 134.501, 0.279144, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -11551.4, -227.303, 28.2452, 5.52171, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -11993.1, 430.405, 2.06373, 3.50951, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -12544.4, -585.417, 39.9823, 3.61003, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -13315.4, 156.13, 17.3477, 3.42468, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -14288.4, 282.216, 32.8061, 1.42505, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -12186.6, -2565.22, 3.99378, 5.56243, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -11812.2, -2954.8, 7.53578, 4.64272, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -12188.7, -3279.94, 58.2791, 3.38058, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -10853.4, -2947.42, 13.2267, 2.66637, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -11110.4, -1833.24, 71.8642, 3.04726, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -7923.56, -1353.23, 134.079, 3.10615, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -7317.94, -1648.82, 282.024, 5.49298, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -6867.31, -1539.84, 241.815, 3.78946, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -6865.55, -1085.4, 240.014, 1.72237, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -6432.8, -1129.57, 316.414, 2.08438, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -6981.6, -2327.72, 241.915, 4.77908, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -7101.03, -3488.31, 242.061, 0.149404, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -6288.81, -3495.4, 251.759, 1.85896, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -9955.71, -3917.06, 23.3458, 1.49706, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -10637.4, -4010.93, 24.0957, 4.64965, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -10567.8, -3377.2, 22.2532, 0.463718, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -5351.23, -2881.58, 340.942, 1.89238, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -5329.98, -3779.33, 310.214, 3.27938, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -5475.18, -1845.84, 399.786, 4.16374, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -5165.52, -874.664, 507.177, 0.888639, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -5210.32, -444.178, 386.333, 3.94619, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -5680.04, -518.92, 396.274, 2.42801, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -5278.01, 320.19, 396.817, 3.98232, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -5119.52, 896.673, 283.769, 5.45257, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -3351.19, -845.896, 1.05955, 4.81562, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -2953.31, -1753.63, 9.57529, 4.87664, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -3948.03, -2877.8, 12.9097, 2.55814, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -4147.41, -4774.48, 119.263, 5.68791, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -3713.28, -5184.28, 41.3112, 3.90584, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -4197.24, -5462.79, 20.3741, 3.26652, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -4669.46, -6367.3, 12.4303, 3.87527, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -2783.43, -5748.71, 342.392, 2.90136, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -3058.89, -4097.95, 266.496, 2.63275, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -1315.55, -3184.51, 37.3032, 5.66597, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -1346.91, -2046.51, 71.2412, 5.96285, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -14.5479, -992.412, 55.9217, 1.41542, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -754.7, -587.971, 17.7663, 0.076312, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -561.995, 122.37, 54.1326, 2.72939, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, 794.556, -422.241, 135.787, 5.5144, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -251.024, 1521.84, 77.0363, 0.536741, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, 1766.7, -671.916, 43.7461, 3.6852, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, 2612.32, -532.802, 88.9998, 4.1319, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, 2841.17, -688.047, 139.329, 5.17475, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, 1883.72, -1685.39, 59.8284, 1.64676, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, 333.738, -2228.58, 137.088, 3.17986, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -187.111, -4346.77, 113.289, 2.22323, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, 1382.98, -3700.87, 76.8748, 6.20442, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, 1986.98, -3651.75, 120.201, 3.85135, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, 2048.81, -5026.21, 75.4181, 0.448974, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, 2646.16, -4012.36, 106.589, 5.68329, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -6831, -2885.87, 8.89237, 0.09265, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -7142.07, -3876.89, 10.4365, 4.54505, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -7755.32, -4978.22, 4.05656, 1.36417, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -8643.99, -4028.61, 44.6902, 5.00294, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -9605.43, -3639.58, 13.3005, 1.91866, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -9049.05, -2726.08, 37.3299, 0.556007, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -7745.67, -3014.53, 40.6366, 2.36322, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -7077.26, -2391.64, -165.643, 4.28744, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -6107.59, -1344.02, -179.34, 2.63419, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -7341.45, -1324.5, -258.669, 2.26662, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -8917.68, -1611.37, 95.6918, 4.6236, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -9372.39, -1066.86, 120.092, 3.02688, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -10846.2, -1591.67, 9.78209, 4.81675, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -11546.3, -2338.79, 625.699, 1.01699, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -10025.4, 417.321, 38.551, 3.21767, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -7972.51, 787.331, -0.783952, 5.5354, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -8026.65, 1602.06, 13.8791, 3.52035, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -7059.65, 1287.74, 6.28425, 0.204411, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -6823.67, 892.906, 33.9618, 3.05934, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -6440.7, -289.145, 3.72787, 0.886915, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -5535.99, 1459.13, 24.8974, 5.78369, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -4610.46, 1635.57, 93.9249, 6.09783, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -3790.81, 1255.52, 160.254, 4.65736, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -4466.8, 2138.55, 7.55253, 2.77712, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -4588.21, 3250.42, 8.96212, 4.01177, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -1776.54, 2854.55, 57.7043, 2.9499, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -1559.75, 982.519, 90.4885, 6.22973, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -474.229, 1228.37, 95.2118, 4.00784, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -453.048, 2513.65, 92.8398, 6.09937, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -1171.79, -1143.4, 34.4302, 0.979336, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -2732.32, -1108, 24.4355, 4.57332, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -2175.01, -343.776, -5.26389, 3.47376, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -2944.82, -153.509, 69.9713, 5.3108, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -2379.65, -1974.58, 96.3395, 2.59096, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -1893.97, -3059.87, 91.6652, 0.513641, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -1484.51, -2138.24, 89.066, 4.937, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -1172.15, -1713.33, 91.6644, 5.60066, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -3516.2, -4316.06, 7.19682, 3.09995, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -4031.84, -3418.76, 39.2476, 1.76322, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -6180.54, -3992.85, 1.72375, 1.91488, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -6212.39, -4581.53, 93.6144, 1.22372, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -5347.12, -3941.22, 85.8656, 2.37274, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -5440.54, -2289.9, 89.439, 0.4226, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -4603.65, -1904.34, 88.1406, 1.94784, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -4874.51, -2162.13, 0.870021, 2.15594, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -839.54, -1982.03, 91.8002, 2.96045, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -590.233, -2516.49, 91.7573, 4.34195, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -229.931, -3013.62, 91.6679, 0.434656, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -1084.74, -3478.63, 63.7432, 6.26232, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 803.927, -2540.83, 91.668, 3.05633, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 1291.19, -298.418, 6.67261, 3.39404, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -104.777, -700.162, 4.12326, 2.17824, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 465.11, 1467.78, 13.5006, 0.344325, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 1689.14, 1051.56, 149.411, 5.50126, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 2735.86, 1279.28, 296.35, 2.43662, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 2912.99, 380.541, 91.6667, 6.19552, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 3812.46, 758.439, 8.32688, 0.870489, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 2634.18, -632.742, 107.959, 4.91921, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 2301.68, -1730.46, 120.162, 3.15994, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 1345.6, -4504.94, 27.3019, 3.21796, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 245.36, -4792.69, 10.3145, 3.30043, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -764.178, -4985.67, 20.812, 3.42371, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 2595.58, -4773.03, 152.234, 2.75063, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 3050.27, -4122.38, 103.599, 2.98702, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 2711.57, -6096.74, 106.824, 3.17317, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 4721.42, -5955.83, 105.055, 5.42805, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 3529.05, -6574.06, 52.0014, 4.64895, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 3736.6, -3326.26, 1012.16, 0.266409, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 4644.54, -4545.97, 887.638, 3.95622, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 4932.27, -2641.73, 1427.55, 0.048865, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 5410.16, -3196.55, 1579.72, 0.63474, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 5939.53, -1213.33, 383.185, 6.28175, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 5404.59, -579.932, 355.886, 5.14215, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 3819.55, -1616.39, 223.001, 2.23305, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 4294.45, 90.475, 43.7438, 2.34665, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 5251.71, 188.508, 16.9118, 3.8232, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 6248.43, 238.149, 21.5925, 0.870112, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 6672.73, 277.876, 9.56811, 2.69381, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 7173.91, -497.945, 32.192, 2.4904, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 6618.83, -3546.25, 682.422, 0.999294, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 5633.3, -4767.9, 778.051, 1.47212, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 5094.36, -4740.24, 847, 2.36353, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -8832.53, 61.3386, 141.049, 4.14439, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -10248.5, -2140.2, 37.5838, 4.70595, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -10744.2, -1550.55, 11.5516, 3.11551, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -11170.2, -593.662, 45.3799, 6.23354, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, 2383.21, 469.474, 35.5246, 3.84555, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 7407.04, -2824.32, 464.228, 0.89404, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 6855.81, -4664.11, 701.468, 0.146364, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 5233, 0, -2722.66, -5946.18, 87.5041, 2.42601, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 5233, 0, -3903.41, -6223.91, 27.3477, 5.42797, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 5233, 0, -4153.51, -3699.22, 207.698, 1.50098, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 5233, 0, -4800.34, -4871.17, 192.193, 0.244346, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 5233, 0, -4639.65, -4038.64, 340.418, 5.60251, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 5233, 0, 4949.68, -1875.75, 1326.2, 0.802851, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -5105.44, -5865.43, 12.639, 0.552859, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -8826.61, -1616.51, 113.559, 3.16558, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -4995.9, 3439.52, -127.078, 2.21484, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -4626.94, 3801.86, -120.077, 3.99377, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -6114.93, 4125.81, -508.381, 4.41785, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -6799.55, 4553.43, -604.845, 1.79384, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -7241.32, 4253.21, -272.205, 1.49303, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -5609.94, 5463.22, -1231.9, 0.493257, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -5838.65, 6804.5, -1015.34, 3.80764, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -6303.28, 6046.86, -788.617, 5.96746, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -6812.62, 6114.48, -616.066, 2.37583, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 646, 1, 0, 0, 0, 0, 804.431, 573.228, -70.4089, 3.09898, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 646, 1, 0, 0, 0, 0, 10.2483, -178.951, 204.158, 1.67739, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 646, 1, 0, 0, 0, 0, 920.542, -372.155, 61.8649, 3.71551, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 646, 1, 0, 0, 0, 0, 990.33, -844.992, 281.8, 5.52192, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 646, 1, 0, 0, 0, 0, 1257.38, 1646.33, 175.056, 2.70626, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 646, 1, 0, 0, 0, 0, 653.766, 1828.59, 337.032, 3.21441, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 646, 1, 0, 0, 0, 0, 446.306, 1633.44, 350.035, 3.77597, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 646, 1, 0, 0, 0, 0, 129.512, 1504.87, 221.367, 3.83487, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 646, 1, 0, 0, 0, 0, 185.555, 825.667, 52.5411, 2.36222, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 646, 1, 0, 0, 0, 0, 850.002, 640.567, -68.6993, 2.31351, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -10530.8, 1162.41, 32.5533, 1.03657, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -224.148, 1522.19, 76.2553, 3.93211, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -2722.83, -5628.99, 377.829, 4.79965, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -5586.98, 5467.29, -1799.79, 4.71239, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -7194.97, -915.403, 165.573, 1.64061, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -7295.31, -980.688, 263.08, 0.907571, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -7536.35, -1209.68, 286.858, 5.18363, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -7593.82, -1127.95, 250.29, 1.69297, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -8459.4, 907.29, 100.172, 3.68265, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, -8770.04, 842.066, 97.718, 3.78736, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, 1195.08, -2611.93, 77.8512, 3.42085, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, 2377.5, -5780.81, 151.45, 2.67035, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 0, 1, 0, 0, 0, 0, 3346.49, -3226.89, 143.951, 3.49066, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -11482.5, -2150.62, 493.757, 1.309, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -3110.26, -3059.42, 33.7547, 2.60054, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, -7988.49, 1557.96, 4.73619, 3.10669, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1, 1, 0, 0, 0, 0, 4265.05, 732.589, -26.9592, 0.034907, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 648, 1, 0, 0, 0, 0, 635.182, 3114.05, 3.31831, 2.11185, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 861, 1, 0, 0, 0, 0, 1387.98, 558.891, 80.5059, 4.81711, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 861, 1, 0, 0, 0, 0, 987.227, 295.698, 45.7831, 0.680678, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 974, 1, 0, 0, 0, 0, -4246.83, 6387.35, 13.2001, 4.67748, 7200, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, -1344.72, 1976.18, 17.2501, 4.13573, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, -1256.23, 792.288, 14.6082, 4.73808, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, -1202.48, 90.3837, 12.496, 2.30089, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, -397.3, -762.016, 121.019, 0.438131, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, -1964.25, 2285.78, 10.1143, 1.92848, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, -2337.68, 847.273, 3.9152, 4.01429, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, -706.194, -977.473, 33.191, 2.05801, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, -2855.82, 551.836, 34.265, 1.28518, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, -1295.24, 4514.09, 128.713, 5.73447, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, -473.188, 3886.24, 77.974, 5.47782, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 590.858, 4285.59, 219.443, 4.17282, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 624.903, 3665.99, 226.264, 4.24973, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 105.226, 4024.09, 252.49, 5.50744, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 480.502, 2921.83, 252.742, 5.69018, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, -547.354, 2931.35, 166.371, 6.26156, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, -1255.62, 4139.92, 59.2184, 6.1823, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, -1567.99, 4798.29, 84.6429, 5.82523, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, -431.087, -1715.3, 12.6888, 4.08087, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, -291.754, -2882.41, 13.6056, 2.58309, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, -80.9948, -3213.06, 170.389, 3.87342, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 559.024, -1363.87, 70.4253, 3.03687, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 1022.83, -2358.97, 155.904, 1.8675, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 2641.27, -507.498, 322.648, 5.39307, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 2753.25, -2345.92, 52.8911, 2.82192, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 2960.81, -662.016, 232.321, 6.1424, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 1772.61, -1867.76, 193.404, 0.698132, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, -698.609, -1470.44, 130.298, 6.07222, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 2524.28, -1162.38, 387.557, 3.9619, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 2784.03, -1793.99, 240.643, 5.41653, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 74.6406, -2089.99, 45.9734, 1.62316, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 1726.33, -486.321, 362.21, 5.41052, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 1737.33, -2776.71, 130.645, 5.21853, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 445.3, -1575.87, 162.088, 0.037015, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, -188.457, 1917.67, 153.118, 2.35928, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 177.429, 1168.39, 218.903, 0.772003, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 203.283, -329.123, 253.877, 3.3148, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, -87.0903, 581.523, 164.262, 0.671189, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, -578.893, 1320.89, 148.742, 3.89276, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 106.563, 1470.72, 390.139, 1.89313, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 345.216, 1545.83, 459.788, 2.5898, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 567.936, 1320.48, 441.99, 6.242, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 644.545, 2129.41, 368.579, 3.87348, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 928.583, 714.139, 401.536, 5.1959, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 1038.51, 2147.93, 310.433, 5.69423, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 1162.62, 689.852, 348.993, 5.57747, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 1579.08, 1988.02, 461.473, 0.024463, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 1366.58, 987.821, 439.072, 4.70117, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 1420.19, 1488.34, 412.066, 2.3688, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 386.543, 1579.42, 525.69, 5.53198, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 697.134, 1706.35, 371.517, 4.83848, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 1189.36, 1600.06, 353.643, 4.68966, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 1049.12, -158.248, 484.311, 6.21979, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 1617.77, -211.451, 469.628, 3.91713, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 1282.68, 4248.37, 187.094, 1.61106, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 1630.76, 3025.66, 320.026, 3.74751, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 1656.48, 2505.31, 302.409, 2.40646, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 1709.36, 3580.35, 224.546, 1.66282, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 2387.32, 4096.95, 214.788, 2.30798, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 2418.07, 3350.92, 293.468, 2.40646, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 2431.99, 4821.43, 189.477, 0.696154, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 1414.71, 4935.43, 133.19, 4.46418, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 3956.59, 5530.75, 157.153, 3.80442, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 2639.08, 5927.72, 81.1692, 3.3237, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 1849.83, 364.875, 482.456, 2.30383, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 2104.58, 1080.42, 486.788, 1.98968, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 3137.31, 550.031, 504.665, 3.12414, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 3527.47, 2704, 756.79, 5.21002, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 3547.54, 1642.85, 839.616, 0.907571, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 4218.76, 639.965, 113.75, 0.837758, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 4611.37, 145.564, 15.2708, 3.1765, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 2650, 2179.2, 581.542, 5.60251, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 3026.9, 3015.19, 534.414, 0.872237, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 4027.72, 1122.32, 498.943, 3.81941, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 3026.2, 1304.45, 648.049, 2.95663, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 3488.15, 2099.12, 1084.04, 2.44286, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 3566.93, 1347.85, 799.437, 0.62072, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 4509.65, 2846.87, 121.437, 3.69913, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 5950.81, 1154.81, 60.7055, 4.3344, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 860, 1, 0, 0, 0, 0, 903.941, 4335.09, 243.735, 2.1293, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 860, 1, 0, 0, 0, 0, 945.076, 4067.9, 199.724, 2.25148, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 860, 1, 0, 0, 0, 0, 1236.27, 3560.11, 102.301, 4.74729, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 860, 1, 0, 0, 0, 0, 648.916, 3041.11, 77.3114, 3.0282, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 860, 1, 0, 0, 0, 0, 1068.58, 3267.88, 129.841, 2.93034, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 860, 1, 0, 0, 0, 0, 407.503, 3565.25, 78.078, 4.46804, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1064, 1, 0, 0, 0, 0, 6253.01, 5630.51, 11.8234, 6.12662, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1064, 1, 0, 0, 0, 0, 6548.86, 5989.4, 4.96601, 4.48336, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1064, 1, 0, 0, 0, 0, 6870.86, 5789.69, 13.9823, 3.18641, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1064, 1, 0, 0, 0, 0, 6980.62, 5184.14, 67.4448, 2.41429, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1064, 1, 0, 0, 0, 0, 7241.84, 5066.13, 75.9508, 4.70229, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1064, 1, 0, 0, 0, 0, 7341.17, 5248.19, 68.2753, 2.41429, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1064, 1, 0, 0, 0, 0, 6514.65, 5247.78, 19.2295, 2.3656, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1064, 1, 0, 0, 0, 0, 6187.46, 5033.37, 38.6207, 3.15358, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 1064, 1, 0, 0, 0, 0, 7215.5, 6355.95, 12.4896, 6.04451, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (6491)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 6491, 870, 1, 0, 0, 0, 0, 4011.19, 1776.64, 884.864, 2.69691, 300, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (65183)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 65183, 1024, 6144, 0, 0, 0, 0, 2009.52, -2022.65, 219.279, 4.7529, 7200, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (65183)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 65183, 1050, 4096, 0, 0, 0, 0, -1255.26, 4140.18, 59.2388, 6.1399, 7200, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (65183)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 65183, 1095, 4096, 0, 0, 0, 0, 1124.79, -156.135, 480.286, 3.45323, 7200, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (65183)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 65183, 1099, 6144, 0, 0, 0, 0, 2320.65, -4207.84, 11.4013, 4.97341, 7200, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (65183)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 65183, 1104, 4096, 0, 0, 0, 0, -1076.92, 1157.61, 16.6278, 2.27632, 7200, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (65183)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 65183, 1126, 4096, 0, 0, 0, 0, 7572.25, 5586.72, 31.1381, 2.01521, 7200, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (65183)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 65183, 1126, 4096, 0, 0, 0, 0, 6732.65, 5882.95, 7.21538, 5.37219, 7200, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (65183)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 65183, 1130, 6144, 0, 0, 0, 0, -5257.63, -415.155, 392, 5.30181, 7200, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (65183)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 65183, 1144, 6144, 0, 0, 0, 0, 1129.11, 912.911, 404.185, 4.33542, 7200, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 654, 1, 0, 0, 5233, 0, -1887.48, 2319.09, 39.6102, 2.33874, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 654, 1, 0, 0, 5233, 0, -1918.78, 2570.06, 1.47525, 1.09956, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 648, 1, 0, 0, 5233, 0, -8115.33, 1520.22, 10.3416, 3.19395, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 648, 1, 0, 0, 5233, 0, 1475.17, 2059.1, 222.899, 0.593412, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 648, 1, 0, 0, 5233, 0, 615.972, 1962.5, 12.7702, 6.02139, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 648, 1, 0, 0, 5233, 0, 716.535, 1830.71, 104.456, 3.89208, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 648, 1, 0, 0, 5233, 0, 858.247, 2331.56, 11.8153, 3.9619, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 648, 1, 0, 0, 5233, 0, 1396.05, 1337.23, 130.325, 3.35103, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 648, 1, 0, 0, 0, 0, -8394.45, 1292.15, 92.8141, 5.79557, 600, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 648, 1, 0, 0, 0, 0, 1380.8, 1361.19, 165.239, 1.48442, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 648, 1, 0, 0, 0, 0, 842.855, 2373.1, 4.17812, 5.25039, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 648, 1, 0, 0, 0, 0, 867.524, 2780.05, 114.394, 0.174533, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 648, 1, 0, 0, 0, 0, 521.91, 2707.84, 105.979, 3.28122, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 648, 1, 0, 0, 0, 0, 1000.78, 3310.56, 3.48974, 0.10472, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 648, 1, 0, 0, 0, 0, 1769.73, 2011.25, 220.906, 2.62563, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 648, 1, 0, 0, 0, 0, 2035.77, 2324.85, 78.911, 1.38469, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 648, 1, 0, 0, 0, 0, 1735.44, 2690.9, 85.8003, 0.23094, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 654, 1, 0, 0, 0, 0, -1892.59, 2323.55, 39.5526, 5.48678, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 654, 1, 0, 0, 0, 0, -1915.03, 2578.06, 1.66019, 4.29297, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 654, 1, 0, 0, 0, 0, -1697.3, 1692.27, 20.2561, 5.04224, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 654, 1, 0, 0, 0, 0, -1686.91, 1618.85, 20.4894, 1.55114, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 654, 1, 0, 0, 0, 0, -1763.07, 1657.24, 21.3349, 5.65092, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 654, 1, 0, 0, 0, 0, -1785.29, 1482.23, 20.3423, 3.05518, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 654, 1, 0, 0, 0, 0, -1750.27, 1399.77, 19.7822, 4.49246, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 654, 1, 0, 0, 0, 0, -1389.71, 1365.53, 35.5658, 0.447654, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 654, 1, 0, 0, 0, 0, -1563.1, 1595.47, 28.1029, 6.09466, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 654, 1, 0, 0, 0, 0, -1627.52, 1490.26, 74.0195, 3.8052, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 654, 1, 0, 0, 0, 0, -1717.06, 1256.42, 0.322952, 1.3076, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 654, 1, 0, 0, 0, 0, -1943.39, 973.869, 76.0311, 3.5947, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 654, 1, 0, 0, 0, 0, -2492.25, 1464.63, 16.7111, 2.5462, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 654, 1, 0, 0, 0, 0, -2152.89, 1665.44, -38.0939, 1.7286, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 654, 1, 0, 0, 0, 0, -2226.85, 1836.73, 13.5439, 1.51655, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 654, 1, 0, 0, 0, 0, -1720.71, 1903.48, 18.5799, 6.03179, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 654, 1, 0, 0, 0, 0, -892.131, 1663.26, 64.7444, 5.58726, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (39660)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 39660, 654, 1, 0, 0, 0, 0, -1213.66, 1121.56, 33.6307, 0.360434, 300, 0, 0, 4120, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 4346.24, -4250.23, 166.318, 0.289394, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 609, 1, 0, 0, 0, 0, 2368.28, -5777.21, 155.37, 2.3, 120, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 609, 1, 0, 0, 0, 0, 2118.83, -5287.42, 85.2, 0.95, 120, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 609, 1, 0, 0, 0, 0, 1883.86, -5780.5, 107.34, 4.17, 120, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 3970.85, -2307.03, 217.485, 4.25958, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 2943.51, -2838.75, 77.2893, 2.88749, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 4312.04, -3605.47, 252, 6.13904, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 1953.14, -3682.8, 176.085, 0.001156, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 734.64, -2981.74, 28.9695, 4.18261, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 104.171, -3700.87, 5.17585, 6.25607, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 2244.9, -4652.06, 223.508, 2.75948, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 1920.4, -5976.54, 13.422, 5.46753, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 717.173, -5886.4, 293.114, 1.61593, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 217.417, -5946.23, 10.9094, 3.24642, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 51.3043, -4649.77, 287.697, 3.38779, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 675.561, -4928.76, 7.72376, 3.99726, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 1814.15, -5616, 224.77, 2.54271, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 1356.91, -4763.44, 198.197, 5.0874, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 1109.8, -4970.67, 36.492, 4.68528, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 5237.88, -2133.31, 248.49, 0.045145, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 4946.27, -2952.8, 293.535, 4.18733, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 5509.18, -4095.99, 368.564, 5.79269, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 5854.34, 767.148, 645.389, 4.01612, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 5052.05, -641.439, 228.638, 3.16474, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 5885.3, 653.89, 173.508, 2.12487, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 5587.11, -586.1, 194.688, 5.45103, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 7065.61, -120.942, 796.406, 5.20285, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 7924.17, -2459.17, 1142.01, 1.87747, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 8094.89, -995.236, 942.172, 5.89557, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 8382.84, -222.418, 846.126, 1.23622, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 6943.08, -552.682, 920.373, 1.01474, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 7087.91, -1432.86, 927.501, 4.60951, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 6067.18, 94.9816, 374.062, 4.72888, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 8194.62, 2864.02, 609.327, 3.74871, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 7226.99, 2167.04, 571.128, 6.0782, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 7897.79, 730.055, 523.272, 4.21445, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 7139.45, 3696.35, 826.757, 5.37133, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 4818, 5469.51, -47.7692, 6.02321, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 5624.24, 5845.53, -58.6562, 3.99531, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 5542.62, 4853.47, -192.129, 3.54292, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 6246.99, 5172.19, -79.4421, 4.39115, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 6268.06, 4407.78, -66.0282, 0.951106, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 5520.52, 3587.64, -9.3605, 1.8229, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 6347.51, 5635.63, 75.2332, 1.00608, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 5026.8, 4498.55, -85.575, 5.46243, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 4413.33, -1981.2, 162.165, 0.036098, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 4468.79, 1266.94, 145.828, 4.84941, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 3416.95, -1280.98, 130.354, 0.187287, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 4820.11, 1019.45, 150.487, 3.07362, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 3579.08, 4092.79, 26.4858, 4.63734, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 4508.47, 5654.1, 86.2964, 1.23342, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 3556.7, 5824.08, 132.913, 4.71116, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 2851.92, 5971.42, 118.196, 3.40191, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 2485.12, 5289.22, 36.5883, 3.13252, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 4222.34, 5348.46, 33.7669, 4.76615, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 4607.22, -968.689, 174.813, 4.26507, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 3667.91, -1044.36, 134.562, 4.56352, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 3842.16, 1430.75, 94.6411, 4.56117, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 2764.03, 1044.03, 27.9599, 6.25448, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 3594.07, 2851.39, 74.1617, 5.3497, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 3798.3, 2056.3, 97.1736, 2.22304, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 3095.15, -1978.29, 87.0188, 5.37799, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (29259)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 29259, 571, 1, 0, 0, 0, 0, 3529.23, -2892.87, 208.628, 1.43293, 600, 0, 0, 7212, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (72676)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 72676, 870, 1, 0, 0, 0, 0, -1100.91, -5227.58, 27.3194, 6.05544, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (72676)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 72676, 870, 1, 0, 0, 0, 0, -928.648, -4674.8, 2.25062, 4.88315, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (72676)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 72676, 870, 1, 0, 0, 0, 0, -920.146, -5039.7, 2.21583, 4.4689, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (72676)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 72676, 870, 1, 0, 0, 0, 0, -745.01, -5722.96, 54.0462, 4.80902, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (72676)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 72676, 870, 1, 0, 0, 0, 0, -661.247, -4865.06, 2.13696, 6.17848, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (72676)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 72676, 870, 1, 0, 0, 0, 0, -649.918, -5113.79, 2.13932, 4.59472, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (72676)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 72676, 870, 1, 0, 0, 0, 0, -548.438, -5652.5, 17.826, 2.74623, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (72676)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 72676, 870, 1, 0, 0, 0, 0, -382.337, -4626.08, 4.33231, 0.673996, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (72676)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 72676, 870, 1, 0, 0, 0, 0, -307.354, -5014.01, 7.67081, 2.49248, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- NPC: Spirit Healer (72676)
SET @CGUID := @CGUID + 1;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 72676, 870, 1, 0, 0, 0, 0, -268.842, -5516.71, 126.7, 1.20491, 120, 0, 0, 1, 0, 0, 0, 0, 0);

