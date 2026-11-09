-- Darkshore: A New Home.
DELETE FROM `event_scripts` WHERE `id`=21678;
INSERT INTO `event_scripts` (`id`,`delay`,`command`,`datalong`,`datalong2`,`dataint`,`x`,`y`,`z`,`o`) VALUES
(21678,3,9,1061900,60,0,0,0,0,0),
(21678,5,8,34349,0,0,0,0,0,0);

DELETE FROM `gameobject` WHERE `guid`=1061900;
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseId`,`phaseGroup`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(1061900,195044,1,1,0,0,4690.67,695.279,1.15692,3.927,0,0,-0.923879,0.382686,-300,255,1);
