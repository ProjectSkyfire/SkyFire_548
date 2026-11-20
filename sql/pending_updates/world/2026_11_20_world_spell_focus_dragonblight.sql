-- These quest spells need a spell focus object near their target: Ley Line Focus Control Talisman (47634, focus 1517); Gather Lumber (47939, focus 1519); Quarterflashs Homing Bot (48731, focus 1531); Planting Wintergarde Mine Bomb (48741, focus 1533); Skytalon Molts (49566, focus 1539).
-- 2 object(s) had no usable template and 13 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject_template` WHERE `entry` IN (188708, 190033);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `data3`) VALUES
(188708, 8, 0, 'Siege Engineer Quarterflash Focus', 1, 1531, 5, 1),
(190033, 8, 5811, 'Alystros the Verdant Keeper Spell Focus', 1, 1539, 50, 1);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 911899 AND 911911;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911899, 188491, 571, 1, 0, 0, 3074.05, 567.258, 20.4002, 3.83728, 0, 0, 0.940111, -0.34087, 300, 100, 1),
(911900, 188491, 571, 1, 0, 0, 3149.66, 484.782, 21.0715, 2.60814, 0, 0, 0.964638, 0.263577, 300, 100, 1),
(911901, 188491, 571, 1, 0, 0, 3060.2, 473.572, 21.4601, 0.778157, 0, 0, 0.379336, 0.925259, 300, 100, 1),
(911902, 188539, 571, 1, 0, 0, 4928.94, 976.366, 213.808, -2.82743, 0, 0, 0, 0, 300, 0, 1),
(911903, 188539, 571, 1, 0, 0, 5122.28, 1034.12, 227.3, 2.30383, 0, 0, 0, 0, 300, 0, 1),
(911904, 188539, 571, 1, 0, 0, 5135.16, 1036.44, 225.743, -0.942477, 0, 0, 0, 0, 300, 0, 1),
(911905, 188539, 571, 1, 0, 0, 5179.04, 1072.72, 221.845, -2.93214, 0, 0, 0, 0, 300, 0, 1),
(911906, 188539, 571, 1, 0, 0, 5222.16, 1092.08, 219.521, -2.37364, 0, 0, 0, 0, 300, 0, 1),
(911907, 188539, 571, 1, 0, 0, 5251.86, 1079, 220.952, 0.087266, 0, 0, 0, 0, 300, 0, 1),
(911908, 188708, 571, 1, 0, 0, 3996.67, -945.023, 105.807, 5.01695, 0, 0, 0.591663, -0.806186, 1200, 0, 1),
(911909, 188711, 571, 1, 0, 0, 3903.78, -882.829, 121.387, -2.96704, 0, 0, 0, 0, 300, 0, 1),
(911910, 188711, 571, 1, 0, 0, 3882.71, -868.336, 111.043, 0, 0, 0, 0, 0, 300, 0, 1),
(911911, 190033, 571, 1, 0, 0, 2646.16, -24.8575, 3.18696, 3.65693, 0, 0, 0.966987, -0.254827, 1200, 0, 1);

DELETE FROM `event_scripts` WHERE `id` IN (18223);
INSERT INTO `event_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES
(18223, 2, 10, 27249, 180000, 0, 2646.16, -24.8575, 3.18696, 8.65693);
