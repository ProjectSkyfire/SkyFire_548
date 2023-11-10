DELETE FROM `playercreateinfo_action` WHERE `race`=8 AND `class`=10 AND `button`=72;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(8, 10, 72, 100780, 0);
DELETE FROM `playercreateinfo_action` WHERE `race`=8 AND `class`=10 AND `button`=0;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(8, 10, 0, 100780, 0);
