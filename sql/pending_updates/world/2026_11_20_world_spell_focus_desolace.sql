-- These quest spells need a spell focus object near their target: Squeeze Melons (67402, focus 1624); Offer Sacrifice (67733, focus 1626).
-- The objects had templates and 2 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject` WHERE `guid` BETWEEN 910988 AND 910989;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(910988, 301021, 1, 1, 0, 0, -90.0278, 771.561, 132.892, 0, 0, 0, 0, 1, 180, 100, 1),
(910989, 301023, 1, 1, 0, 0, -242.208, 693.026, 100.894, 1.97222, 0, 0, 0.833885, 0.551938, 300, 255, 1);
