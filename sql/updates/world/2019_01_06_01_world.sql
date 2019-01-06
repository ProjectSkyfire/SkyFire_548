-- Add Gameobject Loot for Quest: 30479 - A Gift For Gina - (QuestObjective 251943)
DELETE FROM gameobject_loot_template WHERE entry=41463 AND item=80231;
INSERT INTO gameobject_loot_template (entry, item, ChanceOrQuestChance) VALUES
(41463, 80231, -100);
