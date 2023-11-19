DELETE FROM `playercreateinfo_spell` WHERE `racemask`=8388608 AND `classmask`=16 AND `Spell`=585;
INSERT INTO `playercreateinfo_spell` (`racemask`, `classmask`, `Spell`, `Note`) VALUES
(8388608, 16, 585, 'Smite');

DELETE FROM `playercreateinfo_action` WHERE `race`=24 AND `class`=5;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(24, 5, 0, 585, 0),
(24, 5, 9, 107079, 0);
