-- Offer Extinguishing Hope only after the warrior has completed The Rear is Clear.
UPDATE `quest_template`
SET `PrevQuestId`=28823
WHERE `Id`=26391;
