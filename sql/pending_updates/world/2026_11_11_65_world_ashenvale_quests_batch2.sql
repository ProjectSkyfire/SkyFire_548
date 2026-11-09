-- Ashenvale: One Shot. One Kill.
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry` IN (11711,11713,11714);

DELETE FROM `waypoints` WHERE `entry`=1171300;
INSERT INTO `waypoints` (`entry`,`pointid`,`position_x`,`position_y`,`position_z`,`point_comment`) VALUES
(1171300,1,4378.29,-41.5416,79.7871,'Blackwood Tracker and Marosh the Devious'),
(1171300,2,4373.45,-48.2829,85.4378,'Blackwood Tracker and Marosh the Devious'),
(1171300,3,4381.44,-56.9109,86.7869,'Blackwood Tracker and Marosh the Devious');

DELETE FROM `creature_text` WHERE `entry`=11711;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(11711,0,0,'Wait... did you hear that? Something approaches from the west!',12,0,100,0,0,0,'Sentinel Aynasha - Line 0'),
(11711,1,0,'I\'ve run out of arrows! I\'m afraid if any more come you will need to take them on by yourself my friend.',12,0,100,0,0,0,'Sentinel Aynasha - Line 1'),
(11711,2,0,'Praise Elune! I don\'t know if I could have survived the day without you, friend.',12,0,100,0,0,0,'Sentinel Aynasha - Line 2'),
(11711,3,0,'My leg feels much better now, the remedy must be working. If you will excuse me, I must go report to my superiors about what has transpired here.',12,0,100,0,0,0,'Sentinel Aynasha - Line 3'),
(11711,4,0,'Please speak with Sentinel Onaeya at Maestra\'s Post in Ashenvale, she will see to it that you are properly rewarded for your bravery this day.',12,0,100,0,0,0,'Sentinel Aynasha - Line 4');
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid` IN (11711, 11713, 11714);
DELETE FROM `smart_scripts` WHERE `source_type`=9 AND `entryorguid` IN (1171100, 1171101);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(11711,0,0,0,0,0,100,0,0,0,2300,3900,0,11,6660,64,0,0,0,0,19,11713,0,0,0,0,0,0,'Sentinel Aynasha - In Combat - Cast Shoot'),
(11711,0,1,2,19,0,100,0,5713,0,0,0,0,64,1,0,0,0,0,0,16,0,0,0,0,0,0,0,'Sentinel Aynasha - On Quest \'One Shot. One Kill.\' Accepted - Store Targetlist'),
(11711,0,2,5,61,0,100,0,0,0,0,0,0,80,1171100,2,0,0,0,0,1,0,0,0,0,0,0,0,'Sentinel Aynasha - On Quest \'One Shot. One Kill.\' Accepted - Run Actionlist'),
(11711,0,3,0,38,0,100,0,1,1,0,0,0,80,1171101,2,0,0,0,0,1,0,0,0,0,0,0,0,'Sentinel Aynasha - On Data 1 1 Set - Run Actionlist'),
(11711,0,4,0,6,0,100,0,0,0,0,0,0,6,5713,0,0,0,0,0,12,1,0,0,0,0,0,0,'Sentinel Aynasha - On Just Died - Fail Quest \'One Shot. One Kill.\''),
(11711,0,5,0,61,0,100,0,0,0,0,0,0,83,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'Sentinel Aynasha - On Quest \'One Shot. One Kill.\' Accepted - Remove Questgiver Flag'),
(1171100,9,0,0,0,0,100,0,0,0,0,0,0,20,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Sentinel Aynasha - On Script - Disable Autoattack'),
(1171100,9,1,0,0,0,100,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Sentinel Aynasha - On Script - Say Line 0'),
(1171100,9,2,0,0,0,100,0,1000,1000,0,0,0,12,11713,4,20000,0,0,0,8,0,0,0,4390.08,-37.4953,83.251,3.8278,'Sentinel Aynasha - On Script - Summon Blackwood Tracker'),
(1171100,9,3,0,0,0,100,0,0,0,0,0,0,12,11713,4,20000,0,0,0,8,0,0,0,4390.08,-37.4953,83.251,3.8278,'Sentinel Aynasha - On Script - Summon Blackwood Tracker'),
(1171100,9,4,0,0,0,100,0,40000,40000,0,0,0,12,11713,4,20000,0,0,0,8,0,0,0,4390.08,-37.4953,83.251,3.8278,'Sentinel Aynasha - On Script - Summon Blackwood Tracker'),
(1171100,9,5,0,0,0,100,0,0,0,0,0,0,12,11713,4,20000,0,0,0,8,0,0,0,4390.08,-37.4953,83.251,3.8278,'Sentinel Aynasha - On Script - Summon Blackwood Tracker'),
(1171100,9,6,0,0,0,100,0,0,0,0,0,0,12,11713,4,20000,0,0,0,8,0,0,0,4390.08,-37.4953,83.251,3.8278,'Sentinel Aynasha - On Script - Summon Blackwood Tracker'),
(1171100,9,7,0,0,0,100,0,40000,40000,0,0,0,12,11714,4,20000,0,0,0,8,0,0,0,4390.08,-37.4953,83.251,3.8278,'Sentinel Aynasha - On Script - Summon Marosh the Devious'),
(1171100,9,8,0,0,0,100,0,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Sentinel Aynasha - On Script - Say Line 1'),
(1171101,9,0,0,0,0,100,0,0,0,0,0,0,15,5713,0,0,0,0,0,12,1,0,0,0,0,0,0,'Sentinel Aynasha - On Script - Area Explored Or Event Happens (One Shot. One Kill.)'),
(1171101,9,1,0,0,0,100,0,0,0,0,0,0,1,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'Sentinel Aynasha - On Script - Say Line 2'),
(1171101,9,2,0,0,0,100,0,7000,7000,0,0,0,1,3,0,0,0,0,0,1,0,0,0,0,0,0,0,'Sentinel Aynasha - On Script - Say Line 3'),
(1171101,9,3,0,0,0,100,0,7000,7000,0,0,0,1,4,0,0,0,0,0,1,0,0,0,0,0,0,0,'Sentinel Aynasha - On Script - Say Line 4'),
(1171101,9,4,0,0,0,100,0,4000,4000,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Sentinel Aynasha - On Script - Despawn'),
(11713,0,0,0,11,0,100,0,0,0,0,0,0,53,1,1171300,0,0,0,2,1,0,0,0,0,0,0,0,'Blackwood Tracker - On Spawn - Start Waypoint'),
(11714,0,0,0,11,0,100,0,0,0,0,0,0,53,1,1171300,0,0,0,2,1,0,0,0,0,0,0,0,'Marosh the Devious - On Spawn - Start Waypoint'),
(11713,0,1,0,0,0,100,0,6000,12000,30000,40000,0,11,6950,32,0,0,0,0,2,0,0,0,0,0,0,0,'Blackwood Tracker - In Combat - Cast Faerie Fire'),
(11714,0,1,0,6,0,100,0,0,0,0,0,0,45,1,1,0,0,0,0,19,11711,0,0,0,0,0,0,'Marosh the Devious - On Just Died - Set Data 1 1 On Sentinel Aynasha'),
(11714,0,2,0,0,0,100,0,6000,12000,30000,40000,0,11,6950,0,0,0,0,0,2,0,0,0,0,0,0,0,'Marosh the Devious - In Combat - Cast Faerie Fire'),
(11714,0,3,0,0,0,100,0,2000,5000,3000,12000,0,11,3391,0,0,0,0,0,2,0,0,0,0,0,0,0,'Marosh the Devious - In Combat - Cast Thrash');
