DELETE FROM `playercreateinfo_spell` WHERE `racemask`=8388608 AND `classmask`=64 AND `Spell`=403;
INSERT INTO `playercreateinfo_spell` (`racemask`, `classmask`, `Spell`, `Note`) VALUES
(8388608, 64, 403, 'Lightning Bolt');

DELETE FROM `playercreateinfo_action` WHERE `race`=24 AND `class`=7;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(24, 7, 9, 107079, 0),
(24, 7, 0, 403, 0);
