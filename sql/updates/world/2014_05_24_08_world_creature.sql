-- Wild Shaghorn (SPAWN) -- GUID GAPS TAKEN FROM LATEST SKYFIREDB
DELETE FROM creature WHERE guid IN (565, 634, 635);
INSERT INTO creature (guid, id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, MovementType) VALUES
(565, 58892, 870, 1, 1, 108.5632, 1861.42, 174.6885, 4.919823, 120, 0, 0),
(634, 58892, 870, 1, 1, -195.0864, 1825.854, 187.6959, 4.740001, 120, 0, 0),
(635, 58892, 870, 1, 1, -267.8545, 1671.208, 213.5157, 1.184596, 120, 0, 0);
