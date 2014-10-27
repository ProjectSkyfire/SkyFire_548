REVOKE ALL PRIVILEGES ON * . * FROM 'skyfire'@'localhost';

REVOKE ALL PRIVILEGES ON `world` . * FROM 'skyfire'@'localhost';

REVOKE GRANT OPTION ON `world` . * FROM 'skyfire'@'localhost';

REVOKE ALL PRIVILEGES ON `characters` . * FROM 'skyfire'@'localhost';

REVOKE GRANT OPTION ON `characters` . * FROM 'skyfire'@'localhost';

REVOKE ALL PRIVILEGES ON `auth` . * FROM 'skyfire'@'localhost';

REVOKE GRANT OPTION ON `auth` . * FROM 'skyfire'@'localhost';

DROP USER 'skyfire'@'localhost';

DROP DATABASE IF EXISTS `world`;

DROP DATABASE IF EXISTS `characters`;

DROP DATABASE IF EXISTS `auth`;
