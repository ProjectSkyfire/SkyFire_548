-- Offer Extinguishing Hope once Blackrock Invasion is accepted.
-- Blackrock Invasion is unlocked by every class's The Rear is Clear via NextQuestId,
-- so a negative PrevQuestId on Invasion works for all classes (not only warrior 28823).
UPDATE `quest_template`
SET `PrevQuestId`=-26389
WHERE `Id`=26391;
