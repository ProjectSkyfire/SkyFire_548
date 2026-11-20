-- These quest spells need a spell focus object near their target: Place Ceremonial Incense (38336, focus 1442).
-- The objects had templates and 1 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject` WHERE `guid` BETWEEN 911635 AND 911635;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911635, 185139, 530, 1, 0, 0, -2670.1, 4381.25, 35.8616, 2.14675, 0, 0, 0.878817, 0.477159, 180, 100, 1);
