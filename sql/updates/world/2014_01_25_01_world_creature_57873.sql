-- Training Target
SET @CGUID :=258001;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+5;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(@CGUID+0, 57873, 860, 1, 1, 1495.623, 3426.017, 171.0507, 2.844887, 120, 0, 0), -- 57873 (Area: -1)
(@CGUID+1, 57873, 860, 1, 1, 1437.839, 3415.378, 170.9208, 0.4712389, 120, 0, 0), -- 57873 (Area: -1)
(@CGUID+2, 57873, 860, 1, 1, 1490.865, 3444.753, 171.2159, 3.926991, 120, 0, 0), -- 57873 (Area: -1)
(@CGUID+3, 57873, 860, 1, 1, 1455.573, 3444.194, 171.0521, 4.206244, 120, 0, 0), -- 57873 (Area: -1)
(@CGUID+4, 57873, 860, 1, 1, 1455.573, 3444.194, 171.0521, 4.206244, 120, 0, 0), -- 57873 (Area: -1)
(@CGUID+5, 57873, 860, 1, 1, 1455.573, 3444.194, 171.0521, 4.206244, 120, 0, 0); -- 57873 (Area: -1)