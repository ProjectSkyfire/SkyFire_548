-- These quest spells need a spell focus object near their target: Place Arcanite Buoy (25783, focus 1349).
-- The objects had templates and 1 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject` WHERE `guid` BETWEEN 1061242 AND 1061242;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(1061242, 180669, 1, 1, 0, 0, 3512.29, -6618.08, 0.01, 0, 0, 0, 0, 1, 300, 100, 1);
