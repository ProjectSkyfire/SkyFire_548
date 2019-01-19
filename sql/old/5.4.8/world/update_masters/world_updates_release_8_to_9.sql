-- Add Cyclonic Inspiration at Entering Area "Shrine of Two Moons" and "Shrine of Seven Stars"
DELETE FROM `spell_area` WHERE `spell` IN (128943, 131526); -- These spells used only here
INSERT INTO `spell_area` (`spell`, `area`, `autocast`, `racemask`) VALUES
(128943, 6484, 1, 18875469), -- Shrine of Seven Stars
(131526, 6519, 1, 33555378); -- Shrine of Two Moons DROP TABLE `creature_classlevelstats`;
CREATE TABLE `creature_classlevelstats` (
	`level` TINYINT(4) NOT NULL,
	`class` TINYINT(4) NOT NULL,
	`OldContentBaseHP` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '1',
	`CurrentContentBaseHP` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '1',
	`basemana` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '1',
	`basearmor` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '1',
	PRIMARY KEY (`level`, `class`)
)
COLLATE='utf8_general_ci'
ENGINE=MyISAM;
DELETE FROM `creature_classlevelstats`;
INSERT INTO `creature_classlevelstats` (`level`, `class`, `OldContentBaseHP`, `CurrentContentBaseHP`, `basemana`, `basearmor`) VALUES
(1, 1, 40, 40, 0, 8),
(2, 1, 50, 50, 0, 33),
(3, 1, 55, 55, 0, 33),
(4, 1, 60, 60, 0, 68),
(5, 1, 75, 75, 0, 111),
(6, 1, 90, 90, 0, 165),
(7, 1, 100, 100, 0, 230),
(8, 1, 110, 110, 0, 306),
(9, 1, 120, 120, 0, 387),
(10, 1, 141, 141, 0, 463),
(11, 1, 261, 261, 0, 528),
(12, 1, 312, 312, 0, 562),
(13, 1, 321, 321, 0, 596),
(14, 1, 375, 375, 0, 630),
(15, 1, 384, 384, 0, 665),
(16, 1, 432, 432, 0, 700),
(17, 1, 441, 441, 0, 734),
(18, 1, 488, 488, 0, 768),
(19, 1, 533, 533, 0, 802),
(20, 1, 547, 547, 0, 836),
(21, 1, 593, 593, 0, 872),
(22, 1, 603, 603, 0, 906),
(23, 1, 672, 672, 0, 940),
(24, 1, 695, 695, 0, 975),
(25, 1, 788, 788, 0, 1008),
(26, 1, 804, 804, 0, 1043),
(27, 1, 841, 841, 0, 1078),
(28, 1, 867, 867, 0, 1111),
(29, 1, 928, 928, 0, 1145),
(30, 1, 950, 950, 0, 1179),
(31, 1, 1049, 1049, 0, 1213),
(32, 1, 1067, 1067, 0, 1249),
(33, 1, 1145, 1145, 0, 1281),
(34, 1, 1183, 1183, 0, 1317),
(35, 1, 1245, 1245, 0, 1349),
(36, 1, 1270, 1270, 0, 1456),
(37, 1, 1333, 1333, 0, 1568),
(38, 1, 1358, 1358, 0, 1684),
(39, 1, 1447, 1447, 0, 1808),
(40, 1, 1474, 1474, 0, 1938),
(41, 1, 1589, 1589, 0, 2074),
(42, 1, 1612, 1612, 0, 2218),
(43, 1, 1660, 1660, 0, 2369),
(44, 1, 1688, 1688, 0, 2528),
(45, 1, 1793, 1793, 0, 2695),
(46, 1, 1833, 1833, 0, 2750),
(47, 1, 1950, 1950, 0, 2804),
(48, 1, 1976, 1976, 0, 2857),
(49, 1, 2044, 2044, 0, 2912),
(50, 1, 2070, 2070, 0, 2966),
(51, 1, 2171, 2171, 0, 3018),
(52, 1, 2208, 2208, 0, 3060),
(53, 1, 2300, 2300, 0, 3128),
(54, 1, 2334, 2334, 0, 3180),
(55, 1, 2422, 2422, 0, 3234),
(56, 1, 2467, 2467, 0, 3289),
(57, 1, 2563, 2563, 0, 3342),
(58, 1, 2593, 2593, 0, 3396),
(59, 1, 2747, 2747, 0, 3449),
(60, 1, 2937, 2937, 0, 3750),
(61, 1, 3037, 3037, 0, 4047),
(62, 1, 3122, 3122, 0, 4344),
(63, 1, 3207, 3207, 0, 4641),
(64, 1, 3352, 3352, 0, 4937),
(65, 1, 3516, 3516, 0, 5234),
(66, 1, 3635, 3635, 0, 5531),
(67, 1, 3820, 3820, 0, 5829),
(68, 1, 3858, 3858, 0, 6126),
(69, 1, 3974, 3974, 0, 6423),
(70, 1, 4299, 4299, 0, 6719),
(71, 1, 4443, 4443, 0, 7018),
(72, 1, 4608, 4608, 0, 7318),
(73, 1, 4886, 4886, 0, 7618),
(74, 1, 4973, 4973, 0, 7918),
(75, 1, 5118, 5118, 0, 8219),
(76, 1, 5239, 5239, 0, 8520),
(77, 1, 5298, 5298, 0, 8822),
(78, 1, 5408, 5408, 0, 9124),
(79, 1, 5461, 5461, 0, 9426),
(80, 1, 5534, 5534, 0, 9729),
(81, 1, 6013, 6013, 0, 10033),
(82, 1, 6465, 6465, 0, 10356),
(83, 1, 7096, 7096, 0, 10673),
(84, 1, 7500, 7500, 0, 10999),
(85, 1, 7987, 7987, 0, 11334),
(86, 1, 8840, 8840, 0, 11679),
(87, 1, 9585, 9585, 0, 12034),
(88, 1, 10174, 10174, 0, 12399),
(89, 1, 10883, 10883, 0, 12775),
(90, 1, 11494, 17503, 0, 13162),
(91, 1, 11891, 20475, 0, 13560),
(92, 1, 12301, 22939, 0, 13970),
(93, 1, 12725, 26964, 0, 14392),
(94, 1, 20481, 31613, 0, 14826),
(95, 1, 1, 1, 0, 1),
(1, 2, 40, 40, 60, 7),
(2, 2, 50, 50, 69, 19),
(3, 2, 55, 55, 79, 33),
(4, 2, 60, 60, 104, 66),
(5, 2, 75, 75, 115, 109),
(6, 2, 90, 90, 126, 163),
(7, 2, 100, 100, 138, 208),
(8, 2, 110, 110, 165, 303),
(9, 2, 120, 120, 178, 369),
(10, 2, 141, 141, 191, 460),
(11, 2, 261, 261, 205, 526),
(12, 2, 312, 312, 249, 560),
(13, 2, 321, 321, 264, 596),
(14, 2, 375, 375, 295, 630),
(15, 2, 384, 384, 326, 665),
(16, 2, 432, 432, 357, 700),
(17, 2, 441, 441, 390, 734),
(18, 2, 488, 488, 408, 768),
(19, 2, 533, 533, 456, 802),
(20, 2, 547, 547, 490, 836),
(21, 2, 593, 593, 510, 872),
(22, 2, 603, 603, 545, 906),
(23, 2, 672, 672, 581, 940),
(24, 2, 695, 695, 618, 975),
(25, 2, 788, 788, 655, 1008),
(26, 2, 804, 804, 693, 1042),
(27, 2, 841, 841, 732, 1078),
(28, 2, 867, 867, 756, 1110),
(29, 2, 928, 928, 811, 1145),
(30, 2, 950, 950, 852, 1178),
(31, 2, 1049, 1049, 878, 1213),
(32, 2, 1067, 1067, 935, 1248),
(33, 2, 1145, 1145, 963, 1281),
(34, 2, 1183, 1183, 1007, 1316),
(35, 2, 1245, 1245, 1067, 1349),
(36, 2, 1270, 1270, 1097, 1455),
(37, 2, 1333, 1333, 1142, 1567),
(38, 2, 1358, 1358, 1189, 1683),
(39, 2, 1447, 1447, 1236, 1807),
(40, 2, 1474, 1474, 1283, 1937),
(41, 2, 1589, 1589, 1332, 2072),
(42, 2, 1612, 1612, 1381, 2216),
(43, 2, 1660, 1660, 1432, 2367),
(44, 2, 1688, 1688, 1483, 2527),
(45, 2, 1793, 1793, 1534, 2692),
(46, 2, 1833, 1833, 1587, 2749),
(47, 2, 1950, 1950, 1640, 2802),
(48, 2, 1976, 1976, 1695, 2855),
(49, 2, 2044, 2044, 1750, 2910),
(50, 2, 2070, 2070, 1807, 2964),
(51, 2, 2171, 2171, 1864, 3017),
(52, 2, 2208, 2208, 1923, 3072),
(53, 2, 2300, 2300, 1982, 3126),
(54, 2, 2334, 2334, 2041, 3178),
(55, 2, 2422, 2422, 2117, 3232),
(56, 2, 2467, 2467, 2163, 3287),
(57, 2, 2563, 2563, 2241, 3340),
(58, 2, 2593, 2593, 2289, 3394),
(59, 2, 2747, 2747, 2369, 3447),
(60, 2, 2937, 2937, 2434, 3748),
(61, 2, 3037, 3037, 2486, 4044),
(62, 2, 3122, 3122, 2568, 4340),
(63, 2, 3207, 3207, 2620, 4637),
(64, 2, 3352, 3352, 2705, 4933),
(65, 2, 3516, 3516, 2790, 5228),
(66, 2, 3635, 3635, 2846, 5523),
(67, 2, 3820, 3820, 2933, 5821),
(68, 2, 3858, 3858, 2991, 6116),
(69, 2, 3974, 3974, 3080, 6412),
(70, 2, 4299, 4299, 3155, 6708),
(71, 2, 4443, 4443, 3231, 7007),
(72, 2, 4608, 4608, 3309, 7305),
(73, 2, 4886, 4886, 3387, 7604),
(74, 2, 4973, 4973, 3466, 7903),
(75, 2, 5118, 5118, 3561, 8204),
(76, 2, 5239, 5239, 3643, 8503),
(77, 2, 5298, 5298, 3725, 8803),
(78, 2, 5408, 5408, 3809, 9104),
(79, 2, 5461, 5461, 3893, 9405),
(80, 2, 5534, 5534, 3994, 9706),
(81, 2, 6013, 6013, 4081, 10007),
(82, 2, 6465, 6465, 4169, 10253),
(83, 2, 7096, 7096, 4258, 10573),
(84, 2, 7500, 7500, 8726, 10902),
(85, 2, 7987, 7987, 8908, 11241),
(86, 2, 8840, 8840, 9094, 11590),
(87, 2, 9585, 9585, 9310, 11949),
(88, 2, 10174, 10174, 9470, 12319),
(89, 2, 10883, 10883, 9692, 12700),
(90, 2, 11494, 17503, 9916, 13092),
(91, 2, 11891, 20475, 10145, 13496),
(92, 2, 12301, 22939, 10379, 13912),
(93, 2, 12725, 26964, 10618, 14340),
(94, 2, 20481, 31613, 10862, 14781),
(95, 2, 1, 1, 1, 1),
(1, 4, 40, 40, 0, 6),
(2, 4, 50, 50, 0, 18),
(3, 4, 55, 55, 0, 31),
(4, 4, 60, 60, 0, 63),
(5, 4, 75, 75, 0, 102),
(6, 4, 90, 90, 0, 152),
(9, 4, 120, 120, 0, 369),
(8, 4, 110, 110, 0, 303),
(7, 4, 100, 100, 0, 208),
(10, 4, 141, 141, 0, 443),
(11, 4, 261, 261, 0, 488),
(12, 4, 312, 312, 0, 519),
(13, 4, 321, 321, 0, 553),
(14, 4, 375, 375, 0, 577),
(15, 4, 384, 384, 0, 612),
(16, 4, 432, 432, 0, 645),
(17, 4, 441, 441, 0, 676),
(18, 4, 488, 488, 0, 706),
(19, 4, 533, 1, 0, 738),
(20, 4, 547, 547, 0, 769),
(21, 4, 593, 593, 0, 801),
(22, 4, 603, 603, 0, 833),
(23, 4, 672, 672, 0, 863),
(24, 4, 695, 695, 0, 895),
(25, 4, 788, 1, 0, 926),
(26, 4, 804, 804, 0, 957),
(27, 4, 841, 841, 0, 989),
(28, 4, 867, 867, 0, 1020),
(29, 4, 928, 928, 0, 1051),
(30, 4, 950, 950, 0, 1082),
(31, 4, 1049, 1049, 0, 1113),
(32, 4, 1067, 1067, 0, 1146),
(33, 4, 1145, 1145, 0, 1173),
(34, 4, 1183, 1183, 0, 1208),
(35, 4, 1245, 1245, 0, 1237),
(36, 4, 1270, 1270, 0, 1349),
(37, 4, 1333, 1333, 0, 1434),
(38, 4, 1358, 1358, 0, 1538),
(39, 4, 1447, 1447, 0, 1649),
(40, 4, 1474, 1474, 0, 1764),
(41, 4, 1589, 1589, 0, 1886),
(42, 4, 1612, 1612, 0, 2015),
(43, 4, 1660, 1660, 0, 2148),
(44, 4, 1688, 1688, 0, 2303),
(45, 4, 1793, 1793, 0, 2436),
(46, 4, 1833, 1833, 0, 2485),
(47, 4, 1950, 1950, 0, 2535),
(48, 4, 1976, 1976, 0, 2582),
(49, 4, 2044, 2044, 0, 2631),
(50, 4, 2070, 2070, 0, 2680),
(51, 4, 2171, 2171, 0, 2728),
(52, 4, 2208, 2208, 0, 2778),
(53, 4, 2300, 2300, 0, 2826),
(54, 4, 2334, 2334, 0, 2874),
(55, 4, 2422, 2422, 0, 2922),
(56, 4, 2467, 2467, 0, 2972),
(57, 4, 2563, 2563, 0, 3020),
(58, 4, 2593, 2593, 0, 3068),
(59, 4, 2747, 2747, 0, 3117),
(60, 4, 2937, 2937, 0, 3388),
(61, 4, 3037, 3037, 0, 3655),
(62, 4, 3122, 3122, 0, 3922),
(63, 4, 3207, 3207, 0, 4189),
(64, 4, 3352, 3352, 0, 4457),
(65, 4, 3516, 3516, 0, 4724),
(66, 4, 3635, 3635, 0, 5104),
(67, 4, 3820, 3820, 0, 5326),
(68, 4, 3858, 3858, 0, 5527),
(69, 4, 3974, 3974, 0, 5795),
(70, 4, 4299, 4299, 0, 6062),
(71, 4, 4443, 4443, 0, 6332),
(72, 4, 4608, 4608, 0, 6602),
(73, 4, 4886, 4886, 0, 6872),
(74, 4, 4973, 4973, 0, 7143),
(75, 4, 5118, 5118, 0, 7415),
(76, 4, 5239, 5239, 0, 7686),
(77, 4, 5298, 5298, 0, 7958),
(78, 4, 5408, 5408, 0, 8230),
(79, 4, 5461, 5461, 0, 8503),
(80, 4, 5534, 5534, 0, 8776),
(81, 4, 6013, 6013, 0, 9068),
(82, 4, 6465, 6465, 0, 9348),
(83, 4, 7096, 7096, 0, 9589),
(84, 4, 7500, 7500, 0, 9836),
(85, 4, 7987, 7987, 0, 10089),
(86, 4, 8840, 8840, 0, 10348),
(87, 4, 9585, 9585, 0, 10613),
(88, 4, 10174, 10174, 0, 10884),
(89, 4, 10883, 10883, 0, 11161),
(90, 4, 11494, 17503, 0, 11445),
(91, 4, 11891, 20475, 0, 11736),
(92, 4, 12301, 22939, 0, 12034),
(93, 4, 12725, 26964, 0, 12339),
(94, 4, 20481, 31613, 0, 12651),
(95, 4, 1, 1, 0, 1),
(1, 8, 32, 32, 120, 5),
(2, 8, 40, 40, 147, 16),
(3, 8, 44, 44, 174, 28),
(4, 8, 48, 48, 202, 57),
(5, 8, 60, 60, 230, 93),
(6, 8, 72, 72, 259, 139),
(7, 8, 80, 80, 289, 194),
(8, 8, 88, 88, 319, 265),
(9, 8, 96, 96, 350, 339),
(10, 8, 113, 113, 382, 423),
(11, 8, 209, 209, 459, 447),
(12, 8, 250, 250, 537, 475),
(13, 8, 257, 257, 601, 509),
(14, 8, 300, 300, 710, 523),
(15, 8, 307, 307, 790, 559),
(16, 8, 346, 346, 856, 589),
(17, 8, 353, 353, 938, 617),
(18, 8, 390, 390, 1020, 643),
(19, 8, 426, 426, 1118, 674),
(20, 8, 438, 438, 1202, 701),
(21, 8, 474, 474, 1272, 729),
(22, 8, 482, 482, 1357, 759),
(23, 8, 538, 538, 1443, 786),
(24, 8, 556, 556, 1545, 815),
(25, 8, 630, 630, 1633, 843),
(26, 8, 643, 643, 1707, 871),
(27, 8, 673, 673, 1812, 900),
(28, 8, 694, 694, 1977, 928),
(29, 8, 742, 742, 2068, 957),
(30, 8, 760, 760, 2175, 984),
(31, 8, 839, 839, 2253, 1012),
(32, 8, 854, 854, 2362, 1042),
(33, 8, 916, 916, 2457, 1065),
(34, 8, 946, 946, 2553, 1098),
(35, 8, 996, 996, 2680, 1124),
(36, 8, 1016, 1016, 2763, 1241),
(37, 8, 1066, 1066, 2861, 1300),
(38, 8, 1086, 1086, 2975, 1391),
(39, 8, 1158, 1158, 3075, 1489),
(40, 8, 1179, 1179, 3191, 1590),
(41, 8, 1271, 1271, 3293, 1697),
(42, 8, 1290, 1290, 3471, 1811),
(43, 8, 1328, 1328, 3575, 1926),
(44, 8, 1350, 1350, 3680, 2078),
(45, 8, 1434, 1434, 3801, 2177),
(46, 8, 1466, 1466, 3923, 2220),
(47, 8, 1560, 1560, 4031, 2265),
(48, 8, 1581, 1581, 4140, 2307),
(49, 8, 1635, 1635, 4281, 2349),
(50, 8, 1656, 1656, 4393, 2393),
(51, 8, 1737, 1737, 4506, 2437),
(52, 8, 1766, 1766, 4650, 2481),
(53, 8, 1840, 1840, 4765, 2524),
(54, 8, 1867, 1867, 4896, 2567),
(55, 8, 1938, 1938, 5013, 2609),
(56, 8, 1974, 1974, 5206, 2654),
(57, 8, 2050, 2050, 5340, 2698),
(58, 8, 2074, 2074, 5461, 2740),
(59, 8, 2198, 2198, 5598, 2784),
(60, 8, 2350, 2350, 5751, 3025),
(61, 8, 2430, 2430, 5875, 3263),
(62, 8, 2498, 2498, 6015, 3500),
(63, 8, 2566, 2566, 6156, 3736),
(64, 8, 2682, 2682, 6299, 3977),
(65, 8, 2813, 2813, 6443, 4214),
(66, 8, 2908, 2908, 6588, 4460),
(67, 8, 3056, 3056, 6749, 4710),
(68, 8, 3086, 3086, 6882, 4928),
(69, 8, 3179, 3179, 7031, 5167),
(70, 8, 3439, 3439, 7196, 5404),
(71, 8, 3554, 3554, 7332, 5645),
(72, 8, 3686, 3686, 7500, 5886),
(73, 8, 3909, 3909, 7654, 6126),
(74, 8, 3978, 3978, 7809, 6368),
(75, 8, 4094, 4094, 7981, 6610),
(76, 8, 4191, 4191, 8139, 6851),
(77, 8, 4238, 4238, 8313, 7094),
(78, 8, 4326, 4326, 8459, 7335),
(79, 8, 4369, 4369, 8636, 7579),
(80, 8, 4427, 4427, 8814, 7822),
(81, 8, 4810, 4810, 8979, 8102),
(82, 8, 5172, 5172, 9160, 8340),
(83, 8, 5677, 5677, 9328, 8505),
(84, 8, 6000, 6000, 9499, 8673),
(85, 8, 6390, 6390, 9673, 8844),
(86, 8, 7072, 7072, 9850, 9018),
(87, 8, 7668, 7668, 10030, 9195),
(88, 8, 8139, 8139, 10213, 9375),
(89, 8, 8706, 8706, 10399, 9558),
(90, 8, 9195, 14002, 10588, 9744),
(91, 8, 9513, 16380, 10780, 9933),
(92, 8, 9841, 18351, 10975, 10125),
(93, 8, 10180, 21571, 11173, 10320),
(94, 8, 16385, 25290, 11374, 10518),
(95, 8, 1, 1, 1, 1);
UPDATE creature_template SET npcflag=16384, type_flags=2, type_flags2=0 WHERE entry IN
(65183, 39660, 29259, 6491);

UPDATE creature_template SET npcflag=32769, type_flags=2, type_flags2=0 WHERE entry IN
(32004, 31920, 22558, 22526, 13117, 13116, 45078, 72676, 45076,
 45075, 45074, 45073, 45068, 45067, 45066, 31842, 31841);
 
UPDATE creature_template SET minlevel=90, maxlevel=90, exp=4, faction_A=35, faction_H=35 WHERE entry=72676;

DELETE FROM creature_template_addon WHERE entry IN 
(197,35365,41804,41786,25018,25017,25016,25015,25011,54662,
12785,52654,29298,29297,29291,29299,29287,29288,29290,29289,
42317,42489,42366,51128,12943,34715,34721,34723,46164,14827,
12794,54657,12795,54658,12793,54659,35364,36816,36687,36680,
36385,28126,34419,49406,34046,48068,49912,49933,13085,23776,
25070,25077,25079,25083,41196,41199,37170,25078,25093,24924,
24926,36506,43430,43537,42696,42789,54763,55151,57242,57319,
57324,57421,59073,59077,59414,59416,59577,59790,60180,61067,
61068,62266,62293,62440,62530,62559,62760,62810,63506,63507,
65160,65178,65384,65626,65628,66269,66289,66346,66368);
DELETE FROM `areatrigger_tavern` WHERE `id` IN (7983, 8442, 8579);
INSERT INTO `areatrigger_tavern` (`id`, `name`) VALUES
(7983, 'Jade Temple Grounds - Inn'),
(8442, 'Tavern in the Mists - Inn'),
(8579, 'Honeydew Village - Inn');
-- Golden Lotus
DELETE FROM `creature_onkill_reputation` WHERE `creature_id` IN
(72275, 58411, 65170, 72280, 63447, 63674, 59378, 63044, 58928, 65132, 63610, 58927, 65134, 65131, 65133, 63611, 63641, 58455, 63605, 58456);
INSERT INTO `creature_onkill_reputation` (`creature_id`, `RewOnKillRepFaction1`, `RewOnKillRepFaction2`, `MaxStanding1`, `RewOnKillRepValue1`, `MaxStanding2`, `RewOnKillRepValue2`) VALUES
(72275, 1269, 0, 7, 20, 0, 0),     -- Black Bengal Goat + 20 reputation with Golden Lotus
(58411, 1269, 0, 6, 20, 0, 0),     -- Docile Porcupine + 20 reputation with Golden Lotus (stops at Revered)
(65170, 1269, 1359, 7, 20, 5, 10), -- Jade Warrior + 20 reputation with Golden Lotus, + 10 reputation with The Black Prince (stops at Honored)
(72280, 1269, 0, 7, 20, 0, 0),     -- Manifestation of Pride + 20 reputation with Golden Lotus
(63447, 1269, 1359, 7, 20, 5, 10), -- Mogu Statue + 20 reputation with Golden Lotus, + 10 reputation with The Black Prince (stops at Honored)
(63674, 1269, 1359, 7, 20, 5, 10), -- Mogu Statue + 20 reputation with Golden Lotus, + 10 reputation with The Black Prince (stops at Honored)
(59378, 1269, 0, 7, 20, 0, 0),     -- Paleblade Flesheater + 20 reputation with Golden Lotus
(63044, 1269, 0, 6, 20, 0, 0),     -- Paleblade Slithertongue + 20 reputation with Golden Lotus (stops at Revered)
(58928, 1269, 1359, 7, 20, 5, 10), -- Shao-Tien Antiquator + 20 reputation with Golden Lotus, + 10 reputation with The Black Prince (stops at Honored)
(65132, 1269, 1359, 7, 20, 5, 10), -- Shao-Tien Conqueror + 20 reputation with Golden Lotus, + 10 reputation with The Black Prince (stops at Honored)
(63610, 1269, 1359, 7, 20, 6, 10), -- Shao-Tien Dominator + 20 reputation with Golden Lotus, + 10 reputation with The Black Prince (stops at Revered)
(58927, 1269, 1359, 7, 20, 5, 10), -- Shao-Tien Fist + 20 reputation with Golden Lotus, + 10 reputation with The Black Prince (stops at Honored)
(65134, 1269, 1359, 7, 20, 5, 10), -- Shao-Tien Fist + 20 reputation with Golden Lotus, + 10 reputation with The Black Prince (stops at Honored)
(65131, 1269, 1359, 7, 20, 5, 10), -- Shao-Tien Painweaver + 20 reputation with Golden Lotus, + 10 reputation with The Black Prince (stops at Honored)
(65133, 1269, 1359, 7, 20, 5, 10), -- Shao-Tien Sorcerer + 20 reputation with Golden Lotus, + 10 reputation with The Black Prince (stops at Honored)
(63611, 1269, 1359, 7, 20, 5, 10), -- Shao-Tien Soul-Caller + 20 reputation with Golden Lotus, + 10 reputation with The Black Prince (stops at Honored)
(63641, 1269, 0, 7, 20, 0, 0),     -- Shao-Tien Torturer + 20 reputation with Golden Lotus
(58455, 1269, 0, 7, 20, 0, 0),     -- Stillwater Crocolisk + 20 reputation with Golden Lotus
(63605, 1269, 1359, 7, 20, 5, 10), -- Stonebound Watcher + 20 reputation with Golden Lotus, + 10 reputation with The Black Prince (stops at Honored)
(58456, 1269, 0, 7, 20, 0, 0);     -- Thundermaw + 20 reputation with Golden Lotus
-- The Klaxxi
DELETE FROM `creature_onkill_reputation` WHERE `creature_id`=62563;
INSERT INTO `creature_onkill_reputation` (`creature_id`, `RewOnKillRepFaction1`, `RewOnKillRepFaction2`, `MaxStanding1`, `RewOnKillRepValue1`, `MaxStanding2`, `RewOnKillRepValue2`) VALUES
(62563, 1337, 1359, 5, 500, 5, 10); -- Shek'zeer Bladesworn + 500 reputation with The Klaxxi (stops at Honored), + 10 reputation with The Black Prince (stops at Honored)
-- Set InhabitType 4 to some mobs that should be allowed to fly.
UPDATE `creature_template` SET `InhabitType`=4 WHERE `entry` IN
(59641,56171, 57422, 59528, 59235, 63973, 59700, 59112, 69815, 59788, 58440, 57445, 
 56201, 58363, 59757, 59785, 73531);
 
 -- Ordos Health and Mana was wrong parsed.
 UPDATE `creature_template` SET `Health_mod`=1500, `Mana_mod`=0 WHERE `entry`=72057;
-- Sungraze Mushan SAI
SET @ENTRY := 58893;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,0,7200,7300,7250,7350,11,117586,0,0,0,0,0,2,0,0,0,0,0,0,0,"Sungraze Mushan - In Combat - Bludgeon");
-- Weeping Horror SAI
SET @ENTRY := 57649;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,0,7200,8300,7400,8500,11,119556,0,0,0,0,0,2,0,0,0,0,0,0,0,"Weeping Horror - In Combat - Overwhelming Sadness");
-- Dojani Surveyor SAI
SET @ENTRY := 58068;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,0,21800,25400,24300,26700,11,11971,0,0,0,0,0,2,0,0,0,0,0,0,0,"Dojani Surveyor - In Combat - Sunder Armor");
-- Dojani Enforcer SAI
SET @ENFORCER        := 65626;  -- Dojani Enforcer
SET @SPELL_SHOCKWAVE := 129018; -- Shockwave
SET @SPELL_LEAP      := 129017; -- Leap of Victory
SET @SPELL_ENRAGE    := 129016; -- Enrage
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENFORCER;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENFORCER AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENFORCER,0,0,0,0,0,100,0,21800,21900,21700,23100,11,@SPELL_SHOCKWAVE,0,0,0,0,0,2,0,0,0,0,0,0,0,"Dojani Enforcer - In Combat - Shockwave"),
(@ENFORCER,0,1,0,0,0,100,0,13300,14600,14500,15800,11,@SPELL_LEAP,0,0,0,0,0,2,0,0,0,0,0,0,0,"Dojani Enforcer - In Combat - Leap of Victory"),
(@ENFORCER,0,2,0,0,0,100,0,12400,16700,16500,21800,11,@SPELL_ENRAGE,0,0,0,0,0,2,0,0,0,0,0,0,0,"Dojani Enforcer - In Combat - Enrage");
-- Mortbreath Snapper SAI
SET @SNAPPER         := 60201; -- Mortbreath Snapper
SET @SPELL_CHARGE    := 87930; -- Charge
SET @SPELL_JAW_SNAP  := 118990; -- Jaw Snap
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@SNAPPER;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@SNAPPER AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@SNAPPER,0,0,0,9,0,100,0,5,40,1000,1500,11,@SPELL_CHARGE,0,0,0,0,0,2,0,0,0,0,0,0,0,"Mortbreath Snapper - Within 5-40 Range - Charge"),
(@SNAPPER,0,1,0,0,0,100,0,12000,12900,12500,13400,11,@SPELL_JAW_SNAP,0,0,0,0,0,2,0,0,0,0,0,0,0,"Mortbreath Snapper - In Combat - Jaw Snap");
-- Mortbreath Skulker SAI
SET @SKULKER         := 60202; -- Mortbreath Skulker
SET @SPELL_CHARGE    := 87930; -- Charge
SET @SPELL_JAW_SNAP  := 118990; -- Jaw Snap
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@SKULKER;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@SKULKER AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@SKULKER,0,0,0,9,0,100,0,5,40,1000,1500,11,@SPELL_CHARGE,0,0,0,0,0,2,0,0,0,0,0,0,0,"Mortbreath Skulker - Within 5-40 Range - Charge"),
(@SKULKER,0,1,0,0,0,100,0,12000,12900,12500,13400,11,@SPELL_JAW_SNAP,0,0,0,0,0,2,0,0,0,0,0,0,0,"Mortbreath Skulker - In Combat - Jaw Snap");
SET @CRIMSON := 60198; -- Crimson Lory
SET @DUSKY := 60196; -- Dusky Lory
UPDATE `creature_template` SET `InhabitType`=4 WHERE `entry` IN (@DUSKY, @CRIMSON);
SET @GOBJECT := 41602; -- Bronze Gift of the Crane
SET @ITEM := 80938; -- Gift of the Great Crane
DELETE FROM `gameobject_loot_template` WHERE `entry`=@GOBJECT;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(@GOBJECT, @ITEM, -100);
SET @WULI := 63994;
SET @POOL := 12001;
SET @QUEST1 := 31519;
SET @QUEST2 := 31520;
SET @QUEST3 := 31522;
SET @QUEST4 := 31523;
SET @QUEST5 := 31524;
SET @QUEST6 := 31525;
SET @QUEST7 := 31526;
SET @QUEST8 := 31527;
SET @QUEST9 := 31528;

DELETE FROM `creature_queststarter` WHERE `id`=@WULI;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(@WULI, @QUEST1),
(@WULI, @QUEST2),
(@WULI, @QUEST3),
(@WULI, @QUEST4),
(@WULI, @QUEST5),
(@WULI, @QUEST6),
(@WULI, @QUEST7),
(@WULI, @QUEST8),
(@WULI, @QUEST9);

DELETE FROM `creature_questender` WHERE `id`=@WULI;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(@WULI, @QUEST1),
(@WULI, @QUEST2),
(@WULI, @QUEST3),
(@WULI, @QUEST4),
(@WULI, @QUEST5),
(@WULI, @QUEST6),
(@WULI, @QUEST7),
(@WULI, @QUEST8),
(@WULI, @QUEST9);

DELETE FROM `pool_template` WHERE `entry`=@POOL;
INSERT INTO `pool_template` (`entry`, `max_limit`, `description`) VALUES
(@POOL, 1, 'Challenger Wuli - Daily Quests');

DELETE FROM `pool_quest` WHERE `pool_entry`=@POOL;
INSERT INTO `pool_quest` (`entry`, `pool_entry`, `description`) VALUES
(@QUEST1, @POOL, 'A Worthy Challenge: Yan-zhu the Uncasked'),
(@QUEST2, @POOL, 'A Worthy Challenge: Sha of Doubt'),
(@QUEST3, @POOL, 'A Worthy Challenge: Sha of Hatred'),
(@QUEST4, @POOL, 'A Worthy Challenge: Xin the Weaponmaster'),
(@QUEST5, @POOL, 'A Worthy Challenge: Raigonn'),
(@QUEST6, @POOL, 'A Worthy Challenge: Wing Leader Ner''onok'),
(@QUEST7, @POOL, 'A Worthy Challenge: Durand'),
(@QUEST8, @POOL, 'A Worthy Challenge: Flameweaver Koegler'),
(@QUEST9, @POOL, 'A Worthy Challenge: Darkmaster Gandling');
