-- Dustwallow Marsh: spawn Zelfrax, the kill target of Cleansing Witch Hill.
-- Based on Legends of Azeroth 5.4.8.
DELETE FROM `creature` WHERE `guid` BETWEEN 9255500 AND 9255500;
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseId`,`phaseGroup`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`) VALUES
(9255500,23864,1,1,0,0,0,0,-2965.46,-3873.29,33.988,5.37296,60,0,0,0,0,0,0,0,0);
UPDATE `creature_template` SET `unit_flags`=576 WHERE `entry`=23864;
