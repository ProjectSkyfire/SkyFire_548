-- Entry: 49871
-- https://web.archive.org/web/20150222190409/http://www.wowhead.com/npc=49871#drops:mode=
UPDATE `creature_template` SET `mingold`= 2, `maxgold`= 4, `lootid`= 49871 WHERE `entry`= 49871;
DELETE FROM `creature_loot_template` WHERE `entry` = 49871;
INSERT INTO `creature_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`mincountOrRef`,`maxcount`) VALUES
(49871, 62328, 76, 1, 1),
(49871, 3300, 24, 1, 1); 

-- Entry: 49874
-- https://web.archive.org/web/20151019114045/http://www.wowhead.com/npc=49874#drops:mode=
UPDATE `creature_template` SET `mingold`= 2, `maxgold`= 4, `lootid`= 49874, `pickpocketloot`= 49874 WHERE `entry`= 49874;
DELETE FROM `creature_loot_template` WHERE `entry` = 49874;
INSERT INTO `creature_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`mincountOrRef`,`maxcount`) VALUES
(49874, 117, 10, 1, 1),
(49874, 159, 5, 1, 1),
(49874, 828, 1.4, 1, 1),
(49874, 805, 1.2, 1, 1),
(49874, 5571, 1.1, 1, 1),
(49874, 4496, 1.1, 1, 1),
(49874, 5572, 1, 1, 1),
(49874, 2589, 0.3, 1, 2);

DELETE FROM `pickpocketing_loot_template` WHERE `entry` = 49874;
INSERT INTO `pickpocketing_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`mincountOrRef`,`maxcount`) VALUES
(49874, 5367, 1.3, 1, 1),
(49874, 117, 0.4, 1, 1),
(49874, 118, 0.3, 1, 1),
(49874, 4540, 0.3, 1, 1);

-- Entry: 62
-- https://web.archive.org/web/20151019022952/http://www.wowhead.com/npc=62
UPDATE `creature_template` SET `mingold`= 2, `maxgold`= 4, `lootid`= 62 WHERE `entry`= 62;
DELETE FROM `creature_loot_template` WHERE `entry` = 62;
INSERT INTO `creature_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`mincountOrRef`,`maxcount`) VALUES
(62, 56147, 100, 1, 1),
(62, 117, 1.1, 1, 1),
(62, 159, 0.7, 1, 1); 

-- Entry: 50039
-- https://web.archive.org/web/20151019135026/http://www.wowhead.com/npc=50039#drops:mode=
UPDATE `creature_template` SET `mingold`= 2, `maxgold`= 4, `lootid`= 50039, `pickpocketloot`= 50039 WHERE `entry`= 50039;
DELETE FROM `creature_loot_template` WHERE `entry` = 50039;
INSERT INTO `creature_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`mincountOrRef`,`maxcount`) VALUES
(50039, 117, 10, 1, 1),
(50039, 159, 5, 1, 1),
(50039, 5571, 1, 1, 1),
(50039, 4496, 1, 1, 1),
(50039, 5572, 0.9, 1, 1),
(50039, 805, 0.9, 1, 1),
(50039, 828, 0.8, 1, 1),
(50039, 2589, 0.3, 1, 2);

DELETE FROM `pickpocketing_loot_template` WHERE `entry` = 50039;
INSERT INTO `pickpocketing_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`mincountOrRef`,`maxcount`) VALUES
(50039, 5367, 2, 1, 1),
(50039, 117, 1.1, 1, 1),
(50039, 118, 0.5, 1, 1);

-- Entry: 42937
-- https://web.archive.org/web/20170315130812/http://www.wowhead.com/npc=42937#drops:mode=noteworthy
UPDATE `creature_template` SET `mingold`= 2, `maxgold`= 5, `lootid`= 42937, `pickpocketloot`= 42937 WHERE `entry`= 42937;
DELETE FROM `creature_loot_template` WHERE `entry` = 42937;
INSERT INTO `creature_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`mincountOrRef`,`maxcount`) VALUES
(42937, 58361, -100, 1, 1),
(42937, 117, 1.3, 1, 1),
(42937, 159, 0.6, 1, 1),
(42937, 4496, 0.16, 1, 1),
(42937, 5571, 0.14, 1, 1),
(42937, 828, 0.14, 1, 1),
(42937, 5572, 0.12, 1, 1),
(42937, 805, 0.12, 1, 1);

DELETE FROM `pickpocketing_loot_template` WHERE `entry` = 42937;
INSERT INTO `pickpocketing_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`mincountOrRef`,`maxcount`) VALUES
(42937, 5367, 2, 1, 1),
(42937, 117, 1, 1, 1),
(42937, 4540, 0.5, 1, 1),
(42937, 118, 0.4, 1, 1);

-- Entry: 42938
-- https://web.archive.org/web/20180921061852/https://www.wowhead.com/npc=42938/kurtok-the-slayer#drops:mode=noteworthy
UPDATE `creature_template` SET `mingold`= 3, `maxgold`= 6, `lootid`= 42938, `pickpocketloot`= 42938 WHERE `entry`= 42938;
DELETE FROM `creature_loot_template` WHERE `entry` = 42938;
INSERT INTO `creature_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`mincountOrRef`,`maxcount`) VALUES
(42938, 117, 1.9, 1, 1),
(42938, 159, 1, 1, 1);

DELETE FROM `pickpocketing_loot_template` WHERE `entry` = 42938;
INSERT INTO `pickpocketing_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`mincountOrRef`,`maxcount`) VALUES
(42938, 5367, 1, 1, 1);