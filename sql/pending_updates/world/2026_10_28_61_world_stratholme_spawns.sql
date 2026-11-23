-- Stratholme: creature spawns and gameobject spawns that SkyFire lacks, from 4.3.4 (spawnMask 2). A 4.3.4 spawn is imported only if no SkyFire spawn of the same entry is left within 15 yards
-- (one-to-one, nearest pair first). Safe to run twice: each block deletes its reserved id range first.

DELETE FROM `creature` WHERE `guid` BETWEEN 8304100 AND 8304199;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8304100, 16031, 329, 2, 0, 0, 0, 0, 4044.78, -3333.68, 117.26, 4.15388, 7200, 0, 0, 4120, 0, 0, 0, 0, 0);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 910570 AND 910599;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(910570, 181071, 329, 2, 0, 0, 4044.34, -3334.22, 115.06, 2.74016, 0, 0, 0.979924, 0.19937, 300, 255, 1);
