-- Spawn Kadrak
DELETE FROM `creature` WHERE `guid`=100046;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(100046, 8582, 1, 1, 0, 0, 0, 0, 1229.06, -2231.31, 91.7917, 1.58406, 120, 0, 0, 0, 0, 0, 0, 0, 0);
-- Fix equipment of Kadrak
DELETE FROM `creature_equip_template` WHERE `entry`=8582;
INSERT INTO `creature_equip_template` (`entry`, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES
(8582, 1, 34638, 0, 0);

