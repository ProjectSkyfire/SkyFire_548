-- These quest spells need a spell focus object near their target: Dowsing (11758, focus 323).
-- The objects had templates and 1 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject` WHERE `guid` BETWEEN 1061243 AND 1061243;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(1061243, 174796, 1, 1, 0, 0, -7218.71, -2912.15, 6.44067, 3.10899, 0, 0, 0.999867, 0.016301, 600, 100, 1);
