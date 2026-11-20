-- These quest spells need a spell focus object near their target: Place Toxic Fogger (6620, focus 15); Place Lion Carcass (6717, focus 20).
-- The objects had templates and 2 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject` WHERE `guid` BETWEEN 911569 AND 911569;
DELETE FROM `gameobject` WHERE `guid` BETWEEN 1061264 AND 1061264;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911569, 19876, 0, 1, 0, 0, 263.339, -268, 143.757, 1.449, 0, 0, 0.662761, 0.748831, 600, 100, 1),
(1061264, 19594, 1, 1, 0, 0, 1435.41, 0.49, 38.65, 0, 0, 0, 0, 1, 180, 0, 1);
