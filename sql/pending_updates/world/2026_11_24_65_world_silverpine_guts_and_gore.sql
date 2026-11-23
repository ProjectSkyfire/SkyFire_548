-- Guts and Gore (26995) asks for 6 Clean Beast Guts (60742) and nothing dropped them. In Legends of Azeroth 5.4.8 the
-- Worg (1765, level 9 to 10) and the Ferocious Grizzled Bear (1778, level 10 to 11) carry them as a quest drop.
-- The other creatures of that source have a chance below 0.01 percent and are left out.

DELETE FROM `creature_loot_template` WHERE `entry` IN (1765, 1778) AND `item` = 60742;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(1765, 60742, -42.4105, 1, 0, 1, 1),
(1778, 60742, -43.2232, 1, 0, 1, 1);
