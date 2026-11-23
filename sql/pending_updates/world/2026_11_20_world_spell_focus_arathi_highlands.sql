-- These quest spells need a spell focus object near their target: Summon Myzrael (4141, focus 5).
-- 1 object(s) had no usable template and 1 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject_template` WHERE `entry` IN (2705);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `data3`) VALUES
(2705, 8, 244, 'Shards of Myzrael', 3.47, 5, 20, 1);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 911523 AND 911523;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911523, 2705, 0, 1, 0, 0, -943, -3117, 44.421, 3.142, 0, 0, 1, -0.000204, 120, 100, 1);
