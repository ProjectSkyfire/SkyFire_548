DELETE FROM `terrain_swap_defaults` WHERE `TerrainSwapMap` IN (1061, 1062);
INSERT INTO `terrain_swap_defaults` (`MapId`, `TerrainSwapMap`, `Comment`) VALUES
(870, 1061, 'Krasarang Wilds - Domination Point'),
(870, 1062, 'Krasarang Wilds - Lion''s Landing');
