SET @CGUID := 224620;
DELETE FROM `creature` WHERE `guid`=@CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES 
(@CGUID-0, 54608, 860, 1168.2552, 3442.3281, 104.86952, 3.450217962265014648);

DELETE FROM `creature` WHERE `guid`=224621;
