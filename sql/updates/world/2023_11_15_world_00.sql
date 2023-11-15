DELETE FROM `creature_queststarter` WHERE `id`=2077 AND `quest`=31168;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(2077, 31168);

UPDATE `quest_template` SET `RequiredClasses`=512, `PrevQuestId`=28715 WHERE `Id`=31168;

SET @CGUID := 200109;
DELETE FROM `creature` WHERE `guid`=@CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 63331, 1, 10524.5, 786.045, 1329.6, 3.05581);

