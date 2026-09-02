-- mod-prabowow: RBAC permissions for the player commands .xp / .xp rate / .chat.
-- Ids mirror modules/mod-prabowow/src/PraboWoWConfig.h. Linked to role 199
-- ("Role: Player Commands"), which every security level inherits.

INSERT INTO `rbac_permissions` (`id`, `name`) VALUES
(1100, 'Command: xp'),
(1101, 'Command: xp rate'),
(1102, 'Command: chat')
ON DUPLICATE KEY UPDATE `name`=VALUES(`name`);

INSERT IGNORE INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES
(199, 1100),
(199, 1101),
(199, 1102);
