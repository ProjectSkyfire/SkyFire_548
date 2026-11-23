-- These quest spells need a spell focus object near their target: Summon Spider God (11548, focus 304).
-- The object had no usable template and its spawn was missing, so the cast failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject_template` WHERE `entry` IN (142716);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `data3`) VALUES
(142716, 8, 327, 'Shadra''Alor Altar', 2.31, 304, 7, 1);

DELETE FROM `gameobject` WHERE `guid` = 911563;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911563, 142716, 0, 1, 0, 0, -402.15, -2891.36, 86.85, 0, 0, 0, 0, 1, 180, 0, 1);
