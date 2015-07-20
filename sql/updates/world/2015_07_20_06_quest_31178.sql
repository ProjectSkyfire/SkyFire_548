SET @CGUID := 4133;
SET @ENTRY := 63353;
SET @QITEM := 85229;
SET @QUEST := 31178;

DELETE FROM `creature_loot_template` WHERE `entry`=@ENTRY AND `item`=@QITEM;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `mincountOrRef`, `maxcount`) VALUES
(@ENTRY, @QITEM, -100, 10, 15);

UPDATE `creature_template` SET `lootid`=@ENTRY WHERE `entry`=@ENTRY;

DELETE FROM `creature` WHERE `id`=@ENTRY;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+31;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(@CGUID+0, @ENTRY, 870, 1, 1, -485.6127, 4860.229, -36.99725, 4.193093, 120, 5, 1), -- 63353 (Area: 6404) (possible waypoints or random movement)
(@CGUID+1, @ENTRY, 870, 1, 1, -567.5221, 4880.27, -36.19179, 3.002433, 120, 5, 1), -- 63353 (Area: 6404) (possible waypoints or random movement)
(@CGUID+2, @ENTRY, 870, 1, 1, -590.2773, 4885.455, -32.77652, 2.102333, 120, 5, 1), -- 63353 (Area: 6404) (possible waypoints or random movement)
(@CGUID+3, @ENTRY, 870, 1, 1, -615.5737, 4759.131, -14.2982, 0.2018892, 120, 5, 1), -- 63353 (Area: 6404) (possible waypoints or random movement)
(@CGUID+4, @ENTRY, 870, 1, 1, -542.2936, 4961.864, -36.90736, 5.994681, 120, 0, 0), -- 63353 (Area: 6404)
(@CGUID+5, @ENTRY, 870, 1, 1, -657.2409, 4893.39, -36.46717, 5.163762, 120, 0, 0), -- 63353 (Area: 6404)
(@CGUID+6, @ENTRY, 870, 1, 1, -647.1247, 4908.393, -40.43855, 5.994164, 120, 0, 0), -- 63353 (Area: 6404)
(@CGUID+7, @ENTRY, 870, 1, 1, -663.9572, 4782.906, -14.04746, 1.723308, 120, 5, 1), -- 63353 (Area: 6404) (possible waypoints or random movement)
(@CGUID+8, @ENTRY, 870, 1, 1, -710.4708, 4805.142, -17.78273, 1.621698, 120, 5, 1), -- 63353 (Area: 6404) (possible waypoints or random movement)
(@CGUID+9, @ENTRY, 870, 1, 1, -550.0751, 4717.668, -12.4592, 2.898493, 120, 5, 1), -- 63353 (Area: 6404) (possible waypoints or random movement)
(@CGUID+10, @ENTRY, 870, 1, 1, -718.3448, 4669.598, -17.71036, 3.137974, 120, 5, 1), -- 63353 (Area: 6404) (possible waypoints or random movement)
(@CGUID+11, @ENTRY, 870, 1, 1, -670.6005, 4620.181, -15.00905, 2.694242, 120, 5, 1), -- 63353 (Area: 6404) (possible waypoints or random movement)
(@CGUID+12, @ENTRY, 870, 1, 1, -716.7133, 4745.667, -17.4811, 5.31281, 120, 5, 1), -- 63353 (Area: 6404) (possible waypoints or random movement)
(@CGUID+13, @ENTRY, 870, 1, 1, -688.6104, 4598.229, -18.29548, 0.5840777, 120, 5, 1), -- 63353 (Area: 6404) (possible waypoints or random movement)
(@CGUID+14, @ENTRY, 870, 1, 1, -764.9134, 4671.504, -17.60971, 0.6525434, 120, 5, 1), -- 63353 (Area: 6404) (possible waypoints or random movement)
(@CGUID+15, @ENTRY, 870, 1, 1, -734.6917, 4745.625, -13.73545, 4.017858, 120, 5, 1), -- 63353 (Area: 6404)
(@CGUID+16, @ENTRY, 870, 1, 1, -722.9973, 4851.336, -37.8636, 3.745441, 120, 0, 0), -- 63353 (Area: 6404)
(@CGUID+17, @ENTRY, 870, 1, 1, -799.9352, 4655.919, -15.38003, 3.494116, 120, 5, 1), -- 63353 (Area: 6404) (possible waypoints or random movement)
(@CGUID+18, @ENTRY, 870, 1, 1, -767.7728, 4589.348, -17.33992, 1.069766, 120, 5, 1), -- 63353 (Area: 6404) (possible waypoints or random movement)
(@CGUID+19, @ENTRY, 870, 1, 1, -833.5236, 4635.002, -16.95664, 0.1904634, 120, 5, 1), -- 63353 (Area: 6404) (possible waypoints or random movement)
(@CGUID+20, @ENTRY, 870, 1, 1, -766.0928, 4711.553, -14.90228, 2.284564, 120, 0, 0), -- 63353 (Area: 6404
(@CGUID+21, @ENTRY, 870, 1, 1, -829.175, 4749.004, -16.81289, 5.930405, 120, 0, 0), -- 63353 (Area: 6404)
(@CGUID+22, @ENTRY, 870, 1, 1, -829.1957, 4737.711, -14.84352, 5.996852, 120, 0, 0), -- 63353 (Area: 6404)
(@CGUID+23, @ENTRY, 870, 1, 1, -794.3264, 4812.469, -18.0089, 0.7196705, 120, 0, 0), -- 63353 (Area: 6404)
(@CGUID+24, @ENTRY, 870, 1, 1, -871.3889, 4746.918, -15.68197, 0.7526063, 120, 0, 0), -- 63353 (Area: 6404)
(@CGUID+25, @ENTRY, 870, 1, 1, -877.4247, 4687.533, -19.03353, 0.1055897, 120, 5, 1), -- 63353 (Area: 6404) (possible waypoints or random movement)
(@CGUID+26, @ENTRY, 870, 1, 1, -610.9305, 4739.741, -14.2982, 4.300875, 120, 5, 1), -- 63353 (Area: 6404) (possible waypoints or random movement)
(@CGUID+27, @ENTRY, 870, 1, 1, -698.8279, 4685.507, -17.71036, 1.332726, 120, 0, 0), -- 63353 (Area: 6404)
(@CGUID+28, @ENTRY, 870, 1, 1, -649.1629, 4616.548, -15.00905, 3.25264, 120, 0, 0), -- 63353 (Area: 6404)
(@CGUID+29, @ENTRY, 870, 1, 1, -915.1661, 4660.775, -7.967586, 5.81081, 120, 0, 0), -- 63353 (Area: 6404)
(@CGUID+30, @ENTRY, 870, 1, 1, -1703.61, 4211.761, -10.12715, 4.55892, 120, 0, 0), -- 63353 (Area: 6444)
(@CGUID+31, @ENTRY, 870, 1, 1, -1583.649, 4207.795, -39.23859, 5.79342, 120, 0, 0); -- 63353 (Area: 6444)

DELETE FROM `creature_addon` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+31;

DELETE FROM `quest_poi_points` WHERE `QuestID`=@QUEST;
INSERT INTO `quest_poi_points` (`QuestID`, `Id`, `Idx`, `X`, `Y`) VALUES
(@QUEST, 0, 0, -378, 4764), -- 31178 - 0 - 0
(@QUEST, 1, 0, 121, 3225), -- 31178 - 1 - 1
(@QUEST, 2, 0, -785, 4599), -- 31178 - 2 - 2
(@QUEST, 2, 1, -700, 4614), -- 31178 - 2 - 2
(@QUEST, 2, 2, -560, 4679), -- 31178 - 2 - 2
(@QUEST, 2, 3, -519, 4880), -- 31178 - 2 - 2
(@QUEST, 2, 4, -554, 4924), -- 31178 - 2 - 2
(@QUEST, 2, 5, -624, 4980), -- 31178 - 2 - 2
(@QUEST, 2, 6, -989, 4990), -- 31178 - 2 - 2
(@QUEST, 2, 7, -995, 4773), -- 31178 - 2 - 2
(@QUEST, 2, 8, -984, 4708), -- 31178 - 2 - 2
(@QUEST, 2, 9, -925, 4643); -- 31178 - 2 - 2

DELETE FROM `quest_poi` WHERE `QuestID`=@QUEST;
INSERT INTO `quest_poi` (`QuestID`, `Id`,  `ObjIndex`, `MapID`, `WorldMapAreaId`, `FloorID`, `unk3`, `unk4`) VALUES
(@QUEST, 0, -1, 870, 858, 0, 0, 1), -- 31178 - 0
(@QUEST, 1, -1, 870, 858, 0, 0, 7), -- 31178 - 1
(@QUEST, 2, 0, 870, 858, 0, 0, 0); -- 31178 - 2

UPDATE `quest_template` SET `OfferRewardText`='Yes, this will do.', `RequestItemsText`='By drinking the blood of your enemies, you acquire their strength.' WHERE `Id`=@QUEST;
