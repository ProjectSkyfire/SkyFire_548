-- Mist Incarnation (SPAWN) -- GUID GAPS TAKEN FROM LATEST SKYFIREDB
DELETE FROM creature WHERE guid IN (636, 637);
INSERT INTO creature (guid, id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, MovementType) VALUES
(636, 56740, 870, 1, 1, 178.774, 1980.985, 151.4092, 3.856375, 120, 0, 0),
(637, 56740, 870, 1, 1, 140.7752, 2008.367, 150.3346, 0.1608961, 120, 0, 0);
