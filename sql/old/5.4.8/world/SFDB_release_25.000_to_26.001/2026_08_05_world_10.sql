UPDATE `gameobject_template` SET `type`=23, `name`='Meeting Stone', `data0`=90, `data1`=255, `data2`=6622 WHERE `entry`=219216;

DELETE FROM `gameobject` WHERE `guid`=263879;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES (263879, 219216, 1064, 1, 0, 0, 7218.08, 5037.82, 76.1644, 4.29351, 0, 0, 0, 1, 120, 255, 1);
