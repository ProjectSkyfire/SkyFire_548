-- Vashj'ir: quests that follow each other are offered one after the other. The base data had no previous quest for
-- these, so a whole chain was offered at once. Only links that keep every quest reachable for the races that can take
-- it are set; links to a quest of the other faction, to a quest without a giver or ender, or to a quest whose credit
-- nothing grants yet are left out.
UPDATE `quest_template` SET `PrevQuestId` = 25164 WHERE `Id` = 25221;
UPDATE `quest_template` SET `PrevQuestId` = 25222 WHERE `Id` IN (25215, 25216, 25219, 25220, 25360, 25441);
UPDATE `quest_template` SET `PrevQuestId` = 25334 WHERE `Id` = 25164;
UPDATE `quest_template` SET `PrevQuestId` = 25359 WHERE `Id` = 25439;
UPDATE `quest_template` SET `PrevQuestId` = 25371 WHERE `Id` = 27685;
UPDATE `quest_template` SET `PrevQuestId` = 25405 WHERE `Id` IN (25357, 25546);
UPDATE `quest_template` SET `PrevQuestId` = 25440 WHERE `Id` = 25442;
UPDATE `quest_template` SET `PrevQuestId` = 25459 WHERE `Id` = 25638;
UPDATE `quest_template` SET `PrevQuestId` = 25477 WHERE `Id` IN (25377, 25467);
UPDATE `quest_template` SET `PrevQuestId` = 25535 WHERE `Id` = 25537;
UPDATE `quest_template` SET `PrevQuestId` = 25539 WHERE `Id` = 25540;
UPDATE `quest_template` SET `PrevQuestId` = 25540 WHERE `Id` IN (25579, 25580, 25581, 25582);
UPDATE `quest_template` SET `PrevQuestId` = 25546 WHERE `Id` = 25564;
UPDATE `quest_template` SET `PrevQuestId` = 25580 WHERE `Id` = 27393;
UPDATE `quest_template` SET `PrevQuestId` = 25583 WHERE `Id` = 25760;
UPDATE `quest_template` SET `PrevQuestId` = 25587 WHERE `Id` = 25388;
UPDATE `quest_template` SET `PrevQuestId` = 25620 WHERE `Id` = 25658;
UPDATE `quest_template` SET `PrevQuestId` = 25747 WHERE `Id` IN (25749, 25751);
UPDATE `quest_template` SET `PrevQuestId` = 25748 WHERE `Id` = 25752;
UPDATE `quest_template` SET `PrevQuestId` = 25794 WHERE `Id` IN (25812, 25824, 26000, 26007);
UPDATE `quest_template` SET `PrevQuestId` = 25888 WHERE `Id` = 25885;
UPDATE `quest_template` SET `PrevQuestId` = 25893 WHERE `Id` IN (25894, 25895, 25897);
UPDATE `quest_template` SET `PrevQuestId` = 25895 WHERE `Id` = 25898;
UPDATE `quest_template` SET `PrevQuestId` = 25900 WHERE `Id` IN (25907, 25908, 25909);
UPDATE `quest_template` SET `PrevQuestId` = 25917 WHERE `Id` IN (25918, 25920);
UPDATE `quest_template` SET `PrevQuestId` = 25919 WHERE `Id` = 25921;
UPDATE `quest_template` SET `PrevQuestId` = 25987 WHERE `Id` IN (26056, 26070);
UPDATE `quest_template` SET `PrevQuestId` = 26008 WHERE `Id` = 26040;
UPDATE `quest_template` SET `PrevQuestId` = 26015 WHERE `Id` IN (26017, 26018);
UPDATE `quest_template` SET `PrevQuestId` = 26017 WHERE `Id` = 26080;
UPDATE `quest_template` SET `PrevQuestId` = 26018 WHERE `Id` = 26019;
UPDATE `quest_template` SET `PrevQuestId` = 26080 WHERE `Id` IN (25950, 25975, 25977, 25981);
UPDATE `quest_template` SET `PrevQuestId` = 26106 WHERE `Id` = 26014;
UPDATE `quest_template` SET `PrevQuestId` = 26111 WHERE `Id` = 26132;
UPDATE `quest_template` SET `PrevQuestId` = 26140 WHERE `Id` IN (26141, 26142);
UPDATE `quest_template` SET `PrevQuestId` = 26143 WHERE `Id` = 26181;
UPDATE `quest_template` SET `PrevQuestId` = 26219 WHERE `Id` = 26103;
