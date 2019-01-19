-- Parsed Creature loot for Creature id : 56360 
DELETE FROM `pickpocketing_loot_template` WHERE `Entry` = 56360;
UPDATE `creature_template` SET `pickpocketloot`= 56360 WHERE `entry`= 56360;
INSERT INTO `pickpocketing_loot_template` VALUES ( '56360', '117', '0.6968641', '1', '0', '1', '1');
INSERT INTO `pickpocketing_loot_template` VALUES ( '56360', '118', '1.045296', '1', '0', '1', '1');
INSERT INTO `pickpocketing_loot_template` VALUES ( '56360', '4540', '1.219512', '1', '0', '1', '1');
INSERT INTO `pickpocketing_loot_template` VALUES ( '56360', '5367', '0.01045296', '1', '0', '1', '1');
