-- Terrain Swap "Skyfire Gunship"
DELETE FROM terrain_swap_defaults WHERE MapId=0 AND TerrainSwapMap=1066;
INSERT INTO terrain_swap_defaults (MapId, TerrainSwapMap, Comment) VALUES
(0, 1066, 'Stormwind Gunship Pandaria Start');
