UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=212318;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=212319;
DELETE FROM `gameobject_loot_template` WHERE `entry`=42556;
INSERT INTO `gameobject_loot_template` VALUES
(42556, 83130, -100, 1, 0, 1, 2);
