-- Vortex Pinnacle creatures without a loot table get the loot of The Cataclysm Preservation Project 4.3.4, the
-- same source as the other 5-player dungeon creatures. Based on The Cataclysm Preservation Project 4.3.4.

DELETE FROM `creature_loot_template` WHERE `entry` IN (
    45477, 45912, 45915, 45917, 45919, 45922, 45924, 45926, 45928, 45930, 45935
);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(45477, 52328, 37.3908, 1, 0, 2, 6),
(45477, 52506, -100, 1, 0, 1, 1),
(45477, 55854, 3.0219, 1, 0, 1, 1),
(45477, 55855, 3.0586, 1, 0, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(45912, 52328, 37.0803, 1, 0, 2, 6),
(45912, 52506, -100, 1, 0, 1, 1),
(45912, 55854, 3.1419, 1, 0, 1, 1),
(45912, 55855, 3.1477, 1, 0, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(45915, 52328, 35.6566, 1, 0, 2, 6),
(45915, 52506, -100, 1, 0, 1, 1),
(45915, 55854, 3.1151, 1, 0, 1, 1),
(45915, 55855, 2.814, 1, 0, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(45917, 52328, 38.5905, 1, 0, 2, 6),
(45917, 55854, 3.1912, 1, 0, 1, 1),
(45917, 55855, 3.4428, 1, 0, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(45919, 52328, 36.3036, 1, 0, 2, 6),
(45919, 55854, 3.6992, 1, 0, 1, 1),
(45919, 55855, 3.4181, 1, 0, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(45922, 52328, 36.9819, 1, 0, 2, 6),
(45922, 55854, 3.2228, 1, 0, 1, 1),
(45922, 55855, 3.2635, 1, 0, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(45924, 52328, 39.3609, 1, 0, 2, 6),
(45924, 55854, 3.1598, 1, 0, 1, 1),
(45924, 55855, 3.1748, 1, 0, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(45926, 53010, 48.6732, 1, 0, 1, 6),
(45926, 55854, 2.1453, 1, 0, 1, 1),
(45926, 68197, 5.2526, 1, 0, 1, 2),
(45926, 68198, 14.1182, 1, 0, 1, 2);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(45928, 53010, 48.9786, 1, 0, 1, 6),
(45928, 55854, 2.0121, 1, 0, 1, 1),
(45928, 68197, 5.0513, 1, 0, 1, 2),
(45928, 68198, 14.4225, 1, 0, 1, 2);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(45930, 53010, 48.1372, 1, 0, 1, 11),
(45930, 68197, 5.0953, 1, 0, 1, 2),
(45930, 68198, 14.2217, 1, 0, 1, 2);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(45935, 53010, 47.3818, 1, 0, 1, 6),
(45935, 55854, 2.1002, 1, 0, 1, 1),
(45935, 68197, 4.925, 1, 0, 1, 2),
(45935, 68198, 14.0589, 1, 0, 1, 2);
UPDATE `creature_template` SET `lootid` = `entry` WHERE `entry` IN (
    45477, 45912, 45915, 45917, 45919, 45922, 45924, 45926, 45928, 45930, 45935
) AND `lootid` = 0;
