-- Add Creature Loot for Quest: 32318 - Regeneration Takes Time - (QuestObjective 269617)
DELETE FROM creature_loot_template WHERE item=92499 AND entry IN (68026, 68027);
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance) VALUES
(68026, 92499, -100),
(68027, 92499, -100);
