-- High Inquisitor Whitemane, normal mode: besides her old items she drops one roll of the Scarlet Monastery dungeon
-- items (reference 88294, which High Commander Durand rolls twice) and a few consumables.
DELETE FROM `creature_loot_template` WHERE `entry` = 3977 AND `item` IN (1707, 1708, 1710, 3827, 4338, 71635, 88294);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(3977, 1707, 1.7, 1, 0, 1, 1),
(3977, 1708, 0.9, 1, 0, 1, 1),
(3977, 1710, 0.5, 1, 0, 1, 1),
(3977, 3827, 0.2, 1, 0, 1, 1),
(3977, 4338, 1.8, 1, 0, 1, 3),
(3977, 71635, 1.2, 1, 0, 1, 1),
(3977, 88294, 100, 1, 0, -88294, 1);
