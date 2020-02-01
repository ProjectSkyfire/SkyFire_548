TRUNCATE `playercreateinfo_spell_cast`;
INSERT INTO `playercreateinfo_spell_cast` (`raceMask`, `classMask`, `spell`, `note`) VALUES
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

(0, 256, 688, 'Warlock - Summon Imp'),
(0, 512, 103985, 'Monk - Stance of the Fierce'),
(0, 1, 2457, 'Warrior - Battle Stance'),

(8388608, 1, 107922, 'Pandaren - Warrior - Starting Quest'),
(8388608, 4, 107917, 'Pandaren - Hunter - Starting Quest'),
(8388608, 8, 107920, 'Pandaren - Rogue - Starting Quest'),
(8388608, 16, 107919, 'Pandaren - Priest - Starting Quest'),
(8388608, 64, 107921, 'Pandaren - Shaman - Starting Quest'),
(8388608, 128, 107916, 'Pandaren - Mage - Starting Quest'),
(8388608, 512, 107915, 'Pandaren - Monk - Starting Quest'),

(8388608, 1, 108059, 'Pandaren - Warrior - Remove weapon'),
(8388608, 4, 108061, 'Pandaren - Hunter - Remove weapon'),
(8388608, 8, 108058, 'Pandaren - Rogue - Remove weapon'),
(8388608, 16, 108057, 'Pandaren - Priest - Remove weapon'),
(8388608, 64, 108056, 'Pandaren - Shaman - Remove weapon'),
(8388608, 128, 108055, 'Pandaren - Mage - Remove weapon'),
(8388608, 512, 108060, 'Pandaren - Monk - Remove weapon');
