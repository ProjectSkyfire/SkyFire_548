-- These quest spells need a spell focus object near their target: Place Basilisk Meat (83851, focus 1680); Goldmines Fire Totem (82044, focus 1682).
-- 2 object(s) had no usable template and 9 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject_template` WHERE `entry` IN (301073, 301075);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`) VALUES
(301073, 8, 299, 'Stonescale Matriarch''s feeding grounds', 1, 1680, 20),
(301075, 8, 299, 'Blood Trail Lava Pool', 1, 1682, 20);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 1064800 AND 1064808;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(1064800, 301073, 646, 1, 0, 0, -249.83, 540.993, 266.737, 0, 0, 0, 0, 0, 120, 100, 1),
(1064801, 301075, 646, 1, 0, 0, 819.437, 458.907, -68.1925, 0, 0, 0, 0, 0, 300, 100, 1),
(1064802, 301075, 646, 1, 0, 0, 809.74, 423.881, -67.9024, 0, 0, 0, 0, 0, 300, 100, 1),
(1064803, 301075, 646, 1, 0, 0, 800.916, 392.663, -73.8368, 0, 0, 0, 0, 0, 300, 100, 1),
(1064804, 301075, 646, 1, 0, 0, 776.41, 377.999, -70.3379, 0, 0, 0, 0, 0, 300, 100, 1),
(1064805, 301075, 646, 1, 0, 0, 786.675, 348.441, -72.5761, 0, 0, 0, 0, 0, 300, 100, 1),
(1064806, 301075, 646, 1, 0, 0, 774.071, 322.976, -70.4369, 0, 0, 0, 0, 0, 300, 100, 1),
(1064807, 301075, 646, 1, 0, 0, 775.051, 291.591, -69.7522, 0, 0, 0, 0, 0, 300, 100, 1),
(1064808, 301075, 646, 1, 0, 0, 757.958, 273.243, -62.0056, 0, 0, 0, 0, 0, 300, 100, 1);

DELETE FROM `event_scripts` WHERE `id` IN (25897);
INSERT INTO `event_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES
(25897, 1, 10, 44148, 180000, 0, -249.83, 540.993, 268.737, 0);
