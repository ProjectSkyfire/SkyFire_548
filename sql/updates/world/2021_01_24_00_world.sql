-- Staves Proficiency
UPDATE playercreateinfo_spell SET classmask=classmask|1|4|16|64|128|256|512|1024 WHERE spell=227;
-- Polearms Proficiency
UPDATE playercreateinfo_spell SET classmask=classmask|1|2|4|32|512|1024 WHERE spell=200;
-- One-Handed Swords Proficiency
UPDATE playercreateinfo_spell SET classmask=classmask|1|2|4|8|32|128|256|512 WHERE spell=201;
-- One-Handed Maces Proficiency
UPDATE playercreateinfo_spell SET classmask=classmask|1|2|8|16|32|64|512|1024 WHERE spell=198;
-- One-Handed Axes Proficiency
UPDATE playercreateinfo_spell SET classmask=classmask|1|2|4|8|32|64|512 WHERE spell=196;
-- Fist Weapons Proficiency
UPDATE playercreateinfo_spell SET classmask=classmask|1|4|8|64|512|1024 WHERE spell=15590;
-- Leather Proficiency
UPDATE playercreateinfo_spell SET classmask=classmask|1|2|4|8|32|64|512|1024 WHERE spell=9077;

-- Human Racials:
-- Add Sword Specialization
UPDATE playercreateinfo_spell SET classmask=classmask|1|2|4|8|16|32|128|256|512 WHERE spell=20597;
-- Add The Human Spirit
UPDATE playercreateinfo_spell SET classmask=classmask|1|2|4|8|16|32|128|256|512 WHERE spell=20598;
-- Add Diplomacy
UPDATE playercreateinfo_spell SET classmask=classmask|1|2|4|8|16|32|128|256|512 WHERE spell=20599;
-- Add Mace Specialization
UPDATE playercreateinfo_spell SET classmask=classmask|1|2|4|8|16|32|128|256|512 WHERE spell=20864;
-- Add Every Man for Himself
UPDATE playercreateinfo_spell SET classmask=classmask|1|2|4|8|16|32|128|256|512 WHERE spell=59752;
-- Add Languages
UPDATE playercreateinfo_spell SET classmask=classmask|1|2|4|8|16|32|128|256|512 WHERE spell=79738;

-- Orc Racials:
-- Languages
UPDATE playercreateinfo_spell SET classmask=classmask|1|4|8|32|64|128|256|512 WHERE spell=79743;
-- Hardiness
UPDATE playercreateinfo_spell SET classmask=classmask|1|4|8|32|64|128|256|512 WHERE spell=20573;
-- Axe Specialization
UPDATE playercreateinfo_spell SET classmask=classmask|1|4|8|32|64|128|256|512 WHERE spell=20574;

-- Dwarf Racials:
-- Frost Resistance
UPDATE playercreateinfo_spell SET classmask=classmask|1|2|4|8|16|32|64|128|256|512 WHERE spell=20596;
-- Gun Specialization
UPDATE playercreateinfo_spell SET classmask=classmask|1|2|4|8|16|32|64|128|256|512 WHERE spell=20595;
-- Stoneform
UPDATE playercreateinfo_spell SET classmask=classmask|1|2|4|8|16|32|64|128|256|512 WHERE spell=20594;
-- Mace Specialization
UPDATE playercreateinfo_spell SET classmask=classmask|1|2|4|8|16|32|64|128|256|512 WHERE spell=59224;
-- Explorer
UPDATE playercreateinfo_spell SET classmask=classmask|1|2|4|8|16|32|64|128|256|512 WHERE spell=92682;

-- Night Elf Racials:
-- Nature Resistance
UPDATE playercreateinfo_spell SET classmask=classmask|1|4|8|16|32|128|512|1024 WHERE spell=20583;
-- Quickness
UPDATE playercreateinfo_spell SET classmask=classmask|1|4|8|16|32|128|512|1024 WHERE spell=20582;
-- Wisp Spirit
UPDATE playercreateinfo_spell SET classmask=classmask|1|4|8|16|32|128|512|1024 WHERE spell=20585;
-- Elusiveness
UPDATE playercreateinfo_spell SET classmask=classmask|1|4|8|16|32|128|512|1024 WHERE spell=21009;
-- Shadowmeld
UPDATE playercreateinfo_spell SET classmask=classmask|1|4|8|16|32|128|512|1024 WHERE spell=58984;

-- Undead Racials:
-- Underwater Breathing
UPDATE playercreateinfo_spell SET classmask=classmask|1|4|8|16|32|128|256|512 WHERE spell=5227;
-- Will of the Forsaken
UPDATE playercreateinfo_spell SET classmask=classmask|1|4|8|16|32|128|256|512 WHERE spell=7744;
-- Cannibalize
UPDATE playercreateinfo_spell SET classmask=classmask|1|4|8|16|32|128|256|512 WHERE spell=20577;
-- Shadow Resistance
UPDATE playercreateinfo_spell SET classmask=classmask|1|4|8|16|32|128|256|512 WHERE spell=20579;

-- Tauren Racials:
-- War Stomp
UPDATE playercreateinfo_spell SET classmask=classmask|1|2|4|16|32|64|512|1024 WHERE spell=20549;
-- Endurance
UPDATE playercreateinfo_spell SET classmask=classmask|1|2|4|16|32|64|512|1024 WHERE spell=20550;
-- Nature Resistance
UPDATE playercreateinfo_spell SET classmask=classmask|1|2|4|16|32|64|512|1024 WHERE spell=20551;
-- Cultivation
UPDATE playercreateinfo_spell SET classmask=classmask|1|2|4|16|32|64|512|1024 WHERE spell=20552;

-- Gnome Racials:
-- Expansive Mind
UPDATE playercreateinfo_spell SET classmask=classmask|1|8|16|32|128|256|512 WHERE spell=20591;
-- Engineering Specialization
UPDATE playercreateinfo_spell SET classmask=classmask|1|8|16|32|128|256|512 WHERE spell=20593;
-- Arcane Resistance
UPDATE playercreateinfo_spell SET classmask=classmask|1|8|16|32|128|256|512 WHERE spell=20592;
-- Shortblade Specialization
UPDATE playercreateinfo_spell SET classmask=classmask|1|8|16|32|128|256|512 WHERE spell=92680;
-- Escape Artist
UPDATE playercreateinfo_spell SET classmask=classmask|1|8|16|32|128|256|512 WHERE spell=20589;

-- Troll Racials:
-- Bow Specialization
UPDATE playercreateinfo_spell SET classmask=classmask|1|4|8|16|32|64|128|256|512|1024 WHERE spell=26290;
-- Berserking
UPDATE playercreateinfo_spell SET classmask=classmask|1|4|8|16|32|64|128|256|512|1024 WHERE spell=26297;
-- Da Voodoo Shuffle
UPDATE playercreateinfo_spell SET classmask=classmask|1|4|8|16|32|64|128|256|512|1024 WHERE spell=58943;
-- Regeneration
UPDATE playercreateinfo_spell SET classmask=classmask|1|4|8|16|32|64|128|256|512|1024 WHERE spell=20555;
-- Beast Slaying
UPDATE playercreateinfo_spell SET classmask=classmask|1|4|8|16|32|64|128|256|512|1024 WHERE spell=20557;
-- Throwing Specialization
-- NO LONGER EXISTS!
DELETE FROM playercreateinfo_spell WHERE spell=20558;

-- Blood Elf Racials:
-- Arcane Resistance
UPDATE playercreateinfo_spell SET classmask=classmask|1|2|4|8|16|32|128|256|512 WHERE spell=822;
-- Arcane Affinity
UPDATE playercreateinfo_spell SET classmask=classmask|1|2|4|8|16|32|128|256|512 WHERE spell=28877;
-- Arcane Torrent
-- Each Class has diffrent depending on Energize.
-- Arcane Torrent (Rage)
UPDATE playercreateinfo_spell SET classmask=classmask|1 WHERE spell=69179;
-- Arcane Torrent (Mana)
UPDATE playercreateinfo_spell SET classmask=classmask|2|16|128|256 WHERE spell=28730;
-- Arcane Torrent (Focus)
UPDATE playercreateinfo_spell SET classmask=classmask|4 WHERE spell=80483;
-- Arcane Torrent (Energy)
UPDATE playercreateinfo_spell SET classmask=classmask|8 WHERE spell=25046;
-- Arcane Torrent (Runic Power)
UPDATE playercreateinfo_spell SET classmask=classmask|32 WHERE spell=50613;
-- Arcane Torrent (Chi)
DELETE FROM playercreateinfo_spell WHERE spell=129597;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
('512', '512', '129597', 'Arcane Torrent');

-- Draenei Racials:
-- Gemcutting
UPDATE playercreateinfo_spell SET classmask=classmask|1|2|4|16|32|64|128|512 WHERE spell=28875;
-- Shadow Resistance
-- Each Class has diffrent depending on SpellfamilyFlag and Gift of the Naaru
-- MONKS dont have Gift of the Naaru, Instead expel harm.
-- Shadow Resistance (Warrior)
UPDATE playercreateinfo_spell SET classmask=classmask|1 WHERE spell=59221;
-- Shadow Resistance (Paladin)
UPDATE playercreateinfo_spell SET classmask=classmask|2 WHERE spell=59535;
-- Shadow Resistance (Hunter)
UPDATE playercreateinfo_spell SET classmask=classmask|4 WHERE spell=59536;
-- Shadow Resistance (Priest)
UPDATE playercreateinfo_spell SET classmask=classmask|16 WHERE spell=59538;
-- Shadow Resistance (Death Knight)
UPDATE playercreateinfo_spell SET classmask=classmask|32 WHERE spell=59539;
-- Shadow Resistance (Shaman)
UPDATE playercreateinfo_spell SET classmask=classmask|64 WHERE spell=59540;
-- Shadow Resistance (Mage)
UPDATE playercreateinfo_spell SET classmask=classmask|128 WHERE spell=59541;
-- Shadow Resistance (Monk)
DELETE FROM playercreateinfo_spell WHERE spell=132295;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
('1024', '512', '132295', 'Shadow Resistance');
-- Heroic Presence
UPDATE playercreateinfo_spell SET classmask=classmask|1|2|4|32|512 WHERE spell=6562;

-- Pandaren Racials:
-- Epicurean 
DELETE FROM playercreateinfo_spell WHERE spell=107072;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
('8388608', '512', '107072', 'Epicurean');
UPDATE playercreateinfo_spell SET classmask=classmask|1|4|8|16|64|128|512 WHERE spell=107072;
-- Gourmand Gourmand
DELETE FROM playercreateinfo_spell WHERE spell=107073;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
('8388608', '512', '107073', 'Gourmand Gourmand');
UPDATE playercreateinfo_spell SET classmask=classmask|1|4|8|16|64|128|512 WHERE spell=107073;
-- Inner Peace
DELETE FROM playercreateinfo_spell WHERE spell=107074;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
('8388608', '512', '107074', 'Inner Peace');
UPDATE playercreateinfo_spell SET classmask=classmask|1|4|8|16|64|128|512 WHERE spell=107074;
-- Bouncy
DELETE FROM playercreateinfo_spell WHERE spell=107076;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
('8388608', '512', '107076', 'Bouncy');
UPDATE playercreateinfo_spell SET classmask=classmask|1|4|8|16|64|128|512 WHERE spell=107076;
-- Quaking Palm
DELETE FROM playercreateinfo_spell WHERE spell=107079;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
('8388608', '512', '107079', 'Bouncy');
UPDATE playercreateinfo_spell SET classmask=classmask|1|4|8|16|64|128|512 WHERE spell=107079;
