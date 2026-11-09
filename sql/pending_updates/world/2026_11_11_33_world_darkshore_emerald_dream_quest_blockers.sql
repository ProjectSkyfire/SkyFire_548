-- Darkshore: Thessera and the Nightmare Guardians for The Emerald Dream, The Waking Nightmare and Leaving the Dream.
-- Based on Legends of Azeroth 5.4.8.
DELETE FROM `creature` WHERE `guid` BETWEEN 9255020 AND 9255022;
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseId`,`phaseGroup`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`) VALUES
(9255020,33166,1,1,0,0,0,0,5768.42,-164.477,13.8271,2.46091,300,0,0,0,0,0,0,0,0),
(9255021,34398,1,1,0,0,0,0,5821.96,-172.007,-3.14974,2.28414,300,3,0,0,0,1,0,0,0),
(9255022,34398,1,1,0,0,0,0,5839.25,-220.602,1.96119,0.372729,300,0,0,0,0,0,0,0,0);

UPDATE `creature_template` SET `minlevel`=17, `maxlevel`=17, `faction_A`=14, `faction_H`=14, `lootid`=34398 WHERE `entry`=34398;
DELETE FROM `creature_loot_template` WHERE `entry`=34398 AND `item`=46695;
INSERT INTO `creature_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
(34398,46695,-100,1,0,1,1);

