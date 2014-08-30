-- Pandaren Monk [N]
DELETE FROM `playercreateinfo_spell` WHERE `racemask` AND `classmask` IN (8388608,512);
INSERT INTO `playercreateinfo_spell` (`racemask`, `classmask`, `Spell`, `Note`) VALUES 
(8388608, 512, 227, 'Staves'),
(8388608, 512, 9077, 'Leather'),
(8388608, 512, 674, 'Dual Wield'),
(8388608, 512, 15590, 'Fist Weapons'),
(8388608, 512, 196, 'One-Handed Axes'),
(8388608, 512, 198, 'One-Handed Maces'),
(8388608, 512, 201, 'One-Handed Swords'),
(8388608, 512, 200, 'Polearms'),
(8388608, 512, 107076, 'Bouncy'),
(8388608, 512, 107072, 'Epicurean'),
(8388608, 512, 107073, 'Gourmand'),
(8388608, 512, 107074, 'Inner Peace'),
(8388608, 512, 107079, 'Quaking Palm'),
(8388608, 512, 131701, 'Languages'),
(8388608, 512, 108127, 'Language Pandaren Neutral');

 -- Pandaren Hunter [N]
DELETE FROM `playercreateinfo_spell` WHERE `racemask` AND `classmask` IN (8388608,4);
INSERT INTO `playercreateinfo_spell` (`racemask`, `classmask`, `Spell`, `Note`) VALUES 
(8388608, 4, 1180, 'Daggers'),
(8388608, 4, 266, 'Guns'),
(8388608, 4, 5011, 'Crossbows'),
(8388608, 4, 264, 'Bows'),
(8388608, 4, 9077, 'Leather'),
(8388608, 4, 674, 'Dual Wield'),
(8388608, 4, 15590, 'Fist Weapons'),
(8388608, 4, 196, 'One-Handed Axes'),
(8388608, 4, 227, 'Staves'),
(8388608, 4, 201, 'One-Handed Swords'),
(8388608, 4, 197, 'Two-Handed Axes'),
(8388608, 4, 202, 'Two-Handed Swords'),
(8388608, 4, 200, 'Polearms'),
(8388608, 4, 107076, 'Bouncy'),
(8388608, 4, 107072, 'Epicurean'),
(8388608, 4, 107073, 'Gourmand'),
(8388608, 4, 107074, 'Inner Peace'),
(8388608, 4, 107079, 'Quaking Palm'),
(8388608, 4, 131701, 'Languages'),
(8388608, 4, 108127, 'Language Pandaren Neutral');
