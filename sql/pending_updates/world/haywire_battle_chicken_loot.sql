-- Haywire battle chicken loot (issue #1505).
-- Reconstructed creature-specific loot; rates are estimates, not verified retail measurements.
-- Use existing level-range world-drop pools. Preserve all quest-only loot rows.
-- Nested reference pools require group 0; group 1 is only used for a leaf equipment pool.
DELETE FROM `creature_loot_template` WHERE `entry` = 44616 AND `ChanceOrQuestChance` >= 0;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(44616, 813, 83, 1, 2, 1, 1),
(44616, 57059, 17, 1, 2, 1, 1),
(44616, 24103, 5, 1, 0, -24103, 1),
(44616, 24731, 1, 1, 0, -24731, 1),
(44616, 44009, 0.5, 1, 0, -44009, 1);
UPDATE `creature_template` SET `lootid` = 44616 WHERE `entry` = 44616;
