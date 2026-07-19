INSERT INTO `command` (`name`, `permission`, `help`) VALUES
('server restart time', 727, 'Syntax: .server restart time #time [#exit_code]\r\n\r\nRestart the server at the requested server-local wall-clock time. Time accepts HH:MM or HH:MM:SS in 24-hour format. If #time has already passed today, the restart is scheduled for tomorrow. Uses #exit_code or 2 as the program exit code.');
