-- Add RBAC entries for battle pet GM commands.

DELETE FROM `rbac_linked_permissions` WHERE `linkedId` BETWEEN 1000 AND 1003 OR `id` BETWEEN 1000 AND 1003;
DELETE FROM `rbac_permissions` WHERE `id` BETWEEN 1000 AND 1003;

INSERT INTO `rbac_permissions` (`id`, `name`) VALUES
(1000, 'Command: battlepet'),
(1001, 'Command: battlepet levelup'),
(1002, 'Command: battlepet heal'),
(1003, 'Command: battlepet debug');

INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES
(197, 1000),
(197, 1001),
(197, 1002),
(197, 1003);
