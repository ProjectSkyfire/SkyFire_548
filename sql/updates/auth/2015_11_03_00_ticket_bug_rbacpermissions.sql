DELETE FROM `rbac_permissions` WHERE `id` IN (777,778,779,780,781,782,783,784,785,786);
INSERT INTO `rbac_permissions` (`id`, `name`) VALUES
(777, 'Command: .ticket bug'),
(778, 'Command: .ticket bug assign'),
(779, 'Command: .ticket bug close'),
(780, 'Command: .ticket bug closedlist'),
(781, 'Command: .ticket bug comment'),
(782, 'Command: .ticket bug delete'),
(783, 'Command: .ticket bug list'),
(784, 'Command: .ticket bug unassign'),
(785, 'Command: .ticket bug view'),
(786, 'Command: .ticket bug reset');

DELETE FROM `rbac_linked_permissions` WHERE `id` = 198 AND `linkedId` IN (777,778,779,780,781,782,783,784,785,786);
INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES
(198, 777),
(198, 778),
(198, 779),
(198, 780),
(198, 781),
(198, 782),
(198, 783),
(198, 784),
(198, 785),
(198, 786);