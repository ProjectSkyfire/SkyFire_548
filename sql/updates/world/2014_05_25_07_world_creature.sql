-- Golden Lotus Guard (SPAWN) -- GUID GAPS TAKEN FROM LATEST SKYFIREDB
DELETE FROM creature WHERE guid IN (1087, 1088, 1089, 1090, 1091);
INSERT INTO creature (guid, id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, MovementType) VALUES
(1087, 63939, 870, 1, 1, 1257.477, 1705.924, 359.0617, 1.886647, 120, 0, 0), -- Golden Lotus Guard
(1088, 63939, 870, 1, 1, 1248.74, 1739.472, 364.5992, 2.24911, 120, 0, 0), -- Golden Lotus Guard
(1089, 63939, 870, 1, 1, 1242.293, 1735.167, 364.5992, 2.049128, 120, 0, 0), -- Golden Lotus Guard
(1090, 63939, 870, 1, 1, 1254.988, 1773, 346.2802, 4.025175, 120, 0, 0), -- Golden Lotus Guard
(1091, 63939, 870, 1, 1, 1213.052, 1748.385, 364.6724, 0.9247593, 120, 0, 0); -- Golden Lotus Guard