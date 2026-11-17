-- The Skybreaker (13225) and Orgrim's Hammer (13224) could be taken without finishing The Crusaders' Pinnacle (13157),
-- the quest that leads to the people who send you to the gunships.
UPDATE `quest_template` SET `PrevQuestId` = 13157 WHERE `Id` IN (13224, 13225);
