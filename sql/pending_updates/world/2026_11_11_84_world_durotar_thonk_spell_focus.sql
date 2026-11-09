-- Thonk's Spyglass needs spell focus 1657 within 20 yards, which the spot at the top of the Razor Hill watchtower provides.
-- Based on Legends of Azeroth 5.4.8.
DELETE FROM `gameobject_template` WHERE `entry` = 301050;
INSERT INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`size`,`data0`,`data1`) VALUES
(301050,8,299,'top of Razor Hill Watchtower',1,1657,20);

DELETE FROM `gameobject` WHERE `guid` = 1061910;
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseId`,`phaseGroup`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(1061910,301050,1,1,0,0,392.594,-4583.42,76.6358,1.24516,0,0,0.583135,0.812375,300,255,1);
