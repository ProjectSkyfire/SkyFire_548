DROP TABLE `account_access`;
CREATE TABLE `account_access` (
  `id` int unsigned NOT NULL,
  `gmlevel` tinyint unsigned NOT NULL,
  `RealmID` int NOT NULL DEFAULT '-1',
  PRIMARY KEY (`id`,`RealmID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;

DELETE FROM `rbac_permissions` WHERE `id`=228;
INSERT INTO `rbac_permissions` (`id`, `name`) VALUES (228, 'Command: account set gmlevel');

DELETE FROM `rbac_permissions` WHERE `id`=375;
INSERT INTO `rbac_permissions` (`id`, `name`) VALUES (375, 'Command: gm list');
