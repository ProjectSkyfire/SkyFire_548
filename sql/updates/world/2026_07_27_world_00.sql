-- Player aura for Milo's visibility.
DELETE FROM `spell_area` WHERE `spell`=70044;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(70044, 132, 24491, 1, 66, 0);

-- Milo Geartwinge
DELETE FROM `creature_template_addon` WHERE `entry`=37113;
INSERT INTO `creature_template_addon` (`entry`, `auras`) VALUES
(37113, '70045 70042');

SET @CGUID := 171227;
DELETE FROM `creature` WHERE `guid`=@CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 37113, 0, -6245.79, 305.464, 383.936, 2.56563);

-- Milo's Gyro
DELETE FROM `creature_template_addon` WHERE `entry`=37169;
INSERT INTO `creature_template_addon` (`entry`, `auras`) VALUES
(37169, '70045 70042');

SET @CGUID := 171228;
DELETE FROM `creature` WHERE `guid`=@CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 37169, 0, -6243.37, 305.569, 384.311, 2.35619);
