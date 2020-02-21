-- Disable Quest 6086: Training the Beast
DELETE FROM `disables` WHERE `sourceType`=1 AND `entry`=6086;
INSERT INTO `disables` (`sourceType`, `entry`, `flags`, `params_0`, `params_1`, `comment`) VALUES
(1, 6086, 0, '', '', 'Disable Quest 6086: Training the Beast');
