-- Terrain Swap "Hellscream's Fist Gunship"
DELETE FROM terrain_swap_defaults WHERE MapId=1 AND TerrainSwapMap=1074;
INSERT INTO terrain_swap_defaults (MapId, TerrainSwapMap, Comment) VALUES
(1, 1074, 'Orgrimmar Gunship Pandaria Start');
