UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=209903;
DELETE FROM `gameobject_loot_template` WHERE `entry`=40520;
INSERT INTO `gameobject_loot_template` VALUES
(40520, 76333, -100, 1, 0, 1, 1);
