DELETE FROM `command` WHERE `name`='gm note';
INSERT INTO `command` (`name`, `permission`, `help`) VALUES ('gm note', 808, 'Syntax: .gm note $note  Saves an in-game GM note to the auth database with realm, account, character, timestamp, and current map/location. Notes are limited to 255 characters.');
