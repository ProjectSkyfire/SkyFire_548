SET @JFSH  := 3229;
SET @VEBSH := 2578;
SET @VFWSH := 2594;
SET @KWSH  := 3415;
SET @DWSH  := 3422;
SET @TSSH  := 3431;
SET @KLSSH := 3454;
SET @TISH  := 3441;
SET @IGSH  := 3449;

-- JADE FOREST, SPIRIT HEALER
DELETE FROM `creature` WHERE `guid` BETWEEN @JFSH+0 AND @JFSH+14;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(@JFSH+0, 6491, 870, 1, 1, 1726.333, -486.3212, 362.2103, 5.410521, 120, 0, 0), -- Tigers Wood
(@JFSH+1, 6491, 870, 1, 1, 2960.81, -662.0156, 232.3211, 6.142398, 120, 0, 0), -- Honeydew Glade
(@JFSH+2, 6491, 870, 1, 1, 2641.27, -507.4983, 322.6477, 5.393067, 120, 0, 0), -- Ascent of Swirling Wind
(@JFSH+3, 6491, 870, 1, 1, 2524.281, -1162.384, 387.5573, 3.961897, 120, 0, 0), -- Waxwood
(@JFSH+4, 6491, 870, 1, 1, 445.3004, -1575.872, 162.0882, 0.03701474, 120, 0, 0), -- Serpents Overlook
(@JFSH+5, 6491, 870, 1, 1, 559.0243, -1363.87, 70.42532, 3.036873, 120, 0, 0), -- The Thunderwood
(@JFSH+6, 6491, 870, 1, 1, 74.64063, -2089.993, 45.97343, 1.623156, 120, 0, 0), -- Fox Grove
(@JFSH+7, 6491, 870, 1, 1, -291.7535, -2882.408, 13.60562, 2.583087, 120, 0, 0), -- The Bamboo Wilds
(@JFSH+8, 6491, 870, 1, 1, -80.99479, -3213.057, 170.3893, 3.873419, 120, 0, 0), -- The Jade Forest
(@JFSH+9, 6491, 870, 1, 1, 1022.83, -2358.967, 155.9035, 1.867502, 120, 0, 0),  -- Jade Temple Grounds
(@JFSH+10, 6491, 870, 1, 1, 1737.328, -2776.705, 130.6451, 5.218534, 120, 0, 0), -- Mistveil Sea
(@JFSH+11, 6491, 870, 1, 1, 2753.252, -2345.922, 52.89109, 2.821924, 120, 0, 0), -- Sri-La Village
(@JFSH+12, 6491, 870, 1, 1, 1772.613, -1867.764, 193.4039, 0.6981317, 120, 0, 0), -- The Jade Forest
(@JFSH+13, 6491, 870, 1, 1, -431.0868, -1715.299, 12.68875, 4.080874, 120, 0, 0), -- Pawdon Glade
(@JFSH+14, 6491, 870, 1, 1, -698.6094, -1470.443, 130.2984, 6.072221, 120, 0, 0); -- Garroshar Point

-- VALE OF ETERNAL BLOSSOMS, SPIRIT HEALER
DELETE FROM `creature` WHERE `guid` BETWEEN @VEBSH+0 AND @VEBSH+5;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(@VEBSH+0, 6491, 870, 1, 1, 928.5833, 714.1389, 401.5363, 5.1959, 120, 0, 0), -- The Summer Fields
(@VEBSH+1, 6491, 870, 1, 1, 1162.618, 689.8524, 348.9932, 5.577469, 120, 0, 0), -- The Summer Fields
(@VEBSH+2, 6491, 870, 1, 1, 1366.585, 987.8212, 439.0717, 4.701169, 120, 0, 0), -- The Golden Pagoda
(@VEBSH+3, 6491, 870, 1, 1, 1420.188, 1488.34, 412.0659, 2.368797, 120, 0, 0), -- The Emperors Approach
(@VEBSH+4, 6491, 870, 1, 1, 1189.365, 1600.057, 353.643, 4.689661, 120, 0, 0), -- Ruins of Guo-Lai
(@VEBSH+5, 6491, 870, 1, 1, 697.1337, 1706.354, 371.5172, 4.838484, 120, 0, 0); -- Mistfall Village

-- VALLEY OF FOUR WINDS, SPIRIT HEALER
DELETE FROM `creature` WHERE `guid` BETWEEN @VFWSH+0 AND @VFWSH+7;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(@VFWSH+0, 6491, 870, 1, 1, 386.5434, 1579.417, 525.6899, 5.531983, 120, 0, 0), -- Mistfall Village
(@VFWSH+1, 6491, 870, 1, 1, 346.1736, 1543.769, 459.871, 2.448036, 120, 0, 0), -- Skyrange
(@VFWSH+2, 6491, 870, 1, 1, 105.4097, 1469.517, 390.2218, 2.231816, 120, 0, 0), -- Skyrange
(@VFWSH+3, 6491, 870, 1, 1, 177.4288, 1168.389, 218.9032, 0.772003, 120, 0, 0), -- The Hearthland
(@VFWSH+4, 6491, 870, 1, 1, -188.4566, 1917.668, 153.1184, 2.359279, 120, 0, 0), -- Valley of the Four Winds
(@VFWSH+5, 6491, 870, 1, 1, -579.6302, 1320.021, 148.8249, 3.914643, 120, 0, 0), -- Thirsty Alley
(@VFWSH+6, 6491, 870, 1, 1, -87.09028, 581.5226, 164.2621, 0.671189, 120, 0, 0), -- Halfhill
(@VFWSH+7, 6491, 870, 1, 1, 203.283, -329.1233, 253.8773, 3.314796, 120, 0, 0); -- Shangs Stead

-- KRASARANG WILDS, SPIRIT HEALER
DELETE FROM `creature` WHERE `guid` BETWEEN @KWSH+0 AND @KWSH+6;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(@KWSH+0, 6491, 870, 1, 1, -397.3004, -762.0156, 121.0194, 0.438131, 120, 0, 0), -- Zhu Province
(@KWSH+1, 6491, 870, 1, 1, -776.0712, -1027.292, 23.04406, 1.880249, 120, 0, 0), -- Krasang Cove
(@KWSH+2, 6491, 870, 1, 1, -1202.477, 90.38368, 12.49605, 2.300893, 120, 0, 0), -- Dojani River
(@KWSH+3, 6491, 870, 1, 1, -1256.233, 792.2882, 14.6082, 4.738082, 120, 0, 0), -- The Deepwild
(@KWSH+4, 6491, 870, 1, 1, -1344.722, 1976.182, 17.25013, 4.135729, 120, 0, 0), -- Field of Korja
(@KWSH+5, 6491, 870, 1, 1, -1964.247, 2285.778, 10.11434, 1.928479, 120, 0, 0), -- Horde Landing Site
(@KWSH+6, 6491, 870, 1, 1, -2337.675, 847.2726, 3.915207, 4.014289, 120, 0, 0); -- Kea Krak

-- DREAD WASTES, SPIRIT HEALER
DELETE FROM `creature` WHERE `guid` BETWEEN @DWSH+0 AND @DWSH+8;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(@DWSH+0, 6491, 870, 1, 1, -547.3542, 2931.354, 166.3707, 6.261562, 120, 0, 0), -- Kypari Zar
(@DWSH+1, 6491, 870, 1, 1, 480.5017, 2921.834, 252.7419, 5.690177, 120, 0, 0), -- Dread Wastes
(@DWSH+2, 6491, 870, 1, 1, 624.9028, 3665.991, 226.2636, 4.249731, 120, 0, 0), -- Dread Wastes
(@DWSH+3, 6491, 870, 1, 1, 590.8577, 4285.592, 219.4432, 4.172816, 120, 0, 0), -- Rikkilea
(@DWSH+4, 6491, 870, 1, 1, 105.2257, 4024.095, 252.4905, 5.507441, 120, 0, 0), -- Hearth of Fear
(@DWSH+5, 6491, 870, 1, 1, -473.1875, 3886.236, 77.97404, 5.477821, 120, 0, 0), -- The Stinging Trail
(@DWSH+6, 6491, 870, 1, 1, -1255.62, 4139.921, 59.21844, 6.182305, 120, 0, 0), -- Zanvess
(@DWSH+7, 6491, 870, 1, 1, -1295.245, 4514.094, 128.713, 5.734471, 120, 0, 0), -- Zanvess
(@DWSH+8, 6491, 870, 1, 1, -1567.993, 4798.287, 84.64294, 5.825229, 120, 0, 0); -- Whispering Stones

-- TOWNLONG STEPPES, SPIRIT HEALER
DELETE FROM `creature` WHERE `guid` BETWEEN @TSSH+0 AND @TSSH+9;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(@TSSH+0, 6491, 870, 1, 1, 1414.708, 4935.429, 133.1904, 4.46418, 120, 0, 0), -- Townlong Steppes
(@TSSH+1, 6491, 870, 1, 1, 1282.682, 4248.37, 187.0942, 1.611064, 120, 0, 0), -- Townlong Steppes
(@TSSH+2, 6491, 870, 1, 1, 2639.085, 5927.719, 81.16915, 3.323705, 120, 0, 0), -- Sravess
(@TSSH+3, 6491, 870, 1, 1, 1709.363, 3580.348, 224.5457, 1.662819, 120, 0, 0), -- Townlong Steppes
(@TSSH+4, 6491, 870, 1, 1, 1630.757, 3025.664, 320.0259, 3.747507, 120, 0, 0), -- Townlong Steppes
(@TSSH+5, 6491, 870, 1, 1, 1656.484, 2505.312, 302.4094, 2.406457, 120, 0, 0), -- Dampsoil Burrow
(@TSSH+6, 6491, 870, 1, 1, 2418.066, 3350.924, 293.4684, 2.406457, 120, 0, 0), -- Townlong Steppes
(@TSSH+7, 6491, 870, 1, 1, 2387.321, 4096.947, 214.7875, 2.307981, 120, 0, 0), -- The Underbough
(@TSSH+8, 6491, 870, 1, 1, 2431.991, 4821.431, 189.4775, 0.6961535, 120, 0, 0), -- Niuzao Temple
(@TSSH+9, 6491, 870, 1, 1, 3956.588, 5530.747, 157.1528, 3.804418, 120, 0, 0); -- Shanze Dao

-- KUN-LAI SUMMIT, SPIRIT HEALER
DELETE FROM `creature` WHERE `guid` BETWEEN @KLSSH+0 AND @KLSSH+11;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(@KLSSH+0, 6491, 870, 1, 1, 2649.996, 2179.198, 581.5421, 5.602507, 120, 0, 0), -- The Yanguol Advance
(@KLSSH+1, 6491, 870, 1, 1, 3027.131, 1303.646, 648.1666, 3.242034, 120, 0, 0), -- Kun-Lai Summit
(@KLSSH+2, 6491, 870, 1, 1, 3137.311, 550.0313, 504.6653, 3.124139, 120, 0, 0), -- Temple of the White Tiger
(@KLSSH+3, 6491, 870, 1, 1, 4218.76, 639.9653, 113.7496, 0.8377581, 120, 0, 0), -- Zouchin Provice
(@KLSSH+4, 6491, 870, 1, 1, 4611.374, 145.5642, 15.27084, 3.176499, 120, 0, 0), -- Zouchin Provice
(@KLSSH+5, 6491, 870, 1, 1, 2104.58, 1080.422, 486.7882, 1.989675, 120, 0, 0), -- The Yanguol Advance
(@KLSSH+6, 6491, 870, 1, 1, 1849.828, 364.875, 482.4558, 2.303835, 120, 0, 0), -- Binan Village
(@KLSSH+7, 6491, 870, 1, 1, 4027.722, 1122.319, 498.9429, 3.819407, 120, 0, 0), -- Mogushan Terrace
(@KLSSH+8, 6491, 870, 1, 1, 3527.47, 2704.003, 756.7897, 5.210021, 120, 0, 0), -- Shado-Pan Monastery
(@KLSSH+9, 6491, 870, 1, 1, 3547.536, 1642.849, 839.6157, 0.9075712, 120, 0, 0), -- Kun-Lai Summit
(@KLSSH+10, 6491, 870, 1, 1, 3568.925, 1349.953, 799.5275, 0.8635779, 120, 0, 0), -- Valley of Emperors
(@KLSSH+11, 6491, 870, 1, 1, 3488.101, 2099.179, 1084.123, 2.513274, 120, 0, 0); -- Seekers Point

-- TIMELESS ISLE, SPIRIT HEALER
DELETE FROM `creature` WHERE `guid` BETWEEN @TISH+0 AND @TISH+7;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(@TISH+0, 72676, 870, 1, 1, -382.3368, -4626.083, 4.33231, 0.673996, 120, 0, 0), -- Huojin Landing
(@TISH+1, 72676, 870, 1, 1, -928.6476, -4674.804, 2.250625, 4.883155, 120, 0, 0), -- Tushui Landing
(@TISH+2, 72676, 870, 1, 1, -920.1458, -5039.698, 2.215825, 4.468898, 120, 0, 0), -- Old Pijiu
(@TISH+3, 72676, 870, 1, 1, -1100.913, -5227.578, 27.31944, 6.055438, 120, 0, 0), -- Firewalkers Ruins
(@TISH+4, 72676, 870, 1, 1, -745.0104, -5722.963, 54.04625, 4.809022, 120, 0, 0), -- The Blazing Way
(@TISH+5, 72676, 870, 1, 1, -548.4375, -5652.497, 17.82596, 2.746226, 120, 0, 0), -- Whispershade Hollow
(@TISH+6, 72676, 870, 1, 1, -649.9184, -5113.785, 2.139323, 4.594717, 120, 0, 0), -- The Celestial Court
(@TISH+7, 72676, 870, 1, 1, -661.2465, -4865.059, 2.136961, 6.178485, 120, 0, 0); -- Timeless Isle

-- ISLE OF GIANTS, SPIRIT HEALER
DELETE FROM `creature` WHERE `guid` = @IGSH;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(@IGSH, 6491, 870, 1, 1, 5950.813, 1154.814, 60.70548, 4.334397, 120, 0, 0); -- Isle of Giants

