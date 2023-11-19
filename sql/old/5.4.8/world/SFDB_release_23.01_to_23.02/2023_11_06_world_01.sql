DELETE FROM `playercreateinfo_spell` WHERE `Spell`=100780;
INSERT INTO `playercreateinfo_spell` (`racemask`, `classmask`, `Spell`, `Note`) VALUES
(0, 512, 100780, 'Jab');

DELETE FROM `playercreateinfo_action` WHERE `action`=100780;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES 
(1, 10, 0, 100780, 0),
(2, 10, 0, 100780, 0),
(3, 10, 0, 100780, 0),
(4, 10, 0, 100780, 0),
(5, 10, 0, 100780, 0),
(6, 10, 0, 100780, 0),
(7, 10, 0, 100780, 0),
(10, 10, 0, 100780, 0),
(11, 10, 0, 100780, 0),
(24, 10, 0, 100780, 0),
(1, 10, 72, 100780, 0),
(2, 10, 72, 100780, 0),
(3, 10, 72, 100780, 0),
(4, 10, 72, 100780, 0),
(5, 10, 72, 100780, 0),
(6, 10, 72, 100780, 0),
(7, 10, 72, 100780, 0),
(10, 10, 72, 100780, 0),
(11, 10, 72, 100780, 0),
(24, 10, 72, 100780, 0);
