SET @CGUID := 100045;
DELETE FROM `creature` WHERE `guid`=8189276;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 63258, 0, 1, 0, 0, 0, 0, -8907.55, -212.017, 82.1139, 2.83058, 300, 0, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `creature_queststarter` WHERE `quest`=31141;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(197, 31141);

UPDATE `quest_template` SET `RequiredClasses`=512, `PrevQuestId`=31140 WHERE `Id`=31141;

