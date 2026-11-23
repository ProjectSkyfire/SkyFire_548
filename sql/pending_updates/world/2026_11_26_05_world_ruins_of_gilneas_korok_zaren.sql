-- Ruins of Gilneas: "On Whose Orders?" (27364) needs the Orders from High Command (60977) from Master Sergeant Pietro
-- Zaren, and "Korok the Colossus" (27322) starts from Korok's Second Head (60956) dropped by Korok the Colossus. Neither
-- was spawned, neither had a loot id or the drop, and Zaren was friendly (faction 35, LoA has the 7th Legion faction 123).
-- Based on Legends of Azeroth 5.4.8 (spawns, loot rows and faction). Only the quest drops are added.
UPDATE `creature_template` SET `faction_A`=123, `faction_H`=123, `lootid`=45405 WHERE `entry`=45405;
UPDATE `creature_template` SET `lootid`=45301 WHERE `entry`=45301;

DELETE FROM `creature_loot_template` WHERE `entry`=45405 AND `item`=60977;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(45405, 60977, -100, 1, 0, 1, 1);
DELETE FROM `creature_loot_template` WHERE `entry`=45301 AND `item`=60956;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(45301, 60956, 100, 1, 0, 1, 1);

DELETE FROM `creature` WHERE `guid` IN (9311604, 9311605);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9311604, 45405, 0, 1, 0, 0, 0, 0, -1528.83, 1587.73, 26.6202, 1.95023, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(9311605, 45301, 0, 1, 0, 0, 0, 0, -1091.02, 1828.36, 36.4692, 0.289979, 300, 3, 0, 0, 0, 1, 0, 0, 0);
