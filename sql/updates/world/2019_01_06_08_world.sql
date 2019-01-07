-- Add Creature Loot for Quest: 14456 - Rite of Courage - (QuestObjective: 255087)
-- Creature: Bristleback Gun Thief (36708) - Item: Stolen Rifle (49535) 
DELETE FROM creature_loot_template WHERE entry=36708 AND item=49535;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance) VALUES
(36708, 49535, -100);
-- Set lootid in creature_template
UPDATE creature_template SET lootid=36708 WHERE entry=36708;
