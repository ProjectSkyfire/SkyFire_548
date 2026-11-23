-- These quest spells need a spell focus object near their target: Summon Drakurus Image (47110, focus 1509); Cast Net (48794, focus 1534).
-- 1 object(s) had no usable template and 9 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject_template` WHERE `entry` IN (188358);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `data3`) VALUES
(188358, 8, 327, 'Drakuru''s Brazier', 1, 1509, 8, 1);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 911912 AND 911920;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911912, 188358, 571, 1, 0, 0, 4242.92, -2036.32, 237.852, 1.43892, 0, 0, 0.65898, 1, 300, 100, 1),
(911913, 189290, 571, 1, 0, 0, 4746.21, -5215.84, 0.088656, 0.209439, 0, 0, 0, 0, 300, 0, 1),
(911914, 189290, 571, 1, 0, 0, 4653.75, -5382.51, 0.228516, 2.9496, 0, 0, 0, 0, 300, 0, 1),
(911915, 189290, 571, 1, 0, 0, 4463.61, -5414.51, 0.28144, 2.11185, 0, 0, 0, 0, 300, 0, 1),
(911916, 189290, 571, 1, 0, 0, 4294.62, -5465.26, 0.248345, 0.261798, 0, 0, 0, 0, 300, 0, 1),
(911917, 189290, 571, 1, 0, 0, 4195.34, -5384.57, 0.201511, 1.50098, 0, 0, 0, 0, 300, 0, 1),
(911918, 189290, 571, 1, 0, 0, 4192.84, -5198.86, 0.262496, -1.91986, 0, 0, 0, 0, 300, 0, 1),
(911919, 189290, 571, 1, 0, 0, 4086.2, -5255.22, -0.072908, 1.85005, 0, 0, 0, 0, 300, 0, 1),
(911920, 189290, 571, 1, 0, 0, 4071.12, -5302.77, -0.00048, -2.00713, 0, 0, 0, 0, 300, 0, 1);
