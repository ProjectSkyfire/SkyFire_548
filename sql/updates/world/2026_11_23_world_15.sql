-- Loch modan fox loot (issue #1503).
-- Reconstructed creature-specific loot; rates are estimates, not verified retail measurements.
-- Use existing level-range world-drop pools. Preserve all quest-only loot rows.
-- Rare creatures receive one equipment item from a direct equal-chance group.
-- Nested reference pools require group 0; group 1 is only used for a leaf equipment pool.
DELETE FROM `creature_loot_template` WHERE `entry` = 44635 AND `ChanceOrQuestChance` >= 0;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(44635, 3301, 30, 1, 2, 1, 1),
(44635, 7074, 70, 1, 2, 1, 1),
(44635, 24102, 5, 1, 0, -24102, 1),
(44635, 24731, 1, 1, 0, -24731, 1),
(44635, 44009, 0.5, 1, 0, -44009, 1);
-- Complete level 16-17 leather/hide distribution, rounded from Black Bear Patriarch (1189).
DELETE FROM `skinning_loot_template` WHERE `entry` = 44635;
INSERT INTO `skinning_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(44635, 783, 5, 1, 1, 1, 1),
(44635, 2318, 74, 1, 1, 1, 2),
(44635, 2319, 19, 1, 1, 1, 1),
(44635, 4232, 2, 1, 1, 1, 1);
UPDATE `creature_template` SET `lootid` = 44635, `skinloot` = 44635 WHERE `entry` = 44635;

DELETE FROM `creature_loot_template` WHERE `entry` = 45380 AND `ChanceOrQuestChance` >= 0;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(45380, 3301, 30, 1, 2, 1, 1),
(45380, 7074, 70, 1, 2, 1, 1),
(45380, 24076, 100, 1, 1, -24076, 1),
(45380, 24731, 1, 1, 0, -24731, 1),
(45380, 44009, 0.5, 1, 0, -44009, 1);
-- Complete level 16-17 leather/hide distribution, rounded from Black Bear Patriarch (1189).
DELETE FROM `skinning_loot_template` WHERE `entry` = 45380;
INSERT INTO `skinning_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(45380, 783, 5, 1, 1, 1, 1),
(45380, 2318, 74, 1, 1, 1, 2),
(45380, 2319, 19, 1, 1, 1, 1),
(45380, 4232, 2, 1, 1, 1, 1);
UPDATE `creature_template` SET `lootid` = 45380, `skinloot` = 45380 WHERE `entry` = 45380;
