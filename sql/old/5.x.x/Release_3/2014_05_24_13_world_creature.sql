-- Shado-Pan Ranger (SPAWN) -- GUID GAPS TAKEN FROM LATEST SKYFIREDB
DELETE FROM creature WHERE guid IN (703, 835, 837, 842);
INSERT INTO creature (guid, id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, MovementType) VALUES
(703, 64646, 870, 1, 1, 951.5851, 2237.842, 296.2306, 1.595902, 120, 0, 0),
(835, 64646, 870, 1, 1, 955.0903, 2239.918, 296.2306, 1.595902, 120, 0, 0),
(837, 64646, 870, 1, 1, 961.5816, 2239.825, 296.2306, 1.595902, 120, 0, 0),
(842, 64646, 870, 1, 1, 965.3246, 2237.826, 296.2306, 1.595902, 120, 0, 0);