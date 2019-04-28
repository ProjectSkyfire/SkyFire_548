-- GameObjectLoot for Quest: "Stealing is Bad... Re-Stealing is OK"
SET @QITEM := 210768;
SET @GD1 := 41405;
DELETE FROM gameobject_loot_template WHERE entry=@GD1;
INSERT INTO gameobject_loot_template (entry, item, ChanceOrQuestChance) VALUES
(@GD1, @QITEM, -100);
