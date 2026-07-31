-- Orchard Tool
SET @OGUID := 50008;
DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID-8 AND @OGUID-0;
DELETE FROM `gameobject` WHERE `id`=209344;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`) VALUES
(@OGUID-0, 209344, 870, 230.00694, -1608.74658, 76.87658, 2.37364, 0, 0, 0.92718, 0.3746),
(@OGUID-1, 209344, 870, 192.35417, -1559.69274, 91.33008, 5.95157, 0, 0, -0.16504, 0.98628),
(@OGUID-2, 209344, 870, 208.42535, -1676.93225, 69.46959, 0.76794, 0, 0, 0.3746, 0.92718),
(@OGUID-3, 209344, 870, 275.77603, -1624.42016, 73.11734, 5.86430, 0, 0, -0.20791, 0.97814),
(@OGUID-4, 209344, 870, 237.85937, -1666.23266, 71.31127, 5.95157, 0, 0,  -0.16504, 0.98628),
(@OGUID-5, 209344, 870, 291.5816, -1675.0885, 65.80384, 3.42085, 0, 0,  -0.99026, 0.13917),
(@OGUID-6, 209344, 870, 301.32812, -1621.38024, 72.55979, 5.8643, 0, 0,  -0.20791, 0.97814),
(@OGUID-7, 209344, 870, 310.48959, -1575.46008, 74.64648, 5.39306, 0, 0,  -0.43051, 0.90258),
(@OGUID-8, 209344, 870, 341.27777, -1592.413, 72.8801, 5.95157, 0, 0,  -0.16504, 0.98628);

UPDATE `gameobject` SET `state`=1 WHERE `id`=209344;

UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=209344;

-- Orchard Tool
SET @OGUID := 50017;
DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID-8 AND @OGUID-0;
DELETE FROM `gameobject` WHERE `id`=209345;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`) VALUES
(@OGUID-0, 209345, 870, 79.46701, -1603.20837, 89.91703, 2.18166, 0, 0, 0.88701, 0.46174),
(@OGUID-1, 209345, 870, 221.00173, -1557.69616, 89.91703, 2.18166, 0, 0, 0.88701, 0.46174),
(@OGUID-2, 209345, 870, 154.3194, -1659.28125, 76.33884, 2.18166, 0, 0, 0.88701, 0.46174),
(@OGUID-3, 209345, 870, 165.36631, -1553.89233, 95.37915, 0.92502, 0, 0, 0.44619, 0.89493),
(@OGUID-4, 209345, 870, 226.19966, -1638.82287, 74.56347, 5.6025, 0, 0, -0.3338, 0.94264),
(@OGUID-5, 209345, 870, 309.47396, -1645.151, 67.87876, 2.49581, 0, 0, 0.94832, 0.3173),
(@OGUID-6, 209345, 870, 298.43228, -1602.05041, 73.12842, 2.44346, 0, 0, 0.93969, 0.34202),
(@OGUID-7, 209345, 870, 263.82638, -1612.80554, 76.8971, 2.44346, 0, 0, 0.93969, 0.34202),
(@OGUID-8, 209345, 870, 336.24478, -1625.25695, 70.52512, 3.87463, 0, 0, -0.93358, 0.35836);

UPDATE `gameobject` SET `state`=1 WHERE `id`=209345;

UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=209345;

-- Orchard-Supplied Hardware
UPDATE `quest_template` SET `RequestItemsText`='Were there any tools left?' WHERE `Id`=29580;
UPDATE `quest_template` SET `OfferRewardText`='There are no hunters among us, but we\'ll fight to protect our loved ones.' WHERE `Id`=29580;
