-- Class-specific racials were stored on another class's create bar, so new
-- characters logged "player don't known this spell" and dropped the button.
-- Buttons 9 and 81 are the two racial slots.

-- Orc shaman and monk use Blood Fury 33697 (attack power and spell power).
-- 576 = shaman (64) + monk (512). The shaman bar had warrior 20572.
-- The monk bar had warrior 20572 and mage 33702.
DELETE FROM `playercreateinfo_spell` WHERE `racemask` = 2 AND `Spell` = 33697;
INSERT INTO `playercreateinfo_spell` (`racemask`, `classmask`, `Spell`, `Note`) VALUES
(2, 576, 33697, 'Blood Fury');

DELETE FROM `playercreateinfo_action` WHERE `race` = 2 AND `class` = 7 AND `button` = 9;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(2, 7, 9, 33697, 0);

DELETE FROM `playercreateinfo_action` WHERE `race` = 2 AND `class` = 10 AND `button` = 9;
DELETE FROM `playercreateinfo_action` WHERE `race` = 2 AND `class` = 10 AND `button` = 81;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(2, 10, 9, 33697, 0),
(2, 10, 81, 33697, 0);

-- Draenei warriors learn Gift of the Naaru 28880, not paladin 59542.
DELETE FROM `playercreateinfo_action` WHERE `race` = 11 AND `class` = 1 AND `button` = 9;
DELETE FROM `playercreateinfo_action` WHERE `race` = 11 AND `class` = 1 AND `button` = 81;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(11, 1, 9, 28880, 0),
(11, 1, 81, 28880, 0);

-- Draenei monks learn Gift of the Naaru 121093. The bar had paladin 59542 and mage 59548.
DELETE FROM `playercreateinfo_spell` WHERE `racemask` = 1024 AND `classmask` = 512 AND `Spell` = 121093;
INSERT INTO `playercreateinfo_spell` (`racemask`, `classmask`, `Spell`, `Note`) VALUES
(1024, 512, 121093, 'Gift of the Naaru');

DELETE FROM `playercreateinfo_action` WHERE `race` = 11 AND `class` = 10 AND `button` = 9;
DELETE FROM `playercreateinfo_action` WHERE `race` = 11 AND `class` = 10 AND `button` = 81;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(11, 10, 9, 121093, 0),
(11, 10, 81, 121093, 0);

-- Blood elf warriors learn rage torrent 69179. Monks learn 129597.
-- Both bars had the mana version 28730.
DELETE FROM `playercreateinfo_action` WHERE `race` = 10 AND `class` = 1 AND `button` = 9;
DELETE FROM `playercreateinfo_action` WHERE `race` = 10 AND `class` = 1 AND `button` = 81;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(10, 1, 9, 69179, 0),
(10, 1, 81, 69179, 0);

DELETE FROM `playercreateinfo_action` WHERE `race` = 10 AND `class` = 10 AND `button` = 9;
DELETE FROM `playercreateinfo_action` WHERE `race` = 10 AND `class` = 10 AND `button` = 81;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(10, 10, 9, 129597, 0),
(10, 10, 81, 129597, 0);

-- Goblin racials omitted monks. 1021 is every goblin class, including monk.
-- Rocket Jump 69070 and Rocket Barrage 69041 are on the monk bar.
DELETE FROM `playercreateinfo_spell` WHERE `racemask` = 256 AND `Spell` = 69041;
DELETE FROM `playercreateinfo_spell` WHERE `racemask` = 256 AND `Spell` = 69042;
DELETE FROM `playercreateinfo_spell` WHERE `racemask` = 256 AND `Spell` = 69044;
DELETE FROM `playercreateinfo_spell` WHERE `racemask` = 256 AND `Spell` = 69045;
DELETE FROM `playercreateinfo_spell` WHERE `racemask` = 256 AND `Spell` = 69046;
DELETE FROM `playercreateinfo_spell` WHERE `racemask` = 256 AND `Spell` = 69070;
DELETE FROM `playercreateinfo_spell` WHERE `racemask` = 256 AND `Spell` = 69269;
DELETE FROM `playercreateinfo_spell` WHERE `racemask` = 256 AND `Spell` = 79749;
INSERT INTO `playercreateinfo_spell` (`racemask`, `classmask`, `Spell`, `Note`) VALUES
(256, 1021, 69041, 'Rocket Barrage'),
(256, 1021, 69042, 'Time is Money'),
(256, 1021, 69044, 'Best Deals Anywhere'),
(256, 1021, 69045, 'Better Living Through Chemistry'),
(256, 1021, 69046, 'Pack Hobgoblin'),
(256, 1021, 69070, 'Rocket Jump'),
(256, 1021, 69269, 'Language Goblin'),
(256, 1021, 79749, 'Languages (Racial Passive)');

-- Worgen racials were limited to death knights. 1469 is every worgen class.
-- Darkflight 68992 is on the worgen warrior bar.
DELETE FROM `playercreateinfo_spell` WHERE `racemask` = 2097152 AND `Spell` = 68975;
DELETE FROM `playercreateinfo_spell` WHERE `racemask` = 2097152 AND `Spell` = 68976;
DELETE FROM `playercreateinfo_spell` WHERE `racemask` = 2097152 AND `Spell` = 68978;
DELETE FROM `playercreateinfo_spell` WHERE `racemask` = 2097152 AND `Spell` = 68992;
DELETE FROM `playercreateinfo_spell` WHERE `racemask` = 2097152 AND `Spell` = 68996;
DELETE FROM `playercreateinfo_spell` WHERE `racemask` = 2097152 AND `Spell` = 87840;
DELETE FROM `playercreateinfo_spell` WHERE `racemask` = 2097152 AND `Spell` = 94293;
INSERT INTO `playercreateinfo_spell` (`racemask`, `classmask`, `Spell`, `Note`) VALUES
(2097152, 1469, 68975, 'Viciousness'),
(2097152, 1469, 68976, 'Aberration'),
(2097152, 1469, 68978, 'Flayer'),
(2097152, 1469, 68992, 'Darkflight'),
(2097152, 1469, 68996, 'Two Forms'),
(2097152, 1469, 87840, 'Running Wild'),
(2097152, 1469, 94293, 'Enable Worgen Altered Form');
