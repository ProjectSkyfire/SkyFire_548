-- RBAC entry for the battle pet NPC team battle command.

DELETE FROM `rbac_linked_permissions` WHERE `linkedId` = 1018;
DELETE FROM `rbac_permissions` WHERE `id` = 1018;

INSERT INTO `rbac_permissions` (`id`, `name`) VALUES
(1018, 'Command: battlepet npc');

INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES
(197, 1018);
