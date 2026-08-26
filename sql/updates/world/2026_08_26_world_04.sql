-- Restore openable loot for Coldridge Mountaineer's Pouch (item 57540).
-- Fixes startup warning: item_loot_template entry 57540 used but has no records.

DELETE FROM `item_loot_template` WHERE `entry` = 57540;
INSERT INTO `item_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(57540, 57541, 100, 1, 0, 1, 1),
(57540, 57542, 100, 1, 0, 1, 1),
(57540, 57543, 100, 1, 0, 1, 1),
(57540, 57544, 100, 1, 0, 1, 1);
