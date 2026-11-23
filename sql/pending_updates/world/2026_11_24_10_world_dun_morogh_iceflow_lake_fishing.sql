-- Fishing in Iceflow Lake (area 211, in the zone 6457 that holds the gnome start area and Frostmane Hold) gave no loot: the
-- log reports that the area is not defined in `skill_fishing_base_level` and that the loot table 6457 has no records (upstream
-- issue 1405). Skill levels of the five areas and the zone's loot table (26 items, lootmode 1) are as in Legends of Azeroth
-- 5.4.8.

DELETE FROM `skill_fishing_base_level` WHERE `entry` IN (133, 135, 211, 5495, 6457);
INSERT INTO `skill_fishing_base_level` (`entry`, `skill`) VALUES
(133, 10),
(135, 7),
(211, 1),
(5495, -20),
(6457, -20);

DELETE FROM `fishing_loot_template` WHERE `entry` = 6457;
INSERT INTO `fishing_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(6457, 117, 0.97, 1, 1, 1, 1),
(6457, 118, 0.16, 1, 1, 1, 1),
(6457, 159, 0.32, 1, 1, 1, 1),
(6457, 774, 0.16, 1, 1, 1, 1),
(6457, 2589, 1, 1, 1, 1, 2),
(6457, 3300, 0.16, 1, 1, 1, 1),
(6457, 4563, 0.16, 1, 1, 1, 1),
(6457, 5571, 0.16, 1, 1, 1, 1),
(6457, 6289, 15, 1, 1, 1, 1),
(6457, 6291, 23, 1, 1, 1, 1),
(6457, 6292, 0.49, 1, 1, 1, 1),
(6457, 6295, 0.16, 1, 1, 1, 1),
(6457, 8179, 0.16, 1, 1, 1, 1),
(6457, 45190, 12, 1, 1, 1, 1),
(6457, 45194, 13, 1, 1, 1, 1),
(6457, 45195, 0.16, 1, 1, 1, 1),
(6457, 45196, 11, 1, 1, 1, 1),
(6457, 45197, 0.65, 1, 1, 1, 1),
(6457, 45198, 15, 1, 1, 1, 1),
(6457, 45200, 10, 1, 1, 1, 1),
(6457, 45201, 0.32, 1, 1, 1, 1),
(6457, 55973, 0.16, 1, 1, 1, 1),
(6457, 55983, 0.49, 1, 1, 1, 1),
(6457, 56147, 0.16, 1, 1, 1, 1),
(6457, 57058, 0.16, 1, 1, 1, 1),
(6457, 62772, 0.16, 1, 1, 1, 1);
