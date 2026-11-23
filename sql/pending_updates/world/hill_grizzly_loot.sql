-- Hill grizzly loot (issue #1502).
-- Reconstructed creature-specific loot; rates are estimates, not verified retail measurements.
-- Use existing level-range world-drop pools. Preserve all quest-only loot rows.
-- Nested reference pools require group 0; group 1 is only used for a leaf equipment pool.
DELETE FROM `creature_loot_template` WHERE `entry` = 44638 AND `ChanceOrQuestChance` >= 0;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(44638, 3173, 40, 1, 0, 1, 1),
(44638, 3170, 14, 1, 2, 1, 1),
(44638, 4865, 36, 1, 2, 1, 1),
(44638, 7073, 36, 1, 2, 1, 1),
(44638, 11407, 14, 1, 2, 1, 1),
(44638, 24102, 5, 1, 0, -24102, 1),
(44638, 24731, 1, 1, 0, -24731, 1),
(44638, 44009, 0.5, 1, 0, -44009, 1);
-- Complete level 16-17 leather/hide distribution, rounded from Black Bear Patriarch (1189).
DELETE FROM `skinning_loot_template` WHERE `entry` = 44638;
INSERT INTO `skinning_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(44638, 783, 5, 1, 1, 1, 1),
(44638, 2318, 74, 1, 1, 1, 2),
(44638, 2319, 19, 1, 1, 1, 1),
(44638, 4232, 2, 1, 1, 1, 1);
UPDATE `creature_template` SET `lootid` = 44638, `skinloot` = 44638 WHERE `entry` = 44638;
