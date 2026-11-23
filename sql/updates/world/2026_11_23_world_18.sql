-- Twilight encroacher loot (issue #1510).
-- Reconstructed creature-specific loot; rates are estimates, not verified retail measurements.
-- Use existing level-range world-drop pools. Preserve all quest-only loot rows.
-- Nested reference pools require group 0; group 1 is only used for a leaf equipment pool.
DELETE FROM `creature_loot_template` WHERE `entry` = 44729 AND `ChanceOrQuestChance` >= 0;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(44729, 414, 9, 1, 0, 1, 1),
(44729, 858, 3, 1, 0, 1, 1),
(44729, 1179, 5, 1, 0, 1, 1),
(44729, 2455, 1, 1, 0, 1, 1),
(44729, 1210, 0.5, 1, 0, 1, 1),
(44729, 2589, 40, 1, 2, 1, 3),
(44729, 2592, 39, 1, 2, 1, 2),
(44729, 24103, 5, 1, 0, -24103, 1),
(44729, 24731, 1, 1, 0, -24731, 1),
(44729, 44009, 0.5, 1, 0, -44009, 1);
DELETE FROM `pickpocketing_loot_template` WHERE `entry` = 44729;
INSERT INTO `pickpocketing_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(44729, 414, 25.0, 1, 0, 1, 1),
(44729, 4537, 10.71, 1, 0, 1, 1),
(44729, 4541, 28.57, 1, 0, 1, 1),
(44729, 5368, 39.29, 1, 0, 1, 1);
UPDATE `creature_template` SET `lootid` = 44729, `pickpocketloot` = 44729, `mingold` = 103, `maxgold` = 103 WHERE `entry` = 44729;
