-- Shao-Tien Soul-Caller (SPAWN) -- GUID GAPS TAKEN FROM LATEST SKYFIREDB
DELETE FROM creature WHERE guid IN (1001, 1002, 1003, 1004, 1005);
INSERT INTO creature (guid, id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, MovementType) VALUES
(1001, 63611, 870, 1, 1, 1306.271, 1637.281, 346.0056, 4.489841, 120, 0, 0),
(1002, 63611, 870, 1, 1, 1291.514, 1680.908, 348.4517, 3.819538, 120, 0, 0),
(1003, 63611, 870, 1, 1, 1385.582, 1691.684, 340.7757, 6.282299, 120, 0, 0),
(1004, 63611, 870, 1, 1, 1289.866, 1806.226, 344.901, 3.870127, 120, 0, 0),
(1005, 63611, 870, 1, 1, 1295.063, 1817.314, 344.8594, 2.545387, 120, 0, 0);