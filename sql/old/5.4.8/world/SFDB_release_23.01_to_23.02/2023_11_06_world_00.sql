-- The Basics: Hitting Things (Monk)
UPDATE `quest_template` SET `RequiredClasses`=512, `PrevQuestId`=31159 WHERE `Id`=31158;

-- A Rough Start (Monk)
UPDATE `quest_template` SET `RequiredClasses`=512, `PrevQuestId`=31158 WHERE `Id`=31160;

-- Proving Pit (Monk)
UPDATE `quest_template` SET `RequiredClasses`=512, `PrevQuestId`=31160, `NextQuestIdChain`=31162 WHERE `Id`=31161;

-- The Arts of a Monk
UPDATE `quest_template` SET `ZoneOrSort`=368, `Type`=0, `RequiredClasses`=512, `PrevQuestId`=31161, `NextQuestIdChain`=31163 WHERE `Id`=31162;
DELETE FROM `creature_queststarter` WHERE `quest`=31162;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(63310, 31162);

-- More then Expected (Monk)
UPDATE `quest_template` SET `RequiredClasses`=512, `PrevQuestId`=31162 WHERE `Id`=31163;
