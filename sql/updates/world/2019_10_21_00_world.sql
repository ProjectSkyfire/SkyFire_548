-- Creature Spawn: General Nazgrim
SET @CGUID := 1349;
DELETE FROM creature WHERE guid=@CGUID-0;
DELETE FROM creature WHERE id=54870;
INSERT INTO creature (guid, id, map, spawnmask, position_x, position_y, position_z, orientation, spawntimesecs) VALUES
(@CGUID-0, 54870, 1, 1, 1668.33, -4350.26, 26.3507, 2.83324, 300);
