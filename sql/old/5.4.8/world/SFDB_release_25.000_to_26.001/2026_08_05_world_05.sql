DELETE FROM `gameobject` WHERE `guid`=263872;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES (263872, 214525, 996, 248, 0, 0, -1021.25, -3157.25, 30.7474, 1.5719, 0, 0, 0, 1, 7200, 255, 1);

UPDATE `gameobject_template` SET `type`=31, `name`='Instance Portal (Raid 4 Difficulties)', `data0`=2, `data1`=216, `data2`=217, `data3`=216, `data4`=217, `data5`=11471 WHERE  `entry`=214525;
