-- Add Gameobject Loot for Object: Crate of Candles (1586) - Item: Candle of Beckoning (3080) 
DELETE FROM gameobject_loot_template WHERE entry=28239 AND item=3080;
INSERT INTO gameobject_loot_template (entry, item, ChanceOrQuestChance) VALUES
(28239, 3080, -100);
