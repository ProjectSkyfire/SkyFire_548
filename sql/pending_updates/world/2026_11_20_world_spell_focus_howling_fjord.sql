-- These quest spells need a spell focus object near their target: Wyrmcallers Horn (42817, focus 1465); Test Horn (43189, focus 1469); Dummy Self (93908, focus 1469); Summon Plaguehound Tracker (43232, focus 1474); Anuniaqs Net (21014, focus 1567).
-- 3 object(s) had no usable template and 8 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject_template` WHERE `entry` IN (186487, 186599, 186613);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `data3`) VALUES
(186487, 8, 327, 'The Wyrmlord''s Platform', 1, 1465, 50, 1),
(186599, 8, 327, 'Frozen Waterfall', 1, 1469, 20, 1),
(186613, 8, 2770, 'Plaguehound Tracker Focus', 1, 1474, 15, 1);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 911891 AND 911898;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911891, 186487, 571, 1, 0, 0, 2821.75, -3603.67, 245.555, 3.49556, 0, 0, 0.98438, -0.176059, 300, 0, 1),
(911892, 186599, 571, 1, 0, 0, 2950.32, -4557.77, 273.565, 5.97819, 0, 0, 0.151907, -0.988395, 30, 0, 1),
(911893, 186613, 571, 1, 0, 0, 1758.64, -6039.13, 7.46871, 1.07636, 0, 0, 0.512574, 0.858643, 1200, 0, 1),
(911894, 186949, 571, 1, 0, 0, 1481.71, -6410.56, 0.001027, 2.57289, 0, 0, 0.959844, 0.280533, 300, 100, 1),
(911895, 186949, 571, 1, 0, 0, 1874.92, -6281.69, -0.008061, 2.5454, 0, 0, 0.955897, 0.293701, 300, 100, 1),
(911896, 186949, 571, 1, 0, 0, 645.56, -5111.1, 0.001437, 2.72997, 0, 0, 0.978895, 0.204362, 300, 100, 1),
(911897, 186949, 571, 1, 0, 0, 52.2427, -3924.23, 0.002501, 0.127942, 0, 0, 0.063927, 0.997955, 300, 100, 1),
(911898, 186949, 571, 1, 0, 0, -178.445, -3603.42, 0.001124, 1.09791, 0, 0, 0.521796, 0.853071, 300, 100, 1);
