DELETE FROM `disables` WHERE `sourceType` = 2 AND `entry` IN (1008, 1009, 1098, 1011);
INSERT INTO `disables` (`sourceType`, `entry`, `flags`, `params_0`, `params_1`, `comment`) VALUES
(2, 1008, 63, '', '', 'Disabled Raid: Mogu shan Vaults [ALL DIFFICULTIES] - WIP'),
(2, 1009, 63, '', '', 'Disabled Raid: Heart of Fear [ALL DIFFICULTIES] - WIP'),
(2, 1098, 63, '', '', 'Disabled Raid: Throne of Thunder [ALL DIFFICULTIES] - WIP'),
(2, 1011, 3, '', '', 'Disabled Dungeon: Siege of Niuzao Temple [ALL DIFFICULTIES] - WIP');

UPDATE `game_tele` SET `position_x` = 964.450989, `position_y` = -2454.360107, `position_z` = 180.233551, `orientation` = 80.12928 WHERE `ID` = 1003; -- Jade Serpent Temple (updates from the sky to infront of the temple).

UPDATE `game_tele` SET `position_x` = 1426.476929, `position_y` = 5083.146973, `position_z` = 131.158401, `orientation` = 80.12928 WHERE `ID` = 1024; -- Siege Of Niuzao Temple (updates from the outside to infront of the entrance in the hollowed tree).

DELETE FROM `game_tele` WHERE `id`= 1527;
INSERT INTO `game_tele` (`id`, `position_x`, `position_y`, `position_z`, `orientation`, `map`, `name`) VALUES
(1527, 683.242, 2079.95, 371.711, 0.0201836, 870, 'GateOfTheSettingSun');
