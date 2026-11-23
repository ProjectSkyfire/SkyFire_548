-- These quest spells need a spell focus object near their target: Control Robot (29989, focus 1372); Summon Robot (30009, focus 1372).
-- The objects had templates and 1 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject` WHERE `guid` BETWEEN 1061241 AND 1061241;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(1061241, 181825, 1, 1, 0, 0, -4500.54, -864.842, -41.3823, 4.51822, 0, 0, 0.772318, -0.635236, 600, 0, 1);
