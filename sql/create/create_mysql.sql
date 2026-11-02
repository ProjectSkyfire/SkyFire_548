
CREATE DATABASE `world` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;

CREATE DATABASE `characters` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;

CREATE DATABASE `auth` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;

CREATE USER 'skyfire'@'localhost' IDENTIFIED BY 'skyfire';

CREATE USER 'skyfire'@'127.0.0.1' IDENTIFIED BY 'skyfire';

GRANT ALL PRIVILEGES ON `world` . * TO 'skyfire'@'localhost' WITH GRANT OPTION;

GRANT ALL PRIVILEGES ON `characters` . * TO 'skyfire'@'localhost' WITH GRANT OPTION;

GRANT ALL PRIVILEGES ON `auth` . * TO 'skyfire'@'localhost' WITH GRANT OPTION;

GRANT ALL PRIVILEGES ON `world` . * TO 'skyfire'@'127.0.0.1' WITH GRANT OPTION;

GRANT ALL PRIVILEGES ON `characters` . * TO 'skyfire'@'127.0.0.1' WITH GRANT OPTION;

GRANT ALL PRIVILEGES ON `auth` . * TO 'skyfire'@'127.0.0.1' WITH GRANT OPTION;
