-- The Watershed Patrol credits are given when its four side quests are handed in, so the side quests can only be taken once Watershed Patrol is active.
-- Based on Legends of Azeroth 5.4.8.
UPDATE `quest_template` SET `PrevQuestId` = -25188 WHERE `Id` IN (25189, 25190, 25193, 25194);
