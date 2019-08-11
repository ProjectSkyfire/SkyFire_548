-- Disable Quest 8869: Sweet Serenity
DELETE FROM `disables` WHERE `sourceType`=1 AND `entry`=8869;
INSERT INTO `disables` (`sourceType`, `entry`, `flags`, `params_0`, `params_1`, `comment`) VALUES
(1, 8869, 0, '', '', 'Disable Quest 8869: Sweet Serenity');
