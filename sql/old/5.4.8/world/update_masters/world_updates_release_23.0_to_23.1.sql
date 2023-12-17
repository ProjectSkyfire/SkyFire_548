-- Update quest template with new quest text for priest quest.
UPDATE `quest_template` SET `Title`='Learning the Word', `Objectives`='Reach level 3 to learn Shadow Word: Pain, then use it 5 times on training dummies near Northshire Abbey.', `Details`='One of your responsibilities as a priest is caring for the wounded. Another is to go out into the world and be more... proactive about defending them.$B$BAs you do so, you\'ll grow in power and learn all sorts of new things. Why don\'t you go out, get more experience, and then return to demonstrate what you\'ve learned?', `OfferRewardText`='You\'re a quick learner, $n, and the wounded are fortunate to have your aid. Return to me as you grow stronger and I\'ll teach you other spells.', `RequestItemsText`='Well, did you give it a try?' WHERE  `Id`=26919;
UPDATE `quest_template` SET `OfferRewardText`='I think you now understand the power of the Light. The Light giveth hope, $g brother:sister; and the Light taketh from the darkness! BLESSED BE THE LIGHT!' WHERE  `Id`=28809;

UPDATE `quest_template` SET `OfferRewardText`='I think you now understand the power of the Light. The Light giveth hope, $g brother:sister; and the Light taketh from the darkness! BLESSED BE THE LIGHT!' WHERE  `Id`=28810;

UPDATE `quest_template` SET `OfferRewardText`='I think you now understand the power of the Light. The Light giveth hope, $g brother:sister; and the Light taketh from the darkness! BLESSED BE THE LIGHT!' WHERE  `Id`=28811;

UPDATE `quest_template` SET `OfferRewardText`='I think you now understand the power of the Light. The Light giveth hope, $g brother:sister; and the Light taketh from the darkness! BLESSED BE THE LIGHT!' WHERE  `Id`=28812;

UPDATE `quest_template` SET `OfferRewardText`='I think you now understand the power of the Light. The Light giveth hope, $g brother:sister; and the Light taketh from the darkness! BLESSED BE THE LIGHT!' WHERE  `Id`=28813;

UPDATE `quest_template` SET `OfferRewardText`='I think you now understand the power of the Light. The Light giveth hope, $g brother:sister; and the Light taketh from the darkness! BLESSED BE THE LIGHT!' WHERE  `Id`=29082;
UPDATE `creature_template_addon` SET `auras`='29266' WHERE `entry`=237;
UPDATE `creature_template_addon` SET `auras`='29266' WHERE `entry`=238;
UPDATE `creature_template_addon` SET `auras`='29266' WHERE `entry`=582;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=42308;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=42309;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=42311;

DELETE FROM `creature` WHERE `map`=34;
DELETE FROM `gameobject` WHERE `map`=34;
DELETE FROM `creature` WHERE `map`=36;
DELETE FROM `gameobject` WHERE `map`=36;
SET @CGUID := 134075;
SET @OGUID := 101193;

DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID-141 AND @CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 1720, 34, 2, 103.112, 77.7348, -34.8562, 5.94116),
(@CGUID-1, 46300, 34, 2, 100.481, -63.3902, -34.7729, 4.41568),
(@CGUID-2, 46300, 34, 2, 77.1173, -85.7022, -33.8563, 1.36136),
(@CGUID-3, 46299, 34, 2, 136.265, -50.0503, -34.7729, 0.837758),
(@CGUID-4, 46299, 34, 2, 130.365, -39.4408, -33.8563, 4.01426),
(@CGUID-5, 46300, 34, 2, 125.168, -69.6588, -33.8563, 1.36136),
(@CGUID-6, 46299, 34, 2, 100.069, -44.6332, -34.7729, 1.36136),
(@CGUID-7, 46299, 34, 2, 97.0772, -96.3446, -35.1063, 1.36136),
(@CGUID-8, 46300, 34, 2, 126.23, -33.708, -33.8563, 0.785398),
(@CGUID-9, 46383, 34, 2, 172.463, -2.24164, -25.5229, 2.72271),
(@CGUID-10, 46379, 34, 2, 109.06, -15.5223, -26.5229, 1.99201),
(@CGUID-11, 46379, 34, 2, 132.009, 1.1699, -25.5229, 2.53995),
(@CGUID-12, 46382, 34, 2, 111.84, 18.6591, -26.4396, 2.16421),
(@CGUID-13, 46382, 34, 2, 110.96, 24.5613, -26.4396, 3.75246),
(@CGUID-14, 46382, 34, 2, 112.535, 23.3279, -26.4396, 4.13643),
(@CGUID-15, 46375, 34, 2, 98.96, -1.139, -25.6062, -0.0470873),
(@CGUID-16, 46382, 34, 2, 93.2359, -0.0572555, -25.6062, 3.70497),
(@CGUID-17, 46382, 34, 2, 100.528, 3.50848, -25.5229, 3.7705),
(@CGUID-18, 46382, 34, 2, 102.194, 3.97128, -25.5229, 3.56047),
(@CGUID-19, 46379, 34, 2, 108.927, 21.8863, -26.5229, 2.2088),
(@CGUID-20, 46382, 34, 2, 103.314, -31.4143, -26.4396, 1.88496),
(@CGUID-21, 46382, 34, 2, 100.542, -28.7851, -26.4396, 5.86431),
(@CGUID-22, 46382, 34, 2, 101.52, -27.0801, -26.4396, 5.20108),
(@CGUID-23, 46379, 34, 2, 138.974, 3.55755, -25.6062, 5.04733),
(@CGUID-24, 46381, 34, 2, 141.334, -0.052255, -25.6062, 2.70265),
(@CGUID-25, 46381, 34, 2, 151.78, -0.610041, -25.6062, 2.92175),
(@CGUID-26, 46375, 34, 2, 136.11, 2.21884, -25.6062, 5.32628),
(@CGUID-27, 46379, 34, 2, 151.833, 0.695342, -25.6062, 1.42085),
(@CGUID-28, 46379, 34, 2, 104.795, -29.1012, -26.4396, 0.864969),
(@CGUID-29, 46262, 34, 2, 77.8695, -88.568, -33.9396, 6.13929),
(@CGUID-30, 46262, 34, 2, 91.3291, -133.327, -33.9396, 2.51738),
(@CGUID-31, 46262, 34, 2, 113.612, -59.6646, -34.8562, 1.07341),
(@CGUID-32, 46260, 34, 2, 146.644, -49.8948, -34.8562, 1.7966),
(@CGUID-33, 46261, 34, 2, 154.18, -69.5024, -34.8562, 5.11699),
(@CGUID-34, 46262, 34, 2, 151.518, -47.9823, -34.8562, 2.91744),
(@CGUID-35, 46260, 34, 2, 111.092, -98.399, -35.1896, 4.32032),
(@CGUID-36, 46263, 34, 2, 105.885, -101.519, -35.1063, 1.69297),
(@CGUID-37, 46261, 34, 2, 99.3459, -102.593, -35.1063, 0.436332),
(@CGUID-38, 46262, 34, 2, 125.86, -61.6993, -33.9396, 1.00045),
(@CGUID-39, 46260, 34, 2, 119.77, -81.5856, -33.8563, 0),
(@CGUID-40, 46263, 34, 2, 117.771, -75.4089, -33.8563, 0),
(@CGUID-41, 46260, 34, 2, 138.332, -126.504, -33.9396, 2.10357),
(@CGUID-42, 46250, 34, 2, 154.114, 100.941, -35.1896, 3.43802),
(@CGUID-43, 46248, 34, 2, 147.842, 43.3093, -34.8562, 6.19657),
(@CGUID-44, 46248, 34, 2, 179.873, 90.7396, -33.9396, 2.95521),
(@CGUID-45, 46252, 34, 2, 178.448, 98.6488, -33.9396, 0.533069),
(@CGUID-46, 46250, 34, 2, 110.525, 50.7815, -34.7729, 5.39307),
(@CGUID-47, 46252, 34, 2, 112.905, 42.6389, -34.7729, 1.25664),
(@CGUID-48, 46249, 34, 2, 171.285, 135.533, -33.9396, 2.47638),
(@CGUID-49, 46250, 34, 2, 161.415, 100.436, -35.1063, 3.89208),
(@CGUID-50, 46252, 34, 2, 161.406, 97.0804, -35.1063, 2.72271),
(@CGUID-51, 46248, 34, 2, 159.443, 98.5626, -35.1063, 0.191986),
(@CGUID-52, 46248, 34, 2, 132.976, 40.8171, -33.8563, 3.90954),
(@CGUID-53, 46252, 34, 2, 164.711, 135.636, -33.9396, 6.06013),
(@CGUID-54, 46249, 34, 2, 132.665, 123.228, -33.9396, 0.0124326),
(@CGUID-55, 46254, 34, 2, 158.016, 116.564, -35.1063, 4.2586),
(@CGUID-56, 46264, 34, 2, 99.4527, -116.802, -35.1063, 1.09956),
(@CGUID-57, 46381, 34, 2, 164.323, 5.37318, -25.5229, 3.01942),
(@CGUID-58, 46375, 34, 2, 161.964, 5.82574, -25.5229, 5.96903),
(@CGUID-59, 46379, 34, 2, 135.391, 5.12026, -24.3964, -2.53755),
(@CGUID-60, 46382, 34, 2, 120.915, 0.512058, -25.6062, 3.24348),
(@CGUID-61, 46382, 34, 2, 132.188, 5.22533, -25.5229, 3.75246),
(@CGUID-62, 46382, 34, 2, 133.763, 3.99191, -25.5229, 4.13643),
(@CGUID-63, 46375, 34, 2, 100.19, 1.53123, -25.6062, -2.79082),
(@CGUID-64, 46382, 34, 2, 112.539, -18.1699, -26.4396, 3.54302),
(@CGUID-65, 46382, 34, 2, 111.682, -15.3287, -26.4396, 2.3911),
(@CGUID-66, 46382, 34, 2, 113.127, -13.4419, -26.4396, 2.19912),
(@CGUID-67, 46382, 34, 2, 100.28, -0.705868, -25.6062, 4.72614),
(@CGUID-68, 46382, 34, 2, 111.162, -2.44849, -25.5229, 1.93731),
(@CGUID-69, 46382, 34, 2, 114.333, -2.55397, -25.5229, 1.18682),
(@CGUID-70, 46375, 34, 2, 102.054, 0.80921, -25.5229, 3.68265),
(@CGUID-71, 46263, 34, 2, 130.427, -116.848, -33.9396, 2.46724),
(@CGUID-72, 46260, 34, 2, 89.6001, -143.828, -33.9396, 3.61654),
(@CGUID-73, 46262, 34, 2, 116.784, -84.3837, -33.9396, 0.986595),
(@CGUID-74, 46263, 34, 2, 128.403, -51.1683, -33.8563, 1.8326),
(@CGUID-75, 46261, 34, 2, 125.033, -55.2645, -33.8563, 1.8326),
(@CGUID-76, 46263, 34, 2, 109.941, -100.341, -35.1896, -0.611966),
(@CGUID-77, 46263, 34, 2, 105.616, -57.7929, -34.8562, 5.6134),
(@CGUID-78, 46261, 34, 2, 111.376, -45.8854, -34.8562, 5.65932),
(@CGUID-79, 46262, 34, 2, 134.673, -122.773, -33.9396, 5.86751),
(@CGUID-80, 46263, 34, 2, 95.1225, -58.5109, -34.8562, 0.875983),
(@CGUID-81, 46251, 34, 2, 190.948, 92.736, -33.9396, 3.35267),
(@CGUID-82, 46249, 34, 2, 150.557, 45.8164, -34.7729, 3.80482),
(@CGUID-83, 46251, 34, 2, 149.402, 39.6148, -34.7729, 1.25664),
(@CGUID-84, 46252, 34, 2, 119.789, 125.887, -33.9396, 2.52888),
(@CGUID-85, 46248, 34, 2, 118.116, 75.7636, -34.7729, 4.2586),
(@CGUID-86, 46251, 34, 2, 114.621, 71.3001, -34.7729, 0.383972),
(@CGUID-87, 46250, 34, 2, 124.068, 119.782, -33.9396, 2.84049),
(@CGUID-88, 46249, 34, 2, 151.978, 65.7358, -34.7729, 4.2586),
(@CGUID-89, 46251, 34, 2, 151.236, 61.0594, -34.7729, 2.3911),
(@CGUID-90, 46251, 34, 2, 128.467, 60.6504, -33.8563, 5.60251),
(@CGUID-91, 46251, 34, 2, 169.457, 131.567, -33.9396, 0.0420607),
(@CGUID-92, 46249, 34, 2, 115.646, 46.6108, -34.8562, 3.05838),
(@CGUID-93, 46249, 34, 2, 145.945, 87.9651, -33.8563, 3.49066),
(@CGUID-94, 46251, 34, 2, 140.377, 88.779, -33.8563, 5.16617),
(@CGUID-95, 46261, 34, 2, 143.314, -42.6761, -34.8562, 3.36318),
(@CGUID-96, 46261, 34, 2, 130.089, -115.885, -33.9396, 2.24391),
(@CGUID-97, 46262, 34, 2, 115.934, -62.6386, -34.8561, 2.66776),
(@CGUID-98, 46263, 34, 2, 142.243, -74.6631, -34.8562, 0.60646),
(@CGUID-99, 46263, 34, 2, 79.106, -96.1999, -33.9396, 2.88742),
(@CGUID-100, 46260, 34, 2, 99.583, -39.0376, -34.8562, 2.37755),
(@CGUID-101, 46250, 34, 2, 147.363, 102.737, -35.1896, 4.74935),
(@CGUID-102, 46248, 34, 2, 145.88, 111.855, -35.1063, 3.49066),
(@CGUID-103, 46251, 34, 2, 142.502, 111.638, -35.1063, 5.49779),
(@CGUID-104, 46249, 34, 2, 143.643, 107.928, -35.1063, 1.09956),
(@CGUID-105, 46252, 34, 2, 138.8, 68.2195, -33.8563, 3.75246),
(@CGUID-106, 46406, 34, 2, 78.7384, -0.682292, -25.5229, 0),
(@CGUID-107, 46405, 34, 2, 78.6935, 15.392, -26.4396, 0.610865),
(@CGUID-108, 46406, 34, 2, 77.0925, 3.23572, -25.5229, 0),
(@CGUID-109, 46406, 34, 2, 76.7786, -1.54392, -25.5229, 0),
(@CGUID-110, 46405, 34, 2, 89.3952, 9.3915, -26.4396, 3.40339),
(@CGUID-111, 46407, 34, 2, 91.6768, -29.3938, -26.4396, 3.12414),
(@CGUID-112, 46408, 34, 2, 90.7331, -8.76813, -26.4396, 4.13643),
(@CGUID-113, 46405, 34, 2, 90.0439, -2.58535, -25.5229, 0.139626),
(@CGUID-114, 46406, 34, 2, 78.9916, 4.26081, -25.5229, 0),
(@CGUID-115, 46406, 34, 2, 78.7236, -2.62016, -25.5229, 0),
(@CGUID-116, 46407, 34, 2, 78.9247, -27.3705, -26.4396, 1.62316),
(@CGUID-117, 46405, 34, 2, 79.3117, 17.1223, -26.4396, 5.21853),
(@CGUID-118, 46408, 34, 2, 88.6199, -21.0578, -26.4396, 1.72788),
(@CGUID-119, 46405, 34, 2, 79.9497, 22.0916, -26.4396, 5.48033),
(@CGUID-120, 46405, 34, 2, 85.3679, 18.7793, -26.4396, 5.42797),
(@CGUID-121, 46408, 34, 2, 77.3381, -15.1934, -26.4396, 0),
(@CGUID-122, 46407, 34, 2, 91.3948, -16.1498, -26.4396, 3.05433),
(@CGUID-123, 46405, 34, 2, 80.8261, 15.4966, -26.4396, 2.80998),
(@CGUID-124, 46405, 34, 2, 90.0527, 4.0865, -25.5229, 6.05629),
(@CGUID-125, 46411, 34, 2, 85.5236, -17.4515, -26.4396, 0),
(@CGUID-126, 46410, 34, 2, 83.9379, -14.7392, -26.4396, 3.22886),
(@CGUID-127, 46406, 34, 2, 79.0145, 2.2961, -25.5229, 0),
(@CGUID-128, 46407, 34, 2, 79.8477, -31.7496, -26.4396, 0.907571),
(@CGUID-129, 46407, 34, 2, 86.7224, -32.0406, -26.4396, 1.62316),
(@CGUID-130, 46405, 34, 2, 89.7979, -0.81802, -25.5229, 0.0698132),
(@CGUID-131, 46409, 34, 2, 83.5244, 32.7149, -26.4829, 3.29497),
(@CGUID-132, 46482, 34, 2, 88.8999, 21.8505, -26.4421, 4.4855),
(@CGUID-133, 46405, 34, 2, 89.7199, 2.35929, -25.5229, 6.12611),
(@CGUID-134, 46408, 34, 2, 80.187, -6.52612, -26.4396, 4.72984),
(@CGUID-135, 46405, 34, 2, 86.3939, 17.585, -26.4396, 2.33874),
(@CGUID-136, 46405, 34, 2, 90.0658, 17.493, -26.4396, 2.79253),
(@CGUID-137, 46417, 34, 2, 73.7084, 0.704101, -25.5229, 0),
(@CGUID-138, 46407, 34, 2, 90.2458, -32.1359, -26.4396, 1.62316),
(@CGUID-139, 53488, 34, 2, 66.8395, 0.820416, -22.9799, 0),
(@CGUID-140, 46251, 34, 2, 126.643, 48.8055, -33.8563, 5.34071),
(@CGUID-141, 53488, 34, 2, 76.7849, 0.891, -25.5229, 0);

DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID-33 AND @OGUID-0;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@OGUID-0, 206038, 34, 2, 127.26, -125.315, -32.7396, 2.46091),
(@OGUID-1, 206038, 34, 2, 136.253, -50.0208, -34.8562, 2.46091),
(@OGUID-2, 206038, 34, 2, 123.561, -71.0415, -28.8443, 2.46091),
(@OGUID-3, 206039, 34, 2, 143.363, -53.1464, -34.8562, 0),
(@OGUID-4, 206038, 34, 2, 92.377, -116.607, -31.0731, 2.46091),
(@OGUID-5, 206038, 34, 2, 130.461, -39.3777, -33.9396, 2.46091),
(@OGUID-6, 206038, 34, 2, 106.757, -89.6077, -31.5528, 2.46091),
(@OGUID-7, 206039, 34, 2, 118.08, -105.927, -35.1896, 0),
(@OGUID-8, 206038, 34, 2, 159.953, -56.0504, -34.8562, 2.46091),
(@OGUID-9, 206038, 34, 2, 122.655, -48.4107, -31.062, 2.46091),
(@OGUID-10, 206038, 34, 2, 85.4526, -102.806, -32.4805, 2.46091),
(@OGUID-11, 206039, 34, 2, 143.322, -80.2735, -34.8562, 0),
(@OGUID-12, 206038, 34, 2, 100.471, -63.4066, -34.8562, 2.46091),
(@OGUID-13, 206039, 34, 2, 130.886, -127.941, -33.9396, 0),
(@OGUID-14, 206038, 34, 2, 133.514, -112.958, -33.9396, 2.46091),
(@OGUID-15, 206039, 34, 2, 139.263, -37.9274, -34.8562, 0),
(@OGUID-16, 206039, 34, 2, 114.113, -116.77, -35.1896, 0),
(@OGUID-17, 206039, 34, 2, 113.787, -68.6769, -34.4421, -0.890117),
(@OGUID-18, 206038, 34, 2, 133.237, -36.2304, -33.545, 2.46091),
(@OGUID-19, 206039, 34, 2, 104.716, -92.3675, -34.977, 0),
(@OGUID-20, 206038, 34, 2, 128.726, -62.7564, -33.3152, 2.46091),
(@OGUID-21, 206038, 34, 2, 125.139, -69.8723, -33.9396, 2.46091),
(@OGUID-22, 206038, 34, 2, 102.467, -53.2114, -34.8562, 2.46091),
(@OGUID-23, 206038, 34, 2, 97.0995, -96.3035, -35.1896, 2.46091),
(@OGUID-24, 206038, 34, 2, 100.02, -44.5863, -34.8562, 2.46091),
(@OGUID-25, 206038, 34, 2, 118.107, -91.1957, -33.6035, 2.46091),
(@OGUID-26, 206039, 34, 2, 82.5491, -86.3351, -32.9531, 0),
(@OGUID-27, 206038, 34, 2, 113.598, -79.2585, -31.5162, 2.46091),
(@OGUID-28, 206038, 34, 2, 117.186, -49.3382, -33.5413, 2.46091),
(@OGUID-29, 206038, 34, 2, 105.513, -34.6987, -34.8562, 2.46091),
(@OGUID-30, 206039, 34, 2, 136.405, -62.4725, -34.8562, 0),
(@OGUID-31, 206038, 34, 2, 77.1255, -85.7675, -33.9396, 2.46091),
(@OGUID-32, 206038, 34, 2, 126.142, -33.6665, -33.9396, 2.46091),
(@OGUID-33, 206117, 34, 2, 80.1872, -21.7123, -26.5229, 0.95993);UPDATE `creature_template` SET `ScriptName`='boss_hogger' WHERE `entry`=46254;
UPDATE `instance_template` SET `script`='instance_the_stockade' WHERE `map`=34;
DELETE FROM `creature_text` WHERE `entry`=46254;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(46254, 0, 0, 'Forest just setback!', 14, 0, 100, 0, 0, 0, 'Hogger - SAY AGGRO'),
(46254, 1, 0, 'Yiiipe!', 14, 0, 100, 0, 0, 0, 'Hogger - SAY_DEATH'),
(46254, 2, 0, 'Hogger enrages!', 41, 0, 100, 0, 0, 0, 'Hogger - SAY_ENRAGE');
UPDATE `creature_template` SET `ScriptName`='boss_lord_overheat' WHERE `entry`=46264;

DELETE FROM `creature_text` WHERE `entry`=46264;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(46264, 0, 0, 'ALL MUST BURN!', 14, 0, 0, 0, 0, 0, 'Lord Overheat - SAY_AGGRO'),
(46264, 1, 0, 'FIRE... EXTINGUISHED!', 14, 0, 0, 0, 0, 0, 'Lord Overheat - SAY_DEATH');
UPDATE `creature_template` SET `ScriptName`='boss_randolph_moloch' WHERE  `entry`=46383;

DELETE FROM `creature_text` WHERE `entry`=46383;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(46383, 0, 0, 'Allow me to introduce myself. I am Randolph Moloch, and i will be killing you all today.', 14, 0, 0, 0, 0, 0, 'Randolph Moloch - SAY_AGGRO'),
(46383, 1, 0, 'My epic schemes, my great plans! Gone!', 14, 0, 0, 0, 0, 0, 'Randolph Moloch - SAY_DEATH'),
(46383, 2, 0, 'Randolph Moloch vanishes!', 41, 0, 0, 0, 0, 0, 'Randolph Moloch - SAY_VANISH');
-- Hogger
UPDATE `creature_template` SET `lootid`=46254 WHERE `entry`=46254;
DELETE FROM `creature_loot_template` WHERE `entry`=46254;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `groupid`) VALUES
(46254, 2168, 31, 1), -- Corpse Rompers
(46254, 1934, 27, 1), -- Hogger's Trousers
(46254, 1959, 25, 1), -- Cold Iron Pick
(46254, 132569, 12, 1), -- Stolen Jailer's Greaves
(46254, 151074, 6, 1); -- Turnkey's Pauldrons

-- Randolph Moloch
UPDATE `creature_template` SET `lootid`=46383 WHERE `entry`=46383;
DELETE FROM `creature_loot_template` WHERE `entry`=46383;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `groupid`) VALUES
(46383, 63344, 30, 1), -- Standard Issue Prisoner Shoes
(46383, 63345, 26, 1), -- Noble's Robe
(46383, 63346, 25, 1), -- Wicked Dagger
(46383, 132570, 14, 1), -- Stolen Guards Chain Boots
(46383, 151077, 5, 1); -- Cast Iron Waistplate

-- Lord Overheat
UPDATE `creature_template` SET `lootid`=46264 WHERE `entry`=46264;
DELETE FROM `creature_loot_template` WHERE `entry`=46264;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `groupid`) VALUES
(46264, 62305, -100, 0), -- Lord Overheat's Fiery Core
(46264, 4676, 32, 1), -- Skeletal Gauntlets
(46264, 1929, 25, 1), -- Silk-Threaded Trousers
(46264, 5967, 23, 1), -- Girdle of Nobility
(46264, 151076, 5, 1), -- Fire-Hardened Shackles
(46264, 151075, 4, 1); -- Cinderstitch Tunic

DELETE FROM `disenchant_loot_template` WHERE `entry` IN (134, 137, 138, 139, 140, 141, 142, 143, 144, 145);
INSERT INTO `disenchant_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(134, 74249, 75, 1, 1, 1, 6), -- Spirit Dust - 364 - 377 UNCOMMON ARMOR
(134, 74250, 25, 1, 1, 1, 2), -- Mysterious Essence - 364 - 377 UNCOMMON ARMOR
(137, 74249, 75, 1, 1, 1, 6), -- Spirit Dust - 378 - 390 UNCOMMON ARMOR
(137, 74250, 25, 1, 1, 1, 2), -- Mysterious Essence - 378 - 390 UNCOMMON ARMOR
(138, 74249, 75, 1, 1, 1, 6), -- Spirit Dust - 391 - 403 UNCOMMON ARMOR
(138, 74250, 25, 1, 1, 1, 2), -- Mysterious Essence - 391 - 403 UNCOMMON ARMOR
(139, 74249, 75, 1, 1, 1, 6), -- Spirit Dust - 404 - 416 UNCOMMON ARMOR
(139, 74250, 25, 1, 1, 1, 2), -- Mysterious Essence - 404 - 416 UNCOMMON ARMOR
(140, 74249, 75, 1, 1, 1, 6), -- Spirit Dust - 417 - 900 UNCOMMON ARMOR
(140, 74250, 25, 1, 1, 1, 2), -- Mysterious Essence - 417 - 900 UNCOMMON ARMOR
(141, 74249, 75, 1, 1, 1, 6), -- Spirit Dust - 364 - 377 UNCOMMON WEAPONS
(141, 74250, 25, 1, 1, 1, 2), -- Mysterious Essence - 364 - 377 UNCOMMON WEAPONS
(142, 74249, 75, 1, 1, 1, 6), -- Spirit Dust - 378 - 390 UNCOMMON WEAPONS
(142, 74250, 25, 1, 1, 1, 2), -- Mysterious Essence - 378 - 390 UNCOMMON WEAPONS
(143, 74249, 75, 1, 1, 1, 6), -- Spirit Dust - 391 - 403 UNCOMMON WEAPONS
(143, 74250, 25, 1, 1, 1, 2), -- Mysterious Essence - 391 - 403 UNCOMMON WEAPONS
(144, 74249, 75, 1, 1, 1, 6), -- Spirit Dust - 404 - 416 UNCOMMON WEAPONS
(144, 74250, 25, 1, 1, 1, 2), -- Mysterious Essence - 404 - 416 UNCOMMON WEAPONS
(145, 74249, 75, 1, 1, 1, 6), -- Spirit Dust - 417 - 900 UNCOMMON WEAPONS
(145, 74250, 25, 1, 1, 1, 2); -- Mysterious Essence - 417 - 900 UNCOMMON WEAPONS


DELETE FROM `disenchant_loot_template` WHERE `entry` IN (129, 130, 131, 132);
INSERT INTO `disenchant_loot_template` (`entry`, `item`, `groupid`, `maxcount`) VALUES
(129, 87517, 1, 2), -- Ethereal Shard - RARE ARMOR 378 - 424
(130, 87517, 1, 2), -- Ethereal Shard - RARE ARMOR 425 - 900
(131, 87517, 1, 2), -- Ethereal Shard - RARE WEAPONS 378 - 424
(132, 87517, 1, 2); -- Ethereal Shard - RARE WEAPONS 425 - 900

DELETE FROM `disenchant_loot_template` WHERE `entry` IN (135, 136);
INSERT INTO `disenchant_loot_template` (`entry`, `item`, `groupid`) VALUES
(135, 74248, 1), -- Sha Crystal - EPIC ARMOR
(136, 74248, 1); -- Sha Crystal - EPIC WEAPONS
-- Savannah Prowler
UPDATE `creature_template` SET `skinloot`=3425 WHERE `entry`=3425;

-- Echeyakee
UPDATE `creature_template` SET `skinloot`=3475 WHERE `entry`=3475;

-- Raging Agam'ar
DELETE FROM `skinning_loot_template` WHERE `entry`=4514;

-- Ravasaur
UPDATE `creature_template` SET `skinloot`=6505 WHERE `entry`=6505;

-- Sian-Rotam
UPDATE `creature_template` SET `skinloot`=10741 WHERE `entry`=10741;

-- Elder Springpaw
UPDATE `creature_template` SET `skinloot`=15652 WHERE `entry`=15652;
DELETE FROM `spell_loot_template` WHERE `entry` IN
(53831, 53832, 53843, 53844, 53845, 53852, 53856);
INSERT INTO `spell_loot_template` (`entry`, `item`) VALUES
(53831, 39900), -- Bold Bloodstone
(53832, 39905), -- Delicate Bloodstone
(53843, 39907), -- Subtle Sun Crystal
(53844, 39908), -- Flashing Bloodstone
(53845, 39909), -- Smooth Sun Crystal
(53852, 39912), -- Brilliant Bloodstone
(53856, 39918); -- Quick Sun Crystal
DELETE FROM `spell_loot_template` WHERE `entry` IN
(53854, 53857, 53859, 53860, 53861, 53865, 53869, 53870, 53871);
INSERT INTO `spell_loot_template` (`entry`, `item`) VALUES
(53854, 39915), -- Rigid Chalcedony
(53857, 39917), -- Mystic Sun Crystal
(53859, 39934), -- Sovereign Shadow Crystal
(53860, 39935), -- Shifting Shadow Crystal
(53861, 39942), -- Glinting Shadow Crystal
(53865, 39945), -- Mysterious Shadow Crystal
(53869, 39939), -- Defender's Shadow Crystal
(53870, 39933), -- Jagged Dark Jade
(53871, 39940); -- Guardian's Shadow Crystal

DELETE FROM `spell_loot_template` WHERE `entry` IN
(53872, 53873, 53874, 53875, 53876, 53877, 53879, 53880, 53882);
INSERT INTO `spell_loot_template` (`entry`, `item`) VALUES
(53872, 39947), -- Inscribed Huge Citrine
(53873, 39948), -- Etched Shadow Crystal
(53874, 39949), -- Champion's Huge Citrine
(53875, 39950), -- Resplendent Huge Citrine
(53876, 39951), -- Fierce Huge Citrine
(53877, 39952), -- Deadly Huge Citrine
(53879, 39954), -- Lucent Huge Citrine
(53880, 39955), -- Deft Huge Citrine
(53882, 39956); -- Potent Huge Citrine
-- Fix King Dred Scriptname
UPDATE `creature_template` SET `ScriptName`='boss_king_dred' WHERE `entry`=27483;

-- Golden Tiger SAI
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=59753;
DELETE FROM `smart_scripts` WHERE `entryorguid`=59753;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(59753, 0, 1, 0, 0, 0, 100, 0, 4500, 5000, 7500, 8000, 11, 115083, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Golden Tiger - In Combat - Cast Ferocious Claw');
-- Orchard Wasp SAI
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=56201;
DELETE FROM `smart_scripts` WHERE `entryorguid`=56201;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(56201, 0, 1, 0, 0, 0, 100, 0, 3000, 4000, 13000, 14000, 11, 106918, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Orchard Wasp - In Combat - Cast Noxious Venom');
-- Bombing Run
DELETE FROM `spell_area` WHERE `spell`=40200;

-- Valiance Keep Tavern
DELETE FROM `areatrigger_tavern` WHERE `id`=4961;

DELETE FROM `game_graveyard_zone` WHERE `ghost_zone`=4755;
DELETE FROM `game_graveyard_zone` WHERE `ghost_zone` IN (4756, 4757, 4758, 4759);
INSERT INTO `game_graveyard_zone` (`id`, `ghost_zone`, `faction`) VALUES
(1692, 4756, 0), -- Merchant Quarter Graveyard
(1693, 4757, 0), -- Military District Graveyard
(1694, 4758, 0), -- Greymane Square Graveyard
(1695, 4759, 0); -- Cathedral Graveyard
UPDATE `creature` SET `phaseId`=170 WHERE `id` IN
(28529, 28576, 28605, 28606, 28660, 28560, 28766, 28936);
UPDATE `creature` SET `phaseId`=170 WHERE `id` IN
(28525, -- New Avalon Forge
28542, -- Scarlet Hold;
28543, -- New Avalon Town Hall
28544); -- Chapel of the Crimson Flame
SET @OGUID := 61573;
DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID-13 AND @OGUID-0;
DELETE FROM `gameobject` WHERE `id` IN (190800, 191165, 191167, 191170, 191175, 191178, 191190);
-- 190800
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(@OGUID-0, 190800, 609, 1, 0, 0, 2051.61, -5782.35, 100.826, 1.12139, 0, 0, 0, 1, 180, 0, 1),
(@OGUID-1, 190800, 609, 1, 170, 0, 2051.61, -5782.35, 100.826, 1.12139, 0, 0, 0, 1, 180, 0, 1),
-- 191165
(@OGUID-2, 191165, 609, 1, 0, 0, 2266.73, -5799.05, 97.9244, -0.942474, 0, 0, 0, 1, 180, 0, 1),
(@OGUID-3, 191165, 609, 1, 170, 0, 2266.73, -5799.05, 97.9244, -0.942474, 0, 0, 0, 1, 180, 0, 1),
-- 191167
(@OGUID-4, 191167, 609, 1, 0, 0, 2109.78, -5805.76, 96.7227, 2.25147, 0, 0, 0, 1, 180, 0, 1),
(@OGUID-5, 191167, 609, 1, 170, 0, 2109.78, -5805.76, 96.7227, 2.25147, 0, 0, 0, 1, 180, 0, 1),
-- 191170
(@OGUID-6, 191170, 609, 1, 0, 0, 1936.01, -5792.94, 96.9045, 0.855213, 0, 0, 0, 1, 180, 0, 1),
(@OGUID-7, 191170, 609, 1, 170, 0, 1936.01, -5792.94, 96.9045, 0.855213, 0, 0, 0, 1, 180, 0, 1),
-- 191175
(@OGUID-8, 191175, 609, 1, 0, 0, 2054.43, -5880.69, 101.059, 0.541053, 0, 0, 0, 1, 180, 0, 1),
(@OGUID-9, 191175, 609, 1, 170, 0, 2054.43, -5880.69, 101.059, 0.541053, 0, 0, 0, 1, 180, 0, 1),
-- 191178
(@OGUID-10, 191178, 609, 1, 0, 0, 2217.03, -5880.88, 100.885, 1.5708, 0, 0, 0, 1, 180, 0, 1),
(@OGUID-11, 191178, 609, 1, 170, 0, 2217.03, -5880.88, 100.885, 1.5708, 0, 0, 0, 1, 180, 0, 1),
-- 191190
(@OGUID-12, 191190, 609, 1, 0, 0, 2091.49, -5680.32, 100.272, 2.14675, 0, 0, 0, 1, 180, 0, 1),
(@OGUID-13, 191190, 609, 1, 170, 0, 2091.49, -5680.32, 100.272, 2.14675, 0, 0, 0, 1, 180, 0, 1);
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=56309;
DELETE FROM `smart_scripts` WHERE `entryorguid`=56309;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(56309, 0, 0, 1, 1, 0, 100, 0, 0, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grookin Wildtail - Out Of Combat - Allow Combat Movement'),
(56309, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grookin Wildtail - Out Of Combat - Set Phase 0'),
(56309, 0, 2, 3, 4, 0, 100, 0, 0, 0, 0, 0, 11, 105762, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Grookin Wildtail - On Aggro - Cast Sling Derk'),
(56309, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 23, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grookin Wildtail - On Aggro - Increment Phase'),
(56309, 0, 4, 0, 9, 0, 100, 0, 0, 40, 3000, 3500, 11, 105762, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Grookin Wildtail - At 0 - 40 Range - Cast Sling Derk'),
(56309, 0, 5, 6, 3, 0, 100, 0, 0, 15, 0, 0, 21, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grookin Wildtail - At 15% Mana - Allow Combat Movement'),
(56309, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 23, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grookin Wildtail - At 15% Mana - Increment Phase'),
(56309, 0, 7, 0, 9, 0, 100, 1, 35, 80, 0, 0, 21, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grookin Wildtail - At 35 - 80 Range - Allow Combat Movement'),
(56309, 0, 8, 0, 9, 0, 100, 1, 5, 15, 0, 0, 21, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grookin Wildtail - At 5 - 15 Range - Allow Combat Movement'),
(56309, 0, 9, 0, 9, 0, 100, 1, 0, 5, 0, 0, 21, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grookin Wildtail - At 0 - 5 Range - Allow Combat Movement'),
(56309, 0, 10, 0, 3, 0, 100, 0, 25, 100, 100, 100, 23, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grookin Wildtail - At 100% Mana - Increment Phase');
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=59788;
DELETE FROM `smart_scripts` WHERE `entryorguid`=59788;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(59788, 0, 0, 1, 1, 0, 100, 0, 0, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Spirit Darter - Out Of Combat - Allow Combat Movement'),
(59788, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Spirit Darter - Out Of Combat - Set Phase 0'),
(59788, 0, 2, 3, 4, 0, 100, 0, 0, 0, 0, 0, 11, 115394, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Spirit Darter - On Aggro - Cast Mana Flare'),
(59788, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 23, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Spirit Darter - On Aggro - Increment Phase'),
(59788, 0, 4, 0, 9, 0, 100, 0, 0, 40, 2500, 3000, 11, 115394, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Spirit Darter - At 0 - 40 Range - Cast Mana Flare'),
(59788, 0, 5, 6, 3, 0, 100, 0, 0, 15, 0, 0, 21, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Spirit Darter - At 15% Mana - Allow Combat Movement'),
(59788, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 23, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Spirit Darter - At 15% Mana - Increment Phase'),
(59788, 0, 7, 0, 9, 0, 100, 1, 35, 80, 0, 0, 21, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Spirit Darter - At 35 - 80 Range - Allow Combat Movement'),
(59788, 0, 8, 0, 9, 0, 100, 1, 5, 15, 0, 0, 21, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Spirit Darter - At 5 - 15 Range - Allow Combat Movement'),
(59788, 0, 9, 0, 9, 0, 100, 1, 0, 5, 0, 0, 21, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Spirit Darter - At 0 - 5 Range - Allow Combat Movement'),
(59788, 0, 10, 0, 3, 0, 100, 0, 25, 100, 100, 100, 23, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Spirit Darter - At 100% Mana - Increment Phase');
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=59787;
DELETE FROM `smart_scripts` WHERE `entryorguid`=59787;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(59787, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 115385, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sunrise Crane - On Aggro - Cast Rush');

-- Adolescent Mushan SAI
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=56239;
DELETE FROM `smart_scripts` WHERE `entryorguid`=56239;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(56239, 0, 1, 0, 0, 0, 100, 0, 8000, 9000, 22000, 23000, 11, 125384, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Adolescent Mushan - In Combat - Cast Belly Flop');
DELETE FROM `spell_loot_template` WHERE `entry`=51771;
INSERT INTO `spell_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(51771, 38631, 100, 1, 0, 1, 1);

UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=190584;
UPDATE `gameobject_loot_template` SET `ChanceOrQuestChance`=100 WHERE `entry`=24611 AND `item`=38607;

DELETE FROM `conditions` WHERE `SourceGroup`=24611 AND `SourceEntry`=38607;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(4, 24611, 38607, 0, 0, 9, 0, 12619, 0, 0, 0, 0, 0, '', NULL),
(4, 24611, 38607, 0, 0, 2, 0, 38607, 1, 0, 1, 0, 0, '', NULL);

-- Defias Gunpowder
DELETE FROM `gameobject_loot_template` WHERE `entry`=2882;
INSERT INTO `gameobject_loot_template` (`entry`, `item`) VALUES
(2882, 17155);

-- Rattan Switch
DELETE FROM `gameobject_loot_template` WHERE `entry`=40371;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(40371, 72926, -100);

-- Vault of Forbidden Treasures
UPDATE `gameobject_template` SET `name`='Vault of Forbidden Treasures' WHERE `entry`=233029;

-- Ancient Dynastic Tablet
DELETE FROM `gameobject_loot_template` WHERE `entry`=41387;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(41387, 79713, -100);

-- Mutilated Remains
DELETE FROM `gameobject_loot_template` WHERE `entry`=27780;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(27780, 49945, -100);

-- Stolen Watermelon
DELETE FROM `gameobject_loot_template` WHERE `entry`=40518;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(40518, 76298, -100);

-- Gurthani Tablet
DELETE FROM `gameobject_loot_template` WHERE `entry`=42818;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(42818, 85634, -100);
-- Weeping Horror SAI
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=57649;
DELETE FROM `smart_scripts` WHERE `entryorguid`=57649;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(57649, 0, 1, 0, 0, 0, 100, 0, 3000, 4000, 6000, 7000, 11, 119556, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Weeping Horror - In Combat - Cast Overwhelming Sadness');

-- Strong Fishing Pole increment time.
UPDATE `npc_vendor` SET `incrtime`=9000 WHERE `entry`=67181 AND `item`=6365;

DELETE FROM `spell_loot_template` WHERE `entry` IN
(53883, 53884, 53885, 53891, 53892, 53893, 53894, 53917);
INSERT INTO `spell_loot_template` (`entry`, `item`) VALUES
(53883, 39957), -- Veiled Shadow Crystal
(53884, 39958), -- Willful Huge Citrine
(53885, 39959), -- Reckless Huge Citrine
(53891, 39965), -- Stalwart Huge Citrine
(53892, 39966), -- Accurate Shadow Crystal
(53893, 39967), -- Resolute Huge Citrine
(53894, 39968), -- Timeless Shadow Crystal
(53917, 39975); -- Nimble Dark Jade

DELETE FROM `spell_loot_template` WHERE `entry` IN
(53918, 53919, 53920, 53921, 53922, 53923, 53924, 53925);
INSERT INTO `spell_loot_template` (`entry`, `item`) VALUES
(53918, 39976), -- Regal Dark Jade
(53919, 39977), -- Steady Dark Jade
(53920, 39978), -- Forceful Dark Jade
(53921, 39979), -- Purified Shadow Crystal
(53922, 39980), -- Misty Dark Jade
(53923, 39981), -- Lightning Dark Jade
(53924, 39982), -- Turbid Dark Jade
(53925, 39983); -- Energized Dark Jade

DELETE FROM `spell_loot_template` WHERE `entry` IN
(53932, 53933, 53934, 53941, 53943, 54017);
INSERT INTO `spell_loot_template` (`entry`, `item`) VALUES
(53932, 39991), -- Radiant Dark Jade
(53933, 39992), -- Shattered Dark Jade
(53934, 39919), -- Solid Chalcedony
(53941, 39927), -- Sparkling Chalcedony
(53943, 39932), -- Stormy Chalcedony
(54017, 39910); -- Precise Bloodstone
DELETE FROM `spell_loot_template` WHERE `entry` IN
(53854, 53857, 53859, 53860, 53861, 53865, 53869, 53870, 53871, 53872, 53873, 53874, 53875, 53876, 53877, 53879, 53880, 53882);
INSERT INTO `spell_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(53854, 39915, 90, 1, 1, 1, 1), -- Rigid Chalcedony
(53854, 41447, 10, 1, 1, 1, 1), -- Perfect Rigid Chalcedony
(53857, 39917, 90, 1, 1, 1, 1), -- Mystic Sun Crystal
(53857, 41445, 10, 1, 1, 1, 1), -- Perfect Mystic Sun Crystal
(53859, 39934, 90, 1, 1, 1, 1), -- Sovereign Shadow Crystal
(53859, 41461, 10, 1, 1, 1, 1), -- Perfect Sovereign Shadow Crystal
(53860, 39935, 90, 1, 1, 1, 1), -- Shifting Shadow Crystal
(53860, 41450, 10, 1, 1, 1, 1), -- Perfect Shifting Shadow Crystal
(53861, 39942, 90, 1, 1, 1, 1), -- Glinting Shadow Crystal
(53861, 41462, 10, 1, 1, 1, 1), -- Perfect Glinting Shadow Crystal
(53865, 39945, 90, 1, 1, 1, 1), -- Mysterious Shadow Crystal
(53865, 41455, 10, 1, 1, 1, 1), -- Perfect Mysterious Shadow Crystal
(53869, 39939, 90, 1, 1, 1, 1), -- Defender's Shadow Crystal
(53869, 41451, 10, 1, 1, 1, 1), -- Perfect Defender's Shadow Crystal
(53870, 39933, 90, 1, 1, 1, 1), -- Jagged Dark Jade
(53870, 41468, 10, 1, 1, 1, 1), -- Perfect Jagged Dark Jade
(53871, 39940, 90, 1, 1, 1, 1), -- Guardian's Shadow Crystal
(53871, 41453, 10, 1, 1, 1, 1), -- Perfect Guardian's Shadow Crystal
(53872, 39947, 90, 1, 1, 1, 1), -- Inscribed Huge Citrine
(53872, 41492, 10, 1, 1, 1, 1), -- Perfect Inscribed Huge Citrine
(53873, 39948, 90, 1, 1, 1, 1), -- Etched Shadow Crystal
(53873, 41488, 10, 1, 1, 1, 1), -- Perfect Etched Shadow Crystal
(53874, 39949, 90, 1, 1, 1, 1), -- Champion's Huge Citrine
(53874, 41483, 10, 1, 1, 1, 1), -- Perfect Champion's Huge Citrine
(53875, 39950, 90, 1, 1, 1, 1), -- Resplendent Huge Citrine
(53875, 41499, 10, 1, 1, 1, 1), -- Perfect Resplendent Huge Citrine
(53876, 39951, 90, 1, 1, 1, 1), -- Fierce Huge Citrine
(53876, 41489, 10, 1, 1, 1, 1), -- Perfect Fierce Huge Citrine
(53877, 39952, 90, 1, 1, 1, 1), -- Deadly Huge Citrine
(53877, 41484, 10, 1, 1, 1, 1), -- Perfect Deadly Huge Citrine
(53879, 39954, 90, 1, 1, 1, 1), -- Lucent Huge Citrine
(53879, 41493, 10, 1, 1, 1, 1), -- Perfect Lucent Huge Citrine
(53880, 39955, 90, 1, 1, 1, 1), -- Deft Huge Citrine
(53880, 41485, 10, 1, 1, 1, 1), -- Perfect Deft Huge Citrine
(53882, 39956, 90, 1, 1, 1, 1), -- Potent Huge Citrine
(53882, 41495, 10, 1, 1, 1, 1); -- Perfect Potent Huge Citrine

DELETE FROM `spell_loot_template` WHERE `entry` IN
(53883, 53884, 53885, 53891, 53892, 53893, 53894, 53917, 53918, 53919, 53920, 53921, 53922, 53923, 53924, 53925, 53932, 53933, 53934, 53941, 53943, 54017);
INSERT INTO `spell_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(53883, 39957, 90, 1, 1, 1, 1), -- Veiled Shadow Crystal
(53883, 41502, 10, 1, 1, 1, 1), -- Perfect Veiled Shadow Crystal
(53884, 39958, 90, 1, 1, 1, 1), -- Willful Huge Citrine
(53884, 41486, 10, 1, 1, 1, 1), -- Perfect Willful Huge Citrine
(53885, 39959, 90, 1, 1, 1, 1), -- Reckless Huge Citrine
(53885, 41497, 10, 1, 1, 1, 1), -- Perfect Reckless Huge Citrine
(53891, 39965, 90, 1, 1, 1, 1), -- Stalwart Huge Citrine
(53891, 41490, 10, 1, 1, 1, 1), -- Perfect Stalwart Huge Citrine
(53892, 39966, 90, 1, 1, 1, 1), -- Accurate Shadow Crystal
(53892, 41482, 10, 1, 1, 1, 1), -- Perfect Accurate Shadow Crystal
(53893, 39967, 90, 1, 1, 1, 1), -- Resolute Huge Citrine
(53893, 41498, 10, 1, 1, 1, 1), -- Perfect Resolute Huge Citrine
(53894, 39968, 90, 1, 1, 1, 1), -- Timeless Shadow Crystal
(53894, 41452, 10, 1, 1, 1, 1), -- Perfect Timeless Shadow Crystal
(53917, 39975, 90, 1, 1, 1, 1), -- Nimble Dark Jade
(53917, 41481, 10, 1, 1, 1, 1), -- Perfect Nimble Dark Jade
(53918, 39976, 90, 1, 1, 1, 1), -- Regal Dark Jade
(53918, 41464, 10, 1, 1, 1, 1), -- Perfect Regal Dark Jade
(53919, 39977, 90, 1, 1, 1, 1), -- Steady Dark Jade
(53919, 41476, 10, 1, 1, 1, 1), -- Perfect Steady Dark Jade
(53920, 39978, 90, 1, 1, 1, 1), -- Forceful Dark Jade
(53920, 41466, 10, 1, 1, 1, 1), -- Perfect Forceful Dark Jade
(53921, 39979, 90, 1, 1, 1, 1), -- Purified Shadow Crystal
(53921, 41473, 10, 1, 1, 1, 1), -- Perfect Purified Shadow Crystal
(53922, 39980, 90, 1, 1, 1, 1), -- Misty Dark Jade
(53922, 41470, 10, 1, 1, 1, 1), -- Perfect Misty Dark Jade
(53923, 39981, 90, 1, 1, 1, 1), -- Lightning Dark Jade
(53923, 41475, 10, 1, 1, 1, 1), -- Perfect Lightning Dark Jade
(53924, 39982, 90, 1, 1, 1, 1), -- Turbid Dark Jade
(53924, 41480, 10, 1, 1, 1, 1), -- Perfect Turbid Dark Jade
(53925, 39983, 90, 1, 1, 1, 1), -- Energized Dark Jade
(53925, 41467, 10, 1, 1, 1, 1), -- Perfect Energized Dark Jade
(53932, 39991, 90, 1, 1, 1, 1), -- Radiant Dark Jade
(53932, 41478, 10, 1, 1, 1, 1), -- Perfect Radiant Dark Jade
(53933, 39992, 90, 1, 1, 1, 1), -- Shattered Dark Jade
(53933, 41474, 10, 1, 1, 1, 1), -- Perfect Shattered Dark Jade
(53934, 39919, 90, 1, 1, 1, 1), -- Solid Chalcedony
(53934, 41441, 10, 1, 1, 1, 1), -- Perfect Solid Chalcedony
(53941, 39927, 90, 1, 1, 1, 1), -- Sparkling Chalcedony
(53941, 41440, 10, 1, 1, 1, 1), -- Perfect Sparkling Chalcedony
(53943, 39932, 90, 1, 1, 1, 1), -- Stormy Chalcedony
(53943, 41443, 10, 1, 1, 1, 1), -- Perfect Stormy Chalcedony
(54017, 39910, 90, 1, 1, 1, 1), -- Precise Bloodstone
(54017, 41437, 10, 1, 1, 1, 1); -- Perfect Precise Bloodstone
-- Riverblade Slayer SAI
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=58273;
DELETE FROM `smart_scripts` WHERE `entryorguid`=58273;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(58273, 0, 1, 0, 0, 0, 100, 0, 1000, 2000, 9000, 10000, 11, 119561, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Riverblade Slayer - In Combat - Cast Bloodletting'),
(58273, 0, 2, 0, 0, 0, 100, 0, 2000, 3000, 10000, 11000, 11, 119569, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Riverblade Slayer - In Combat - Cast Savage Strikes');
-- Viseclaw Fisher SAI
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=58880;
DELETE FROM `smart_scripts` WHERE `entryorguid`=58880;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(58880, 0, 1, 0, 0, 0, 100, 0, 3500, 4000, 12500, 13000, 11, 117372, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Viseclaw Fisher - In Combat - Cast Vise Claw');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=25 AND `SourceEntry`=1062;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(25, 0, 1062, 0, 0, 8, 0, 32109, 0, 0, 0, 0, 0, '', 'Set Lions Landing Terrain Swap if q32109 rewarded');

SET @CGUID := 20452;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID-153 AND @CGUID-0;
DELETE FROM `creature` WHERE phaseId=1740;
INSERT INTO `creature` (`guid`, `id`, `map`, `phaseId`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 66300, 870, 1740, -693.17, -1533.35, 1.66123, 5.6526),
(@CGUID-1, 60969, 870, 1740, -521.443, -1661.16, 9.7653, 0.619156),
(@CGUID-2, 60969, 870, 1740, -526.787, -1640.14, 11.1097, 2.2842),
(@CGUID-3, 60969, 870, 1740, -542.027, -1685.39, 8.95413, 6.01798),
(@CGUID-4, 66300, 870, 1740, -706.09, -1595.96, 19.1336, 4.81694),
(@CGUID-5, 66300, 870, 1740, -687.339, -1582.91, 9.69681, 4.48864),
(@CGUID-6, 64861, 870, 1740, -703.965, -1379.85, 2.26812, 1.71761),
(@CGUID-7, 66300, 870, 1740, -663.251, -1511.8, 1.82429, 3.72602),
(@CGUID-8, 66300, 870, 1740, -632.741, -1735, 13.7935, 0.847532),
(@CGUID-9, 66300, 870, 1740, -619.306, -1547.81, 6.47988, 0.336244),
(@CGUID-10, 66300, 870, 1740, -601.29, -1630.27, 13.91, 4.73054),
(@CGUID-11, 66300, 870, 1740, -589.503, -1559.53, 10.6973, 4.86328),
(@CGUID-12, 66300, 870, 1740, -546.314, -1737.52, 6.64183, 2.53614),
(@CGUID-13, 66300, 870, 1740, -519.521, -1694.77, 4.56581, 2.90135),
(@CGUID-14, 60969, 870, 1740, -545.134, -1627.34, 11.8887, 4.11496),
(@CGUID-15, 60969, 870, 1740, -556.435, -1660.92, 11.4588, 2.52217),
(@CGUID-16, 60969, 870, 1740, -573.087, -1696.28, 9.66258, 4.30346),
(@CGUID-17, 63001, 870, 1740, -769.376, -1718.35, 10.3793, 0.571556),
(@CGUID-18, 64862, 870, 1740, -700.873, -1375.9, 2.4772, 3.81969),
(@CGUID-19, 64863, 870, 1740, -707.691, -1376.36, 2.82653, 5.56404),
(@CGUID-20, 64864, 870, 1740, -704.754, -1373.41, 2.82413, 5.20592),
(@CGUID-21, 60969, 870, 1740, -626.448, -1673.53, 12.7571, 5.17737),
(@CGUID-22, 60969, 870, 1740, -620.148, -1624.22, 14.4113, 4.04299),
(@CGUID-23, 63001, 870, 1740, -704.902, -1784.72, 0.619543, 4.13334),
(@CGUID-24, 66398, 870, 1740, -685.601, -1760.25, 4.99058, 3.59821),
(@CGUID-25, 60969, 870, 1740, -553.821, -1722.95, 8.18455, 2.35779),
(@CGUID-26, 60969, 870, 1740, -613.891, -1710.62, 11.4044, 4.96924),
(@CGUID-27, 60969, 870, 1740, -597.173, -1660.39, 12.5794, 5.83868),
(@CGUID-28, 60969, 870, 1740, -582.973, -1620.9, 12.7726, 2.72396),
(@CGUID-29, 60969, 870, 1740, -582.479, -1737.75, 7.13656, 1.70513),
(@CGUID-30, 66327, 870, 1740, -617.335, -1649.12, 14.9641, 0.00104314),
(@CGUID-31, 66327, 870, 1740, -595.776, -1679.54, 12.0511, 2.03915),
(@CGUID-32, 66398, 870, 1740, -639.059, -1536.78, 4.35705, 4.03747),
(@CGUID-33, 66398, 870, 1740, -676.292, -1662.48, 16.5258, 3.90334),
(@CGUID-34, 66398, 870, 1740, -720.665, -1744.37, 28.1839, 2.59683),
(@CGUID-35, 67012, 870, 1740, -834.534, -1788.18, 5.83421, 1.68817),
(@CGUID-36, 66398, 870, 1740, -722.37, -1687.16, 16.4952, 1.53419),
(@CGUID-37, 66480, 870, 1740, -708.673, -1378.38, 2.81248, 5.79004),
(@CGUID-38, 66460, 870, 1740, -702.513, -1374.19, 2.74806, 4.17605),
(@CGUID-39, 66407, 870, 1740, -830.578, -1800.87, 4.27553, 4.71511),
(@CGUID-40, 66398, 870, 1740, -602.588, -1519.66, 0.785093, 0.847184),
(@CGUID-41, 66398, 870, 1740, -645.641, -1681.91, 13.6694, 0.788452),
(@CGUID-42, 66398, 870, 1740, -650.18, -1732.28, 19.599, 0.604274),
(@CGUID-43, 66398, 870, 1740, -639.754, -1629.03, 15.6095, 5.47572),
(@CGUID-44, 66398, 870, 1740, -650.912, -1500.23, 6.05765, 2.65753),
(@CGUID-45, 66398, 870, 1740, -665.517, -1526.82, 2.13175, 3.76729),
(@CGUID-46, 66398, 870, 1740, -555.284, -1578.68, 11.2858, 3.74549),
(@CGUID-47, 66398, 870, 1740, -536.667, -1732.32, 5.96415, 2.41069),
(@CGUID-48, 66398, 870, 1740, -699.112, -1685.93, 15.5931, 4.4166),
(@CGUID-49, 66398, 870, 1740, -725.45, -1600.77, 26.1466, 5.09873),
(@CGUID-50, 66398, 870, 1740, -742.243, -1676.33, 17.2624, 2.90079),
(@CGUID-51, 66398, 870, 1740, -793.683, -1644.25, 3.77594, 3.93594),
(@CGUID-52, 66397, 870, 1740, -615.152, -1593.42, 13.078, 3.97831),
(@CGUID-53, 66397, 870, 1740, -595.764, -1728.97, 8.37397, 5.4649),
(@CGUID-54, 66398, 870, 1740, -772.393, -1453.24, 2.85, 1.96559),
(@CGUID-55, 66398, 870, 1740, -775.504, -1765, 10.8532, 2.27482),
(@CGUID-56, 66397, 870, 1740, -573.081, -1578.81, 13.6213, 1.37864),
(@CGUID-57, 66398, 870, 1740, -787.651, -1478.18, 9.22481, 5.08834),
(@CGUID-58, 66397, 870, 1740, -564.436, -1721.71, 8.6993, 0.864035),
(@CGUID-59, 66398, 870, 1740, -798.812, -1475.04, 2.99632, 1.84071),
(@CGUID-60, 66397, 870, 1740, -547.749, -1646.2, 12.2289, 0.709485),
(@CGUID-61, 66398, 870, 1740, -884.437, -1651.09, 5.97229, 2.06158),
(@CGUID-62, 66398, 870, 1740, -862.459, -1592.65, 0.564273, 3.62061),
(@CGUID-63, 66398, 870, 1740, -792.242, -1700.04, 11.0949, 0.0380077),
(@CGUID-64, 66398, 870, 1740, -836.513, -1751.51, 9.15496, 5.65203),
(@CGUID-65, 66397, 870, 1740, -617.927, -1697.76, 13.1778, 5.92671),
(@CGUID-66, 66398, 870, 1740, -750.759, -1662.48, 8.86153, 3.90334),
(@CGUID-67, 66398, 870, 1740, -765.805, -1630.1, 12.4995, 5.27739),
(@CGUID-68, 66327, 870, 1740, -579.038, -1649.69, 12.5396, 4.1613),
(@CGUID-69, 66398, 870, 1740, -853.471, -1666.9, 5.02679, 0.880736),
(@CGUID-70, 66432, 870, 1740, -695.899, -1764.63, -0.439216, 1.48591),
(@CGUID-71, 66459, 870, 1740, -677.752, -1660.92, 16.9508, 2.92562),
(@CGUID-72, 66459, 870, 1740, -548.578, -1644.58, 12.5325, 1.16084),
(@CGUID-73, 66459, 870, 1740, -541.701, -1670.18, 10.4768, 4.75615),
(@CGUID-74, 66459, 870, 1740, -636.375, -1707.59, 15.8075, 4.57127),
(@CGUID-75, 66459, 870, 1740, -728.698, -1740.57, 28.3332, 1.41007),
(@CGUID-76, 66459, 870, 1740, -563.62, -1667.96, 11.6768, 2.18869),
(@CGUID-77, 66432, 870, 1740, -673.113, -1516.81, 1.68649, 4.02426),
(@CGUID-78, 66432, 870, 1740, -662.248, -1696.47, 16.0672, 1.52581),
(@CGUID-79, 66459, 870, 1740, -687.165, -1543.02, 3.01408, 0.352922),
(@CGUID-80, 66459, 870, 1740, -645.688, -1648.8, 15.6231, 0.0381972),
(@CGUID-81, 66433, 870, 1740, -601.665, -1630.36, 15.5804, 3.43168),
(@CGUID-82, 66433, 870, 1740, -697.743, -1669.03, 16.73, 5.05343),
(@CGUID-83, 66432, 870, 1740, -700.2, -1699.16, 16.3441, 2.27009),
(@CGUID-84, 66459, 870, 1740, -733.795, -1643.04, 16.3729, 0.986718),
(@CGUID-85, 66459, 870, 1740, -617.418, -1696.35, 13.2602, 2.92562),
(@CGUID-86, 66459, 870, 1740, -557.445, -1716.84, 8.72042, 1.99489),
(@CGUID-87, 66459, 870, 1740, -590.898, -1676.48, 11.7354, 1.46816),
(@CGUID-88, 66459, 870, 1740, -605.543, -1648.81, 13.2636, 4.56975),
(@CGUID-89, 66459, 870, 1740, -584.26, -1651.97, 12.8295, 0.512062),
(@CGUID-90, 66459, 870, 1740, -587.917, -1737.55, 7.51425, 4.56975),
(@CGUID-91, 66290, 870, 1740, -652.103, -1547.49, 5.08312, 0.289532),
(@CGUID-92, 66290, 870, 1740, -665.463, -1592.38, 16.0288, 3.63219),
(@CGUID-93, 66282, 870, 1740, -697.645, -1634.88, 19.7308, 0.211779),
(@CGUID-94, 66290, 870, 1740, -667.432, -1569.37, 6.1631, 1.65613),
(@CGUID-95, 66282, 870, 1740, -686.09, -1681.73, 15.2589, 2.51064),
(@CGUID-96, 66282, 870, 1740, -687.629, -1660.68, 17.5506, 2.54598),
(@CGUID-97, 66282, 870, 1740, -670.014, -1672.62, 15.708, 0.515729),
(@CGUID-98, 66282, 870, 1740, -669.013, -1697.94, 17.7117, 0.635109),
(@CGUID-99, 66282, 870, 1740, -672.803, -1712.95, 18.3744, 4.79144),
(@CGUID-100, 55336, 870, 1740, -722.834, -1899.92, 6.61636, 0.571556),
(@CGUID-101, 55336, 870, 1740, -771.599, -1719.24, 10.2894, 2.45337),
(@CGUID-102, 66290, 870, 1740, -660.203, -1527.69, 2.06662, 1.80692),
(@CGUID-103, 6491, 870, 1740, -698.609, -1470.44, 130.298, 6.07222),
(@CGUID-104, 66339, 870, 1740, -617.698, -1549.19, 6.55295, 0.366405),
(@CGUID-105, 66339, 870, 1740, -691.852, -1550.96, 3.14284, 1.59693),
(@CGUID-106, 66339, 870, 1740, -606.733, -1571.73, 12.7578, 0.178842),
(@CGUID-107, 66339, 870, 1740, -609.142, -1619.83, 14.0233, 4.00197),
(@CGUID-108, 66339, 870, 1740, -730.04, -1642.63, 17.0327, 4.8105),
(@CGUID-109, 66339, 870, 1740, -661.392, -1698.86, 18.0548, 4.48237),
(@CGUID-110, 66339, 870, 1740, -562.365, -1671.02, 11.3371, 5.06228),
(@CGUID-111, 66339, 870, 1740, -785.458, -1701.52, 10.9884, 4.62372),
(@CGUID-112, 66339, 870, 1740, -690.167, -1761.89, 2.76573, 4.62372),
(@CGUID-113, 66383, 870, 1740, -772.682, -1748.49, 12.8537, 1.84081),
(@CGUID-114, 66384, 870, 1740, -819.253, -1762.69, 11.2071, 3.22703),
(@CGUID-115, 66396, 870, 1740, -833.631, -1798.49, 4.62255, 1.60125),
(@CGUID-116, 66527, 870, 1740, -667.13, -1467.31, 130.161, 4.58842),
(@CGUID-117, 66480, 870, 1740, -667.516, -1468.07, 130.164, 2.71268),
(@CGUID-118, 55370, 870, 1740, -662.151, -1466.17, 130.309, 1.6543),
(@CGUID-119, 66480, 870, 1740, -660.319, -1461.62, 130.312, 4.6953),
(@CGUID-120, 66473, 870, 1740, -675.224, -1462.23, 130.226, 0.536965),
(@CGUID-121, 40789, 870, 1740, -673.983, -1476.68, 130.199, 0),
(@CGUID-122, 62167, 870, 1740, -677.547, -1475.91, 130.202, 6.24045),
(@CGUID-123, 62167, 870, 1740, -662, -1480.1, 130.204, 0),
(@CGUID-124, 64866, 870, 1740, -663.403, -1482.78, 130.201, 2.78045),
(@CGUID-125, 66292, 870, 1740, -664.672, -1483.3, 130.2, 2.83811),
(@CGUID-126, 66473, 870, 1740, -684.747, -1459.56, 130.226, 0.536965),
(@CGUID-127, 54615, 870, 1740, -655.898, -1483.83, 130.203, 2.93827),
(@CGUID-128, 66297, 870, 1740, -653.568, -1482.47, 130.202, 5.09636),
(@CGUID-129, 66480, 870, 1740, -648.542, -1478.49, 130.201, 2.86804),
(@CGUID-130, 62167, 870, 1740, -688.347, -1472.92, 130.209, 0),
(@CGUID-131, 54618, 870, 1740, -679.983, -1487.32, 130.358, 3.4583),
(@CGUID-132, 62167, 870, 1740, -647.285, -1483.47, 130.323, 0),
(@CGUID-133, 66473, 870, 1740, -668.727, -1492.03, 130.226, 5.05802),
(@CGUID-134, 66473, 870, 1740, -682.349, -1488.49, 130.226, 5.05802),
(@CGUID-135, 66473, 870, 1740, -695.578, -1456.86, 130.226, 0.536965),
(@CGUID-136, 66480, 870, 1740, -645.436, -1486.74, 131.878, 0.586023),
(@CGUID-137, 66473, 870, 1740, -691.776, -1485.94, 130.226, 5.05802),
(@CGUID-138, 66480, 870, 1740, -690.918, -1487.86, 130.455, 1.76594),
(@CGUID-139, 62167, 870, 1740, -703.663, -1468.91, 130.528, 0),
(@CGUID-140, 54617, 870, 1740, -680.066, -1501.44, 131.345, 4.45011),
(@CGUID-141, 66510, 870, 1740, -681.34, -1501.11, 131.348, 4.3478),
(@CGUID-142, 66509, 870, 1740, -682.819, -1501.05, 131.353, 4.44575),
(@CGUID-143, 66473, 870, 1740, -702.776, -1483.18, 130.226, 5.05802),
(@CGUID-144, 62167, 870, 1740, -688.083, -1500.18, 131.455, 0),
(@CGUID-145, 66480, 870, 1740, -704.297, -1481.72, 130.455, 5.62627),
(@CGUID-146, 64860, 870, 1740, -716.361, -1457.17, 133.022, 6.0639),
(@CGUID-147, 62167, 870, 1740, -705.247, -1497.45, 134.454, 0),
(@CGUID-148, 64861, 870, 1740, -720.67, -1472.24, 133.022, 5.85648),
(@CGUID-149, 62167, 870, 1740, -717.971, -1421.71, 134.771, 0),
(@CGUID-150, 62167, 870, 1740, -739.512, -1503, 134.77, 0),
(@CGUID-151, 40789, 870, 1740, -729.542, -1418.86, 101.101, 0),
(@CGUID-152, 40789, 870, 1740, -750.875, -1498.72, 101.101, 0),
(@CGUID-153, 54616, 870, 1740, -564.589, -1595.41, 14.1616, 0);SET @OGUID := 101313;
DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID-119 AND @OGUID-0;
DELETE FROM `gameobject` WHERE `phaseId`=1740;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `phaseId`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@OGUID-0, 209328, 870, 1740, -536.667, -1515.83, 6.90606, 2.04653),
(@OGUID-1, 209328, 870, 1740, -746.105, -1592.65, 26.103, 2.04653),
(@OGUID-2, 209328, 870, 1740, -704.217, -1508.85, 3.34308, 2.04653),
(@OGUID-3, 209328, 870, 1740, -657.676, -1501.87, 3.49413, 2.04653),
(@OGUID-4, 209328, 870, 1740, -843.842, -1592.65, 1.82281, 2.04653),
(@OGUID-5, 209328, 870, 1740, -801.955, -1613.6, 2.86431, 2.04653),
(@OGUID-6, 209328, 870, 1740, -680.946, -1767.23, 2.46407, 2.04653),
(@OGUID-7, 209328, 870, 1740, -741.451, -1683.43, 17.2872, 2.04653),
(@OGUID-8, 209328, 870, 1740, -746.105, -1683.43, 17.1854, 2.04653),
(@OGUID-9, 209328, 870, 1740, -601.826, -1557.73, 11.7642, 2.04653),
(@OGUID-10, 209328, 870, 1740, -518.051, -1746.28, -17.6051, 2.04653),
(@OGUID-11, 209328, 870, 1740, -713.526, -1830.08, 4.71763, 2.04653),
(@OGUID-12, 209328, 870, 1740, -722.834, -1578.68, 34.0204, 2.04653),
(@OGUID-13, 209328, 870, 1740, -625.096, -1571.7, 18.1951, 2.04653),
(@OGUID-14, 209328, 870, 1740, -662.33, -1627.57, 24.6109, 2.04653),
(@OGUID-15, 209328, 870, 1740, -746.105, -1851.03, 4.78908, 2.04653),
(@OGUID-16, 209328, 870, 1740, -653.021, -1397.12, 1.24897, 2.04653),
(@OGUID-17, 209328, 870, 1740, -760.067, -1906.9, 5.10909, 2.04653),
(@OGUID-18, 209328, 870, 1740, -680.946, -1369.18, 5.88025, 2.04653),
(@OGUID-19, 209328, 870, 1740, -755.413, -1725.33, 27.4403, 2.04653),
(@OGUID-20, 209328, 870, 1740, -694.909, -1718.35, 17.4731, 2.04653),
(@OGUID-21, 209328, 870, 1740, -797.301, -1704.38, 10.2189, 2.04653),
(@OGUID-22, 209328, 870, 1740, -708.871, -1871.98, 6.85791, 2.04653),
(@OGUID-23, 209328, 870, 1740, -741.451, -1634.55, 16.2216, 2.04653),
(@OGUID-24, 209328, 870, 1740, -722.834, -1725.33, 27.3802, 2.04653),
(@OGUID-25, 209328, 870, 1740, -685.601, -1767.23, 2.81279, 2.04653),
(@OGUID-26, 209311, 870, 1740, -722.834, -1725.33, 27.3802, 2.04653),
(@OGUID-27, 209328, 870, 1740, -806.609, -1613.6, 2.15379, 2.04653),
(@OGUID-28, 209311, 870, 1740, -680.946, -1767.23, 2.46407, 2.04653),
(@OGUID-29, 209328, 870, 1740, -685.601, -1376.17, 4.71566, 2.04653),
(@OGUID-30, 209328, 870, 1740, -671.638, -1578.68, 10.178, 2.04653),
(@OGUID-31, 209311, 870, 1740, -801.955, -1613.6, 2.86431, 2.04653),
(@OGUID-32, 209328, 870, 1740, -727.488, -1725.33, 27.487, 2.04653),
(@OGUID-33, 209311, 870, 1740, -653.021, -1397.12, 1.24897, 2.04653),
(@OGUID-34, 209354, 870, 1740, -532.013, -1620.58, 10.9777, 2.04653),
(@OGUID-35, 209354, 870, 1740, -741.451, -1613.6, 22.5242, 2.04653),
(@OGUID-36, 209354, 870, 1740, -634.405, -1229.52, 1.36574, 2.04653),
(@OGUID-37, 209354, 870, 1740, -746.105, -1613.6, 23.3926, 2.04653),
(@OGUID-38, 209354, 870, 1740, -653.021, -1648.52, 16.943, 2.04653),
(@OGUID-39, 209354, 870, 1740, -573.901, -1753.27, 0.331631, 2.04653),
(@OGUID-40, 209354, 870, 1740, -527.359, -1683.43, 6.6755, 2.04653),
(@OGUID-41, 209354, 870, 1740, -727.488, -1746.28, 28.7766, 2.04653),
(@OGUID-42, 209354, 870, 1740, -620.442, -1739.3, 10.2278, 2.04653),
(@OGUID-43, 209354, 870, 1740, -518.051, -1725.33, 2.10044, 2.04653),
(@OGUID-44, 209354, 870, 1740, -597.171, -1627.57, 13.842, 2.04653),
(@OGUID-45, 209354, 870, 1740, -592.517, -1697.4, 9.87165, 2.04653),
(@OGUID-46, 209349, 870, 1740, -592.517, -1697.4, 9.87165, 2.04653),
(@OGUID-47, 209353, 870, 1740, -606.48, -1788.18, -0.017914, 2.04653),
(@OGUID-48, 209349, 870, 1740, -620.442, -1739.3, 10.2278, 2.04653),
(@OGUID-49, 209349, 870, 1740, -727.488, -1746.28, 28.7766, 2.04653),
(@OGUID-50, 209353, 870, 1740, -573.901, -1753.27, 0.331631, 2.04653),
(@OGUID-51, 209353, 870, 1740, -876.422, -1459.97, 0.361797, 2.04653),
(@OGUID-52, 209349, 870, 1740, -741.451, -1613.6, 22.5242, 2.04653),
(@OGUID-53, 209353, 870, 1740, -522.705, -1718.35, 2.15691, 2.04653),
(@OGUID-54, 209353, 870, 1740, -848.496, -1439.02, 0.327805, 2.04653),
(@OGUID-55, 209349, 870, 1740, -555.284, -1599.63, 12.8203, 2.04653),
(@OGUID-56, 209349, 870, 1740, -746.105, -1613.6, 23.3926, 2.04653),
(@OGUID-57, 209353, 870, 1740, -522.705, -1725.33, 2.83003, 2.04653),
(@OGUID-58, 209353, 870, 1740, -778.684, -1404.1, 1.07718, 2.04653),
(@OGUID-59, 209349, 870, 1740, -648.367, -1648.52, 15.8, 2.04653),
(@OGUID-60, 209349, 870, 1740, -648.367, -1655.5, 15.865, 2.04653),
(@OGUID-61, 209349, 870, 1740, -597.171, -1627.57, 13.842, 2.04653),
(@OGUID-62, 209349, 870, 1740, -564.592, -1564.72, 12.0085, 2.04653),
(@OGUID-63, 209353, 870, 1740, -606.48, -1795.17, 1.99826, 2.04653),
(@OGUID-64, 209353, 870, 1740, -741.451, -1278.4, 1.09296, 2.04653),
(@OGUID-65, 209349, 870, 1740, -527.359, -1683.43, 6.6755, 2.04653),
(@OGUID-66, 209353, 870, 1740, -746.105, -1278.4, 0.668877, 2.04653),
(@OGUID-67, 209349, 870, 1740, -508.742, -1585.67, -5.15367, 2.04653),
(@OGUID-68, 209353, 870, 1740, -694.909, -1222.53, 0.188126, 2.04653),
(@OGUID-69, 209349, 870, 1740, -657.676, -1697.4, 18.0272, 2.04653),
(@OGUID-70, 209353, 870, 1740, -955.542, -1313.32, 0.5177, 2.04653),
(@OGUID-71, 209353, 870, 1740, -899.692, -1348.23, 1.42731, 2.04653),
(@OGUID-72, 209353, 870, 1740, -694.909, -1376.17, 3.01361, 2.04653),
(@OGUID-73, 209349, 870, 1740, -653.021, -1648.52, 16.943, 2.04653),
(@OGUID-74, 209353, 870, 1740, -722.834, -1390.13, 4.47963, 2.04653),
(@OGUID-75, 209353, 870, 1740, -634.405, -1229.52, 1.36574, 2.04653),
(@OGUID-76, 209311, 870, 1740, -657.676, -1501.87, 3.49413, 2.04653),
(@OGUID-77, 209311, 870, 1740, -741.451, -1634.55, 16.2216, 2.04653),
(@OGUID-78, 209311, 870, 1740, -760.067, -1906.9, 5.10909, 2.04653),
(@OGUID-79, 209311, 870, 1740, -746.105, -1683.43, 17.1854, 2.04653),
(@OGUID-80, 209311, 870, 1740, -680.946, -1369.18, 5.88025, 2.04653),
(@OGUID-81, 209311, 870, 1740, -662.33, -1627.57, 24.6109, 2.04653),
(@OGUID-82, 209311, 870, 1740, -713.526, -1830.08, 4.71763, 2.04653),
(@OGUID-83, 209311, 870, 1740, -741.451, -1683.43, 17.2872, 2.04653),
(@OGUID-84, 209311, 870, 1740, -746.105, -1592.65, 26.103, 2.04653),
(@OGUID-85, 209311, 870, 1740, -722.834, -1578.68, 34.0204, 2.04653),
(@OGUID-86, 209311, 870, 1740, -755.413, -1725.33, 27.4403, 2.04653),
(@OGUID-87, 209311, 870, 1740, -797.301, -1704.38, 10.2189, 2.04653),
(@OGUID-88, 209311, 870, 1740, -843.842, -1592.65, 1.82281, 2.04653),
(@OGUID-89, 209311, 870, 1740, -708.871, -1871.98, 6.85791, 2.04653),
(@OGUID-90, 209311, 870, 1740, -746.105, -1851.03, 4.78908, 2.04653),
(@OGUID-91, 209311, 870, 1740, -685.601, -1767.23, 2.81279, 2.04653),
(@OGUID-92, 209311, 870, 1740, -625.096, -1571.7, 18.1951, 2.04653),
(@OGUID-93, 209311, 870, 1740, -601.826, -1557.73, 11.7642, 2.04653),
(@OGUID-94, 209311, 870, 1740, -727.488, -1725.33, 27.487, 2.04653),
(@OGUID-95, 209311, 870, 1740, -694.909, -1718.35, 17.4731, 2.04653),
(@OGUID-96, 209311, 870, 1740, -704.217, -1508.85, 3.34308, 2.04653),
(@OGUID-97, 209311, 870, 1740, -806.609, -1613.6, 2.15379, 2.04653),
(@OGUID-98, 209311, 870, 1740, -685.601, -1376.17, 4.71566, 2.04653),
(@OGUID-99, 209311, 870, 1740, -518.051, -1746.28, -17.6051, 2.04653),
(@OGUID-100, 209311, 870, 1740, -671.638, -1578.68, 10.178, 2.04653),
(@OGUID-101, 209311, 870, 1740, -536.667, -1515.83, 6.90606, 2.04653),
(@OGUID-102, 215413, 870, 1740, -659.58, -1624.87, 24.7405, 0),
(@OGUID-103, 215413, 870, 1740, -763.505, -1907.63, 4.13763, 0),
(@OGUID-104, 215413, 870, 1740, -712.564, -1599.82, 21.2561, 0),
(@OGUID-105, 215413, 870, 1740, -600.339, -1556.95, 11.0291, 0),
(@OGUID-106, 215413, 870, 1740, -693.104, -1720.87, 17.9123, 0),
(@OGUID-107, 215413, 870, 1740, -711.543, -1873.02, 7.03714, 0),
(@OGUID-108, 215413, 870, 1740, -774.406, -1651.54, 7.847, 4.10444),
(@OGUID-109, 215413, 870, 1740, -690.137, -1639.48, 22.6329, 0),
(@OGUID-110, 215413, 870, 1740, -626.901, -1568.95, 19.3761, 0),
(@OGUID-111, 215413, 870, 1740, -539.023, -1515.88, 7.22319, 0),
(@OGUID-112, 215413, 870, 1740, -719.719, -1580.22, 34.7358, 2.19961),
(@OGUID-113, 215413, 870, 1740, -745.795, -1683.22, 16.7408, 0),
(@OGUID-114, 215413, 870, 1740, -683.483, -1770.67, 3.44034, 0),
(@OGUID-115, 215413, 870, 1740, -654.88, -1398.11, 1.581, 0),
(@OGUID-116, 215413, 870, 1740, -799.497, -1705.29, 8.33333, 2.09624),
(@OGUID-117, 215413, 870, 1740, -670.429, -1583.05, 11.1423, 1.30049),
(@OGUID-118, 215413, 870, 1740, -714.641, -1829.57, 4.09966, 0),
(@OGUID-119, 215413, 870, 1740, -755.984, -1729.34, 27.5763, 0);
DELETE FROM `achievement_reward` WHERE `entry`=7385;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`, `item`, `sender`, `subject`, `text`) VALUES (7385, 0, 0, 87528, 73649, 'Honorary Brewmaster Keg', '$n,$b$bThanks for all your hard work crafting some of the finest brews we\'ve ever had!$b$bAs a token of our appreciation, the other Brewmasters and I have decided to give you this Honorary Brewmaster Keg.$b$bEnjoy!$b$b-Mama Stormstout');
DELETE FROM `achievement_reward` WHERE `entry`=7479;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`) VALUES ('7479', '318', '318');
DELETE FROM `achievement_reward` WHERE `entry`=7520;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`, `item`, `sender`, `subject`, `text`) VALUES ('7520', '125', '125', '43300', '3057', 'The Loremaster', 'News of your accomplishments has traveled far. The winds of turmoil howl through our lands. Those who stand to challenge evil are our only hope.$B$BOnly those who listen to the winds understand the debts of our fallen heroes have paid to protect our people. May a hero such as yourself live long to tell the tales of your adventures. For only then will we remember just how much we have to be thankful for.$B$BOur appreciation runes deep, Loremaster.$B$B-Cairne Bloodhoof');
DELETE FROM `achievement_reward` WHERE `entry`=8300;
INSERT INTO `achievement_reward` (`entry`, `item`, `sender`, `subject`) VALUES ('8300', '94191', '64582', 'Stunted Direhorn');
DELETE FROM `achievement_reward` WHERE `entry`=8340;
INSERT INTO `achievement_reward` (`entry`, `item`, `sender`, `subject`, `text`) VALUES (8340, 98543, 67260, 'Wraps of the Blood-Soaked Brawler', 'Let\'s take a quick inventory: You\'ve defeated a shark, a robotic crocolisk, a pile of ooze, a pair of penguins, and a whole roomful of deadly brawlers from around the world.\r\n\r\nYeah. I\'d say you earned a shirt.');
DELETE FROM `achievement_reward` WHERE `entry`=8343;
INSERT INTO `achievement_reward` (`entry`, `item`, `sender`, `subject`, `text`) VALUES (8343, 98543, 67260, 'Wraps of the Blood-Soaked Brawler', 'Let\'s take a quick inventory: You\'ve defeated a shark, a robotic crocolisk, a pile of ooze, a pair of penguins, and a whole roomful of deadly brawlers from around the world.\r\n\r\nYeah. I\'d say you earned a shirt.');

ALTER TABLE `achievement_reward`
	ADD COLUMN `spell` MEDIUMINT(7) NOT NULL DEFAULT '0' AFTER `item`;

DELETE FROM `achievement_reward` WHERE `entry`=8454;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`, `item`, `spell`, `sender`, `subject`, `text`) VALUES (8454, 0, 0, 0, -148392, 0, NULL, NULL);
DELETE FROM `achievement_reward` WHERE `entry`=8345;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`, `item`, `spell`, `sender`, `subject`, `text`) VALUES (8345, 0, 0, 98618, 0, 32216, 'Hearthsteed', '$n,\r\n\r\nThis fine magical construct arrived\r\nonly yesterday, and you were the first\r\nadventurer I thought of.\r\n\r\nEnjoy!\r\n\r\nMei Francis\r\nExotic Mounts\r\nDalaran');
DELETE FROM `achievement_reward` WHERE `entry`=8398;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`, `item`, `spell`, `sender`, `subject`, `text`) VALUES (8398, 0, 0, 0, -148396, 0, NULL, NULL);
DELETE FROM `achievement_reward` WHERE `entry`=8399;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`, `item`, `spell`, `sender`, `subject`, `text`) VALUES (8399, 0, 0, 0, -148396, 0, NULL, NULL);
DELETE FROM `achievement_reward` WHERE `entry`=8335;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`, `item`, `spell`, `sender`, `subject`, `text`) VALUES (8335, 0, 0, 93195, 0, 67260, 'Another Brawler\'s Pass', 'You keep pumping out those wins down here at Brawl\'gar!\r\n\r\nHere\'s another invitation.  Invite some of your friends to come join in the carnage.\r\n\r\nOr, you can just sell it to the highest bidder.  That\'s what I\'d do.\r\n\r\n- Boss Bazzelflange');

DELETE FROM `achievement_reward` WHERE `entry`=8337;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`, `item`, `spell`, `sender`, `subject`, `text`) VALUES (8337, 0, 0, 93228, 0, 67260, 'Another Brawler\'s Pass', 'You keep pumping out those wins down here at Brawl\'gar!\r\n\r\nHere\'s another invitation.  Invite some of your friends to come join in the carnage.\r\n\r\nOr, you can just sell it to the highest bidder.  That\'s what I\'d do.\r\n\r\n- Boss Bazzelflange');
-- Spawn of Galakras
DELETE FROM `creature_template` WHERE `entry`=73672;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (73672, 0, 0, 0, 0, 0, 51479, 0, 0, 0, 'Spawn of Galakras', NULL, NULL, 0, 90, 90, 4, 0, 14, 14, 0, 1, 1.14286, 1, 9839, 14339, 0, 42299, 1, 2000, 2000, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 209, 1, 0, 0, '', 18414);

-- Korkron War Wolf
DELETE FROM `creature_template` WHERE `entry`=73677;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (73677, 0, 0, 0, 0, 0, 51481, 51482, 51483, 0, 'Kor\'kron War Wolf', NULL, NULL, 0, 90, 90, 4, 0, 14, 14, 0, 1, 1.14286, 1, 9839, 14339, 0, 42299, 1, 2000, 2000, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 140, 1, 0, 0, '', 18414);

-- Boss Bazzelflange
DELETE FROM `creature_template` WHERE `entry`=67260;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (67260, 0, 0, 0, 0, 0, 36679, 0, 0, 0, 'Boss Bazzelflange', NULL, NULL, 0, 90, 90, 4, 0, 125, 125, 0, 1, 1.14286, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 0, 0, 7, 4096, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 20, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 18414);
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE  `entry`=53565;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE  `entry`=65469;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE  `entry`=57753;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE  `entry`=57752;

UPDATE `creature` SET `MovementType`=2 WHERE `guid`=224456;

DELETE FROM `creature_addon` WHERE `guid`=120680;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (120680, 0, 0, 1, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=120679;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (120679, 0, 0, 1, 0, 0, NULL);

DELETE FROM `smart_scripts` WHERE `entryorguid`=-120680;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(-120680, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 1, 5, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 5'),
(-120680, 0, 1, 0, 52, 0, 100, 0, 5, 53565, 0, 0, 1, 6, 15000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 5 Over - Say Text Line 6'),
(-120680, 0, 2, 0, 52, 0, 100, 0, 6, 53565, 0, 0, 1, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 6 Over - Say Text Line 7');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-120679;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(-120679, 0, 0, 0, 1, 0, 100, 0, 20000, 20000, 120000, 120000, 1, 1, 16000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - OOC - Say Text Line 1'),
(-120679, 0, 1, 0, 52, 0, 100, 0, 1, 53565, 0, 0, 1, 2, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 2'),
(-120679, 0, 2, 0, 52, 0, 100, 0, 2, 53565, 0, 0, 1, 3, 11000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 2 Over - Say Text Line 3'),
(-120679, 0, 3, 0, 52, 0, 100, 0, 3, 53565, 0, 0, 1, 4, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 3 Over - Say Text Line 4'),
(-120679, 0, 4, 0, 52, 0, 100, 0, 4, 53565, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 53565, 3, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 4 Over - Set Data 1 1');

DELETE FROM `smart_scripts` WHERE `entryorguid`=53565;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(53565, 0, 0, 1, 38, 0, 100, 0, 1, 1, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 1 1 - Set Phase 1'),
(53565, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 17, 510, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 1 1 (Link) - Set Emote State (510)'),
(53565, 0, 2, 3, 60, 1, 100, 0, 1000, 1000, 5000, 5000, 11, 108967, 0, 0, 0, 0, 0, 11, 0, 4, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Update - Cast Spell \'Jab\''),
(53565, 0, 3, 4, 61, 1, 100, 0, 0, 0, 0, 0, 10, 507, 509, 511, 543, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Update (Link) - Play Random Emote (507, 509, 511, 543)'),
(53565, 0, 4, 0, 61, 1, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Update (Link) - Play Random Sound (33643, 33645, 33646)'),
(53565, 0, 5, 6, 38, 0, 100, 0, 2, 2, 0, 0, 22, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 2 2 - Set Phase 2'),
(53565, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 17, 30, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 2 2 (Link) - Clear Emote'),
(53565, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 2 2 (Link) - Say Text Line 0'),
(53565, 0, 8, 0, 38, 0, 100, 0, 3, 1, 0, 0, 80, 5356500, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 3 1 - Start Script'),
(53565, 0, 9, 0, 38, 0, 100, 0, 3, 2, 0, 0, 80, 5356502, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 3 2 - Start Script'),
(53565, 0, 10, 0, 38, 0, 100, 0, 3, 3, 0, 0, 80, 5356503, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 3 3 - Start Script'),
(53565, 0, 11, 0, 38, 0, 100, 0, 3, 4, 0, 0, 80, 5356504, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 3 4 - Start Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=65469;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(65469, 0, 0, 1, 38, 0, 100, 0, 1, 1, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 1 1 - Set Phase 1'),
(65469, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 17, 510, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 1 1 (Link) - Set Emote State (510)'),
(65469, 0, 2, 3, 60, 1, 100, 0, 1000, 1000, 5000, 5000, 11, 108967, 0, 0, 0, 0, 0, 11, 0, 4, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Update - Cast Spell \'Jab\''),
(65469, 0, 3, 4, 61, 1, 100, 0, 0, 0, 0, 0, 10, 507, 509, 511, 543, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Update (Link) - Play Random Emote (507, 509, 511, 543)'),
(65469, 0, 4, 0, 61, 1, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Update (Link) - Play Random Sound (33643, 33645, 33646)'),
(65469, 0, 5, 6, 38, 0, 100, 0, 2, 2, 0, 0, 22, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 2 2 - Set Phase 2'),
(65469, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 17, 30, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 2 2 (Link) - Clear Emote'),
(65469, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 2 2 (Link) - Say Text Line 0'),
(65469, 0, 8, 0, 38, 0, 100, 0, 3, 1, 0, 0, 80, 5356500, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 3 1 - Start Script'),
(65469, 0, 9, 0, 38, 0, 100, 0, 3, 2, 0, 0, 80, 5356502, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 3 2 - Start Script'),
(65469, 0, 10, 0, 38, 0, 100, 0, 3, 3, 0, 0, 80, 5356503, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 3 3 - Start Script'),
(65469, 0, 11, 0, 38, 0, 100, 0, 3, 4, 0, 0, 80, 5356504, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 3 4 - Start Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5356500;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(5356500, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 5, 507, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - Play Emote (507)'),
(5356500, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - Play Random Sound (33643, 33645, 33646)');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5356501;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(5356501, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 5, 508, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - Play Emote (508)'),
(5356501, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - Play Random Sound (33643, 33645, 33646)');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5356502;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(5356502, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 5, 509, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - Play Emote (509)'),
(5356502, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - Play Random Sound (33643, 33645, 33646)');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5356503;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(5356503, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 5, 511, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - Play Emote (511)'),
(5356503, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - Play Random Sound (33643, 33645, 33646)');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5356504;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(5356504, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 5, 543, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - Play Emote (543)'),
(5356504, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - Play Random Sound (33643, 33645, 33646)');

DELETE FROM `smart_scripts` WHERE `entryorguid`=57748;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(57748, 0, 0, 0, 1, 0, 100, 0, 0, 0, 5000, 5000, 87, 5774800, 5774801, 5774802, 5774803, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Qun - OOC - Start Random Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5774800;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(5774800, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 10, 507, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Qun - Play Emote (507)'),
(5774800, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Qun - Play Random Sound (33643, 33645, 33646)'),
(5774800, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 45, 3, 1, 0, 0, 0, 0, 11, 0, 15, 0, 0, 0, 0, 0, 'Instructor Qun - Set Data 3 1');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5774801;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(5774801, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 10, 509, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Qun - Play Emote'),
(5774801, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Qun - Play Random Sound (33643, 33645, 33646)'),
(5774801, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 45, 3, 2, 0, 0, 0, 0, 11, 0, 15, 0, 0, 0, 0, 0, 'Instructor Qun - Set Data 3 2');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5774802;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(5774802, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 10, 511, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Qun - Play Emote'),
(5774802, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Qun - Play Random Sound (33643, 33645, 33646)'),
(5774802, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 45, 3, 3, 0, 0, 0, 0, 11, 0, 15, 0, 0, 0, 0, 0, 'Instructor Qun - Set Data 3 3');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5774803;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(5774803, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 10, 543, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Qun - Play Emote'),
(5774803, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Qun - Play Random Sound (33643, 33645, 33646)'),
(5774803, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 45, 3, 4, 0, 0, 0, 0, 11, 0, 15, 0, 0, 0, 0, 0, 'Instructor Qun - Set Data 3 4');

DELETE FROM `smart_scripts` WHERE `entryorguid`=57752;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(57752, 0, 0, 1, 1, 0, 100, 0, 0, 0, 5000, 5000, 10, 507, 509, 511, 543, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Quiet Lam - OOC - Play Random Emote (507, 509, 511, 543)'),
(57752, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 'Quiet Lam - Play Random Sound (33643, 33645, 33646)');

DELETE FROM `smart_scripts` WHERE `entryorguid`=57753;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(57753, 0, 0, 1, 1, 0, 100, 0, 0, 0, 5000, 5000, 10, 507, 509, 511, 543, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ironfist Zhou - OOC - Play Random Emote (507, 509, 511, 543)'),
(57753, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 'Ironfist Zhou - Play Random Sound (33643, 33645, 33646)');

DELETE FROM `creature_text` WHERE `entry`=53565;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES 
(53565, 0, 0, 'Such skill for a new trainee.', 12, 0, 100, 511, 0, 0, 'Aspiring Trainee'),
(53565, 0, 1, 'That target didn\'t stand a chance.', 12, 0, 100, 511, 0, 0, 'Aspiring Trainee'),
(53565, 0, 2, 'One day you will have to teach me your secrets.', 12, 0, 100, 509, 0, 0, 'Aspiring Trainee'),
(53565, 0, 3, 'You are bound for a great things, trainee.', 12, 0, 100, 508, 0, 0, 'Aspiring Trainee'),
(53565, 0, 4, 'I must train harder so I can be like you.', 12, 0, 100, 1, 0, 0, 'Aspiring Trainee'),
(53565, 0, 5, 'I hope the instructors saw that!', 12, 0, 100, 1, 0, 0, 'Aspiring Trainee'),
(53565, 0, 6, 'I must work as hard as you.', 12, 0, 100, 1, 0, 0, 'Aspiring Trainee'),
(53565, 0, 7, 'Your form is impeccable.', 12, 0, 100, 1, 0, 0, 'Aspiring Trainee'),
(53565, 1, 0, 'My money\'s on Lam. Did you see him take down Chu during training last week?', 12, 0, 100, 1, 0, 0, 'Aspiring Trainee'),
(53565, 2, 0, 'His stamina is nothing against Lam\'s strength.', 12, 0, 100, 0, 0, 0, 'Aspiring Trainee'),
(53565, 3, 0, 'He has the legs of an ox! Have you ever been kicked by him in training?', 12, 0, 100, 1, 0, 0, 'Aspiring Trainee'),
(53565, 4, 0, 'Raise you a keg of Stormshout Gold. It\'ll be your loss.', 12, 0, 100, 1, 0, 0, 'Aspiring Trainee'),
(53565, 5, 0, 'That\'s where you\'re mistaken, friend. Zhou is as sturdy as an oak.', 12, 0, 100, 1, 0, 0, 'Aspiring Trainee'),
(53565, 6, 0, 'Zhou has the stamina of three men, so he can out-stay most opponents.', 12, 0, 100, 0, 0, 0, 'Aspiring Trainee'),
(53565, 7, 0, 'Four coppers on Zhou.', 12, 0, 100, 0, 0, 0, 'Aspiring Trainee');

DELETE FROM `creature_text` WHERE `entry`=65469;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES 
(65469, 0, 0, 'Such skill for a new trainee.', 12, 0, 100, 511, 0, 0, 'Aspiring Trainee to Training Target'),
(65469, 0, 1, 'That target didn\'t stand a chance.', 12, 0, 100, 509, 0, 0, 'Aspiring Trainee to Training Target'),
(65469, 0, 2, 'One day you will have to teach me your secrets.', 12, 0, 100, 509, 0, 0, 'Aspiring Trainee to Training Target'),
(65469, 0, 3, 'You are bound for a great things, trainee.', 12, 0, 100, 508, 0, 0, 'Aspiring Trainee to Training Target');
DELETE FROM `achievement_reward` WHERE `entry`=8643;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`) VALUES ('8643', '378', '378');
DELETE FROM `achievement_reward` WHERE `entry`=8644;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`) VALUES (8644, 42, 42);
DELETE FROM `achievement_reward` WHERE `entry`=8645;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`) VALUES (8645, 43, 43);
DELETE FROM `achievement_reward` WHERE `entry`=8649;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`) VALUES (8649, 45, 45);
DELETE FROM `achievement_reward` WHERE `entry`=8667;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`) VALUES (8667, 42, 42);
DELETE FROM `achievement_reward` WHERE `entry`=8668;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`) VALUES (8668, 43, 43);
DELETE FROM `achievement_reward` WHERE `entry`=8670;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`) VALUES (8670, 45, 45);
DELETE FROM `achievement_reward` WHERE `entry`=8679;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`) VALUES (8679, 382, 0);
DELETE FROM `achievement_reward` WHERE `entry`=8680;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`) VALUES (8680, 0, 383);
DELETE FROM `achievement_reward` WHERE `entry`=8293;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`, `item`, `spell`, `sender`, `subject`, `text`) VALUES (8293, 0, 0, 97558, 0, 71965, 'Tito\'s Basket', 'Congrats!');
DELETE FROM `achievement_reward` WHERE `entry`=8124;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`, `item`, `spell`) VALUES (8124, 0, 0, 0, -136400);
DELETE FROM `achievement_reward` WHERE `entry`=7860;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`, `item`, `spell`) VALUES (7860, 0, 0, 0, -133023);
DELETE FROM `achievement_reward` WHERE `entry`=7862;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`, `item`, `spell`) VALUES (7862, 0, 0, 0, -133023);
DELETE FROM `achievement_reward` WHERE `entry`=7501;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`, `item`, `spell`, `sender`, `subject`, `text`) VALUES (7501, 0, 0, 89736, 0, 71965, 'Venus', 'Congrats!');
DELETE FROM `achievement_reward` WHERE `entry`=7500;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`, `item`, `spell`, `sender`, `subject`, `text`) VALUES (7500, 0, 0, 85578, 0, 71965, 'Feral Vermling', 'Congrats!');
DELETE FROM `achievement_reward` WHERE `entry`=8641;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`, `item`, `sender`, `subject`) VALUES (8641, 0, 0, 103533, 10540, 'Vicious Saddle');
DELETE FROM `achievement_reward` WHERE `entry`=8642;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`, `item`, `sender`, `subject`) VALUES (8642, 0, 0, 103533, 29611, 'Vicious Saddle');
DELETE FROM `achievement_reward` WHERE `entry`=8507;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`) VALUES ('8507', '385', '385');
DELETE FROM `achievement_reward` WHERE `entry`=8508;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`) VALUES ('8508', '386', '386');
DELETE FROM `achievement_reward` WHERE `entry`=8509;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`) VALUES ('8509', '387', '387');
DELETE FROM `achievement_reward` WHERE `entry`=8293;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`, `item`, `spell`, `sender`, `subject`, `text`) VALUES (8293, 0, 0, 97558, 0, 71965, 'Tito\'s Basket', 'Congrats!');
DELETE FROM `achievement_reward` WHERE `entry`=8121;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`) VALUES ('8121', '341', '341');
DELETE FROM `achievement_reward` WHERE `entry`=8023;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`) VALUES ('8023', '338', '338');
UPDATE `creature_template` SET `faction_A`=2082, `faction_H`=2082, `unit_flags`=16777224, `ScriptName`='npc_eye_of_acherus' WHERE `entry`=28511;

DELETE FROM `spell_script_names` WHERE `spell_id`=51904 AND `ScriptName`='spell_q12641_death_comes_from_on_high_summon_ghouls';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (51904, 'spell_q12641_death_comes_from_on_high_summon_ghouls');

DELETE FROM `creature_text` WHERE `entry`=28511;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES (28511, 0, 0, 'The Eye of Acherus launches towards its destination.', 42, 0, 100, 0, 0, 0, 'Eye of Acherus');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES (28511, 1, 0, 'The Eye of Acherus is in your control.', 42, 0, 100, 0, 0, 0, 'Eye of Acherus');

UPDATE `quest_template` SET `SpecialFlags`=32, `DetailsEmote2`=396, `DetailsEmote3`=397, `DetailsEmote4`=396 WHERE `Id`=12641;
