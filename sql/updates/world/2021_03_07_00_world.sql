SET @OGUID := 20282;
DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID-2 AND @OGUID-0;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(@OGUID-0, 226777, 609, 1, 0, 0, 2427.28, -5544.45, 420.863, -0.983229, 0, 0, 0.292372, 0.956305, 180, 0, 1),
(@OGUID-1, 226777, 609, 1, 0, 0, 2509.31, -5560.39, 420.863, -2.55402, 0, 0, 0.292372, 0.956305, 180, 0, 1),
(@OGUID-2, 226777, 609, 1, 0, 0, 2493.37, -5642.43, 420.863, 2.16421, 0, 0, 0.292372, 0.956305, 180, 0, 1);

UPDATE gameobject_template SET type=8, name='Runeforge', flags=flags|0x00000020, size=2.03, data0=1552, data1=15 WHERE entry=226777;
