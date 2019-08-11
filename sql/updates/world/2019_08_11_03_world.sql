-- Disable Quest 1719: The Affray
DELETE FROM `disables` WHERE `sourceType`=1 AND `entry`=1719;
INSERT INTO `disables` (`sourceType`, `entry`, `flags`, `params_0`, `params_1`, `comment`) VALUES
(1, 1719, 0, '', '', 'Disable Quest 1719: The Affray');
