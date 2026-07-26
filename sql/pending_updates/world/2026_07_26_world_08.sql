-- Lockdown in Anvilmar requires reward of Give 'em What-For
UPDATE `quest_template` SET `PrevQuestId`=24470 WHERE `Id`=24473;

-- Lockdown in Anvilmar Push First Things First: We're Gonna Need Some Beer
UPDATE `quest_template` SET `NextQuestIdChain`=24474 WHERE `Id`=24473;

-- Forgotten Dwarven Artifact
SET @OGUID := 101331;
DELETE FROM `gameobject` WHERE `id`=201608;
DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID-17 AND @OGUID-0;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation1`, `rotation2`, `rotation3`) VALUES
(@OGUID-0, 201608, 0, -6102.98, 258.332, 403.977, 1.309, 0, 0.608763, 0.793352),
(@OGUID-1, 201608, 0, -6150.86, 257.252, 391.48, 1.309, 0, 0.608763, 0.793352),
(@OGUID-2, 201608, 0, -6183.74, 334.217, 403.852, 1.309, 0, 0.608763, 0.793352),
(@OGUID-3, 201608, 0, -6162.12, 456.379, 392.468, 3.45576, 0, 0.987688, -0.156438),
(@OGUID-4, 201608, 0, -6209.38, 618.205, 395.131, 1.309, 0, 0.608763, 0.793352),
(@OGUID-5, 201608, 0, -6137.58, 593.753, 390.729, 1.309, 0, 0.608763, 0.793352),
(@OGUID-6, 201608, 0, -6152.58, 601.82, 386.766, 4.06662, 0, 0.894934, -0.446199),
(@OGUID-7, 201608, 0, -6134.71, 808.967, 399.927, 1.309, 0, 0.608763, 0.793352),
(@OGUID-8, 201608, 0, -6152.56, 664.191, 389.408, 1.309, 0, 0.608763, 0.793352),
(@OGUID-9, 201608, 0, -6155.35, 645.653, 386.634, 2.84488, 0, 0.989015, 0.147813),
(@OGUID-10, 201608, 0, -6162.45, 680.719, 389.47, 1.309, 0, 0.608763, 0.793352),
(@OGUID-11, 201608, 0, -6193.4, 705.288, 387.297, 1.309, 0, 0.608763, 0.793352),
(@OGUID-12, 201608, 0, -6176.87, 691.719, 386.228, 1.309, 0, 0.608763, 0.793352),
(@OGUID-13, 201608, 0, -6201.45, 720.903, 386.645, 1.309, 0, 0.608763, 0.793352),
(@OGUID-14, 201608, 0, -6184.74, 760.061, 388.003, 1.309, 0, 0.608763, 0.793352),
(@OGUID-15, 201608, 0, -6178.01, 755.175, 388.938, 1.309, 0, 0.608763, 0.793352),
(@OGUID-16, 201608, 0, -6155.05, 776.575, 387.995, 1.309, 0, 0.608763, 0.793352),
(@OGUID-17, 201608, 0, -6221.27, 756.88, 390.07, 1.309, 0, 0.608763, 0.793352);

-- Whitebeard Needs Ye requires reward of Make Hay While the Sun Shines
UPDATE `quest_template` SET `PrevQuestId`=24486 WHERE `Id`=24487;

-- Ice and Fire requires reward of Whitebeard Needs Ye
UPDATE `quest_template` SET `PrevQuestId`=24487 WHERE `Id`=218;
