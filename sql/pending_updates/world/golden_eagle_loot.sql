-- Golden eagle loot (issue #1504).
-- Reconstructed creature-specific loot; rates are estimates, not verified retail measurements.
-- Use existing level-range world-drop pools. Preserve all quest-only loot rows.
-- Nested reference pools require group 0; group 1 is only used for a leaf equipment pool.
DELETE FROM `creature_loot_template` WHERE `entry` = 44628 AND `ChanceOrQuestChance` >= 0;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(44628, 5115, 27, 1, 0, 1, 1),
(44628, 6889, 63, 1, 0, 1, 1),
(44628, 7096, 65, 1, 0, 1, 1),
(44628, 17056, 8, 1, 0, 1, 1),
(44628, 1210, 0.5, 1, 0, 1, 1),
(44628, 24102, 5, 1, 0, -24102, 1),
(44628, 24731, 1, 1, 0, -24731, 1),
(44628, 44009, 0.5, 1, 0, -44009, 1);
UPDATE `creature_template` SET `lootid` = 44628 WHERE `entry` = 44628;
