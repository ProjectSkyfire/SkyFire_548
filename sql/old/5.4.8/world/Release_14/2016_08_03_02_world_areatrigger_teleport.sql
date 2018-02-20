DELETE FROM `areatrigger_teleport` WHERE `id` IN (7838, 7839, 8027, 8028, 8313, 8314, 8881, 8882);
INSERT INTO `areatrigger_teleport` (`id`, `name`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(7838, 'Mogu\'shan Vaults (Entrance)', 1008, 3861.55, 1045.11, 490.17, 0),
(7839, 'Mogu\'shan Vaults (Exit)', 870, 3982.09, 1111.19, 497.31, 0),
(8027, 'Terrace of Endless Spring (Entrance)', 996, -1020.99, -3145.79, 28.3346, 0),
(8028, 'Terrace of Endless Spring (Exit)', 870, 955.523, -55.8068, 512.152, 0),
(8313, 'Heart of Fear (Exit)', 870, 168.066, 4053.84, 256.013, 0),
(8314, 'Heart of Fear (Entrance)', 1009, -2378.92, 459.879, 422.441, 0),
(8881, 'Throne Of Thunder - Exit Target', 1064, 7241.18, 5038.31, 76.22, 2.339),
(8882, 'Throne Of Thunder - Entrance Target', 1098, 5892.45, 6610.35, 106.108, 4.5203);
