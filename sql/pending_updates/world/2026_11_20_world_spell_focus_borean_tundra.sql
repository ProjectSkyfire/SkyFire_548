-- These quest spells need a spell focus object near their target: Place Sage Aeires Totem (45653, focus 1491); Signal Alliance (45958, focus 1498); Scuttle Wrecked Flying Machine (46171, focus 1502); Summon Jenny (46338, focus 1502); Cure Caribou Hide (46387, focus 1503).
-- 3 object(s) had no usable template and 8 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject_template` WHERE `entry` IN (187894, 188019, 188086);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `data3`) VALUES
(187894, 8, 0, 'Borean Crossroads Spell Focus', 1, 1498, 20, 1),
(188019, 8, 5932, 'Crashed Flying Machine', 1, 1502, 15, 1),
(188086, 8, 5932, 'Geyser Fields Steam Vent', 1, 1503, 15, 1);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 911883 AND 911890;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911883, 187690, 571, 1, 0, 0, 4022.26, 3604.13, 104.033, -0.436332, 0, 0, 0, 0, 300, 0, 1),
(911884, 187690, 571, 1, 0, 0, 4125.65, 3637.9, 91.7764, -0.994837, 0, 0, 0, 0, 300, 0, 1),
(911885, 187690, 571, 1, 0, 0, 3746.69, 3519.83, 69.3417, -1.95477, 0, 0, 0, 0, 300, 0, 1),
(911886, 187894, 571, 1, 0, 0, 2944.28, 5382.93, 60.5831, 2.15382, 0, 0, 0.880497, 0.474052, 1200, 0, 1),
(911887, 188019, 571, 1, 0, 0, 3392.5, 5134.9, 13.0373, 2.25405, 0, 0, 0.814276, 0.580478, 120, 0, 1),
(911888, 188086, 571, 1, 0, 0, 3649.06, 4197.95, 16.817, 0.492043, 0, 0, 0.243547, 0.969889, 300, 0, 1),
(911889, 188086, 571, 1, 0, 0, 3850.5, 4884.73, -4.55864, 3.9046, 0, 0, 0.928105, -0.372318, 300, 0, 1),
(911890, 188086, 571, 1, 0, 0, 4241.54, 5153.61, 13.0754, 2.20814, 0, 0, 0.893046, 0.449965, 1200, 0, 1);
