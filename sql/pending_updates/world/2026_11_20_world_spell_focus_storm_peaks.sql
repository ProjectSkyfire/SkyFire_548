-- These quest spells need a spell focus object near their target: Throw Ice (56099, focus 1590).
-- The objects had templates and 8 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject` WHERE `guid` BETWEEN 911934 AND 911941;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911934, 192124, 571, 1, 0, 0, 7192, -3386.61, 846.416, 2.11185, 0, 0, 0, 1, 300, 100, 1),
(911935, 192124, 571, 1, 0, 0, 7206.48, -3460.64, 835.851, -2.37364, 0, 0, 0, 1, 300, 100, 1),
(911936, 192124, 571, 1, 0, 0, 7217.79, -3378.62, 846.734, -0.872664, 0, 0, 0, 1, 300, 100, 1),
(911937, 192124, 571, 1, 0, 0, 7227.85, -3415.96, 840.174, 1.55334, 0, 0, 0, 1, 300, 100, 1),
(911938, 192124, 571, 1, 0, 0, 7222.42, -3441.84, 837.193, 2.14675, 0, 0, 0, 1, 300, 100, 1),
(911939, 192124, 571, 1, 0, 0, 7207.47, -3438.53, 838.874, 2.89724, 0, 0, 0, 1, 300, 100, 1),
(911940, 192124, 571, 1, 0, 0, 7232.06, -3474.28, 850.768, -2.35619, 0, 0, 0, 1, 300, 100, 1),
(911941, 192124, 571, 1, 0, 0, 7206.62, -3482, 833.563, -3.00195, 0, 0, 0, 1, 300, 100, 1);
