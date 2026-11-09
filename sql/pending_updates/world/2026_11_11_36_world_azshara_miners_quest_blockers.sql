-- Azshara: the Mountainfoot Miner script of Stone Cold and Refleshification and the object spawns of Renewable Resource and Azsharite Experiment Number One.
-- Based on Legends of Azeroth 5.4.8.
DELETE FROM `gameobject` WHERE `guid` BETWEEN 1061300 AND 1061305;
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseId`,`phaseGroup`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(1061300,195602,1,1,0,0,4664.73,-6156.47,103.56,-1.39626,0,0,-0.642787,0.766045,300,100,1),
(1061301,195602,1,1,0,0,4654.2,-6159.17,105.185,0,0,0,0,1,300,100,1),
(1061302,195602,1,1,0,0,4649.98,-6140.24,102.685,0.523599,0,0,0.258819,0.965926,300,100,1),
(1061303,195602,1,1,0,0,4656.46,-6161.66,105.185,0.523599,0,0,0.258819,0.965926,300,100,1),
(1061304,195602,1,1,0,0,4658.87,-6164.62,105.185,-1.91986,0,0,-0.819152,0.573577,300,100,1),
(1061305,196462,1,1,0,0,2264.47,-5572.77,35.1225,1.02974,0,0,0.492423,0.870356,300,100,1);

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=35257;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid` IN (35257);
DELETE FROM `smart_scripts` WHERE `source_type`=9 AND `entryorguid` IN (3525700);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(35257,0,0,0,8,0,100,0,67032,0,0,0,0,11,46598,0,0,0,0,0,7,0,0,0,0,0,0,0,'Mountainfoot Miner - On Spellhit - Cast Ride Vehicle Hardcoded'),
(35257,0,1,2,75,0,100,1,0,35091,10,1000,0,28,46598,0,0,0,0,0,21,5,0,0,0,0,0,0,'Mountainfoot Miner - On Distance Creature - Remove Aura Ride Vehicle Hardcoded'),
(35257,0,2,3,61,0,100,0,0,0,0,0,0,28,67032,0,0,0,0,0,21,5,0,0,0,0,0,0,'Mountainfoot Miner - Linked To Id 1 - Remove Aura Lugging a Goblin'),
(35257,0,3,4,61,0,100,0,0,0,0,0,0,15,14165,0,0,0,0,0,21,5,0,0,0,0,0,0,'Mountainfoot Miner - Linked To Id 2 - Quest Credit'),
(35257,0,4,0,61,0,100,0,0,0,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mountainfoot Miner - Linked To Id 3 - Despawn'),
(35257,0,5,0,8,0,100,1,67362,0,0,0,0,80,3525700,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mountainfoot Miner - On Spellhit - Timed action list'),
(3525700,9,0,0,0,0,100,0,0,0,0,0,0,28,67023,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mountainfoot Miner - On Script - Remove aura'),
(3525700,9,1,0,0,0,100,0,0,0,0,0,0,28,67024,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mountainfoot Miner - On Script - Remove aura'),
(3525700,9,2,0,0,0,100,0,500,500,0,0,0,33,35375,0,0,0,0,0,7,0,0,0,0,0,0,0,'Mountainfoot Miner - On Script - Kill credit'),
(3525700,9,3,0,0,0,100,0,0,0,0,0,0,11,67367,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mountainfoot Miner - On Script - Cast spell'),
(3525700,9,4,0,0,0,100,0,1000,1000,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Mountainfoot Miner - On Script - Talk'),
(3525700,9,5,0,0,0,100,0,0,0,0,0,0,59,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mountainfoot Miner - On Script - Set run'),
(3525700,9,6,0,0,0,100,0,3500,3500,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mountainfoot Miner - On Script - Despawn');
DELETE FROM `creature_text` WHERE `entry` IN (35257);
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(35257,0,0,'I''m free!',12,0,100,71,0,0,''),
(35257,0,1,'I''m outta here...',12,0,100,71,0,0,''),
(35257,0,2,'The nightmare is over!',12,0,100,71,0,0,''),
(35257,0,3,'I can still clock in for the time I was petrified, right?',12,0,100,71,0,0,''),
(35257,0,4,'Give that $gman:woman; a raise!',12,0,100,71,0,0,''),
(35257,0,5,'No job is worth this!',12,0,100,71,0,0,''),
(35257,0,6,'I quit, I QUIT!',12,0,100,71,0,0,''),
(35257,0,7,'That was surprisingly restful.',12,0,100,71,0,0,'');

