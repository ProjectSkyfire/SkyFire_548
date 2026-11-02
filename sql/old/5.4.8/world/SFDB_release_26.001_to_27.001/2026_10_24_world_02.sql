-- Utgarde Keep (map 574, dungeon, WotLK): missing creatures, gameobjects, loot and scripts
-- Idempotent: every INSERT is preceded by a DELETE of its keys.

-- creature_template updates (difficulty slots, loot ids, SmartAI)
UPDATE `creature_template` SET `pickpocketloot`=23956, `AIName`='SmartAI' WHERE `entry`=23956 AND `ScriptName`='';
UPDATE `creature_template` SET `pickpocketloot`=23960 WHERE `entry`=23960;
UPDATE `creature_template` SET `pickpocketloot`=23961 WHERE `entry`=23961;
UPDATE `creature_template` SET `pickpocketloot`=24071 WHERE `entry`=24071;
UPDATE `creature_template` SET `pickpocketloot`=24078, `AIName`='SmartAI' WHERE `entry`=24078 AND `ScriptName`='';
UPDATE `creature_template` SET `pickpocketloot`=24079 WHERE `entry`=24079;
UPDATE `creature_template` SET `pickpocketloot`=24080 WHERE `entry`=24080;
UPDATE `creature_template` SET `pickpocketloot`=24082, `AIName`='SmartAI' WHERE `entry`=24082 AND `ScriptName`='';
UPDATE `creature_template` SET `pickpocketloot`=24084 WHERE `entry`=24084;
UPDATE `creature_template` SET `pickpocketloot`=24085 WHERE `entry`=24085;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=24849 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=28410 AND `ScriptName`='';

-- pickpocketing_loot_template (10 loot ids)
DELETE FROM `pickpocketing_loot_template` WHERE `entry` IN (23956,23960,23961,24071,24078,24079,24080,24082,24084,24085);
INSERT INTO `pickpocketing_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
(23956,22829,5.0,1,0,1,1),
(23956,27854,8.0769,1,0,1,1),
(23956,27855,6.5385,1,0,1,1),
(23956,29569,18.0769,1,0,1,1),
(23956,29570,20.3846,1,0,1,1),
(23956,33447,3.8462,1,0,1,1),
(23956,35953,5.7692,1,0,1,1),
(23956,36862,1.9231,1,0,1,1),
(23956,38260,16.1538,1,0,1,1),
(23956,40202,6.5385,1,0,1,1),
(23956,43575,17.6923,1,0,1,1),
(23960,22829,3.9514,1,0,1,1),
(23960,27854,7.9027,1,0,1,1),
(23960,27855,7.5988,1,0,1,1),
(23960,29569,23.1003,1,0,1,1),
(23960,29570,19.4529,1,0,1,1),
(23960,33447,4.5593,1,0,1,1),
(23960,35953,4.8632,1,0,1,1),
(23960,36862,0.304,1,0,1,1),
(23960,38260,19.1489,1,0,1,1),
(23960,40202,5.1672,1,0,1,1),
(23960,43575,14.8936,1,0,1,1),
(23961,22829,7.6046,1,0,1,1),
(23961,27854,10.6464,1,0,1,1),
(23961,27855,6.0837,1,0,1,1),
(23961,29569,23.5741,1,0,1,1),
(23961,29570,23.1939,1,0,1,1),
(23961,33447,6.0837,1,0,1,1),
(23961,35953,3.8023,1,0,1,1),
(23961,36862,0.5935,1,0,1,1),
(23961,38260,16.3498,1,0,1,1),
(23961,40202,4.943,1,0,1,1),
(23961,43575,11.4068,1,0,1,1),
(24071,27854,4.7619,1,0,1,1),
(24071,27855,6.3492,1,0,1,1),
(24071,29569,11.1111,1,0,1,1),
(24071,29570,28.5714,1,0,1,1),
(24071,33447,9.5238,1,0,1,1),
(24071,35953,7.9365,1,0,1,1),
(24071,38260,22.2222,1,0,1,1),
(24071,40202,4.7619,1,0,1,1),
(24071,43575,15.873,1,0,1,1),
(24078,22829,3.8462,1,0,1,1),
(24078,27854,7.6923,1,0,1,1),
(24078,27855,8.8462,1,0,1,1),
(24078,29569,26.1538,1,0,1,1),
(24078,29570,24.6154,1,0,1,1),
(24078,33447,5.0,1,0,1,1),
(24078,35953,5.3846,1,0,1,1),
(24078,36862,1.1538,1,0,1,1),
(24078,38260,11.5385,1,0,1,1),
(24078,40202,4.2308,1,0,1,1),
(24078,43575,11.1538,1,0,1,1),
(24079,22829,1.8987,1,0,1,1),
(24079,27854,9.4937,1,0,1,1),
(24079,27855,6.962,1,0,1,1),
(24079,29569,18.3544,1,0,1,1),
(24079,29570,19.6203,1,0,1,1),
(24079,33447,3.7975,1,0,1,1),
(24079,35953,6.962,1,0,1,1),
(24079,36862,1.2658,1,0,1,1),
(24079,38260,17.7215,1,0,1,1),
(24079,40202,6.3291,1,0,1,1),
(24079,43575,16.4557,1,0,1,1),
(24080,22829,5.7692,1,0,1,1),
(24080,27854,6.1538,1,0,1,1),
(24080,27855,8.8462,1,0,1,1),
(24080,29569,18.8462,1,0,1,1),
(24080,29570,26.5385,1,0,1,1),
(24080,33447,6.1538,1,0,1,1),
(24080,35953,6.1538,1,0,1,1),
(24080,36862,0.3846,1,0,1,1),
(24080,38260,15.3846,1,0,1,1),
(24080,40202,4.6154,1,0,1,1),
(24080,43575,12.3077,1,0,1,1),
(24082,22829,3.3835,1,0,1,1),
(24082,27854,9.3985,1,0,1,1),
(24082,27855,6.015,1,0,1,1),
(24082,29569,18.0451,1,0,1,1),
(24082,29570,20.6767,1,0,1,1),
(24082,33447,6.391,1,0,1,1),
(24082,35953,5.2632,1,0,1,1),
(24082,36862,1.1278,1,0,1,1),
(24082,38260,18.0451,1,0,1,1),
(24082,40202,6.015,1,0,1,1),
(24082,43575,14.6617,1,0,1,1),
(24084,22829,7.5472,1,0,1,1),
(24084,27859,28.3019,1,0,1,1),
(24084,29569,37.7358,1,0,1,1),
(24084,29575,40.8805,1,0,1,1),
(24085,22829,4.9505,1,0,1,1),
(24085,27854,9.901,1,0,1,1),
(24085,27855,6.9307,1,0,1,1),
(24085,29569,26.7327,1,0,1,1),
(24085,29570,16.8317,1,0,1,1),
(24085,33447,5.9406,1,0,1,1),
(24085,35953,8.9109,1,0,1,1),
(24085,36862,2.9703,1,0,1,1),
(24085,38260,12.8713,1,0,1,1),
(24085,40202,5.9406,1,0,1,1),
(24085,43575,15.8416,1,0,1,1);

-- creature spawns
DELETE FROM `creature` WHERE `guid` IN (8202624,8202625,8202626,8202627,8202628,8202629,8202630,8202631,8202632,8202633,8202634,8202635,8202636);
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseId`,`phaseGroup`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`) VALUES
(8202624,24079,574,6,0,0,0,0,259.631,-68.8825,24.6786,0.458669,7200,0,0,35928,0,2,0,0,0),
(8202625,24079,574,6,0,0,0,0,280.745,-50.0778,24.6786,4.94722,7200,0,0,35928,0,2,0,0,0),
(8202626,29735,574,6,0,0,0,0,226.909,-4.73227,178.507,2.86731,7200,0,0,8982,100,0,0,0,0),
(8202627,24071,574,6,0,0,0,0,97.0282,-201.792,174.736,1.55213,7200,0,0,35928,0,0,0,0,0),
(8202628,24071,574,6,0,0,0,0,160.241,-175.871,180.5,3.55883,7200,0,0,35928,0,0,0,0,0),
(8202629,24069,574,6,0,0,0,0,97.2803,-196.694,174.548,4.79583,7200,0,0,37164,0,0,0,0,0),
(8202630,24069,574,6,0,0,0,0,105.044,-182.942,174.342,1.26546,7200,0,0,37164,0,0,0,0,0),
(8202631,23961,574,6,0,0,0,0,332.04,251.326,30.7448,2.27124,7200,0,0,35928,0,0,0,0,0),
(8202632,23961,574,6,0,0,0,0,322.455,253.196,30.7452,0.515877,7200,0,0,35928,0,0,0,0,0),
(8202633,23961,574,6,0,0,0,0,122.182,84.3827,65.712,5.62166,7200,0,0,35928,0,0,0,0,0),
(8202634,23961,574,6,0,0,0,0,107.583,43.4611,65.6883,1.07814,7200,0,0,35928,0,0,0,0,0),
(8202635,53488,574,6,0,0,16925,0,165.356,-82.8194,12.635,0.244346,7200,0,0,12600,0,0,0,0,0),
(8202636,53488,574,6,0,0,16925,0,194.578,-72.5972,18.8992,0.244346,7200,0,0,12600,0,0,0,0,0);

-- creature_addon
DELETE FROM `creature_addon` WHERE `guid` IN (125878,125879,125880,125882,125884,125885,125974,125975,125976,125977,8202624,8202625,8202633,8202635,8202636);
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES
(8202624,82026240,0,0,0,0,''),
(8202625,82026250,0,0,0,0,''),
(8202633,0,0,1,0,0,''),
(8202635,0,0,0,1,0,'99201'),
(8202636,0,0,0,1,0,'99201'),
(125977,1259770,0,0,0,0,''),
(125976,1259760,0,0,0,0,''),
(125878,1258780,0,0,0,0,''),
(125879,1258790,0,0,0,0,''),
(125882,1258820,0,0,0,0,''),
(125880,1258800,0,0,0,0,''),
(125885,1258850,0,0,0,0,''),
(125884,1258840,0,0,0,0,''),
(125975,1259750,0,0,0,0,''),
(125974,1259740,0,0,0,0,'');

-- waypoint_data
DELETE FROM `waypoint_data` WHERE `id` IN (1258780,1258790,1258800,1258820,1258840,1258850,1259740,1259750,1259760,1259770,82026240,82026250);
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_flag`,`action`,`action_chance`,`wpguid`) VALUES
(82026240,1,259.631,-68.8825,24.6786,0.0,5000,0,0,100,0),
(82026240,2,256.511,-58.6003,24.6786,0.0,0,0,0,100,0),
(82026240,3,290.02,-47.0099,24.6786,0.0,5000,0,0,100,0),
(82026240,4,286.507,-35.421,24.6786,0.0,5000,0,0,100,0),
(82026240,5,290.096,-47.8619,24.6786,0.0,5000,0,0,100,0),
(82026240,6,256.288,-59.1197,24.6786,0.0,0,0,0,100,0),
(82026250,1,280.745,-50.0778,24.6786,0.0,5000,0,0,100,0),
(82026250,2,276.664,-39.1555,24.6786,0.0,5000,0,0,100,0),
(82026250,3,251.504,-47.0664,24.6786,0.0,0,0,0,100,0),
(82026250,4,248.261,-35.8251,24.6786,0.0,10000,0,0,100,0),
(82026250,5,252.243,-45.7248,24.6786,0.0,0,0,0,100,0),
(82026250,6,276.835,-38.2716,24.6786,0.0,0,0,0,100,0),
(1259770,1,79.5987,249.934,42.8676,0.0,0,0,0,100,0),
(1259770,2,100.406,259.884,42.8676,0.0,0,0,0,100,0),
(1259770,3,109.236,246.305,42.8676,0.0,0,0,0,100,0),
(1259770,4,125.254,254.267,42.8676,0.0,0,0,0,100,0),
(1259770,5,108.572,243.408,42.8676,0.0,0,0,0,100,0),
(1259770,6,100.267,258.966,42.8676,0.0,0,0,0,100,0),
(1259760,1,116.633,75.8048,65.7023,0.0,0,0,0,100,0),
(1259760,2,126.125,43.6627,65.6939,0.0,0,0,0,100,0),
(1259760,3,106.27,37.1763,65.6765,0.0,0,0,0,100,0),
(1259760,4,126.339,43.6035,65.6914,0.0,0,0,0,100,0),
(1258780,1,55.1685,31.2962,115.03,0.0,0,0,0,100,0),
(1258780,2,67.922,27.4249,115.051,0.0,0,0,0,100,0),
(1258780,3,42.8449,48.285,115.03,0.0,0,0,0,100,0),
(1258790,1,59.9154,35.0999,115.031,0.0,0,0,0,100,0),
(1258790,2,46.0972,47.3118,115.031,0.0,0,0,0,100,0),
(1258790,3,57.0322,27.1267,115.031,0.0,0,0,0,100,0),
(1258820,1,43.7297,-19.3308,118.774,0.0,0,0,0,100,0),
(1258820,2,53.4564,-40.7105,118.774,0.0,0,0,0,100,0),
(1258820,3,70.7952,-22.0016,118.774,0.0,0,0,0,100,0),
(1258800,1,48.052,-14.6854,118.774,0.0,0,0,0,100,0),
(1258800,2,54.8632,-41.0873,118.774,0.0,0,0,0,100,0),
(1258800,3,68.8884,-23.8151,118.774,0.0,0,0,0,100,0),
(1258850,1,95.6507,208.877,49.373,0.0,0,0,0,100,0),
(1258850,2,75.0682,213.753,49.3563,0.0,0,0,0,100,0),
(1258850,3,71.4364,186.853,49.3468,0.0,0,0,0,100,0),
(1258840,1,95.4095,206.566,49.3706,0.0,0,0,0,100,0),
(1258840,2,73.9829,210.271,49.3375,0.0,0,0,0,100,0),
(1258840,3,71.767,189.91,49.3317,0.0,0,0,0,100,0),
(1259750,1,96.1878,184.699,49.3634,0.0,0,0,0,100,0),
(1259750,2,96.984,208.488,49.3732,0.0,0,0,0,100,0),
(1259740,1,120.979,269.869,42.8637,0.0,0,0,0,100,0),
(1259740,2,140.564,279.031,42.8667,0.0,0,0,0,100,0),
(1259740,3,152.66,265.87,42.8667,0.0,0,0,0,100,0),
(1259740,4,141.898,278.166,42.8667,0.0,0,0,0,100,0);
UPDATE `creature` SET `MovementType`=2 WHERE `guid`=125977;
UPDATE `creature` SET `MovementType`=2 WHERE `guid`=125976;
UPDATE `creature` SET `MovementType`=2 WHERE `guid`=125878;
UPDATE `creature` SET `MovementType`=2 WHERE `guid`=125879;
UPDATE `creature` SET `MovementType`=2 WHERE `guid`=125882;
UPDATE `creature` SET `MovementType`=2 WHERE `guid`=125880;
UPDATE `creature` SET `MovementType`=2 WHERE `guid`=125885;
UPDATE `creature` SET `MovementType`=2 WHERE `guid`=125884;
UPDATE `creature` SET `MovementType`=2 WHERE `guid`=125975;
UPDATE `creature` SET `MovementType`=2 WHERE `guid`=125974;

-- gameobject spawns
DELETE FROM `gameobject` WHERE `guid` IN (901364,901365);
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseId`,`phaseGroup`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(901364,193059,574,6,0,0,168.341,-125.067,12.4746,1.68632,0.0,0.0,0.74675,0.665105,7200,100,1),
(901365,193059,574,6,0,0,118.053,30.8171,65.693,2.71423,0.0,0.0,0.977257,0.212058,7200,100,1);

-- linked_respawn
DELETE FROM `linked_respawn` WHERE `guid`=8202626 AND `linkType`=0;
DELETE FROM `linked_respawn` WHERE `guid`=8202628 AND `linkType`=0;
DELETE FROM `linked_respawn` WHERE `guid`=8202630 AND `linkType`=0;
DELETE FROM `linked_respawn` WHERE `guid`=8202627 AND `linkType`=0;
DELETE FROM `linked_respawn` WHERE `guid`=8202629 AND `linkType`=0;
DELETE FROM `linked_respawn` WHERE `guid`=8202632 AND `linkType`=0;
DELETE FROM `linked_respawn` WHERE `guid`=8202631 AND `linkType`=0;
DELETE FROM `linked_respawn` WHERE `guid`=8202634 AND `linkType`=0;
DELETE FROM `linked_respawn` WHERE `guid`=8202633 AND `linkType`=0;
INSERT INTO `linked_respawn` (`guid`,`linkedGuid`,`linkType`) VALUES
(8202626,126023,0),
(8202628,126026,0),
(8202630,126026,0),
(8202627,126026,0),
(8202629,126026,0),
(8202632,126023,0),
(8202631,126023,0),
(8202634,126023,0),
(8202633,126023,0);

-- smart_scripts
DELETE FROM `smart_scripts` WHERE `entryorguid`=23956 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=24078 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=24082 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=24849 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=28410 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(23956,0,0,1,1,0,100,13,0,0,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Strategist - Out of Combat - Disable Combat Movement (No Repeat) (Dungeon)'),
(23956,0,1,0,61,0,100,0,0,0,0,0,0,20,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Strategist - Out of Combat - Stop Attacking (No Repeat) (Dungeon)'),
(23956,0,2,3,4,0,100,5,0,0,0,0,0,11,42772,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Strategist - On Aggro - Cast \'Hurl Dagger\' (No Repeat) (Normal Dungeon)'),
(23956,0,3,0,61,0,100,0,0,0,0,0,0,23,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Strategist - On Aggro - Increment Phase By 1 (No Repeat) (Normal Dungeon)'),
(23956,0,4,5,9,0,100,4,5,30,2300,5000,0,11,42772,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Strategist - Within 5-30 Range - Cast \'Hurl Dagger\' (Normal Dungeon)'),
(23956,0,5,0,61,0,100,0,0,0,0,0,0,40,2,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Strategist - Within 5-30 Range - Set Sheath Ranged (Normal Dungeon)'),
(23956,0,6,7,4,0,100,9,0,0,0,0,0,11,59685,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Strategist - On Aggro - Cast \'Hurl Dagger\' (No Repeat) (Heroic Dungeon)'),
(23956,0,7,0,61,0,100,0,0,0,0,0,0,23,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Strategist - On Aggro - Increment Phase By 1 (No Repeat) (Heroic Dungeon)'),
(23956,0,8,9,9,0,100,8,5,30,2300,5000,0,11,59685,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Strategist - Within 5-30 Range - Cast \'Hurl Dagger\' (Heroic Dungeon)'),
(23956,0,9,0,61,0,100,0,0,0,0,0,0,40,2,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Strategist - Within 5-30 Range - Set Sheath Ranged (Heroic Dungeon)'),
(23956,0,10,11,9,0,100,13,25,80,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Strategist - Within 25-80 Range - Enable Combat Movement (No Repeat) (Dungeon)'),
(23956,0,11,0,61,0,100,0,0,0,0,0,0,20,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Strategist - Within 25-80 Range - Start Attacking (No Repeat) (Dungeon)'),
(23956,0,12,13,9,0,100,13,0,5,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Strategist - Within 0-5 Range - Enable Combat Movement (No Repeat) (Dungeon)'),
(23956,0,13,14,61,0,100,0,0,0,0,0,0,40,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Strategist - Within 0-5 Range - Set Sheath Melee (No Repeat) (Dungeon)'),
(23956,0,14,0,61,0,100,0,0,0,0,0,0,20,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Strategist - Within 0-5 Range - Start Attacking (No Repeat) (Dungeon)'),
(23956,0,15,16,9,0,100,13,5,15,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Strategist - Within 5-15 Range - Disable Combat Movement (No Repeat) (Dungeon)'),
(23956,0,16,0,61,0,100,0,0,0,0,0,0,20,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Strategist - Within 5-15 Range - Stop Attacking (No Repeat) (Dungeon)'),
(23956,0,17,0,0,0,100,4,4500,5000,11000,16000,0,11,54962,1,0,0,0,0,5,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Strategist - In Combat - Cast \'Ticking Bomb\' (No Repeat) (Dungeon)'),
(23956,0,18,0,0,0,100,8,4500,5000,11000,16000,0,11,60227,1,0,0,0,0,5,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Strategist - In Combat - Cast \'Ticking Time Bomb\' (No Repeat) (Dungeon)'),
(23956,0,19,0,0,0,100,12,7500,9000,13000,15000,0,11,42972,1,0,0,0,0,5,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Strategist - In Combat - Cast \'Blind\' (No Repeat) (Dungeon)'),
(23956,0,20,0,7,0,100,13,0,0,0,0,0,40,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Strategist - On Evade - Set Sheath Melee (No Repeat) (Dungeon)'),
(24078,0,0,0,0,0,100,4,2000,2000,7000,7000,0,11,15572,1,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Metalworker - In Combat - Cast \'Sunder Armor\''),
(24078,0,1,0,0,0,100,8,2000,2000,7000,7000,0,11,59608,1,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Metalworker - In Combat - Cast \'Sunder Armor\''),
(24078,0,2,3,2,0,100,13,0,30,0,0,0,11,8599,1,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Metalworker - Between 0-30% Health - Cast \'Enrage\' (No Repeat) (Dungeon)'),
(24078,0,3,0,61,0,100,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Metalworker - Between 0-30% Health - Say Line 0 (No Repeat) (Dungeon)'),
(24078,0,4,0,11,0,100,12,0,0,0,0,0,11,43929,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Metalworker - On Spawn - Cast Fiery Weapon Proc'),
(24082,0,0,1,1,0,100,13,0,0,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Handler - Out of Combat - Disable Combat Movement (No Repeat) (Dungeon)'),
(24082,0,1,0,61,0,100,0,0,0,0,0,0,20,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Handler - Out of Combat - Stop Attacking (No Repeat) (Dungeon)'),
(24082,0,2,3,4,0,100,5,0,0,0,0,0,11,54983,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Handler - On Aggro - Cast \'Throw\' (No Repeat) (Normal Dungeon)'),
(24082,0,3,0,61,0,100,0,0,0,0,0,0,23,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Handler - On Aggro - Increment Phase By 1 (No Repeat) (Normal Dungeon)'),
(24082,0,4,5,9,0,100,4,8,25,2300,5000,0,11,54983,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Handler - Within 8-25 Range - Cast \'Throw\' (Normal Dungeon)'),
(24082,0,5,0,61,0,100,0,0,0,0,0,0,40,2,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Handler - Within 8-25 Range - Set Sheath Ranged (Normal Dungeon)'),
(24082,0,6,7,4,0,100,9,0,0,0,0,0,11,59696,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Handler - On Aggro - Cast \'Throw\' (No Repeat) (Heroic Dungeon)'),
(24082,0,7,0,61,0,100,0,0,0,0,0,0,23,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Handler - On Aggro - Increment Phase By 1 (No Repeat) (Heroic Dungeon)'),
(24082,0,8,9,9,0,100,8,8,25,2300,5000,0,11,59696,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Handler - Within 8-25 Range - Cast \'Throw\' (Heroic Dungeon)'),
(24082,0,9,0,61,0,100,0,0,0,0,0,0,40,2,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Handler - Within 8-25 Range - Set Sheath Ranged (Heroic Dungeon)'),
(24082,0,10,11,9,0,100,13,25,80,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Handler - Within 25-80 Range - Enable Combat Movement (No Repeat) (Dungeon)'),
(24082,0,11,0,61,0,100,0,0,0,0,0,0,20,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Handler - Within 25-80 Range - Start Attacking (No Repeat) (Dungeon)'),
(24082,0,12,13,9,0,100,13,0,5,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Handler - Within 0-5 Range - Enable Combat Movement (No Repeat) (Dungeon)'),
(24082,0,13,14,61,0,100,0,0,0,0,0,0,40,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Handler - Within 0-5 Range - Set Sheath Melee (No Repeat) (Dungeon)'),
(24082,0,14,0,61,0,100,0,0,0,0,0,0,20,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Handler - Within 0-5 Range - Start Attacking (No Repeat) (Dungeon)'),
(24082,0,15,16,9,0,100,13,5,15,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Handler - Within 5-15 Range - Disable Combat Movement (No Repeat) (Dungeon)'),
(24082,0,16,0,61,0,100,0,0,0,0,0,0,20,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Handler - Within 5-15 Range - Stop Attacking (No Repeat) (Dungeon)'),
(24082,0,17,0,0,0,100,4,2000,3000,8000,9500,0,11,38621,1,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Handler - In Combat - Cast \'Debilitating Strike\' (No Repeat) (Dungeon)'),
(24082,0,18,0,0,0,100,8,2000,3000,8000,9500,0,11,59695,1,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Handler - In Combat - Cast \'Debilitating Strike\' (No Repeat) (Dungeon)'),
(24082,0,19,0,16,0,100,4,43664,100,15000,21000,0,11,43664,32,0,0,0,0,7,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Handler - On Friendly Unit Missing Buff \'Unholy Rage\' - Cast \'Unholy Rage\' (No Repeat) (Dungeon)'),
(24082,0,20,0,16,0,100,8,59694,100,15000,21000,0,11,59694,32,0,0,0,0,7,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Handler - On Friendly Unit Missing Buff \'Unholy Rage\' - Cast \'Unholy Rage\' (No Repeat) (Dungeon)'),
(24082,0,21,0,7,0,100,13,0,0,0,0,0,40,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Handler - On Evade - Set Sheath Melee (No Repeat) (Dungeon)'),
(24849,0,0,1,1,0,100,13,0,0,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Rider - Out of Combat - Disable Combat Movement (No Repeat) (Dungeon)'),
(24849,0,1,0,61,0,100,0,0,0,0,0,0,20,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Rider - Out of Combat - Stop Attacking (No Repeat) (Dungeon)'),
(24849,0,2,3,4,0,100,5,0,0,0,0,0,11,43665,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Rider - On Aggro - Cast \'Throw\' (No Repeat) (Normal Dungeon)'),
(24849,0,3,0,61,0,100,0,0,0,0,0,0,23,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Rider - On Aggro - Increment Phase By 1 (No Repeat) (Normal Dungeon)'),
(24849,0,4,5,9,1,100,4,5,30,9000,17000,0,11,43665,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Rider - Within 5-30 Range - Cast \'Throw\' (Phase 1) (Normal Dungeon)'),
(24849,0,5,0,61,1,100,0,0,0,0,0,0,40,2,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Rider - Within 5-30 Range - Set Sheath Ranged (Phase 1) (Normal Dungeon)'),
(24849,0,6,7,4,0,100,9,0,0,0,0,0,11,59603,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Rider - On Aggro - Cast \'Throw\' (No Repeat) (Heroic Dungeon)'),
(24849,0,7,0,61,0,100,0,0,0,0,0,0,23,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Rider - On Aggro - Increment Phase By 1 (No Repeat) (Heroic Dungeon)'),
(24849,0,8,9,9,1,100,8,5,30,9000,17000,0,11,59603,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Rider - Within 5-30 Range - Cast \'Throw\' (Phase 1) (Heroic Dungeon)'),
(24849,0,9,0,61,1,100,0,0,0,0,0,0,40,2,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Rider - Within 5-30 Range - Set Sheath Ranged (Phase 1) (Heroic Dungeon)'),
(24849,0,10,11,9,1,100,13,25,80,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Rider - Within 25-80 Range - Enable Combat Movement (Phase 1) (No Repeat) (Dungeon)'),
(24849,0,11,0,61,1,100,0,0,0,0,0,0,20,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Rider - Within 25-80 Range - Start Attacking (Phase 1) (No Repeat) (Dungeon)'),
(24849,0,12,13,9,1,100,13,0,5,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Rider - Within 0-5 Range - Enable Combat Movement (Phase 1) (No Repeat) (Dungeon)'),
(24849,0,13,14,61,1,100,0,0,0,0,0,0,40,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Rider - Within 0-5 Range - Set Sheath Melee (Phase 1) (No Repeat) (Dungeon)'),
(24849,0,14,0,61,1,100,0,0,0,0,0,0,20,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Rider - Within 0-5 Range - Start Attacking (Phase 1) (No Repeat) (Dungeon)'),
(24849,0,15,16,9,1,100,13,5,15,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Rider - Within 5-15 Range - Disable Combat Movement (Phase 1) (No Repeat) (Dungeon)'),
(24849,0,16,0,61,1,100,0,0,0,0,0,0,20,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Rider - Within 5-15 Range - Stop Attacking (Phase 1) (No Repeat) (Dungeon)'),
(24849,0,17,0,0,0,100,4,6000,7500,11000,17000,0,11,31551,1,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Rider - In Combat - Cast \'Piercing Jab\' (Phase 1) (No Repeat) (Dungeon)'),
(24849,0,18,0,0,0,100,8,6000,7500,11000,17000,0,11,59605,1,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Rider - In Combat - Cast \'Piercing Jab\' (Phase 1) (No Repeat) (Dungeon)'),
(24849,0,19,0,0,0,100,4,3000,4500,9000,12500,0,11,32908,1,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Rider - In Combat - Cast \'Wing Clip\' (Phase 1) (No Repeat) (Dungeon)'),
(24849,0,20,0,0,0,100,8,3000,4500,9000,12500,0,11,59604,1,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Rider - In Combat - Cast \'Wing Clip\' (Phase 1) (No Repeat) (Dungeon)'),
(24849,0,21,0,7,0,100,13,0,0,0,0,0,40,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Proto-Drake Rider - On Evade - Set Sheath Melee (Phase 1) (No Repeat) (Dungeon)'),
(28410,0,0,0,0,0,100,12,0,0,2400,3800,0,11,51587,64,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Spiritualist - Combat CMC - Cast \'Lightning Bolt\' (Dungeon)'),
(28410,0,1,0,0,0,100,12,1500,4000,8000,10000,0,11,51588,1,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Spiritualist - Combat - Cast \'Flame Shock\' (Dungeon)'),
(28410,0,2,0,14,0,100,12,7000,100,7000,7000,0,11,51586,1,0,0,0,0,7,0,0,0,0.0,0.0,0.0,0.0,'Dragonflayer Spiritualist - Friendly At 7000 Health - Cast \'Healing Wave\' (Dungeon)');

-- creature_text
DELETE FROM `creature_text` WHERE `entry` IN (22515,24078,24080);
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(22515,1,0,'The Gate of the Blue Sapphire is under attack!',41,0,100.0,0,0,0,'World Trigger - SOA - The Gate of the Blue Sapphire is under attack'),
(22515,2,0,'The Gate of the Green Emerald is under attack!',41,0,100.0,0,0,0,'World Trigger - SOA - The Gate of the Green Emerald is under attack'),
(22515,3,0,'The Gate of the Red Sun is under attack!',41,0,100.0,0,0,0,'World Trigger - SOA - The Gate of the Red Sun is under attack'),
(22515,4,0,'The Gate of the Purple Amethyst is under attack!',41,0,100.0,0,0,0,'World Trigger - SOA - The Gate of the Purple Amethyst is under attack'),
(22515,5,0,'The Gate of the Yellow Moon is under attack!',41,0,100.0,0,0,0,'World Trigger - SOA - The Gate of the Yellow Moon is under attack'),
(22515,6,0,'The Gate of the Yellow Moon was destroyed!',41,0,100.0,0,0,0,'World Trigger - SOA - The Gate of the Yellow Moon was destroyed'),
(22515,7,0,'The Gate of the Purple Amethyst was destroyed!',41,0,100.0,0,0,0,'World Trigger - SOA - The Gate of the Purple Amethyst was destroyed'),
(22515,8,0,'The Gate of the Red Sun was destroyed!',41,0,100.0,0,0,0,'World Trigger - SOA - The Gate of the Red Sun was destroyed'),
(22515,9,0,'The Gate of the Green Emerald was destroyed!',41,0,100.0,0,0,0,'World Trigger - SOA - The Gate of the Green Emerald was destroyed'),
(22515,10,0,'The Gate of the Blue Sapphire was destroyed!',41,0,100.0,0,0,0,'World Trigger - SOA - The Gate of the Blue Sapphire was destroyed'),
(22515,11,0,'The Eastern Graveyard has been captured by the Alliance!',41,0,100.0,0,0,8212,'World Trigger - SOA - The Eastern Graveyard has been captured by the Alliance'),
(22515,12,0,'The Western Graveyard has been captured by the Alliance!',41,0,100.0,0,0,8212,'World Trigger - SOA - The Western Graveyard has been captured by the Alliance'),
(22515,13,0,'The Southern Graveyard has been captured by the Alliance!',41,0,100.0,0,0,8212,'World Trigger - SOA - The Southern Graveyard has been captured by the Alliance'),
(22515,14,0,'The Eastern Graveyard has been captured by the Horde!',41,0,100.0,0,0,8174,'World Trigger - SOA - The Eastern Graveyard has been captured by the Horde'),
(22515,15,0,'The Western Graveyard has been captured by the Horde!',41,0,100.0,0,0,8174,'World Trigger - SOA - The Western Graveyard has been captured by the Horde'),
(22515,16,0,'The Southern Graveyard has been captured by the Horde!',41,0,100.0,0,0,8174,'World Trigger - SOA - The Southern Graveyard has been captured by the Horde'),
(22515,17,0,'The relic chamber is under attack!',41,0,100.0,0,0,0,'World Trigger - SOA - The relic chamber is under attack'),
(22515,18,0,'The chamber has been breached! The titan relic is vulnerable!',41,0,100.0,0,0,0,'World Trigger - SOA - The chamber has been breached'),
(24078,0,0,'%s goes into a frenzy!',16,0,100.0,0,0,0,'Dragonflayer Metalworker'),
(24080,0,0,'I spit on you!',12,0,100.0,0,0,0,'Dragonflayer Weaponsmith');

-- conditions
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=8 AND `SourceGroup`=24084 AND `SourceEntry`=29575 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=12 AND `ConditionTarget`=0 AND `ConditionValue1`=12 AND `ConditionValue2`=0 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorType`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(8,24084,29575,0,0,12,0,12,0,0,0,0,0,'',NULL);

-- creature_template_addon
DELETE FROM `creature_template_addon` WHERE `entry` IN (23956,23960,23961,24069,24071,24078,24080,24082,24083,24084,24085,24849,24864,28410,28419,29735,30531);
INSERT INTO `creature_template_addon` (`entry`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES
(23956,0,0,0,1,0,''),
(23960,0,0,0,1,0,''),
(23961,0,0,0,0,0,'42770'),
(24069,0,0,0,1,0,''),
(24071,0,0,0,0,0,'54775'),
(24078,0,0,0,1,233,'43929'),
(24080,0,0,0,1,0,''),
(24082,0,0,0,1,0,''),
(24083,0,0,0,1,0,''),
(24084,0,0,0,1,0,''),
(24085,0,0,0,1,0,''),
(24849,0,0,0,1,0,'54775'),
(24864,0,0,0,1,0,''),
(28410,0,0,0,1,0,''),
(28419,0,0,0,1,0,''),
(29735,0,0,0,1,0,'18950'),
(30531,0,0,0,4097,0,'25824');

-- lfg_dungeon_template
DELETE FROM `lfg_dungeon_template` WHERE `dungeonId`=242;
INSERT INTO `lfg_dungeon_template` (`dungeonId`,`position_x`,`position_y`,`position_z`,`orientation`,`requiredItemLevel`) VALUES
(242,0.0,0.0,0.0,0.0,180);

-- conditions (spell targets)
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` IN (13,17) AND `SourceEntry` IN (43664,59694);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorType`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(13,3,43664,0,0,31,0,3,23960,0,0,0,0,'',NULL),
(13,3,43664,0,1,31,0,3,23961,0,0,0,0,'',NULL),
(13,3,43664,0,2,31,0,3,24083,0,0,0,0,'',NULL),
(13,3,59694,0,0,31,0,3,23960,0,0,0,0,'',NULL),
(13,3,59694,0,1,31,0,3,23961,0,0,0,0,'',NULL),
(13,3,59694,0,2,31,0,3,24083,0,0,0,0,'',NULL);
