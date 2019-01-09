SET @GGUID := 70100;
DELETE FROM gameobject WHERE guid BETWEEN @GGUID-1 AND @GGUID-0;
INSERT INTO gameobject (guid, id, map, spawnMask, phaseId, phaseGroup, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state) VALUES
(@GGUID-0, 2904, 1, 1, 0, 0, -753.256, -149.158, -29.0568, 3.08918, 0, 0, 0, 1, 120, 255, 1),
(@GGUID-1, 2909, 1, 1, 0, 0, -753.649, -149.057, -29.6249, -3.14159, 0, 0, 0, 1, 120, 255, 1);
