-- Add Gameobject Loot for Quest: 30064 - Saving the Sutras - (QuestObjective 251805)
DELETE FROM gameobject_loot_template WHERE item=77432 AND entry=40881;
INSERT INTO gameobject_loot_template (entry, item, ChanceOrQuestChance) VALUES
(40881, 77432, -100);
