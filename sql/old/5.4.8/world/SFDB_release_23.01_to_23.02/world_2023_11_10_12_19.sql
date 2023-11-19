DELETE FROM `playercreateinfo_spell` WHERE `racemask`=8388608 AND `classmask`=4 AND `Spell`=982;
INSERT INTO `playercreateinfo_spell` (`racemask`, `classmask`, `Spell`, `Note`) VALUES
(8388608, 4, 982, 'Revive Pet');

DELETE FROM `playercreateinfo_spell` WHERE `racemask`=8388608 AND `classmask`=4 AND `Spell`=3044;
INSERT INTO `playercreateinfo_spell` (`racemask`, `classmask`, `Spell`, `Note`) VALUES
(8388608, 4, 3044, 'Arcane Shot');

DELETE FROM `playercreateinfo_action` WHERE `race`=24 AND `class`=3;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(24, 3, 11, 982, 0),
(24, 3, 10, 9, 48),
(24, 3, 9, 107079, 0),
(24, 3, 0, 3044, 0);
