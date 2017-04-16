/*Table structure for table `playercreateinfo_spell` */

DROP TABLE IF EXISTS `playercreateinfo_spell`;

CREATE TABLE `playercreateinfo_spell` (
  `racemask` int(10) unsigned NOT NULL DEFAULT '0',
  `classmask` int(10) unsigned NOT NULL DEFAULT '0',
  `Spell` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `Note` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`racemask`,`classmask`,`Spell`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Data for the table `playercreateinfo_spell` */

INSERT INTO `playercreateinfo_spell`(`racemask`,`classmask`,`Spell`,`Note`) VALUES 
(0,0,6603,'Auto attack'),
(0, 0, 113873, "Enable Remove talent"),
(0, 0, 111621, "Enable Remove glyph"),
(0, 0, 45927, "Summon Friend"),
(0, 0, 6478, "Opening"),
(0, 0, 22027, "Remove Insignia"),
(0, 0, 22810, "Opening - No Text"),
(0, 0, 21651, "Opening"),
(0, 0, 21652, "Closing"),
(0, 0, 2382, "Generic"),
(0, 0, 3365, "Opening"),
(0, 0, 3050, "Detect"),
(0, 0, 9125, "Generic"),
(0, 0, 8386, "Attacking"),
(0, 0, 7266, "Duel"),
(0, 0, 7267, "Grovel"),
(0, 0, 7355, "Stuck"),
(0, 0, 6233, "Closing"),
(0, 0, 6246, "Closing"),
(0, 0, 6247, "Opening"),
(0, 0, 6477, "Opening"),
(0, 0, 61437, "Opening"),
(0, 0, 68398, "Opening"),
(0, 0, 96220, "Opening"),
(0, 0, 203, "Unarmed"),
(2098253, 0, 668, "Language Common"),
(0, 13, 264, "Bows"),
(0, 13, 266, "Guns"),
(0, 67, 107, "Block"),
(0, 0, 81, "Dodge"),
(0, 1, 32215, "Victorious State"),
(0, 1, 5301, "Defensive State (DND)"),
(0, 0, 204, "Defense"),
(0, 0, 522, "SPELLDEFENSE (DND)"),
(2, 493, 79743, "Languages"),
(4, 511, 79739, "Languages"),
(8, 1213, 76252, "Languages"),
(8, 1213, 671, "Language Darnassian"),
(16, 189, 17737, "Language Gutterspeak"),
(16, 256, 17747, "Language Gutterspeak"),
(32, 1079, 670, "Language Taurahe"),
(64, 441, 7340, "Language Gnomish"),
(128, 1533, 7341, "Language Troll"),
(256, 509, 69269, "Language Goblin"),
(512, 447, 813, "Language Thalassian"),
(1024, 247, 29932, "Language Draenei"),
(1024, 247, 79741, "Languages"),
(2097152, 1469, 79742, "Languages"),
(8388608, 0, 108127, "Language pandaren neutral"),
(0, 39, 202, "Two-Handed Swords"),
(0, 111, 196, "One-Handed Axes"),
(0, 13, 5011, "Crossbows"),
(0, 103, 197, "Two-Handed Axes"),
(0, 1501, 1180, "Daggers"),
(0, 1123, 199, "Two-Handed Maces"),
(0, 1101, 15590, "Fist Weapons"),
(0, 1063, 200, "Polearms"),
(0, 1493, 227, "Staves"),
(0, 67, 9116, "Shield"),
(0, 35, 8737, "Mail"),
(0, 1135, 9077, "Leather"),
(0, 9, 2567, "Thrown"),
(0, 0, 9078, "Cloth"),
(0, 1147, 198, "One-Handed Maces"),
(0, 431, 201, "One-Handed Swords"),
(1573, 2, 76271, "Armor Skills"),
(1573, 2, 76294, "Weapon Skills"),
(2099135, 4, 76249, "Weapon Skills"),
(2099135, 4, 76250, "Armor Skills"),
(2098143, 8, 76273, "Armor Skills"),
(2098143, 8, 76297, "Weapon Skills"),
(2099197, 16, 76301, "Weapon Skills"),
(2099197, 16, 76279, "Armor Skills"),
(2099167, 128, 76298, "Weapon Skills"),
(2099167, 128, 76276, "Armor Skills"),
(2098135, 256, 76299, "Weapon Skills"),
(2098135, 256, 76277, "Armor Skills"),
(1414, 64, 76272, "Armor Skills"),
(1414, 64, 76296, "Weapon Skills"),
(2097320, 1024, 76300, "Weapon Skills"),
(2097320, 1024, 76275, "Armor Skills"),
(0, 32, 750, "Plate Mail"),
(0, 0, 134732, "Tired of the fight");

/* For races */

INSERT INTO `playercreateinfo_spell`(`racemask`,`classmask`,`Spell`,`Note`) VALUES

-- Humans

(1,0,59752,'Every Man for Himself (Racial) Humans'),
(1,0,20599,'Diplomacy (Racial Passive) Humans '),
(1,0,20597,'Sword Specialization (Racial Passive) Humans'),
(1,0,20864,'Mace Specialization (Racial Passive) Humans'),
(1,0,20598,'The human spirit (Racial Passive) Humans'),
(1,0,79738,'Language Humans ​​(Racial Passive) Humans'),

-- Dwarves

(4,0,20594,'Stone Shape (Racial)'),
(4,0,92682,'Researcher (Racial Passive)'),
(4,0,20595,'Dwarves ( NO idea ,Racial Passive)'),
(4,0,20596,'Frost Resistance (Racial Passive)'),
(4,0,59224,'Mace Specialization (Racial Passive) Dwarves'),
(4,0,79739,'Language Dwarves ​​(Racial Passive) Dwarves'),

-- Night Elfs

(8,0,58984,'Shadowmeld (Racial) Night Elfs'),
(8,0,20585,'Wisp Spirit (Racial Passive) Night Elfs'),
(8,0,21009,'Elusiveness (Racial Passive) Night Elfs'),
(8,0,20582,'Quickness (Racial Passive) Night Elfs'),
(8,0,20583,'Nature Resistance (Racial Passive) Night Elfs'),
(8,0,76252,'Language Night Elfs ​​(Racial Passive) Night Elfs'),

-- Gnomes

(64,0,20589,'Escape Artist (Racial) Gnomes'),
(64,0,20593,'Calling - Engineer! (Racial Passive) Gnomes'),
(64,0,20591,'Inquisitive mind (Racial Passive) Gnomes'),
(64,0,20592,'Arcane Resistance (Racial Passive) Gnomes'),
(64,0,92680,'Specializing in short blades (Racial Passive) Gnomes'),
(64,0,79740,'Language Gnomes ​​(Racial Passive) Gnomes'),

-- Draeneis

(1024,0,28880,'Gift of the Naaru (Racial) Draeneis'),
(1024,0,6562,'Fighting spirit (Racial Passive) Draeneis'),
(1024,0,28875,'Carving (Racial Passive) Draeneis'),
(1024,0,132295,'Shadow Resistance (Racial Passive) Draeneis'),
(1024,0,79741,'Language Draeneis ​​(Racial Passive) Draeneis'),

-- Worgen

(2097152,0,79738,'Language Worgen ​​(Racial Passive) Worgen'),

-- Orcs
(2,45,20572,'Blood Fury (Racial) Orcs Attack Power'),
(2,384,20572,'Blood Fury (Racial) Orcs Attack Power'),
(2,32,20572,'Blood Fury (Racial) Orcs Attack Power'),
(2,64,20572,'Blood Fury (Racial) Orcs Attack Power'),
(2,128,33702,'Blood Fury (Racial) Orcs Spell Power'),
(2,256,33702,'Blood Fury (Racial) Orcs Spell Power'),
(2,0,20575,'Authoritativeness (Racial Passive) Orcs'),
(2,0,20574,'Axe Specialization (Racial Passive) Orcs'),
(2,0,20573,'Hardness (Racial Passive) Orcs'),
(2,0,79743,'Language Orcs ​​(Racial Passive) Orcs'),

-- Undead
(16,0,7744,'Will of the Forsaken (Racial) Undead'),
(16,0,20577,'Cannibalism (Racial) Undead'),
(16,0,5227,'Touch the Dead (Racial Passive) Undead'),
(16,0,20579,'Shadow Resistance (Racial Passive) Undead'),
(16,0,79747,'Language Undead (Racial Passive) Undead'),

-- Tauren
(32,0,74606,'War Stomp (Racial) Tauren'),
(32,0,20550,'Tempering (Racial Passive) Tauren'),
(32,0,20552,'Sprouting (Racial Passive) Tauren'),
(32,0,20551,'Nature Resistance (Racial Passive) Tauren'),
(32,0,79746,'Language Tauren (Racial Passive) Tauren'),

-- Troll
(128,0,26297,'Berserk (Racial) Troll'),
(128,0,58943,'Resourcefulness voodoo (Racial Passive) Troll'),
(128,0,26290,'Keen eye (Racial Passive) Troll'),
(128,0,20555,'Regeneration (Racial Passive) Troll'),
(128,0,20557,'Killing animals (Racial Passive) Troll'),
(128,0,79744,'Language Troll (Racial Passive) Troll'),

-- Blood Elf
(512,0,25046,'Arcane Torrent (Racial) Blood Elf'),
(512,0,28877,'Kinship Enchanting (Racial Passive) Blood Elf'),
(512,0,822,'Arcane Resistance (Racial Passive) Blood Elf'),
(512,0,79748,'Language Blood Elf (Racial Passive) Blood Elf'),

-- Goblin
(256,0,69041,'Rocket Barrage (Racial) Goblin'),
(256,0,69070,'Rocket Jump (Racial) Goblin'),
(256,0,69042,'Time is money - (Racial Passive) Goblin'),
(256,0,69045,'Chemistry (Racial Passive) Goblin'),
(256,0,69044,'Best prices! (Racial Passive) Goblin'),
(256,0,79749,'Language Goblin (Racial Passive) Goblin'),

-- Pandaren
(8388608,0,107079,'Shake his hand (Racial) Pandaren'),
(8388608,0,107074,'Inner Harmony (Racial Passive) Pandaren'),
(8388608,0,107073,'Gurman (Racial Passive) Pandaren'),
(8388608,0,107076,'Jumping (Racial Passive) Pandaren'),
(8388608,0,107072,'Epicureanism (Racial Passive) Pandaren'),
(8388608,0,131701,'Language Pandaren (Racial Passive) Pandaren');

/* For clases */

INSERT INTO `playercreateinfo_spell`(`racemask`,`classmask`,`Spell`,`Note`) VALUES 
-- Warriors
(0,1,76268,'Available armor Warrior'),
(0,1,76290,'Weapon Skills Warrior'),
-- Paly
(0,2,76271,'Available armor Paly'),
(0,2,76294,'Weapon Skills Paly'),
-- Hunter
(0,4,674,'Fight with a weapon in each hand (passive) Hunter'),
(0,4,76250,'Available armor (passive) Hunter'),
(0,4,76249,'Weapon Skills Hunter'),
-- Rogue
(0,8,674,'Fight with a weapon in each hand (passive) Rogue'),
(0,8,76273,'Available armor (passive) Rogue'),
(0,8,76297,'Weapon Skills Rogue'),
-- Priest
(0,16,5019,'Friendly magic wand Priest'),
(0,16,76279,'Available armor (passive) Priest'),
(0,16,76301,'Weapon Skills Priest'),
-- Shamy
(0,64,76272,'Available armor (passive) Shaman'),
(0,64,76296,'Weapon Skills Shaman'),
-- Mage
(0,128,5019,'Friendly magic wand Mage'),
(0,128,76276,'Available armor (passive) Mage'),
(0,128,76298,'Weapon Skills Mage'),
-- Warlock
(0,256,5019,'Friendly magic wand Warlock'),
(0,256,76277,'Available armor (passive) Warlock'),
(0,256,76299,'Weapon Skills Warlock'),
-- Druid
(0,1024,76275,'Available armor (passive) Druid'),
(0,1024,76300,'Weapon Skills Druid'),
-- DK
(0,32,76282,'Available armor (passive) Druid'),
(0,32,76292,'Weapon Skills Druid'),
-- Moonk
(0,512,106904,'Available armor (passive) Moonk'),
(0,512,106902,'Weapon Skills Moonk');