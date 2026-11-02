-- Silty Oozeling
UPDATE `creature_template` SET `lootid`=41145 WHERE `entry`=41145;

DELETE FROM `creature_loot_template` WHERE `entry`=41145;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES 
(41145, 55234, -10, 1, 0, 1, 1),
(41145, 62771, 35, 1, 0, 1, 1),
(41145, 3673, 35, 1, 0, 1, 1),
(41145, 3670, 16, 1, 0, 1, 1),
(41145, 3674, 14, 1, 0, 1, 1);

-- Slavering Ooze
UPDATE `creature_template` SET `lootid`=41147 WHERE `entry`=41147;

DELETE FROM `creature_loot_template` WHERE `entry`=41147;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES 
(41147, 62771, 42, 1, 0, 1, 1),
(41147, 3673, 32, 1, 0, 1, 1),
(41147, 3670, 16, 1, 0, 1, 1),
(41147, 3674, 9, 1, 0, 1, 1);
