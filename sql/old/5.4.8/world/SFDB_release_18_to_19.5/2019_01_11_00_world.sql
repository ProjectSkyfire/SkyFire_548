-- Creature Spawns for Training Dummies on Sunstrider Isle.
SET @CGUID := 5572;
DELETE FROM creature WHERE id=44937;
DELETE FROM creature WHERE guid BETWEEN @CGUID-5 AND @CGUID-0;
INSERT INTO creature (guid, id, map, position_x, position_y, position_z, orientation, curhealth) VALUES
(@CGUID-0, 44937, 530, 10325, -6403.6, 37.23173, 2.932153, 0),
(@CGUID-1, 44937, 530, 10334.4, -6386.11, 36.51433, 2.391101, 0),
(@CGUID-2, 44937, 530, 10373.3, -6377.87, 37.28583, 1.291544, 0),
(@CGUID-3, 44937, 530, 10328.4, -6393.98, 37.14723, 2.635447, 0),
(@CGUID-4, 44937, 530, 10363.9, -6375.25, 37.01423, 1.58825, 0),
(@CGUID-5, 44937, 530, 10381.8, -6381.9, 37.83083, 0.9773844, 0);
