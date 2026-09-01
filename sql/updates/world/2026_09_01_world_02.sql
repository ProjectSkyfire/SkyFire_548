-- Thazz'ril's Pick (gameobject 178087, lootId 14980) had no gameobject_loot_template rows
DELETE FROM `gameobject_loot_template` WHERE `entry` = 14980 AND `item` = 16332;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(14980, 16332, 100, 1, 0, 1, 1);
