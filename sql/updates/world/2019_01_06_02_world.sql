-- Add Gameobject Loot for Quest: 30476 - A Gift For Old Hillpaw - (QuestObjective 251957)
DELETE FROM gameobject_loot_template WHERE entry=41461 AND item=80229;
INSERT INTO gameobject_loot_template (entry, item, ChanceOrQuestChance) VALUES
(41461, 80229, -100);
