-- Add command help for packet logging GM commands.

DELETE FROM `command` WHERE `name` IN (
    'packet',
    'packet start',
    'packet stop',
    'packet char',
    'packet char start',
    'packet char stop'
);

INSERT INTO `command` (`name`, `permission`, `help`) VALUES
('packet', 1004, 'Syntax: .packet $subcommand\r\n\r\nManage packet logging. Use .packet start, .packet stop, .packet char start "name", or .packet char stop "name".'),
('packet start', 1005, 'Syntax: .packet start\r\n\r\nEnable packet logging for all active sessions.'),
('packet stop', 1006, 'Syntax: .packet stop\r\n\r\nDisable packet logging for all sessions.'),
('packet char', 1007, 'Syntax: .packet char $subcommand "name"\r\n\r\nManage packet logging for one character.'),
('packet char start', 1008, 'Syntax: .packet char start "name"\r\n\r\nEnable packet logging for the named character.'),
('packet char stop', 1009, 'Syntax: .packet char stop "name"\r\n\r\nDisable packet logging for the named character.');
