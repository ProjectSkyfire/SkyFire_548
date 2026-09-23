-- Ashes in Ashenvale (26408): require The Battle for Darkshore (13897) before accept
UPDATE `quest_template` SET `PrevQuestId`=13897 WHERE `Id`=26408;
