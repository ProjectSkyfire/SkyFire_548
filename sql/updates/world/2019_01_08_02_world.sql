-- Creature Spawns for Camp Sungraze in Mulgore.
SET @CGUID := 3043;
DELETE FROM creature WHERE guid BETWEEN @CGUID-5 AND @CGUID-0;
INSERT INTO creature (guid, id, map, spawnMask, phaseId, phaseGroup, position_x, position_y, position_z, orientation) VALUES
(@CGUID-0, 2957, 1, 1, 0, 0, -734.851, -505.565, -23.9306, 5.83003),
(@CGUID-1, 2987, 1, 1, 0, 0, -807.877, -498.523, -32.1909, 0),
(@CGUID-2, 3052, 1, 1, 0, 0, -794.766, -503.547, -32.1923, 1.65806),
(@CGUID-3, 3233, 1, 1, 0, 0, -789.854, -494.505, -32.1923, 3.33358),
(@CGUID-4, 10636, 1, 1, 0, 0, -808.7, -512.774, -31.948, 0),
(@CGUID-5, 37024, 1, 1, 0, 0, -798.271, -486.757, -32.1923, 1.62316);
