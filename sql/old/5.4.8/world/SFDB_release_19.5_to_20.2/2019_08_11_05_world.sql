-- Disable Quest 1518: Call of Earth
DELETE FROM `disables` WHERE `sourceType`=1 AND `entry`=1518;
INSERT INTO `disables` (`sourceType`, `entry`, `flags`, `params_0`, `params_1`, `comment`) VALUES
(1, 1518, 0, '', '', 'Disable Quest 1518: Call of Earth');
