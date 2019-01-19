-- Krik'thik War Wagon (SPAWN) -- GUID GAPS TAKEN FROM LATEST SKYFIREDB
DELETE FROM creature WHERE guid IN (682, 683, 694);
INSERT INTO creature (guid, id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, MovementType) VALUES
(682, 64274, 870, 1, 1, 936.1059, 2384.063, 300.3779, 4.711033, 120, 0, 0),
(683, 64274, 870, 1, 1, 959.5573, 2386.632, 300.3779, 4.76486, 120, 0, 0),
(694, 64274, 870, 1, 1, 845.9809, 2403.641, 316.0761, 4.252653, 120, 0, 0);