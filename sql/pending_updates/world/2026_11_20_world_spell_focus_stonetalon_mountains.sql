-- These quest spells need a spell focus object near their target: Set NG-5 Charge (Blue) (6626, focus 16); Set NG-5 Charge (Red) (6630, focus 17).
-- The objects had templates and 2 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject` WHERE `guid` BETWEEN 910986 AND 910987;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(910986, 19591, 1, 1, 0, 0, 1099.77, 141.176, 5.09, 2.885, 0, 0, 0.991781, 0.127945, 600, 100, 1),
(910987, 19600, 1, 1, 0, 0, 1080.38, -444, 1.624, 2.837, 0, 0, 0.988425, 0.151708, 600, 100, 1);
