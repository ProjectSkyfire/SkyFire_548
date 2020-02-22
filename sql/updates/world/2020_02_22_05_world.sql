SET @CGUID := 50055;
DELETE FROM `creature` WHERE `guid`=@CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 59022, 870, -187.585, -2334.35, 35.8858, 2.25154);
