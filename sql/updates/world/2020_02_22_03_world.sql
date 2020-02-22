SET @CGUID := 50054;
DELETE FROM `creature` WHERE `guid`=@CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 66292, 870, -664.906, -1483.26, 130.2, 4.70616);
