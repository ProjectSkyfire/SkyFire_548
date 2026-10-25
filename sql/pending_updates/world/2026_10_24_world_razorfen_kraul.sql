-- Razorfen Kraul (map 47, dungeon, Classic): missing creatures, gameobjects, loot and scripts
-- Idempotent: every INSERT is preceded by a DELETE of its keys.

-- creature_template updates (difficulty slots, loot ids, SmartAI)
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4421 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4422 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4424 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4427 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4438 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4440 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4442 AND `ScriptName`='';
UPDATE `creature_template` SET `skinloot`=4514 WHERE `entry`=4514;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4516 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4517 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4520 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4522 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4523 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4525 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4530 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4531 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4532 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4623 AND `ScriptName`='';
UPDATE `creature_template` SET `pickpocketloot`=4625 WHERE `entry`=4625;
UPDATE `creature_template` SET `pickpocketloot`=4842 WHERE `entry`=4842;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4842 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=6035 AND `ScriptName`='';
UPDATE `creature_template` SET `pickpocketloot`=6168 WHERE `entry`=6168;

-- creature_template levels (normal difficulty)
UPDATE `creature_template` SET `minlevel`=33, `maxlevel`=34 WHERE `entry`=4438;
UPDATE `creature_template` SET `minlevel`=33, `maxlevel`=33 WHERE `entry`=4514;
UPDATE `creature_template` SET `minlevel`=33, `maxlevel`=34 WHERE `entry`=4517;
UPDATE `creature_template` SET `minlevel`=33, `maxlevel`=34 WHERE `entry`=4522;
UPDATE `creature_template` SET `minlevel`=33, `maxlevel`=34 WHERE `entry`=4523;
UPDATE `creature_template` SET `minlevel`=33, `maxlevel`=34 WHERE `entry`=4531;
UPDATE `creature_template` SET `minlevel`=34, `maxlevel`=34 WHERE `entry`=6035;

-- skinning_loot_template (1 loot ids)
DELETE FROM `skinning_loot_template` WHERE `entry` IN (4514);
INSERT INTO `skinning_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
(4514,783,5.0,1,1,1,1),
(4514,2318,40.0,1,1,1,2),
(4514,2319,0.0,1,1,1,2),
(4514,4232,5.0,1,1,1,1);

-- pickpocketing_loot_template (3 loot ids)
DELETE FROM `pickpocketing_loot_template` WHERE `entry` IN (4625,4842,6168);
INSERT INTO `pickpocketing_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
(4625,2287,9.0909,1,0,1,1),
(4625,5369,7.2727,1,0,1,1),
(4842,3771,4.0,1,0,1,1),
(4842,16883,21.4286,1,0,1,1),
(6168,929,7.1429,1,0,1,1),
(6168,3770,21.4286,1,0,1,1),
(6168,5375,14.2857,1,0,1,1),
(6168,16882,21.4286,1,0,1,1);

-- creature spawns
DELETE FROM `creature` WHERE `guid` IN (8203853,8203854,8203857,8203858,8203859,8203860,8203861,8203862,8203863,8203864,8203865,8203866);
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseId`,`phaseGroup`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`) VALUES
(8203853,4525,47,2,0,0,4647,0,2141.15,1931.89,67.333,2.83722,7200,0,0,3868,1007,0,0,0,0),
(8203854,4842,47,2,0,0,0,0,2118.58,1695.32,80.3316,1.46786,28800,0,0,8072,1067,0,0,0,0),
(8203857,4531,47,2,0,0,4453,0,2188.02,1905.97,70.8019,2.62629,7200,0,0,4652,0,2,0,0,0),
(8203858,4436,47,2,0,0,6108,0,2088.56,1550.67,81.383,5.03446,7200,0,0,4440,0,0,0,0,0),
(8203859,4532,47,2,0,0,6105,0,2035.35,1475.01,68.3324,2.78651,7200,0,0,4652,0,0,0,0,0),
(8203860,4532,47,2,0,0,6105,0,2216.94,1604.38,81.3396,0.883549,7200,0,0,4652,0,0,0,0,0),
(8203861,6035,47,2,0,0,6106,0,2184.33,1480.96,72.8359,0.993872,7200,0,0,4440,0,0,0,0,0),
(8203862,6035,47,2,0,0,6106,0,2126.28,1492.22,73.0666,-2.84361,7200,0,0,4440,0,0,0,0,0),
(8203863,44401,47,2,0,0,4588,0,1945.06,1562.81,82.3541,1.18682,7200,0,0,1057,0,0,0,0,0),
(8203864,44415,47,2,0,0,33857,0,2194.63,1911.18,71.0248,2.89725,7200,0,0,39625,0,0,0,0,0),
(8203865,44402,47,2,0,0,4568,0,1942.24,1556.64,81.5848,5.3058,7200,0,0,1221,0,0,0,0,0),
(8203866,4522,47,2,0,0,6110,0,2188.08,1536.19,72.055,6.20646,7200,0,0,3712,963,0,0,0,0);

-- rare spawn pools
DELETE FROM `creature` WHERE `guid` IN (8203867,8203868);
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseId`,`phaseGroup`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`) VALUES
(8203867,4539,47,2,0,0,0,0,2203.29,1980.04,56.9565,3.70469,7200,0,0,5815,0,0,0,0,0),
(8203868,4623,47,2,0,0,0,0,2206.6,1640.77,85.8901,5.03572,7200,0,0,4652,0,0,0,0,0);
DELETE FROM `pool_template` WHERE `entry` IN (202484,202485,202486);
INSERT INTO `pool_template` (`entry`,`max_limit`,`description`) VALUES
(202484,1,'Razorfen Kraul - Blind Hunter'),
(202485,1,'Razorfen Kraul - Razorfen Spearhide'),
(202486,1,'Razorfen Kraul - Earthcaller Halmgar');
DELETE FROM `pool_creature` WHERE `guid` IN (87485,8203867,87387,8203868,87327,8203854);
INSERT INTO `pool_creature` (`guid`,`pool_entry`,`chance`,`description`) VALUES
(87485,202484,6,'Razorfen Kraul - Blind Hunter'),
(8203867,202484,0,'Razorfen Kraul - Greater Kraul Bat'),
(87387,202485,12,'Razorfen Kraul - Razorfen Spearhide'),
(8203868,202485,0,'Razorfen Kraul - Quilguard Champion'),
(87327,202486,0,'Razorfen Kraul - Razorfen Earthbreaker'),
(8203854,202486,12,'Razorfen Kraul - Earthcaller Halmgar');

-- creature_addon
DELETE FROM `creature_addon` WHERE `guid` IN (8203853,8203857,8203858,8203859,8203860,8203861,8203862,8203863,8203864,8203865,8203866);
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES
(8203853,0,0,0,1,0,''),
(8203857,82038570,0,0,1,0,''),
(8203858,0,0,0,1,0,''),
(8203859,0,0,0,1,0,''),
(8203860,0,0,0,1,0,''),
(8203861,0,0,131072,1,0,'22766'),
(8203862,0,0,0,1,0,''),
(8203863,0,0,0,1,0,''),
(8203864,0,0,0,1,0,'82971'),
(8203865,0,0,0,1,0,''),
(8203866,0,0,0,1,0,'');

-- waypoint_data
DELETE FROM `waypoint_data` WHERE `id` IN (82038570);
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_flag`,`action`,`action_chance`,`wpguid`) VALUES
(82038570,1,2192.38,1903.5,71.0586,0.0,0,0,0,100,0),
(82038570,2,2193.25,1903.0,71.0586,0.0,0,0,0,100,0),
(82038570,3,2190.9,1904.34,71.086,0.0,0,0,0,100,0),
(82038570,4,2183.02,1908.8,70.3085,0.0,0,0,0,100,0),
(82038570,5,2180.77,1910.07,70.0415,0.0,0,0,0,100,0),
(82038570,6,2179.25,1910.94,69.8276,0.0,0,0,0,100,0),
(82038570,7,2179.25,1910.94,69.8276,0.0,0,0,0,100,0);

-- gameobject spawns
DELETE FROM `gameobject` WHERE `guid` IN (901745);
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseId`,`phaseGroup`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(901745,1733,47,2,0,0,2194.74,1990.35,56.5626,0.244345,0.0,0.0,0.121869,0.992546,1200,100,1);

-- smart_scripts
DELETE FROM `smart_scripts` WHERE `entryorguid`=4421 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4422 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4424 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4427 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4438 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4440 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4442 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4516 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4517 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4520 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4522 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4523 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4525 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4530 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4531 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4532 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4623 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4842 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=6035 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(4421,0,0,0,4,0,100,5,0,0,0,0,0,22,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Set Phase 1 on Aggro'),
(4421,0,1,0,4,1,100,5,0,0,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Stop Moving on Aggro'),
(4421,0,2,0,4,1,100,5,0,0,0,0,0,11,8292,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Cast bolt on Aggro'),
(4421,0,3,0,9,1,100,4,0,40,3400,4700,0,11,8292,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Cast bolt'),
(4421,0,4,0,9,1,100,4,40,100,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Start Moving when not in bolt Range'),
(4421,0,5,0,9,1,100,4,10,15,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Stop Moving at 15 Yards'),
(4421,0,6,0,9,1,100,4,0,40,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Stop Moving when in bolt Range'),
(4421,0,7,0,3,1,100,4,0,15,0,0,0,22,2,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Set Phase 2 at 15% Mana'),
(4421,0,8,0,3,2,100,4,0,15,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Start Moving at 15% Mana'),
(4421,0,9,0,3,2,100,4,30,100,100,100,0,22,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Set Phase 1 When Mana is above 30%'),
(4421,0,10,0,0,1,100,4,9000,13000,9000,15000,0,11,8361,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Purity'),
(4421,0,11,0,14,1,100,4,2000,40,15000,21000,0,11,25058,0,0,0,0,0,7,0,0,0,0.0,0.0,0.0,0.0,'Cast Renew on Friendlies'),
(4421,0,12,0,6,1,100,5,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Say Text on Death'),
(4421,0,13,0,3,2,100,4,0,10,0,0,0,11,8358,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Mana Spike at 15% Mana'),
(4421,0,14,0,2,1,100,5,0,50,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Say Text at 50% HP'),
(4421,0,15,0,4,1,100,5,0,0,0,0,0,1,2,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Say Text on Aggro'),
(4421,0,16,0,2,1,100,5,0,60,0,0,0,1,3,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Say Text at 60% HP'),
(4422,0,0,1,4,0,100,5,0,0,0,0,0,11,8260,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Rushing Charge on Aggro'),
(4422,0,1,3,61,0,100,5,0,0,0,0,0,39,15,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Call for Help on Aggro'),
(4422,0,2,0,2,0,100,5,0,40,0,0,0,11,8269,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Frenzy at 40% HP'),
(4422,0,3,0,2,0,100,5,0,40,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Say Text at 40% HP'),
(4422,0,4,0,2,0,100,5,0,60,0,0,0,11,8269,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Frenzy at 60% HP'),
(4422,0,5,0,2,0,100,5,0,60,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Say Text at 60% HP'),
(4422,0,6,0,9,0,100,4,0,5,21000,35000,0,11,8555,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Cast Left for Dead on Close'),
(4422,0,7,0,0,0,100,4,8000,14000,25000,31000,0,11,8285,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Rampage'),
(4424,0,0,0,2,0,100,4,0,75,13000,18000,0,11,14900,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Chain Heal at 75% HP'),
(4424,0,1,0,0,0,100,4,9000,15000,30000,45000,0,11,8286,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Summon Boar Spirit'),
(4424,0,2,0,0,0,100,4,1000,3000,40000,60000,0,11,30931,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Battle Shout'),
(4427,0,0,0,14,0,100,4,1000,40,13000,18000,0,11,12491,0,0,0,0,0,7,0,0,0,0.0,0.0,0.0,0.0,'Cast Healing Wave on Friendlies'),
(4438,0,0,0,0,0,100,4,4000,7000,7000,11000,0,11,8259,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Whirling Barrage'),
(4438,0,1,0,0,0,100,4,1000,3000,60000,70000,0,11,8148,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Thorns Aura'),
(4440,0,0,0,9,0,100,4,0,10,30000,35000,0,11,8376,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Earthgrab Totem on Close'),
(4440,0,1,0,0,0,100,4,9000,14000,30000,40000,0,11,4971,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Healing Ward'),
(4442,0,0,0,9,0,100,4,0,5,12000,17000,0,11,11972,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Cast Shield Bash on Close'),
(4442,0,1,0,0,0,100,4,1000,3000,12000,18000,0,11,3248,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Improved Blocking'),
(4516,0,0,0,0,0,100,4,0,0,2400,3800,0,11,9672,64,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Death\'s Head Adept - In Combat CMC - Cast \'Frostbolt\' (Normal Dungeon)'),
(4516,0,1,0,0,0,100,4,3000,6000,10000,15000,0,11,113,1,0,0,0,0,5,0,0,0,0.0,0.0,0.0,0.0,'Death\'s Head Adept - In Combat - Cast \'Chains of Ice\' (Normal Dungeon)'),
(4516,0,2,0,2,0,100,5,0,15,0,0,0,25,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Death\'s Head Adept - Between 0-15% Health - Flee For Assist (No Repeat) (Normal Dungeon)'),
(4517,0,1,0,0,0,100,4,0,0,2400,3800,0,11,9613,64,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Death\'s Head Priest - In Combat CMC - Cast \'Shadow Bolt\' (No Repeat) (Normal Dungeon)'),
(4517,0,3,0,2,0,100,5,0,15,0,0,0,25,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Death\'s Head Priest - Between 0-15% Health - Flee For Assist (No Repeat) (Normal Dungeon)'),
(4520,0,0,0,4,0,100,5,0,0,0,0,0,22,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Set Phase 1 on Aggro'),
(4520,0,1,0,4,1,100,5,0,0,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Stop Moving on Aggro'),
(4520,0,2,0,4,1,100,5,0,0,0,0,0,11,20824,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Cast bolt on Aggro'),
(4520,0,3,0,9,1,100,4,0,40,3400,4700,0,11,20824,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Cast bolt'),
(4520,0,4,0,9,1,100,4,40,100,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Start Moving when not in bolt Range'),
(4520,0,5,0,9,1,100,4,10,15,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Stop Moving at 15 Yards'),
(4520,0,6,0,9,1,100,4,0,40,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Stop Moving when in bolt Range'),
(4520,0,7,0,3,1,100,4,0,15,0,0,0,22,2,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Set Phase 2 at 15% Mana'),
(4520,0,8,0,3,2,100,4,0,15,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Start Moving at 15% Mana'),
(4520,0,9,0,3,2,100,4,30,100,100,100,0,22,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Set Phase 1 When Mana is above 30%'),
(4520,0,10,0,2,2,100,5,0,15,0,0,0,22,3,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Set Phase 3 at 15% HP'),
(4520,0,11,0,2,3,100,5,0,15,0,0,0,25,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Flee at 15% HP'),
(4520,0,12,0,7,3,100,5,0,0,0,0,0,22,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Reset on Evade'),
(4520,0,13,0,2,3,100,5,0,15,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Say Text at 15% HP'),
(4520,0,14,0,1,0,100,5,1000,1000,0,0,0,11,8270,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Summon Earth Rumbler OOC'),
(4522,0,0,0,11,0,100,5,0,0,0,0,0,11,8271,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Summon Wind Howler on Spawn'),
(4522,0,1,0,9,0,100,4,0,30,13000,16000,0,11,6728,0,0,0,0,0,5,0,0,0,0.0,0.0,0.0,0.0,'Cast Enveloping Winds on Close'),
(4523,0,0,0,9,0,100,4,0,25,13000,18000,0,11,13281,0,0,0,0,0,5,0,0,0,0.0,0.0,0.0,0.0,'Cast Earth Shock on Close'),
(4523,0,1,0,9,0,100,4,0,10,9000,12000,0,11,6524,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Ground Tremor on Close'),
(4525,0,0,0,9,0,100,4,0,25,13000,18000,0,11,15501,0,0,0,0,0,5,0,0,0,0.0,0.0,0.0,0.0,'Cast Earth Shock on Close'),
(4525,0,1,0,9,0,100,4,0,30,20000,35000,0,11,8272,0,0,0,0,0,5,0,0,0,0.0,0.0,0.0,0.0,'Cast Mind Tremor on Close'),
(4530,0,0,0,4,0,100,5,0,0,0,0,0,22,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Set Phase 1 on Aggro'),
(4530,0,1,0,4,1,100,5,0,0,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Stop Moving on Aggro'),
(4530,0,2,0,4,1,100,5,0,0,0,0,0,11,6660,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Cast Shoot on Aggro'),
(4530,0,3,0,9,1,100,4,5,30,3500,4100,0,11,6660,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Cast Shoot'),
(4530,0,4,0,9,1,100,4,30,100,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Start Moving when not in Shoot Range'),
(4530,0,5,0,9,1,100,4,9,15,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Stop Moving at 15 Yards'),
(4530,0,6,0,9,1,100,4,0,5,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Start Moving when not in Shoot Range'),
(4530,0,7,0,9,1,100,4,5,30,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Stop Moving when in Shoot Range'),
(4530,0,8,0,2,1,100,5,0,15,0,0,0,22,2,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'At 15% HP - Set Phase 2'),
(4530,0,9,0,2,2,100,5,0,15,0,0,0,25,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Flee at 15% HP'),
(4530,0,10,0,7,2,100,5,0,0,0,0,0,22,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Reset on Evade'),
(4530,0,11,0,2,2,100,5,0,15,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Say Text at 15% HP'),
(4530,0,12,0,11,0,100,5,0,0,0,0,0,11,8274,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Summon Tamed Battleboar on Spawn'),
(4531,0,0,0,4,0,100,5,0,0,0,0,0,22,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Set Phase 1 on Aggro'),
(4531,0,1,0,4,1,100,5,0,0,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Stop Moving on Aggro'),
(4531,0,2,0,4,1,100,5,0,0,0,0,0,11,6660,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Cast Shoot on Aggro'),
(4531,0,3,0,9,1,100,4,5,30,3500,4100,0,11,6660,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Cast Shoot'),
(4531,0,4,0,9,1,100,4,30,100,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Start Moving when not in Shoot Range'),
(4531,0,5,0,9,1,100,4,9,15,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Stop Moving at 15 Yards'),
(4531,0,6,0,9,1,100,4,0,5,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Start Moving when not in Shoot Range'),
(4531,0,7,0,9,1,100,4,5,30,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Stop Moving when in Shoot Range'),
(4531,0,8,0,2,1,100,5,0,15,0,0,0,22,2,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'At 15% HP - Set Phase 2'),
(4531,0,9,0,2,2,100,5,0,15,0,0,0,25,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Flee at 15% HP'),
(4531,0,10,0,7,2,100,5,0,0,0,0,0,22,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Reset on Evade'),
(4531,0,11,0,2,2,100,5,0,15,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Say Text at 15% HP'),
(4531,0,12,0,11,0,100,5,0,0,0,0,0,11,8274,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Summon Tamed Battleboar on Spawn'),
(4531,0,13,0,0,1,100,4,6000,9000,9000,14000,0,11,6984,0,0,0,0,0,5,0,0,0,0.0,0.0,0.0,0.0,'Cast Frost Shot'),
(4532,0,0,0,4,0,100,5,0,0,0,0,0,22,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Set Phase 1 on Aggro'),
(4532,0,1,0,4,1,100,5,0,0,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Stop Moving on Aggro'),
(4532,0,2,0,4,1,100,5,0,0,0,0,0,11,6660,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Cast Shoot on Aggro'),
(4532,0,3,0,9,1,100,4,5,30,3500,4100,0,11,6660,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Cast Shoot'),
(4532,0,4,0,9,1,100,4,30,100,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Start Moving when not in Shoot Range'),
(4532,0,5,0,9,1,100,4,9,15,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Stop Moving at 15 Yards'),
(4532,0,6,0,9,1,100,4,0,5,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Start Moving when not in Shoot Range'),
(4532,0,7,0,9,1,100,4,5,30,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Stop Moving when in Shoot Range'),
(4532,0,8,0,2,1,100,5,0,15,0,0,0,22,2,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'At 15% HP - Set Phase 2'),
(4532,0,9,0,2,2,100,5,0,15,0,0,0,25,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Flee at 15% HP'),
(4532,0,10,0,7,2,100,5,0,0,0,0,0,22,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Reset on Evade'),
(4532,0,11,0,2,2,100,5,0,15,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Say Text at 15% HP'),
(4532,0,12,0,11,0,100,5,0,0,0,0,0,11,8276,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Summon Tamed Hyena on Spawn'),
(4532,0,13,0,0,1,100,4,5000,8000,12000,18000,0,11,8275,0,0,0,0,0,5,0,0,0,0.0,0.0,0.0,0.0,'Cast Poisoned Shot'),
(4623,0,0,0,0,0,100,4,1000,3000,240000,240000,0,11,8258,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Devotion Aura'),
(4623,0,1,0,4,0,100,5,0,0,0,0,0,11,7164,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Defensive Stance on Aggro'),
(4623,0,2,0,0,0,100,4,4000,4000,9000,13000,0,11,15572,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Cast Sunder Armor'),
(4842,0,0,0,4,0,100,5,0,0,0,0,0,22,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Set Phase 1 on Aggro'),
(4842,0,1,0,4,1,100,5,0,0,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Stop Moving on Aggro'),
(4842,0,2,0,4,1,100,5,0,0,0,0,0,11,20824,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Cast bolt on Aggro'),
(4842,0,3,0,9,1,100,4,0,40,3400,4700,0,11,20824,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Cast bolt'),
(4842,0,4,0,9,1,100,4,40,100,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Start Moving when not in bolt Range'),
(4842,0,5,0,9,1,100,4,10,15,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Stop Moving at 15 Yards'),
(4842,0,6,0,9,1,100,4,0,40,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Stop Moving when in bolt Range'),
(4842,0,7,0,3,1,100,4,0,15,0,0,0,22,2,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Set Phase 2 at 15% Mana'),
(4842,0,8,0,3,2,100,4,0,15,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Start Moving at 15% Mana'),
(4842,0,9,0,3,2,100,4,30,100,100,100,0,22,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Set Phase 1 When Mana is above 30%'),
(4842,0,10,0,1,0,100,5,1000,1000,0,0,0,11,8270,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Summon Earth Rumbler Pet OOC'),
(4842,0,11,0,0,1,100,4,5000,5000,19000,22000,0,11,2484,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Earthbind Totem'),
(6035,0,0,0,11,0,100,5,0,0,0,0,0,11,22766,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Cast Sneak on Spawn'),
(6035,0,1,0,67,0,100,4,3900,6900,0,0,0,11,7159,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Casts Backstab');

-- creature_text
DELETE FROM `creature_text` WHERE `entry` IN (4420,4421,4422,4514,4520,4530,4531,4532,4781);
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(4420,0,0,'Victory! For Agamaggan!',14,0,100.0,0,0,5812,'Overlord Ramtusk'),
(4421,0,0,'Troublesome whelps. I\'ll teach you to interfere!',14,0,100.0,0,0,5813,'Charlga Razorflank - on initial aggro'),
(4421,1,0,'Who\'s next?',14,0,100.0,0,0,5814,'Charlga Razorflank - killing a player'),
(4421,2,0,'You outsiders will pay for encroaching on our land!',14,0,100.0,0,0,5815,'Charlga Razorflank - on 75% hp reached'),
(4421,3,0,'Bah! My power rules here!',14,0,100.0,0,0,5816,'Charlga Razorflank - on 50% hp reached'),
(4421,4,0,'Our new allies will avenge us!',14,0,100.0,0,0,5818,'Charlga Razorflank - on 25% hp reached'),
(4422,0,0,'%s goes into a frenzy!',16,0,100.0,0,0,0,'Agathelos the Raging'),
(4514,0,0,'%s goes into a frenzy!',16,0,100.0,0,0,0,'Agathelos the Raging'),
(4520,0,0,'%s attempts to run away in fear!',12,0,100.0,0,0,0,'combat Flee'),
(4530,0,0,'%s attempts to run away in fear!',12,0,100.0,0,0,0,'combat Flee'),
(4531,0,0,'%s attempts to run away in fear!',12,0,100.0,0,0,0,'combat Flee'),
(4532,0,0,'%s attempts to run away in fear!',12,0,100.0,0,0,0,'combat Flee'),
(4781,0,0,'%s nervously crawls from the crate.',16,0,100.0,0,0,0,'Snufflenose Gopher'),
(4781,1,0,'%s wiggles his whiskers at $n.',16,0,100.0,0,0,0,'Snufflenose Gopher'),
(4781,2,0,'%s sniffs at the ground...',16,0,100.0,0,0,0,'Snufflenose Gopher');

-- creature_template_addon
DELETE FROM `creature_template_addon` WHERE `entry` IN (4842,6021,44401,44402,44415);
INSERT INTO `creature_template_addon` (`entry`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES
(4842,0,0,0,1,0,''),
(6021,0,0,0,1,0,''),
(44401,0,0,0,1,0,''),
(44402,0,0,0,1,0,''),
(44415,0,0,0,1,0,'82971');
