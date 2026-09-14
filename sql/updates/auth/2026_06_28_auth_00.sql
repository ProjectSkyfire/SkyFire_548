DELETE FROM `rbac_permissions` WHERE `id`=809;
INSERT INTO `rbac_permissions` (`id`, `name`) VALUES (809, 'Command: debug rest');

DELETE FROM `rbac_linked_permissions` WHERE `id`=198 AND `linkedId`=809;
INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES (198, 809);
