-- Commander nazrim loot (issue #1509).
-- Reconstructed creature-specific loot; rates are estimates, not verified retail measurements.
-- Use existing level-range world-drop pools. Preserve all quest-only loot rows.
-- Nested reference pools require group 0; group 1 is only used for a leaf equipment pool.
DELETE FROM `creature_loot_template` WHERE `entry` = 44197 AND `ChanceOrQuestChance` >= 0;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(44197, 858, 3, 1, 0, 1, 1),
(44197, 1179, 5, 1, 0, 1, 1),
(44197, 2287, 10, 1, 0, 1, 1),
(44197, 2589, 76, 1, 0, 1, 1),
(44197, 4541, 10, 1, 0, 1, 1),
(44197, 2455, 1, 1, 0, 1, 1),
(44197, 24101, 5, 1, 0, -24101, 1),
(44197, 24730, 1, 1, 0, -24730, 1),
(44197, 44009, 0.5, 1, 0, -44009, 1);
DELETE FROM `pickpocketing_loot_template` WHERE `entry` = 44197;
INSERT INTO `pickpocketing_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(44197, 818, 12.0, 1, 0, 1, 1),
(44197, 858, 16.0, 1, 0, 1, 1),
(44197, 2287, 56.0, 1, 0, 1, 1),
(44197, 4541, 24.0, 1, 0, 1, 1);
UPDATE `creature_template` SET `lootid` = 44197, `pickpocketloot` = 44197, `mingold` = 16, `maxgold` = 16 WHERE `entry` = 44197;
