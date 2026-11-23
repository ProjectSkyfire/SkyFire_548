-- Durotar: spawn Rezlak, the giver and ender of Winds in the Desert and Securing the Lines.
-- Based on Legends of Azeroth 5.4.8.
DELETE FROM `creature` WHERE `guid` BETWEEN 9255000 AND 9255000;
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseId`,`phaseGroup`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`) VALUES
(9255000,3293,1,1,0,0,0,1,999.763,-4414.4,14.4718,5.42797,300,0,0,0,0,0,0,0,0);
