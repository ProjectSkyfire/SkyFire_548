SET @CGUID := 60857;

DELETE FROM `creature` WHERE `guid`=@CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `phaseid`, `phasegroup`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 63310, 1, 0, 0, -1149.39, -5441.25, 12.0429, 0.0860126);

DELETE FROM `creature_queststarter` WHERE id=37951 AND quest=31159;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
('37951', '31159');

-- Warrior Quest
UPDATE `quest_template` SET `RequiredClasses`='1' WHERE `Id`=24607;
-- Monk Quest
UPDATE `quest_template` SET `RequiredClasses`='512' WHERE `Id`=31159;
