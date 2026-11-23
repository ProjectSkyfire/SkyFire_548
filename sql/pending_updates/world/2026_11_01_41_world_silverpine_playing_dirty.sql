-- Playing Dirty: Rabid Worgs and Giant Rabid Bears drop the Diseased Organs while the quest is active.
DELETE FROM `creature_loot_template` WHERE `item` = 60793 AND `entry` IN (1766, 1797);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(1766, 60793, -44, 1, 0, 1, 1),
(1797, 60793, -51, 1, 0, 1, 1);
