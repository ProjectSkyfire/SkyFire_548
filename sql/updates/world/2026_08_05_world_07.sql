DELETE FROM `gameobject` WHERE `guid`=263874;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES (263874, 214513, 1011, 262, 0, 0, 1447.43, 5093.14, 144.047, 0.711748, 0, 0, 0, 1, 7200, 255, 1);

UPDATE `gameobject_template` SET `name`='Instance Portal (Party + Heroic + Challenge)' WHERE  `entry`=214513;

