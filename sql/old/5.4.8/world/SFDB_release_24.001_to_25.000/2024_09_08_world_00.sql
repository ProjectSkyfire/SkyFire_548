-- Stolen Malt Sack
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=210001;

DELETE FROM `gameobject_loot_template` WHERE `entry`=40870;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(40870, 77033, -100);
