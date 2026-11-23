-- Tirisfal Glades: Captain Perrine (1662) never dropped the Urgent Scarlet Memorandum (52077) for players on its quest.
-- The Cataclysm Preservation Project 4.3.4 drops it at 100% for them.
DELETE FROM `creature_loot_template` WHERE `entry` = 1662 AND `item` = 52077;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(1662, 52077, -100, 1, 0, 1, 1);
