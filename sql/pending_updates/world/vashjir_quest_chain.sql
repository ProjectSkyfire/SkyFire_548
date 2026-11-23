-- Vashj'ir: "Can't Start a Fire Without a Spark" (25602) required "Raw Materials" (25384), a quest that has no giver and no
-- ender in any data, so Captain Samir could never offer it. The 5.4.8 data makes it follow "Ain't Too Proud to Beg" (25598).
UPDATE `quest_template` SET `PrevQuestId` = 25598 WHERE `Id` = 25602;
