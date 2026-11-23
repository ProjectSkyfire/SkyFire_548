-- These quest spells need a spell focus object near their target: Place Feralfen Totem (32037, focus 1391).
-- 1 object(s) had no usable template and 1 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject_template` WHERE `entry` IN (182176);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `data3`) VALUES
(182176, 8, 0, 'Boha''mu Stairs', 1, 1391, 8, 1);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 911636 AND 911636;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911636, 182176, 530, 1, 0, 0, -345.061, 7255.62, 45.8609, 3.14854, 0, 0, 0.999994, -0.003474, 600, 0, 1);
