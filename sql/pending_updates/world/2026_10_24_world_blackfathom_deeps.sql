-- Blackfathom Deeps (map 48, dungeon, Classic): missing creatures, gameobjects, loot and scripts
-- Idempotent: every INSERT is preceded by a DELETE of its keys.

-- creature_template updates (difficulty slots, loot ids, SmartAI)
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4799 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4805 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4807 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4809 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4810 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4811 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4812 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4813 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4814 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4815 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4818 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4819 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4820 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4821 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4822 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4827 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4831 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4887 AND `ScriptName`='';
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=12902 AND `ScriptName`='';

-- creature_template levels (normal difficulty)
UPDATE `creature_template` SET `minlevel`=24, `maxlevel`=24 WHERE `entry`=4824;

-- creature spawns
DELETE FROM `creature` WHERE `guid` IN (8203782,8203783,8203784,8203785,8203786,8203787,8203788,8203789,8203790,8203793,8203794,8203795,8203796);
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseId`,`phaseGroup`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`) VALUES
(8203782,4823,48,2,0,0,9565,0,-725.606,-119.012,-36.7311,1.76685,7200,0,0,1953,0,0,0,0,0),
(8203783,4823,48,2,0,0,9565,0,-754.272,-86.6351,-40.954,5.91189,7200,0,0,1953,0,0,0,0,0),
(8203784,4823,48,2,0,0,9565,0,-778.401,-93.9601,-40.2276,1.73305,7200,0,0,1953,0,0,0,0,0),
(8203785,4820,48,2,0,0,3617,0,-452.023,13.514,-48.0725,3.72216,7200,0,0,2112,3090,0,0,0,0),
(8203786,4815,48,2,0,0,2835,0,-292.147,87.837,-51.933,2.42193,7200,0,0,2420,0,0,0,0,0),
(8203787,4815,48,2,0,0,2835,0,-343.459,5.38988,-71.7586,3.65459,7200,0,0,2420,0,0,0,0,0),
(8203788,12736,48,2,0,0,12953,0,-157.078,74.129,-45.5308,1.5708,7200,0,0,562,1633,0,0,0,0),
(8203789,44375,48,2,0,0,4084,0,-164.933,76.5771,-45.8097,0.418879,7200,0,0,605,0,0,0,0,0),
(8203790,44387,48,2,0,0,33847,0,-732.92,21.3355,-30.3792,0.855211,7200,0,0,2053,0,0,0,0,0),
(8203793,33256,48,2,0,0,28528,0,-167.009,79.2494,-45.7184,0.0,7200,0,0,2490,2725,0,0,0,0),
(8203794,33260,48,2,0,0,28530,0,-158.536,74.219,-45.7543,0.890118,7200,0,0,2605,0,0,0,0,0),
(8203795,33258,48,2,0,0,28520,0,-164.906,79.3527,-46.014,3.33358,7200,0,0,3025,0,0,0,0,0),
(8203796,33261,48,2,0,0,28529,0,-156.314,74.0989,-45.4139,0.0,7200,0,0,521,0,0,0,0,0);

-- creature_addon
DELETE FROM `creature_addon` WHERE `guid` IN (26147,27433,27434,8203782,8203783,8203784,8203785,8203786,8203787,8203788,8203789,8203790);
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES
(8203782,0,0,0,1,0,'8393'),
(8203783,0,0,0,1,0,'8393'),
(8203784,0,0,0,1,0,'8393'),
(8203785,0,0,0,1,0,''),
(8203786,0,0,0,1,0,''),
(8203787,0,0,0,1,0,''),
(8203788,0,0,0,1,0,'82910'),
(8203789,0,0,0,1,0,''),
(8203790,0,0,0,1,0,'82950 83056 83065'),
(26147,261470,0,131072,1,0,'22766'),
(27434,274340,0,0,1,0,''),
(27433,274330,0,0,1,0,'');

-- waypoint_data
DELETE FROM `waypoint_data` WHERE `id` IN (261470,274330,274340);
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_flag`,`action`,`action_chance`,`wpguid`) VALUES
(261470,1,-620.188,198.421,-52.231,0.0,0,0,0,100,0),
(261470,2,-620.477,197.463,-52.231,0.0,0,0,0,100,0),
(261470,3,-622.062,190.729,-52.446,0.0,0,0,0,100,0),
(261470,4,-624.911,178.625,-54.3966,0.0,0,0,0,100,0),
(261470,5,-627.176,169.004,-55.7868,0.0,0,0,0,100,0),
(261470,6,-630.408,160.317,-56.8742,0.0,0,0,0,100,0),
(261470,7,-632.041,152.222,-57.6498,0.0,0,0,0,100,0),
(261470,8,-632.974,147.597,-58.1807,0.0,0,0,0,100,0),
(261470,9,-633.675,144.12,-58.2602,0.0,0,0,0,100,0),
(261470,10,-634.593,139.572,-58.1207,0.0,0,0,0,100,0),
(261470,11,-635.307,136.031,-58.1722,0.0,0,0,0,100,0),
(261470,12,-636.804,128.613,-58.0144,0.0,0,0,0,100,0),
(261470,13,-636.804,128.613,-58.0144,0.0,0,0,0,100,0),
(274340,1,-841.193,-376.695,-33.8843,0.0,0,0,0,100,0),
(274340,2,-841.939,-376.028,-33.8843,0.0,0,0,0,100,0),
(274340,3,-842.011,-372.193,-33.885,0.0,0,0,0,100,0),
(274340,4,-842.158,-364.446,-33.9043,0.0,0,0,0,100,0),
(274340,5,-842.295,-357.267,-33.8689,0.0,0,0,0,100,0),
(274340,6,-842.332,-355.278,-33.9798,0.0,0,0,0,100,0),
(274340,7,-842.332,-355.278,-33.9798,0.0,0,0,0,100,0),
(274330,1,-848.446,-454.865,-33.8922,0.0,0,0,0,100,0),
(274330,2,-848.446,-453.865,-33.8922,0.0,0,0,0,100,0),
(274330,3,-846.784,-454.431,-33.985,0.0,0,0,0,100,0),
(274330,4,-845.793,-454.768,-33.9681,0.0,0,0,0,100,0),
(274330,5,-843.6,-449.661,-33.9288,0.0,0,0,0,100,0),
(274330,6,-843.135,-448.578,-33.9149,0.0,0,0,0,100,0),
(274330,7,-842.023,-445.989,-33.9332,0.0,0,0,0,100,0),
(274330,8,-841.083,-443.798,-33.8905,0.0,0,0,0,100,0),
(274330,9,-838.965,-438.867,-33.8905,0.0,0,0,0,100,0),
(274330,10,-838.571,-437.95,-33.8905,0.0,0,0,0,100,0),
(274330,11,-838.571,-437.95,-33.8905,0.0,0,0,0,100,0);
UPDATE `creature` SET `MovementType`=2 WHERE `guid`=26147;
UPDATE `creature` SET `MovementType`=2 WHERE `guid`=27434;
UPDATE `creature` SET `MovementType`=2 WHERE `guid`=27433;

-- gameobject spawns
DELETE FROM `gameobject` WHERE `guid` IN (901735);
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseId`,`phaseGroup`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(901735,1732,48,2,0,0,-761.542,-166.294,-52.6799,1.97222,0.0,0.0,0.833885,0.551938,600,100,1);

-- smart_scripts
DELETE FROM `smart_scripts` WHERE `entryorguid`=4799 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4805 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4807 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4809 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4810 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4811 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4812 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4813 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4814 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4815 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4818 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4819 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4820 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4821 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4822 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4827 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4831 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4887 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=12902 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=177964 AND `source_type`=1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(4799,0,0,0,0,0,100,4,0,0,2400,3800,0,11,9613,64,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Fallenroot Hellcaller - In Combat CMC - Cast \'Shadow Bolt\' (Normal Dungeon)'),
(4799,0,1,0,0,0,100,4,12000,15000,30000,45000,0,11,8129,1,0,0,0,0,5,0,0,0,0.0,0.0,0.0,0.0,'Fallenroot Hellcaller - In Combat - Cast \'Mana Burn\' (Normal Dungeon)'),
(4805,0,0,0,1,0,100,4,1000,1000,1800000,1800000,0,11,12544,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Blackfathom Sea Witch - Out of Combat - Cast \'Frost Armor\' (No Repeat)'),
(4805,0,1,0,0,0,100,4,7000,10000,16000,20000,0,11,122,1,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Blackfathom Sea Witch - In Combat - Cast \'Frost Nova\' (No Repeat)'),
(4805,0,3,0,2,0,100,5,0,15,0,0,0,25,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Blackfathom Sea Witch - Between 0-15% Health - Flee For Assist (No Repeat)'),
(4807,0,0,0,0,0,100,4,5000,9000,16000,20000,0,11,8379,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Blackfathom Myrmidon - In Combat - Cast \'Disarm\' (No Repeat)'),
(4809,0,1,0,2,0,100,4,0,50,22000,25000,0,11,8362,1,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Twilight Acolyte - Between 0-50% Health - Cast \'Renew\' (No Repeat)'),
(4810,0,0,0,0,0,100,4,5000,9000,8000,12000,0,11,8374,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Twilight Reaver - In Combat - Cast \'Arcing Smash\' (No Repeat)'),
(4810,0,1,0,2,0,100,5,0,15,0,0,0,25,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Twilight Reaver - Between 0-15% Health - Flee For Assist (No Repeat)'),
(4811,0,0,0,1,0,100,5,1000,1000,0,0,0,11,8372,1,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Twilight Aquamancer - Out of Combat - Cast \'Summon Aqua Guardian\' (No Repeat)'),
(4811,0,1,0,0,0,100,4,0,40,2400,3800,0,11,9672,64,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Twilight Aquamancer - In Combat CMC - Cast \'Frostbolt\' (Normal Dungeon)'),
(4811,0,2,0,2,0,100,5,0,15,0,0,0,25,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Twilight Aquamancer - Between 0-15% Health - Flee For Assist (No Repeat) (Normal Dungeon)'),
(4812,0,0,0,0,0,100,4,5000,9000,12000,20000,0,11,18972,32,0,0,0,0,5,0,0,0,0.0,0.0,0.0,0.0,'Twilight Loreseeker - In Combat - Cast \'Slow\' (No Repeat) (Normal Dungeon)'),
(4812,0,1,0,2,0,100,4,0,30,10000,10000,0,11,8365,1,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Twilight Loreseeker - Between 0-30% Health - Cast \'Enlarge\' (No Repeat) (Normal Dungeon)'),
(4813,0,0,0,1,0,100,5,1000,1000,0,0,0,11,12746,1,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Twilight Shadowmage - Out of Combat - Cast \'Summon Voidwalker\' (No Repeat) (Normal Dungeon)'),
(4813,0,1,0,0,0,100,4,0,0,2400,3800,0,11,9613,64,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Twilight Shadowmage - In Combat CMC - Cast \'Shadow Bolt\' (Normal Dungeon)'),
(4813,0,2,0,0,0,100,4,18000,26000,24000,30000,0,11,7645,1,0,0,0,0,6,0,0,0,0.0,0.0,0.0,0.0,'Twilight Shadowmage - In Combat - Cast \'Dominate Mind\' (Normal Dungeon)'),
(4813,0,3,0,2,0,100,5,0,15,0,0,0,25,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Twilight Shadowmage - Between 0-15% Health - Flee For Assist (No Repeat) (Normal Dungeon)'),
(4814,0,0,0,0,0,100,4,4000,7000,9000,12000,0,11,13728,0,0,0,0,0,5,0,0,0,0.0,0.0,0.0,0.0,'Twilight Elementalist - In Combat - Cast \'Earth Shock\' (No Repeat) (Normal Dungeon)'),
(4814,0,1,0,0,0,100,4,5000,9000,13000,17000,0,11,15039,0,0,0,0,0,5,0,0,0,0.0,0.0,0.0,0.0,'Twilight Elementalist - In Combat - Cast \'Flame Shock\' (No Repeat) (Normal Dungeon)'),
(4814,0,2,0,0,0,100,4,1000,2500,11000,15000,0,11,12548,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Twilight Elementalist - In Combat - Cast \'Frost Shock\' (No Repeat) (Normal Dungeon)'),
(4814,0,3,0,0,0,100,4,3000,6000,7000,12000,0,11,11824,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Twilight Elementalist - In Combat - Cast \'Shock\' (No Repeat) (Normal Dungeon)'),
(4815,0,0,0,0,0,100,4,5000,9000,16000,20000,0,11,8379,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Murkshallow Snapclaw - In Combat - Cast \'Disarm\' (No Repeat) (Normal Dungeon)'),
(4818,0,0,0,4,0,100,5,0,0,0,0,0,11,7164,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Blindlight Murloc - On Aggro - Cast \'Defensive Stance\' (No Repeat) (Normal Dungeon)'),
(4819,0,0,0,2,0,100,4,0,50,50000,60000,0,11,8382,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Blindlight Muckdweller - Between 0-50% Health - Cast \'Leech Poison\' (No Repeat) (Normal Dungeon)'),
(4820,0,0,0,0,0,100,4,0,0,2400,3800,0,11,9532,64,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Blindlight Oracle - In Combat CMC - Cast \'Lightning Bolt\' (Normal Dungeon)'),
(4820,0,2,0,2,0,100,4,0,49,22000,22000,0,11,8362,1,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Blindlight Oracle - Between 0-50% Health - Cast \'Renew\' (Normal Dungeon)'),
(4820,0,3,0,2,0,100,5,0,15,0,0,0,25,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Blindlight Oracle - Between 0-15% Health - Flee For Assist (No Repeat) (Normal Dungeon)'),
(4821,0,0,0,2,0,100,5,0,15,0,0,0,25,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Skittering Crustacean - Between 0-15% Health - Flee For Assist (No Repeat) (Normal Dungeon)'),
(4822,0,0,0,2,0,100,5,0,15,0,0,0,25,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Snapping Crustacean - Between 0-15% Health - Flee For Assist (No Repeat) (Normal Dungeon)'),
(4827,0,0,0,0,0,100,4,3000,6000,20000,25000,0,11,3604,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Deep Pool Threshfin - In Combat - Cast \'Tendon Rip\' (No Repeat) (Normal Dungeon)'),
(4831,0,0,0,4,0,100,5,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Lady Sarevess - On Aggro - Say Line 0 (No Repeat) (Normal Dungeon)'),
(4831,0,1,0,0,0,100,4,0,0,2300,3900,0,11,6660,64,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Lady Sarevess - In Combat CMC - Cast \'Shoot\' (Normal Dungeon)'),
(4831,0,2,0,0,0,100,4,9000,12000,12000,15000,0,11,8435,1,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Lady Sarevess - In Combat - Cast \'Forked Lightning\' (Normal Dungeon)'),
(4831,0,4,0,0,0,100,4,7000,9000,9000,13000,0,11,246,0,0,0,0,0,5,0,0,0,0.0,0.0,0.0,0.0,'Lady Sarevess - In Combat - Cast \'Slow\' (Normal Dungeon)'),
(4831,0,5,0,6,0,100,5,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Lady Sarevess - On Just Died - Say Line 1 (Normal Dungeon)'),
(4887,0,0,0,0,0,100,4,5000,8000,9000,13000,0,11,5568,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Ghamoo-ra - In Combat - Cast \'Trample\''),
(12902,0,0,0,1,0,100,5,0,0,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Lorgus Jett - Out of Combat - Disable Combat Movement (No Repeat)'),
(12902,0,1,0,1,0,100,4,1000,1000,600000,600000,0,11,12550,1,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Lorgus Jett - Out of Combat - Cast \'Lightning Shield\' (No Repeat)'),
(12902,0,2,3,4,0,100,5,0,0,0,0,0,11,12167,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Lorgus Jett - On Aggro - Cast \'Lightning Bolt\' (No Repeat) (Normal Dungeon)'),
(12902,0,3,0,61,0,100,0,0,0,0,0,0,23,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Lorgus Jett - On Aggro - Increment Phase By 1 (No Repeat) (Normal Dungeon)'),
(12902,0,4,0,9,1,100,4,0,40,2400,3800,0,11,12167,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Lorgus Jett - Within 0-40 Range - Cast \'Lightning Bolt\' (No Repeat) (Normal Dungeon)'),
(12902,0,5,6,3,1,100,5,0,15,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Lorgus Jett - Between 0-15% Mana - Enable Combat Movement (Phase 1) (No Repeat) (Normal Dungeon)'),
(12902,0,6,0,61,1,100,0,0,0,0,0,0,23,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Lorgus Jett - Between 0-15% Mana - Increment Phase By 1 (Phase 1) (No Repeat) (Normal Dungeon)'),
(12902,0,7,0,9,1,100,5,35,80,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Lorgus Jett - Within 35-80 Range - Enable Combat Movement (Phase 1) (No Repeat) (Normal Dungeon)'),
(12902,0,8,0,9,1,100,5,5,15,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Lorgus Jett - Within 5-15 Range - Disable Combat Movement (Phase 1) (No Repeat) (Normal Dungeon)'),
(12902,0,9,0,9,1,100,5,0,5,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Lorgus Jett - Within 0-5 Range - Enable Combat Movement (Phase 1) (No Repeat) (Normal Dungeon)'),
(12902,0,10,0,3,0,100,4,30,100,100,100,0,23,0,1,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Lorgus Jett - Between 30-100% Mana - Decrement Phase By 1 (Phase 1) (No Repeat) (Normal Dungeon)'),
(177964,1,0,1,70,0,100,0,1,0,0,0,0,12,12876,6,300000,1,0,0,8,0,0,0,-780.44,-79.3731,-44.0216,0.680678,'Fathom Stone - On Gameobject State Changed - Summon Creature \'Baron Aquanis\'');
UPDATE `gameobject_template` SET `AIName`='SmartGameObjectAI' WHERE `entry`=177964 AND `ScriptName`='';

-- creature_text
DELETE FROM `creature_text` WHERE `entry` IN (4831);
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(4831,0,0,'You should not be here! Slay them!',14,0,100.0,0,0,5799,'Lady Sarevess'),
(4831,1,0,'Paltry kill.',14,0,100.0,0,0,5801,'Lady Sarevess');

-- creature_template_addon
DELETE FROM `creature_template_addon` WHERE `entry` IN (4977,4978,6729,12736,33256,33258,33260,33261,44375,44387);
INSERT INTO `creature_template_addon` (`entry`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES
(4977,0,0,0,1,0,''),
(4978,0,0,0,1,0,''),
(6729,0,0,0,1,0,''),
(12736,0,0,0,1,0,'82910'),
(33256,0,0,0,1,0,''),
(33258,0,0,0,1,0,''),
(33260,0,0,8,1,0,''),
(33261,0,0,0,1,0,''),
(44375,0,0,0,1,0,''),
(44387,0,0,0,1,0,'82950 83056 83065');
