-- Rageroar Grunt (38658)
UPDATE `creature_template` SET `lootid`=38658, `pickpocketloot`=38658 WHERE `entry`=38658;

DELETE FROM `creature_loot_template` WHERE `entry`=38658;
INSERT INTO `creature_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
(38658,929,1.9,1,0,1,1),
(38658,1205,2.5,1,0,1,1),
(38658,1708,2.6,1,0,1,1),
(38658,1710,11.6279,1,0,1,1),
(38658,2592,15.6,1,0,1,3),
(38658,3770,5.208,1,0,1,1),
(38658,3771,5.2309,1,0,1,1),
(38658,4306,59.8,1,0,1,2),
(38658,4544,9.3023,1,0,1,1),
(38658,5427,27.907,1,0,1,1),
(38658,6661,2.1364,1,0,1,1),
(38658,6663,1.992,1,0,1,1);

DELETE FROM `pickpocketing_loot_template` WHERE `entry`=38658;
INSERT INTO `pickpocketing_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
(38658,1529,0.463,1,0,1,1),
(38658,1710,8.3333,1,0,1,1),
(38658,3771,12.037,1,0,1,1),
(38658,4544,12.037,1,0,1,1),
(38658,5427,31.9444,1,0,1,1),
(38658,16883,48.1481,1,0,1,1);
