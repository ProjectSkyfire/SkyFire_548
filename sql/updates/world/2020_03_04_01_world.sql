UPDATE `creature_template` SET `lootid`=56201 WHERE `entry`=56201;

DELETE FROM `creature_loot_template` WHERE `entry`=56201 AND `item`=75221;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(56201, 75221, -100);
