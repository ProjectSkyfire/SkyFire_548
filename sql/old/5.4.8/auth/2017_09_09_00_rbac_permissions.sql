DELETE FROM `rbac_permissions` WHERE `id` IN (805, 806, 807);
INSERT INTO `rbac_permissions` (`id`, `name`) VALUES
(805, 'Command: .account boost'),
(806, 'Command: .account boost add'),
(807, 'Command: .account boost delete');

DELETE FROM `rbac_linked_permissions` WHERE `id` = 192 AND `linkedId` IN (805, 806, 807);
INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES
(192, 805),
(192, 806),
(192, 807);
