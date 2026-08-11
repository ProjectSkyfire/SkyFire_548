-- Corithras Moonrage (3515) sniff spawn (missing from base world)
DELETE FROM `creature` WHERE `guid`=8202166;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8202166, 3515, 1, 1, 0, 0, 0, 0, 9736.91, 956.063, 1294.19, 5.96903, 300, 0, 0, 1, 0, 0, 0, 0, 0);
