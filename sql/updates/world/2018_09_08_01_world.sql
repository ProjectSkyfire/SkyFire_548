DELETE FROM `command` WHERE `permission` = 776;
INSERT INTO `command` (`name`, `permission`, `help`) VALUES
('server uptime',     776, 'Syntax: .server uptime, Only display the current uptime.');
