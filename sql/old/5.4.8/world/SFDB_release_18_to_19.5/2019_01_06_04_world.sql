-- Add Gameobject Loot for Quest: 30459 - All of the Arrows - (QuestObjective 252921)
DELETE FROM gameobject_loot_template WHERE entry=41400 AND item=79806;
INSERT INTO gameobject_loot_template (entry, item, ChanceOrQuestChance) VALUES
(41400, 79806, -100);
