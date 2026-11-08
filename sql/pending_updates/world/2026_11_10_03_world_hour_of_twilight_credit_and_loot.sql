-- Hour of Twilight: the Dungeon Finder entry and the boss loot. Idempotent: every INSERT is preceded by a DELETE of its keys.

DELETE FROM `lfg_dungeon_template` WHERE `dungeonId`=439;
INSERT INTO `lfg_dungeon_template` (`dungeonId`, `position_x`, `position_y`, `position_z`, `orientation`, `requiredItemLevel`) VALUES
(439, 4927.03, 315.03, 100.157, 4.71239, 353);

DELETE FROM `creature_loot_template` WHERE `entry` IN (54590, 54968, 54938);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(54590, 72849, 0, 1, 1, 1, 1),
(54590, 72850, 0, 1, 1, 1, 1),
(54590, 72851, 0, 1, 1, 1, 1),
(54590, 72853, 0, 1, 1, 1, 1),
(54590, 72854, 0, 1, 1, 1, 1),
(54590, 76150, 0, 1, 1, 1, 1),
(54590, 77957, 35, 1, 0, 1, 1),
(54938, 52078, 100, 1, 0, 1, 2),
(54938, 71637, 35, 1, 0, 1, 1),
(54938, 71715, 5, 1, 0, 1, 1),
(54938, 72861, 0, 1, 1, 1, 1),
(54938, 72862, 0, 1, 2, 1, 1),
(54938, 72863, 0, 1, 1, 1, 1),
(54938, 72864, 0, 1, 2, 1, 1),
(54938, 72865, 0, 1, 1, 1, 1),
(54938, 72866, 0, 1, 2, 1, 1),
(54938, 72867, 0, 1, 1, 1, 1),
(54938, 72868, 0, 1, 2, 1, 1),
(54938, 72869, 0, 1, 1, 1, 1),
(54938, 72870, 0, 1, 2, 1, 1),
(54938, 72900, 0, 1, 1, 1, 1),
(54938, 72901, 0, 1, 2, 1, 1),
(54968, 22206, 15, 1, 0, 1, 1),
(54968, 71638, 35, 1, 0, 1, 1),
(54968, 72855, 0, 1, 1, 1, 1),
(54968, 72856, 0, 1, 1, 1, 1),
(54968, 72857, 0, 1, 1, 1, 1),
(54968, 72859, 0, 1, 1, 1, 1),
(54968, 72860, 0, 1, 1, 1, 1),
(54968, 76151, 0, 1, 1, 1, 1),
(54968, 77957, 5, 1, 0, 1, 1);
UPDATE `creature_template` SET `lootid`=`entry` WHERE `entry` IN (54590, 54968, 54938);
