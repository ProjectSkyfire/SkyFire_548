-- Proto strider and optimo loot (issue #1506).
-- Reconstructed creature-specific loot; rates are estimates, not verified retail measurements.
-- Use existing level-range world-drop pools. Preserve all quest-only loot rows.
-- Rare creatures receive one equipment item from a direct equal-chance group.
-- Nested reference pools require group 0; group 1 is only used for a leaf equipment pool.
DELETE FROM `creature_loot_template` WHERE `entry` = 44576 AND `ChanceOrQuestChance` >= 0;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(44576, 813, 82, 1, 2, 1, 1),
(44576, 57059, 18, 1, 2, 1, 1),
(44576, 24103, 5, 1, 0, -24103, 1),
(44576, 24731, 1, 1, 0, -24731, 1),
(44576, 44009, 0.5, 1, 0, -44009, 1);
UPDATE `creature_template` SET `lootid` = 44576 WHERE `entry` = 44576;

DELETE FROM `creature_loot_template` WHERE `entry` = 45399 AND `ChanceOrQuestChance` >= 0;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(45399, 813, 82, 1, 2, 1, 1),
(45399, 57059, 18, 1, 2, 1, 1),
(45399, 24077, 100, 1, 1, -24077, 1),
(45399, 24731, 1, 1, 0, -24731, 1),
(45399, 44009, 0.5, 1, 0, -44009, 1);
UPDATE `creature_template` SET `lootid` = 45399 WHERE `entry` = 45399;
