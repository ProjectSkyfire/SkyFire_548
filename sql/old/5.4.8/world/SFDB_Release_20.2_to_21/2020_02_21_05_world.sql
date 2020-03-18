SET @CGUID := 100052;
DELETE FROM `creature` WHERE `guid`=@CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 55789, 0, -8458.65, 353.062, 135.569, 5.40221);
