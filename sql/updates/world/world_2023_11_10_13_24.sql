DELETE FROM `playercreateinfo_spell` WHERE `racemask`=8388608 AND `classmask`=8 AND `Spell`=1752;
INSERT INTO `playercreateinfo_spell` (`racemask`, `classmask`, `Spell`, `Note`) VALUES
(8388608, 8, 1752, 'Sinister Strike');

DELETE FROM `playercreateinfo_action` WHERE `race`=24 AND `class`=4;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(24, 4, 9, 107079, 0),
(24, 4, 0, 1752, 0);
