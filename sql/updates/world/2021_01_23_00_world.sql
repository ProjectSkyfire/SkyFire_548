SET @CGUID := 100055;
DELETE FROM `creature` WHERE `guid`=@CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 66292, 0, -7879.77, 1279.755, 358.477, 4.70616);
