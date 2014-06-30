-- Gilneas Default Terrain Map Swap
DELETE FROM phase_definitions WHERE zoneId=4755;
INSERT INTO phase_definitions (zoneId, entry, phasemask, phaseId, terrainswapmap, flags, comment) VALUES
(4755, 1, 0, 170, 638, 0, 'Gilneas: Default Terrainswap');
