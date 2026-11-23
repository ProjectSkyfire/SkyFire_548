-- Mother Smolderweb's Smolderweb Egg and Quartermaster Zigris' Worg Carrier are in the dungeon journal but in no
-- loot table.
DELETE FROM `creature_loot_template` WHERE (`entry` = 10596 AND `item` = 68673) OR (`entry` = 9736 AND `item` = 12264);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(10596, 68673, 56.8975, 1, 0, 1, 1),
(9736, 12264, 50, 1, 0, 1, 1);
