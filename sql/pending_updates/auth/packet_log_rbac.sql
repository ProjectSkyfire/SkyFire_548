-- Add RBAC entries for packet logging GM commands.

DELETE FROM `rbac_linked_permissions` WHERE `linkedId` BETWEEN 1004 AND 1009 OR `id` BETWEEN 1004 AND 1009;
DELETE FROM `rbac_permissions` WHERE `id` BETWEEN 1004 AND 1009;

INSERT INTO `rbac_permissions` (`id`, `name`) VALUES
(1004, 'Command: packet'),
(1005, 'Command: packet start'),
(1006, 'Command: packet stop'),
(1007, 'Command: packet char'),
(1008, 'Command: packet char start'),
(1009, 'Command: packet char stop');

INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES
(197, 1004),
(197, 1005),
(197, 1006),
(197, 1007),
(197, 1008),
(197, 1009);
