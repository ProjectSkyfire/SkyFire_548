-- These quest spells need a spell focus object near their target: Unbathed Concoction (62981, focus 1600); Throw Blood (63797, focus 1603); Throw Accursed Ore (64074, focus 1605); Create Mystlash Oil (65196, focus 1607); Cleanse Elunes Tear (65514, focus 1614).
-- 4 object(s) had no usable template and 8 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject_template` WHERE `entry` IN (301004, 301007, 301009, 301015);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`) VALUES
(301004, 8, 299, 'Light of Elune', 1, 1600, 20),
(301007, 8, 299, 'Splintertree Tower', 1, 1605, 20),
(301009, 8, 299, 'Zoram''gar Forge', 1, 1607, 20),
(301015, 8, 299, 'Moonwell of Purity', 1, 1614, 20);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 1061233 AND 1061240;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(1061233, 194575, 1, 1, 0, 0, 1820.81, -2654.86, 55.1298, 0, 0, 0, 0, 1, 300, 255, 1),
(1061234, 301004, 1, 1, 0, 0, 2904.23, -1398.34, 207.376, 1.47611, 0, 0, 0.672851, 0.739778, 300, 100, 1),
(1061235, 301004, 1, 1, 0, 0, 3073.29, 614.129, 5.61695, 4.86668, 0, 0, 0.650509, -0.759498, 300, 100, 1),
(1061236, 301007, 1, 1, 0, 0, 2256.27, -2571.19, 125.773, 2.77959, 0, 0, 0.983664, 0.180014, 300, 255, 1),
(1061237, 301007, 1, 1, 0, 0, 2246.33, -2502.47, 118.568, 3.46682, 0, 0, 0.986808, -0.161897, 300, 255, 1),
(1061238, 301009, 1, 1, 0, 0, 3560.43, 1309.29, 40.3981, 5.75959, 0, 0, 0, 0, 300, 100, 1),
(1061239, 301009, 1, 1, 0, 0, 3305.99, 1037.28, 4.05734, 5.83873, 0, 0, 0.220403, -0.975409, 300, 255, 1),
(1061240, 301015, 1, 1, 0, 0, 2903.9, -1398.09, 207.354, 1.73874, 0, 0, 0.763923, 0.645307, 300, 100, 1);
