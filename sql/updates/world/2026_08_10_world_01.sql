-- Mangled Body
SET @CGUID := 100107;
DELETE FROM `creature` WHERE `guid`=@CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 38878, 1, -1874.34375, -3227.51562, 82.83399, 2.72271);

DELETE FROM `creature_template_addon` WHERE `entry`=38878;
INSERT INTO `creature_template_addon` (`entry`, `bytes2`, `auras`) VALUES
(38878, 1, '29266');

-- Terrortooth Runner
SET @CGUID := 100188;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID-15 AND @CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 37083, 1, -1897.85632, -3202.8894, 87.77674, 3.65153),
(@CGUID-1, 37083, 1, -1844.0769, -3187.593, 82.87798, 1.97235),
(@CGUID-2, 37083, 1, -1830.99841, -3317.79785, 114.43338, 3.47817),
(@CGUID-3, 37083, 1, -1854.33081, -3225.49438, 82.26019, 4.28833),
(@CGUID-4, 37083, 1, -1961.26281, -3185.98168, 92.72601, 6.21475),
(@CGUID-5, 37083, 1, -2004.9989, -3186.24389, 91.78253, 3.78055),
(@CGUID-6, 37083, 1, -2030.43115, -3217.59912, 91.56949, 3.52391),
(@CGUID-7, 37083, 1, -2031.3507, -3143.92919, 113.95429, 3.34818),
(@CGUID-8, 37083, 1, -1844.80261, -3044.13159, 91.37416, 4.96114),
(@CGUID-9, 37083, 1, -1849.78576, -2982.48583, 87.78672, 4.41719),
(@CGUID-10, 37083, 1, -1898.30578, -2944.16186, 90.1848, 3.13848),
(@CGUID-11, 37083, 1, -1932.17175, -2925.64746, 91.71369, 2.836),
(@CGUID-12, 37083, 1, -1898.44982, -2865.0852, 88.20968, 1.04889),
(@CGUID-13, 37083, 1, -1848.01086, -2778.76074, 87.8162, 4.84352),
(@CGUID-14, 37083, 1, -1817.66235, -2847.277, 87.23609, 5.50607),
(@CGUID-15, 37083, 1, -1870.75671, -2751.24536, 91.46912, 1.5786);

UPDATE `creature` SET `spawndist`=5 WHERE `guid` BETWEEN @CGUID-15 AND @CGUID-0;
UPDATE `creature` SET `MovementType`=1 WHERE `guid` BETWEEN @CGUID-15 AND @CGUID-0;

-- Terrortooth Scytheclaw
SET @CGUID := 100201;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID-12 AND @CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 37084, 1, -1966.67443, -3158.43481, 92.18515, 4.03242),
(@CGUID-1, 37084, 1, -1979.06713, -3186.46777, 91.78776, 2.93218),
(@CGUID-2, 37084, 1, -1919.72399, -3171.51391, 98.74291, 5.58505),
(@CGUID-3, 37084, 1, -2013.54357, -3180.35156, 91.84873, 1.61414),
(@CGUID-4, 37084, 1, -2014.845, -3208.92138, 91.38003, 3.39589),
(@CGUID-5, 37084, 1, -2039.13439, -3220.9187, 91.75808, 0.6849),
(@CGUID-6, 37084, 1, -2036.94689, -3260.30615, 91.74482, 4.46542),
(@CGUID-7, 37084, 1, -2073.69433, -3186.38647, 122.17117, 2.0519),
(@CGUID-8, 37084, 1, -2061.4082, -3154.89868, 117.78116, 2.0231),
(@CGUID-9, 37084, 1, -1967.38024, -2950.34545, 91.67481, 0.36651),
(@CGUID-10, 37084, 1, -1923.88024, -2965.66674, 91.6771, 0.36651),
(@CGUID-11, 37084, 1, -1932.89953, -2867.75732, 91.11415, 6.22536),
(@CGUID-12, 37084, 1, -1836.35461, -2741.22412, 90.1706, 4.98696);

UPDATE `creature` SET `spawndist`=5 WHERE `guid` BETWEEN @CGUID-12 AND @CGUID-0;
UPDATE `creature` SET `MovementType`=1 WHERE `guid` BETWEEN @CGUID-12 AND @CGUID-0;

-- Towering Plainstrider
SET @CGUID := 100873;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID-8 AND @CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 37085, 1, -1906.00756, -3281.23657, 74.88368, 1.24472),
(@CGUID-1, 37085, 1, -1930.63647, -3328.44287, 66.93544, 0.24877),
(@CGUID-2, 37085, 1, -1965.53552, -3348.6665, 59.703, 5.09124),
(@CGUID-3, 37085, 1, -1998.1383, -3387.46289, 62.7457, 3.31637),
(@CGUID-4, 37085, 1, -1850.85424, -2887.79223, 83.89178, 3.39157),
(@CGUID-5, 37085, 1, -1855.04748, -2800.69018, 86.22749, 0.66428),
(@CGUID-6, 37085, 1, -1926.72729, -2836.21899, 92.66056, 0.27105),
(@CGUID-7, 37085, 1, -1984.5177, -2775.76318, 92.50521, 2.50677),
(@CGUID-8, 37085, 1, -1950.26525, -2699.44799, 96.22448, 3.23154);

UPDATE `creature` SET `spawndist`=5 WHERE `guid` BETWEEN @CGUID-8 AND @CGUID-0;
UPDATE `creature` SET `MovementType`=1 WHERE `guid` BETWEEN @CGUID-8 AND @CGUID-0;

-- Hecklefang Scavenger
SET @CGUID := 102411;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID-10 AND @CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 37085, 1, -1830.12634, -3255.43432, 84.49085, 0),
(@CGUID-1, 37085, 1, -1789.84643, -3227.83154, 94.08807, 3.41452),
(@CGUID-2, 37085, 1, -1805.28662, -3192.73779, 88.38681, 5.68814),
(@CGUID-3, 37085, 1, -1806.38171, -3298.49414, 94.23509, 3.4164),
(@CGUID-4, 37085, 1, -1797.18762, -3268.80371, 92.11549, 0.06254),
(@CGUID-5, 37085, 1, -1914.3142, -3003.84545, 91.81597, 1.13446),
(@CGUID-6, 37085, 1, -1812.41149, -2975.6892, 87.26551, 3.94444),
(@CGUID-7, 37085, 1, -1810.81213, -3005.99267, 87.95318, 1.43553),
(@CGUID-8, 37085, 1, -1796.63891, -2957.64233, 86.86505, 2.44346),
(@CGUID-9, 37085, 1, -1801.32019, -2897.41381, 88.15282, 0.68572),
(@CGUID-10, 37085, 1, -1830.57116, -2804.79345, 86.23836, 2.10519);

UPDATE `creature` SET `spawndist`=5 WHERE `guid` BETWEEN @CGUID-10 AND @CGUID-0;
UPDATE `creature` SET `MovementType`=1 WHERE `guid` BETWEEN @CGUID-10 AND @CGUID-0;

-- Elder Zhevra
SET @CGUID := 103293;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID-2 AND @CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 37088, 1, -1849.73828, -3279.05664, 82.10183, 5.54063),
(@CGUID-1, 37088, 1, -1801.10766, -2984.06762, 88.27171, 1.50098),
(@CGUID-2, 37088, 1, -1863.41601, -2890.3789, 84.64671, 1.78);

UPDATE `creature` SET `spawndist`=5 WHERE `guid` BETWEEN @CGUID-2 AND @CGUID-0;
UPDATE `creature` SET `MovementType`=1 WHERE `guid` BETWEEN @CGUID-2 AND @CGUID-0;

-- Barrens Vulture
SET @CGUID := 104786;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID-4 AND @CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 34640, 1, -1904.72204, -3396.2019, 107.34256, 5.21561),
(@CGUID-1, 34640, 1, -1924.30554, -3397.50244, 117.70237, 5.59093),
(@CGUID-2, 34640, 1, -1975.92797, -2946.1416, 130.39692, 1.77207),
(@CGUID-3, 34640, 1, -1963.63378, -2955.24023, 117.88148, 5.35199),
(@CGUID-4, 34640, 1, -1973.55151, -2943.04882, 132.83312, 4.73341);

UPDATE `creature` SET `spawndist`=5 WHERE `guid` BETWEEN @CGUID-4 AND @CGUID-0;
UPDATE `creature` SET `MovementType`=1 WHERE `guid` BETWEEN @CGUID-4 AND @CGUID-0;

-- Adder
SET @CGUID := 104787;
DELETE FROM `creature` WHERE `guid`=@CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 3300, 1, -1808.30041, -2965.87939, 87.052, 3.73663);

UPDATE `creature` SET `spawndist`=5 WHERE `guid`=@CGUID-0;
UPDATE `creature` SET `MovementType`=1 WHERE `guid`=@CGUID-0;

-- Adder
SET @CGUID := 104807;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID-2 AND @CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 61325, 1, -1961.47509, -2926.64721, 93.44142, 5.97378),
(@CGUID-1, 61325, 1, -1823.30676, -2951.55957, 86.8162, 2.09657);

UPDATE `creature` SET `spawndist`=5 WHERE `guid` BETWEEN @CGUID-2 AND @CGUID-0;
UPDATE `creature` SET `MovementType`=1 WHERE `guid` BETWEEN @CGUID-2 AND @CGUID-0;

-- Emerald Boa
SET @CGUID := 104856;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID-3 AND @CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 49725, 1, -1883.11022, -2724.91015, 95.45731, 3.08764),
(@CGUID-1, 49725, 1, -1832.55065, -2767.12646, 86.29451, 5.60612),
(@CGUID-2, 49725, 1, -1947.18554, -2720.21093, 95.02212, 2.41478);

UPDATE `creature` SET `spawndist`=5 WHERE `guid` BETWEEN @CGUID-3 AND @CGUID-0;
UPDATE `creature` SET `MovementType`=1 WHERE `guid` BETWEEN @CGUID-3 AND @CGUID-0;

-- Small Frog
SET @CGUID := 104858;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID-1 AND @CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 13321, 1, -1914.09252, -2765.63745, 93.52925, 1.98514),
(@CGUID-1, 13321, 1, -1844.84753, -2788.39892, 86.46032, 1.61263);

UPDATE `creature` SET `spawndist`=5 WHERE `guid` BETWEEN @CGUID-1 AND @CGUID-0;
UPDATE `creature` SET `MovementType`=1 WHERE `guid` BETWEEN @CGUID-1 AND @CGUID-0;

-- Northwatch Supply Crate
SET @OGUID := 101355;
DELETE FROM `gameobject` WHERE `guid`BETWEEN @OGUID-23 AND @OGUID-0;
DELETE FROM `gameobject` WHERE `id`=202405;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`) VALUES
(@OGUID-0, 202405, 1, -1953.92541, -3160.46875, 92.62018, 2.47836, 0, 0, 0.94551, 0.32556),
(@OGUID-1, 202405, 1, -1931.73437, -3194.22753, 92.67456, 0, 0, 0, 0, 1),
(@OGUID-2, 202405, 1, -1955.375, -3195.6997, 93.4611, 0, 0, 0, 0, 1),
(@OGUID-3, 202405, 1, -1972.0122, -3193.42358, 92.75446, 0, 0, 0, 0, 1),
(@OGUID-4, 202405, 1, -1946.32287, -3176.86108, 92.82822, 0, 0, 0, 0, 1),
(@OGUID-5, 202405, 1, -1980.03649, -3191.28833, 92.07072, 0.92502, 0, 0, 0.44619, 0.89493),
(@OGUID-6, 202405, 1, -2009, -3212.29174, 91.75086, 0.92502, 0, 0, 0, 1),
(@OGUID-7, 202405, 1, -2017.47229, -3219.49145, 91.22251, 0.92502, 0, 0, 0, 1),
(@OGUID-8, 202405, 1, -2043.48962, -3231.23608, 91.8623, 0.50614, 0, 0, 0.25037, 0.96814),
(@OGUID-9, 202405, 1, -1984.47045, -3157.50708, 92.77123, 0, 0, 0, 0, 1),
(@OGUID-10, 202405, 1, -2033.6997, -3195.2395, 91.66945, 2.21656, 0, 0, 0.89493, 0.44619),
(@OGUID-11, 202405, 1, -2041.54516, -3259.19458, 91.80572, 0, 0, 0, 0, 1),
(@OGUID-12, 202405, 1, -2005.47045, -3171.99829, 92.29486, 0, 0, 0, 0, 1),
(@OGUID-13, 202405, 1, -1998.08166, -3165.78637, 93.19645, 0, 0, 0, 0, 1),
(@OGUID-14, 202405, 1, -2006.86462, -3206.05395, 91.74589, 2.23401, 0, 0, 0.89879, 0.43837),
(@OGUID-15, 202405, 1, -2021.276, -3260.96533, 92.60643, 3.14159, 0, 0, -1, 0),
(@OGUID-16, 202405, 1, -2013.16662, -3232.2727, 92.23892, 0, 0, 0, 0, 1),
(@OGUID-17, 202405, 1, -2039.73608, -3211.47753, 91.63974, 0, 0, 0, 0, 1),
(@OGUID-18, 202405, 1, -2031.74304, -3266.5747, 92.04747, 1.98967, 0, 0, 0.83866, 0.54464),
(@OGUID-19, 202405, 1, -2033.6997, -3195.2395, 91.66945, 2.21656, 0, 0, 0.89493, 0.44619),
(@OGUID-20, 202405, 1, -1972.0122, -3193.42358, 92.75446, 0, 0, 0, 0, 1),
(@OGUID-21, 202405, 1, -2006.86462, -3206.05395, 91.74589, 2.23401, 0, 0, 0.89879, 0.43837),
(@OGUID-22, 202405, 1, -2043.48962, -3231.23608, 91.8623, 0.50614, 0, 0, 0.25037, 0.96814),
(@OGUID-23, 202405, 1, -1980.03649, -3191.28833, 92.07072, 0.92502, 0, 0, 0.44619, 0.89493);

UPDATE `gameobject` SET `state`=1 WHERE `guid` BETWEEN @OGUID-23 AND @OGUID-0;

-- Khadgar's Whisker
SET @OGUID := 101358;
DELETE FROM `gameobject` WHERE `guid`BETWEEN @OGUID-2 AND @OGUID-0;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`) VALUES
(@OGUID-0, 2043, 1, -1920.48999, -2757.12988, 93.9606, 0, 0, 0, 0, 1),
(@OGUID-1, 2043, 1, -1842.60412, -2623.7102, 98.18881, 0, 0, 0, 0, 1),
(@OGUID-2, 2043, 1, -2052.74658, -2677.44628, 92.22993, 0, 0, 0, 0, 1);

UPDATE `gameobject` SET `state`=1 WHERE `guid` BETWEEN @OGUID-2 AND @OGUID-0;

-- Kingsblood
SET @OGUID := 101360;
DELETE FROM `gameobject` WHERE `guid`BETWEEN @OGUID-1 AND @OGUID-0;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`) VALUES
(@OGUID-0, 1624, 1, -1973.44104, -3357.51049, 59.25651, 0, 0, 0, 0, 1),
(@OGUID-1, 1624, 1, -1815.80554, -2930.32641, 86.25987, 0, 0, 0, 0, 1);

UPDATE `gameobject` SET `state`=1 WHERE `guid` BETWEEN @OGUID-1 AND @OGUID-0;

-- Iron Deposit
SET @OGUID := 101361;
DELETE FROM `gameobject` WHERE `guid`=@OGUID-0;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`) VALUES
(@OGUID-0, 1735, 1, -1969.4757, -2985.17016, 93.51696, 0, 0, 0, 0, 1);

UPDATE `gameobject` SET `state`=1 WHERE `guid`=@OGUID-0;

-- Gold Vein
SET @OGUID := 101362;
DELETE FROM `gameobject` WHERE `guid`=@OGUID-0;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`) VALUES
(@OGUID-0, 1734, 1, -2004.60766, -3139.32983, 103.69174, 0, 0, 0, 0, 1);

UPDATE `gameobject` SET `state`=1 WHERE `guid`=@OGUID-0;

-- Goldthorn
SET @OGUID := 101363;
DELETE FROM `gameobject` WHERE `guid`=@OGUID-0;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`) VALUES
(@OGUID-0, 2046, 1, -2067.32299, -3177.19799, 122.0634, 0, 0, 0, 0, 1);

UPDATE `gameobject` SET `state`=1 WHERE `guid`=@OGUID-0;
