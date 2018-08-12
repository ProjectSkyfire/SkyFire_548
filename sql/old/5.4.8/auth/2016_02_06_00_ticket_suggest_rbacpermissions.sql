DELETE FROM `rbac_permissions` WHERE `id` IN (787,788,789,790,791,792,793,794,795,796);
INSERT INTO `rbac_permissions` (`id`, `name`) VALUES
(787, 'Command: .ticket suggest'),
(788, 'Command: .ticket suggest assign'),
(789, 'Command: .ticket suggest close'),
(790, 'Command: .ticket suggest closedlist'),
(791, 'Command: .ticket suggest comment'),
(792, 'Command: .ticket suggest delete'),
(793, 'Command: .ticket suggest list'),
(794, 'Command: .ticket suggest unassign'),
(795, 'Command: .ticket suggest view'),
(796, 'Command: .ticket suggest reset');

DELETE FROM `rbac_linked_permissions` WHERE `id` = 198 AND `linkedId` IN (787,788,789,790,791,792,793,794,795,796);
INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES
(198, 787),
(198, 788),
(198, 789),
(198, 790),
(198, 791),
(198, 792),
(198, 793),
(198, 794),
(198, 795),
(198, 796);