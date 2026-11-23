-- Well of Eternity: the Dungeon Finder entry and the boss loot. Idempotent: every INSERT is preceded by a DELETE of its keys.

DELETE FROM `lfg_dungeon_template` WHERE `dungeonId`=437;
INSERT INTO `lfg_dungeon_template` (`dungeonId`, `position_x`, `position_y`, `position_z`, `orientation`, `requiredItemLevel`) VALUES
(437, 3238.55, -4998.39, 194.093, 2.35619, 353);

DELETE FROM `creature_loot_template` WHERE `entry`=55085;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(55085, 72827, 0, 1, 1, 1, 1),
(55085, 72828, 0, 1, 1, 1, 1),
(55085, 72829, 0, 1, 1, 1, 1),
(55085, 72830, 0, 1, 1, 1, 1),
(55085, 72831, 0, 1, 1, 1, 1),
(55085, 72832, 0, 1, 1, 1, 1);
UPDATE `creature_template` SET `lootid`=55085 WHERE `entry`=55085;

DELETE FROM `gameobject_loot_template` WHERE `entry` IN (209541, 210025);
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(209541, 52078, 100, 1, 0, 1, 2),
(209541, 71715, 5, 1, 0, 1, 1),
(209541, 72839, 0, 1, 1, 1, 1),
(209541, 72840, 0, 1, 2, 1, 1),
(209541, 72841, 0, 1, 1, 1, 1),
(209541, 72842, 0, 1, 2, 1, 1),
(209541, 72843, 0, 1, 1, 1, 1),
(209541, 72844, 0, 1, 2, 1, 1),
(209541, 72845, 0, 1, 1, 1, 1),
(209541, 72846, 0, 1, 2, 1, 1),
(209541, 72847, 0, 1, 1, 1, 1),
(209541, 72848, 0, 1, 2, 1, 1),
(209541, 72898, 0, 1, 1, 1, 1),
(209541, 72899, 0, 1, 2, 1, 1),
(210025, 22206, 15, 1, 0, 1, 1),
(210025, 71635, 30, 1, 0, 1, 1),
(210025, 72833, 0, 1, 1, 1, 1),
(210025, 72834, 0, 1, 1, 1, 1),
(210025, 72835, 0, 1, 1, 1, 1),
(210025, 72836, 0, 1, 1, 1, 1),
(210025, 72837, 0, 1, 1, 1, 1),
(210025, 72838, 0, 1, 1, 1, 1);
