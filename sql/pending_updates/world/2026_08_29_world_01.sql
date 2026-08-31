-- Sarkoth / Hana'zua false quest chain requirement (Valley of Trials)
UPDATE `quest_template` SET `PrevQuestId` = 0 WHERE `Id` = 25129;
UPDATE `quest_template` SET `NextQuestId` = 0 WHERE `Id` = 25128;
UPDATE `quest_template` SET `NextQuestIdChain` = 0 WHERE `Id` = 25128;
