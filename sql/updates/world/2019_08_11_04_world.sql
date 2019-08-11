-- Disable Quest 1661: The Tome of Nobility
DELETE FROM `disables` WHERE `sourceType`=1 AND `entry`=1661;
INSERT INTO `disables` (`sourceType`, `entry`, `flags`, `params_0`, `params_1`, `comment`) VALUES
(1, 1661, 0, '', '', 'Disable Quest 1661: The Tome of Nobility');
