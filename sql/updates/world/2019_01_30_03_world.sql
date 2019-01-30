-- CreatureLoot for Multiple Quests named "A Rough Start"
SET @QITEM := 50222;
SET @WILDMANECAT := 38046;
UPDATE creature_template SET lootid=@WILDMANECAT WHERE entry=@WILDMANECAT;
DELETE FROM creature_loot_template WHERE entry=@WILDMANECAT;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@WILDMANECAT, @QITEM, -100, 1, 0, 1, 1);
