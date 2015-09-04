-- Parsed Creature loot for Creature id : 39004 
DELETE FROM `creature_loot_template` WHERE `Entry` = 39004;
INSERT INTO `creature_loot_template` VALUES ( '39004', '4888', '-60.30136', '1', '0', '1', '1');
INSERT INTO `creature_loot_template` VALUES ( '39004', '5523', '8.7102', '1', '0', '1', '1');
INSERT INTO `creature_loot_template` VALUES ( '39004', '52080', '-100', '1', '0', '1', '1');
UPDATE `creature_template` SET `lootid`= 39004 WHERE `entry`= 39004;

-- Parsed Creature loot for Creature id : 39072 
DELETE FROM `creature_loot_template` WHERE `Entry` = 39072;
INSERT INTO `creature_loot_template` VALUES ( '39072', '159', '11.67247', '1', '0', '1', '1');
INSERT INTO `creature_loot_template` VALUES ( '39072', '5523', '100', '1', '0', '1', '1');
INSERT INTO `creature_loot_template` VALUES ( '39072', '6303', '32.75261', '1', '0', '1', '1');
INSERT INTO `creature_loot_template` VALUES ( '39072', '50018', '-30.68118', '1', '0', '1', '1');
UPDATE `creature_template` SET `lootid`= 39072 WHERE `entry`= 39072;
