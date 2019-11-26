-- NPC ID : 63367 Name : Brewmaster Boof
UPDATE `creature_template` SET `minlevel`='82', `maxlevel`='90' WHERE `entry`=63367;
UPDATE `creature_template` SET `npcflag`='643' WHERE `entry`=63367;

-- NPC ID : 64101 Name : Taijing the Zyklon
UPDATE `creature_template` SET `minlevel`='90', `maxlevel`='90' WHERE `entry`=64101;
UPDATE `creature_template` SET `npcflag`='643' WHERE `entry`=64101;

-- NPC ID : 72007 Name : Master Kukuru
UPDATE `creature_template` SET `minlevel`='90', `maxlevel`='90' WHERE `entry`=72007;
UPDATE `creature_template` SET `npcflag`='2176' WHERE `entry`=72007;

-- NPC ID : 72993 Name : Lao-Chen the Iron Belly
UPDATE `creature_template` SET `minlevel`='90', `maxlevel`='90' WHERE `entry`=72993;
UPDATE `creature_template` SET `npcflag`='643' WHERE `entry`=72993;

-- NPC ID : 73004 Name : Brewmaster Blanche
UPDATE `creature_template` SET `minlevel`='90', `maxlevel`='90' WHERE `entry`=73004;
UPDATE `creature_template` SET `npcflag`='643' WHERE `entry`=73004;

-- NPC ID : 73010 Name : Brother Rabbitsfoot
UPDATE `creature_template` SET `minlevel`='88', `maxlevel`='88' WHERE `entry`=73010;
UPDATE `creature_template` SET `npcflag`='643' WHERE `entry`=73010;

-- NPC ID : 73305 Name : Mistweaver Ai
UPDATE `creature_template` SET `minlevel`='89', `maxlevel`='90' WHERE `entry`=73305;
UPDATE `creature_template` SET `npcflag`='643' WHERE `entry`=73305;

-- NPC ID : 73305 Name : Brother Noodle 
UPDATE `creature_template` SET `minlevel`='90', `maxlevel`='90' WHERE `entry`=73812;
UPDATE `creature_template` SET `npcflag`='649' WHERE `entry`=73812;

-- NPC ID : 73819 Name : Ku-Mo
UPDATE `creature_template` SET `minlevel`='89', `maxlevel`='89' WHERE `entry`=73819;
UPDATE `creature_template` SET `npcflag`='128' WHERE `entry`=73819;

-- Vendor Stuff

-- Vendor Taijing the Zyklon
DELETE FROM `npc_vendor` WHERE `entry`=64101;
INSERT INTO `npc_vendor` VALUES (64101, 0, 81414, 0, 0, 0, 1),
(64101, 0, 81415, 0, 0, 0, 1),
(64101, 0, 74636, 0, 0, 0, 1),
(64101, 0, 81406, 0, 0, 0, 1),
(64101, 0, 81407, 0, 0, 0, 1);

-- Vendor Master Kukuru
DELETE FROM `npc_vendor` WHERE `entry`=72007;
INSERT INTO `npc_vendor` VALUES (72007, 0, 101538, 0, 0, 5211, 1);

-- Vendor Lao-Chen the Iron Belly
DELETE FROM `npc_vendor` WHERE `entry`=72993;
INSERT INTO `npc_vendor` VALUES (72993, 0, 81415, 0, 0, 0, 1),
(72993, 0, 105706, 0, 0, 0, 1),
(72993, 0, 81407, 0, 0, 0, 1);

-- Vendor Brewmaster Blanche
DELETE FROM `npc_vendor` WHERE `entry`=73004;
INSERT INTO `npc_vendor` VALUES (73004, 0, 81415, 0, 0, 0, 1),
(73004, 0, 105702, 0, 0, 0, 1),
(73004, 0, 81407, 0, 0, 0, 1);

-- Vendor Brother Rabbitsfoot
DELETE FROM `npc_vendor` WHERE `entry`=73010;
INSERT INTO `npc_vendor` VALUES
(73010, 0, 104140, 0, 0, 0, 1),
(73010, 0, 104141, 0, 0, 0, 1),
(73010, 0, 104142, 0, 0, 0, 1),
(73010, 0, 104143, 0, 0, 0, 1),
(73010, 0, 104144, 0, 0, 0, 1),
(73010, 0, 104145, 0, 0, 0, 1),
(73010, 0, 104146, 0, 0, 0, 1),
(73010, 0, 104148, 0, 0, 0, 1),
(73010, 0, 104149, 0, 0, 0, 1),
(73010, 0, 104150, 0, 0, 0, 1),
(73010, 0, 104151, 0, 0, 0, 1),
(73010, 0, 104152, 0, 0, 0, 1),
(73010, 0, 104153, 0, 0, 0, 1),
(73010, 0, 104154, 0, 0, 0, 1),
(73010, 0, 104155, 0, 0, 0, 1);

-- Vendor Chao of the Hundred Crabs
UPDATE `npc_vendor` SET `incrtime`='60000' WHERE `entry`=67181 AND `item`=6365;

-- Fix Missing Item : Timeless Essence of the Black Dragonflight
DELETE FROM `item_template` WHERE `entry`=102457;
INSERT INTO `item_template` (`entry`, `name`, `displayid`, `Quality`, `spellid_1`, `description`) VALUES ('102457', 'Timeless Essence of the Black Dragonflight', '125694', '1', '146428', 'An undulating sphere of infinite blackness.');

-- Fix Missing Item : Fire-Watcher's Oath
DELETE FROM `item_template` WHERE `entry`=102463;
INSERT INTO `item_template` (`entry`, `name`, `displayid`, `Quality`, `class`, `subclass`, `spellid_1`, `spellcooldown_1`, `description`, `Material`, `WDBVerified`) VALUES ('102463', 'Fire-Watcher\'s Oath', '129132', '3', '15', '4', '148429', '600000', 'Ordon Fire-Watchers carry the ashes of their victims in a special pouch that keeps them smouldering.', '4', '15595');
-- Fix Mising Item : Kukuru's Cache Key
INSERT INTO `item_template` (`entry`, `class`, `name`, `displayid`, `Quality`, `ItemLevel`, `stackable`, `spellid_1`, `bonding`) VALUES ('101538', '13', 'Kukuru\'s Cache Key', '75937', '3', '1', '20', '144528', '1');
