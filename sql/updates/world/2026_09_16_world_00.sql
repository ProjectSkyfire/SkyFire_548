DELETE FROM `command` WHERE `name` = 'server shutdown time';
INSERT INTO `command` (`name`, `permission`, `help`) VALUES
('server shutdown time', 734, 'Syntax: .server shutdown time HH:MM[:SS]\r\n\r\nGracefully shut down at the requested server-local time (24-hour format). If the time has passed today, schedule for tomorrow. Cancel with .server shutdown cancel.');
