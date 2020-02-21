DELETE FROM `spell_target_position` WHERE `id`=125060;
INSERT INTO `spell_target_position` (`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
('125060', '1', '870', '3138.64', '-721.332', '324.9845', '0.38');

DELETE FROM `terrain_swap_defaults` WHERE `MapId`=870 AND `TerrainSwapMap`=1076;
INSERT INTO `terrain_swap_defaults` (`MapId`, `TerrainSwapMap`, `Comment`) VALUES
(870, 1076, 'Jade Forest Horde Starting Terrain');
