-- Gnome Escape Artist
DELETE FROM `playercreateinfo_action` WHERE `action`=20589;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES 
(7, 1, 9, 20589, 0),
(7, 1, 81, 20589, 0),
(7, 4, 9, 20589, 0),
(7, 5, 9, 20589, 0),
(7, 6, 9, 20589, 0),
(7, 8, 9, 20589, 0),
(7, 9, 9, 20589, 0),
(7, 10, 9, 20589, 0),
(7, 10, 81, 20589, 0);

-- Warrior Auto Cast Battle Stance 
DELETE FROM `playercreateinfo_spell_cast` WHERE `racemask`=0 AND `classmask`=1 AND `Spell`=2457;
INSERT INTO `playercreateinfo_spell_cast` (`racemask`, `classmask`, `Spell`, `Note`) VALUES
(0, 1, 2457, 'Warrior - Battle Stance');

-- Warrior Auto Attack
DELETE FROM `playercreateinfo_action` WHERE  `class`=1 AND `action`=6603;
DELETE FROM `playercreateinfo_action` WHERE  `class`=1 AND `action`=88163;
DELETE FROM `playercreateinfo_action` WHERE  `class`=1 AND `button` IN (108, 96, 84);
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(1, 1, 0, 88163, 0),
(2, 1, 0, 88163, 0),
(3, 1, 0, 88163, 0),
(4, 1, 0, 88163, 0),
(5, 1, 0, 88163, 0),
(6, 1, 0, 88163, 0),
(7, 1, 0, 88163, 0),
(8, 1, 0, 88163, 0),
(9, 1, 0, 88163, 0),
(10, 1, 0, 88163, 0),
(11, 1, 0, 88163, 0),
(22, 1, 0, 88163, 0),
(24, 1, 0, 88163, 0),
(1, 1, 72, 88163, 0),
(2, 1, 72, 88163, 0),
(3, 1, 72, 88163, 0),
(4, 1, 72, 88163, 0),
(5, 1, 72, 88163, 0),
(6, 1, 72, 88163, 0),
(7, 1, 72, 88163, 0),
(8, 1, 72, 88163, 0),
(9, 1, 72, 88163, 0),
(10, 1, 72, 88163, 0),
(11, 1, 72, 88163, 0),
(22, 1, 72, 88163, 0),
(24, 1, 72, 88163, 0);

-- Warrior Heroic Strike
DELETE FROM `playercreateinfo_spell` WHERE `Spell`=78;
INSERT INTO `playercreateinfo_spell` (`classmask`, `Spell`, `Note`) VALUES
(1, 78, 'Heroic Strike');

DELETE FROM `playercreateinfo_action` WHERE `action`=88161;
DELETE FROM `playercreateinfo_action` WHERE `action`=78;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
 (1, 1, 1, 78, 0),
 (2, 1, 1, 78, 0),
 (3, 1, 1, 78, 0),
 (4, 1, 1, 78, 0),
 (5, 1, 1, 78, 0),
 (6, 1, 1, 78, 0),
 (7, 1, 1, 78, 0),
 (8, 1, 1, 78, 0),
 (9, 1, 1, 78, 0),
 (10, 1, 1, 78, 0),
 (11, 1, 1, 78, 0),
 (22, 1, 1, 78, 0),
 (1, 1, 73, 78, 0),
 (2, 1, 73, 78, 0),
 (3, 1, 73, 78, 0),
 (4, 1, 73, 78, 0),
 (5, 1, 73, 78, 0),
 (6, 1, 73, 78, 0),
 (7, 1, 73, 78, 0),
 (8, 1, 73, 78, 0),
 (9, 1, 73, 78, 0),
 (10, 1, 73, 78, 0),
 (11, 1, 73, 78, 0),
 (22, 1, 73, 78, 0);

-- Warrior Racials
DELETE FROM `playercreateinfo_action` WHERE `class`=1 AND `button` IN (9, 81);
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES 
(1, 1, 9, 59752, 0), -- Every Man for Himself (Racial)
(2, 1, 9, 20572, 0), -- Blood Fury (Racial) 
(3, 1, 9, 20594, 0), -- Stoneform (Racial)
(4, 1, 9, 58984, 0), -- Shadowmeld (Racial)
(5, 1, 9, 20577, 0), -- Cannibalize (Racial)
(6, 1, 9, 20549, 0), -- War Stomp (Racial)
(7, 1, 9, 20589, 0), -- Escape Artist (Racial)
(8, 1, 9, 26297, 0), -- Berserking (Racial)
(9, 1, 9, 69070, 0), -- Rocket Jump (Racial)
(10, 1, 9, 28730, 0), -- Arcane Torrent (Racial)
(11, 1, 9, 59542, 0), -- Gift of the Naaru (Racial)
(22, 1, 9, 68992, 0), -- Darkflight (Racial)
(24, 1, 9, 107079, 0), -- Quaking Palm (Racial)
(1, 1, 81, 59752, 0), -- Every Man for Himself (Racial)
(2, 1, 81, 20572, 0), -- Blood Fury (Racial) 
(3, 1, 81, 20594, 0), -- Stoneform (Racial)
(4, 1, 81, 58984, 0), -- Shadowmeld (Racial)
(5, 1, 81, 20577, 0), -- Cannibalize (Racial)
(6, 1, 81, 20549, 0), -- War Stomp (Racial)
(7, 1, 81, 20589, 0), -- Escape Artist (Racial)
(8, 1, 81, 26297, 0), -- Berserking (Racial)
(9, 1, 81, 69070, 0), -- Rocket Jump (Racial)
(10, 1, 81, 28730, 0), -- Arcane Torrent (Racial)
(11, 1, 81, 59542, 0), -- Gift of the Naaru (Racial)
(22, 1, 81, 68992, 0), -- Darkflight (Racial)
(24, 1, 81, 107079, 0); -- Quaking Palm (Racial)

