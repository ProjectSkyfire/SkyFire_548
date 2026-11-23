-- Bobcat loot (issue #1501).
-- Reconstructed creature-specific loot; rates are estimates, not verified retail measurements.
-- Use existing level-range world-drop pools. Preserve all quest-only loot rows.
-- Nested reference pools require group 0; group 1 is only used for a leaf equipment pool.
DELETE FROM `creature_loot_template` WHERE `entry` = 44188 AND `ChanceOrQuestChance` >= 0;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(44188, 5135, 22, 1, 2, 1, 1),
(44188, 7074, 78, 1, 2, 1, 1),
(44188, 24102, 5, 1, 0, -24102, 1),
(44188, 24731, 1, 1, 0, -24731, 1),
(44188, 44009, 0.5, 1, 0, -44009, 1);
-- Complete level 16-17 leather/hide distribution, rounded from Black Bear Patriarch (1189).
DELETE FROM `skinning_loot_template` WHERE `entry` = 44188;
INSERT INTO `skinning_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(44188, 783, 5, 1, 1, 1, 1),
(44188, 2318, 74, 1, 1, 1, 2),
(44188, 2319, 19, 1, 1, 1, 1),
(44188, 4232, 2, 1, 1, 1, 1);
UPDATE `creature_template` SET `lootid` = 44188, `skinloot` = 44188 WHERE `entry` = 44188;
