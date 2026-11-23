-- Ammen Vale: "What Must Be Done..." (9293) points its automatic next offer at itself (NextQuestIdChain 9293), so
-- "Healing the Lake" (9294) is never offered after it and the chain check of 9293 compares the quest with itself.
-- Cataclysm 4.3.4 data (quest_template.RewardNextQuest) has 9294, and the 5.4.8 data lists 9294 as the next quest.
UPDATE `quest_template` SET `NextQuestIdChain` = 9294 WHERE `Id` = 9293;
