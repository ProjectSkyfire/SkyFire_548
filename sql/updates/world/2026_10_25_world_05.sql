-- Ragefire Chasm: boss tables only held old world drops; add the bosses' own rare items
DELETE FROM `creature_loot_template` WHERE `entry`=11517 AND `item`=68194;
DELETE FROM `creature_loot_template` WHERE `entry`=11519 AND `item`=68195;
DELETE FROM `creature_loot_template` WHERE `entry`=61408 AND `item`=82772;
DELETE FROM `creature_loot_template` WHERE `entry`=61408 AND `item`=82879;
DELETE FROM `creature_loot_template` WHERE `entry`=61408 AND `item`=82880;
DELETE FROM `creature_loot_template` WHERE `entry`=61412 AND `item`=82877;
DELETE FROM `creature_loot_template` WHERE `entry`=61412 AND `item`=82881;
DELETE FROM `creature_loot_template` WHERE `entry`=61412 AND `item`=82882;
DELETE FROM `creature_loot_template` WHERE `entry`=61463 AND `item`=82878;
DELETE FROM `creature_loot_template` WHERE `entry`=61463 AND `item`=82884;
DELETE FROM `creature_loot_template` WHERE `entry`=61463 AND `item`=82885;
DELETE FROM `creature_loot_template` WHERE `entry`=61528 AND `item`=82883;
DELETE FROM `creature_loot_template` WHERE `entry`=61528 AND `item`=82886;
DELETE FROM `creature_loot_template` WHERE `entry`=61528 AND `item`=82888;
INSERT INTO `creature_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
(11517,68194,100.0,1,0,1,1),
(11519,68195,100.0,1,0,1,1),
(61408,82772,0,1,1,1,1),
(61408,82879,0,1,1,1,1),
(61408,82880,0,1,1,1,1),
(61412,82877,0,1,1,1,1),
(61412,82881,0,1,1,1,1),
(61412,82882,0,1,1,1,1),
(61463,82878,0,1,1,1,1),
(61463,82884,0,1,1,1,1),
(61463,82885,0,1,1,1,1),
(61528,82883,0,1,1,1,1),
(61528,82886,0,1,1,1,1),
(61528,82888,0,1,1,1,1);
