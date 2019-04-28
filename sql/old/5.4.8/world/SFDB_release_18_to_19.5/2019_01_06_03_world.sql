-- Add Gameobject Loot for Quest: 30469 - Repossession - (QuestObjective 252081)
DELETE FROM gameobject_loot_template WHERE entry=41423 AND item=79880;
INSERT INTO gameobject_loot_template (entry, item, ChanceOrQuestChance) VALUES
(41423, 79880, -100);
