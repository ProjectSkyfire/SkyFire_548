INSERT INTO `rbac_permissions` (`id`, `name`) VALUES
(775, 'Command: modify currency')
ON DUPLICATE KEY UPDATE `name`=VALUES(`name`);

INSERT IGNORE INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES
(198, 775);
