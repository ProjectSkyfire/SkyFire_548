DELETE FROM `playercreateinfo_spell_cast`
WHERE `Spell` IN (79593, 79594, 79595, 79596, 79597, 79598, 79599, 79600, 79601, 79602, 79603, 107917, 107924, 108061);

INSERT INTO `playercreateinfo_spell_cast` (`racemask`, `classmask`, `Spell`, `Note`) VALUES
(1, 4, 79597, 'Human - Hunter - Young Wolf'),
(2, 4, 79598, 'Orc - Hunter - Young Boar'),
(4, 4, 79593, 'Dwarf - Hunter - Young Bear'),
(8, 4, 79602, 'Night Elf - Hunter - Young Cat'),
(16, 4, 79600, 'Undead - Hunter - Young Widow'),
(32, 4, 79603, 'Tauren - Hunter - Young Tallstrider'),
(128, 4, 79599, 'Troll - Hunter - Young Raptor'),
(256, 4, 79595, 'Goblin - Hunter - Young Crab'),
(512, 4, 79594, 'Blood Elf - Hunter - Young Dragonhawk'),
(1024, 4, 79601, 'Draenei - Hunter - Young Moth'),
(2097152, 4, 79596, 'Worgen - Hunter - Young Mastiff'),
(8388608, 4, 107924, 'Pandaren - Hunter - Wise Turtle'),
(8388608, 4, 107917, 'Pandaren - Hunter - Starting Quest'),
(8388608, 4, 108061, 'Pandaren - Hunter - Remove weapon');
