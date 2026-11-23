-- Mogrosh replacement loot (issue #1511).
-- Reconstructed creature-specific loot; rates are estimates, not verified retail measurements.
-- Use existing level-range world-drop pools. Preserve all quest-only loot rows.
-- Do not import modern-retail Mo'grosh weapon drops: their availability in 5.4.8 is not established.
-- Nested reference pools require group 0; group 1 is only used for a leaf equipment pool.
DELETE FROM `creature_loot_template` WHERE `entry` = 44758 AND `ChanceOrQuestChance` >= 0;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(44758, 858, 3, 1, 0, 1, 1),
(44758, 1179, 5, 1, 0, 1, 1),
(44758, 2287, 10, 1, 0, 1, 1),
(44758, 2455, 1, 1, 0, 1, 1),
(44758, 4541, 10, 1, 0, 1, 1),
(44758, 1210, 0.5, 1, 0, 1, 1),
(44758, 2589, 38, 1, 2, 1, 3),
(44758, 2592, 39, 1, 2, 1, 2),
(44758, 24103, 5, 1, 0, -24103, 1),
(44758, 24731, 1, 1, 0, -24731, 1),
(44758, 44009, 0.5, 1, 0, -44009, 1);
DELETE FROM `pickpocketing_loot_template` WHERE `entry` = 44758;
INSERT INTO `pickpocketing_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(44758, 818, 3.64, 1, 0, 1, 1),
(44758, 858, 25.45, 1, 0, 1, 1),
(44758, 1210, 5.45, 1, 0, 1, 1),
(44758, 2287, 49.09, 1, 0, 1, 1),
(44758, 4541, 20.0, 1, 0, 1, 1);
UPDATE `creature_template` SET `lootid` = 44758, `pickpocketloot` = 44758, `mingold` = 33, `maxgold` = 33 WHERE `entry` = 44758;

DELETE FROM `creature_loot_template` WHERE `entry` = 44760 AND `ChanceOrQuestChance` >= 0;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(44760, 858, 3, 1, 0, 1, 1),
(44760, 1179, 5, 1, 0, 1, 1),
(44760, 2287, 10, 1, 0, 1, 1),
(44760, 2455, 1, 1, 0, 1, 1),
(44760, 4541, 10, 1, 0, 1, 1),
(44760, 1210, 0.5, 1, 0, 1, 1),
(44760, 2589, 38, 1, 2, 1, 3),
(44760, 2592, 39, 1, 2, 1, 2),
(44760, 24103, 5, 1, 0, -24103, 1),
(44760, 24731, 1, 1, 0, -24731, 1),
(44760, 44009, 0.5, 1, 0, -44009, 1);
DELETE FROM `pickpocketing_loot_template` WHERE `entry` = 44760;
INSERT INTO `pickpocketing_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(44760, 818, 8.0, 1, 0, 1, 1),
(44760, 858, 32.0, 1, 0, 1, 1),
(44760, 2287, 44.0, 1, 0, 1, 1),
(44760, 4541, 24.0, 1, 0, 1, 1);
UPDATE `creature_template` SET `lootid` = 44760, `pickpocketloot` = 44760, `mingold` = 34, `maxgold` = 34 WHERE `entry` = 44760;
