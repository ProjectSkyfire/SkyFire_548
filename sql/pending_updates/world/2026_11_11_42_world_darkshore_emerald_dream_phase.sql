-- Darkshore: copies of Thessera, the Nightmare Guardians and the dream guide in phase 170, so the Emerald Dream chain can be done by a player who holds the dream phase.
-- Based on Legends of Azeroth 5.4.8.
DELETE FROM `creature` WHERE `guid` BETWEEN 9255023 AND 9255026;
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseId`,`phaseGroup`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`) VALUES
(9255023,33091,1,1,170,0,0,0,5919.36,194.224,64.4445,0.645772,300,0,0,0,0,0,0,0,0),
(9255024,33166,1,1,170,0,0,0,5768.42,-164.477,13.8271,2.46091,300,0,0,0,0,0,0,0,0),
(9255025,34398,1,1,170,0,0,0,5821.96,-172.007,-3.14974,2.28414,300,3,0,0,0,1,0,0,0),
(9255026,34398,1,1,170,0,0,0,5839.25,-220.602,1.96119,0.372729,300,0,0,0,0,0,0,0,0);
