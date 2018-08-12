DELETE FROM `rbac_permissions` WHERE `id` IN (800, 801, 802);
INSERT INTO `rbac_permissions` (`id`, `name`) VALUES
(800, 'Command: .reload quest_objective'),
(801, 'Command: .reload quest_objective_effects'),
(802, 'Command: .reload locales_quest_objective');

DELETE FROM `rbac_linked_permissions` WHERE `id` = 196 AND `linkedId` IN (800, 801, 802);
INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES
(196, 800),
(196, 801),
(196, 802);
