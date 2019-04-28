-- Add Creature Loot for Quest: 14460 - Rite of Honor - (QuestObjective: 256362)
-- Creature: Chief Squealer Thornmantle (36712) - Item: Mane of Thornmantle (50473) 
DELETE FROM creature_loot_template WHERE entry=36712 AND item=50473;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance) VALUES
(36712, 50473, -100);
-- Set lootid in creature_template
UPDATE creature_template SET lootid=36712 WHERE entry=36712;
