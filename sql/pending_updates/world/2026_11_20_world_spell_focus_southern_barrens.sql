-- Burning Pitch (71030) needs spell focus 1642 near each Silithid Mound; the 22 mounds (201943) had no template and no spawn, so the bunnies that
-- summon the silithid swarms could not be hit by the spell. The mound script is the one of the 5.4.8 data (it toggles the mound phase on data set).
-- Based on Legends of Azeroth 5.4.8.
DELETE FROM `gameobject_template` WHERE `entry` IN (201943);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `AIName`) VALUES
(201943, 8, 0, 'Silithid Mound', 2, 1642, 16, 'SmartGameObjectAI');

DELETE FROM `gameobject` WHERE `guid` BETWEEN 910938 AND 910959;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(910938, 201943, 1, 1, 0, 0, -3192.46, -2220.29, 101.203, 0, 0, 0, 0, 0, 0, 0, 1),
(910939, 201943, 1, 1, 0, 0, -3172.76, -2213.14, 99.9466, 0, 0, 0, 0, 0, 0, 0, 1),
(910940, 201943, 1, 1, 0, 0, -3173.69, -2224.49, 103.163, 0, 0, 0, 0, 0, 0, 0, 1),
(910941, 201943, 1, 1, 0, 0, -3124.18, -2174.15, 104.72, 0, 0, 0, 0, 0, 0, 0, 1),
(910942, 201943, 1, 1, 0, 0, -3141.29, -2172.42, 105.415, 0, 0, 0, 0, 0, 0, 0, 1),
(910943, 201943, 1, 1, 0, 0, -3286.97, -1855.27, 101.884, 0, 0, 0, 0, 0, 0, 0, 1),
(910944, 201943, 1, 1, 0, 0, -3084.42, -1687.71, 97.0614, 0, 0, 0, 0, 0, 0, 0, 1),
(910945, 201943, 1, 1, 0, 0, -3217.91, -1859.82, 100.11, 0, 0, 0, 0, 0, 0, 0, 1),
(910946, 201943, 1, 1, 0, 0, -3207.31, -1850.13, 96.9181, 0, 0, 0, 0, 0, 0, 0, 1),
(910947, 201943, 1, 1, 0, 0, -3220.4, -1846.63, 95.7526, 0, 0, 0, 0, 0, 0, 0, 1),
(910948, 201943, 1, 1, 0, 0, -3276.5, -1842.19, 96.89, 0, 0, 0, 0, 0, 0, 0, 1),
(910949, 201943, 1, 1, 0, 0, -3108.77, -2187.94, 102.912, 0, 0, 0, 0, 0, 0, 0, 1),
(910950, 201943, 1, 1, 0, 0, -3126.91, -2185.94, 102.978, 0, 0, 0, 0, 0, 0, 0, 1),
(910951, 201943, 1, 1, 0, 0, -3183.92, -2235.41, 102.975, 0, 0, 0, 0, 0, 0, 0, 1),
(910952, 201943, 1, 1, 0, 0, -3125.19, -1788.59, 96.2424, 0, 0, 0, 0, 0, 0, 0, 1),
(910953, 201943, 1, 1, 0, 0, -3139.92, -1783.65, 96.9983, 0, 0, 0, 0, 0, 0, 0, 1),
(910954, 201943, 1, 1, 0, 0, -3105.27, -1713.22, 95.0152, 0, 0, 0, 0, 0, 0, 0, 1),
(910955, 201943, 1, 1, 0, 0, -3097.78, -1725.65, 96.8763, 0, 0, 0, 0, 0, 0, 0, 1),
(910956, 201943, 1, 1, 0, 0, -3157.93, -1780.33, 97.3306, 0, 0, 0, 0, 0, 0, 0, 1),
(910957, 201943, 1, 1, 0, 0, -3096.19, -1689.97, 100.659, 0, 0, 0, 0, 0, 0, 0, 1),
(910958, 201943, 1, 1, 0, 0, -3117.33, -1717.97, 99.3189, 0, 0, 0, 0, 0, 0, 0, 1),
(910959, 201943, 1, 1, 0, 0, -3118.93, -1777.08, 96.3375, 0, 0, 0, 0, 0, 0, 0, 1);

DELETE FROM `smart_scripts` WHERE `source_type` = 1 AND `entryorguid` = 201943;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(201943, 1, 0, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Silithid Mound - On Data Set 0 1 - Set Phase 1'),
(201943, 1, 1, 0, 38, 0, 100, 0, 0, 2, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Silithid Mound - On Data Set 0 2 - Set Phase 0');
