-- Netherstorm: Wind Trader Marid. His gossip option (A Not-So-Modest Proposal) had no script, so choosing it did
-- nothing: Marid now answers, walks off, turns hostile and despawns after three minutes.

-- Wind Trader Marid
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (20071);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` IN (2007100);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(20071,0,0,1,62,0,100,1,8071,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'On gossip 8071 option 0 selected - Talk 0'),
(20071,0,1,2,61,0,100,0,0,0,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Linked to id 0 - close gossip menu'),
(20071,0,2,3,61,0,100,0,0,0,0,0,0,83,3,0,0,0,0,0,1,0,0,0,0,0,0,0,'Linked to id 1 - remove npc flag 1|2'),
(20071,0,3,0,61,0,100,0,0,0,0,0,0,67,1,4000,4000,0,0,0,1,0,0,0,0,0,0,0,'Linked to id 2 - trigger timed event 1 after 4 second'),
(20071,0,4,0,59,0,100,0,1,0,0,0,0,53,0,20071,0,0,0,0,1,0,0,0,0,0,0,0,'On timed event 1 triggered - waypoint 20071 start'),
(20071,0,5,0,58,0,100,0,0,0,0,0,0,80,2007100,2,0,0,0,0,1,0,0,0,0,0,0,0,'On waypoint ended - trigger timed action list'),
(20071,0,6,7,11,0,100,0,0,0,0,0,0,82,3,0,0,0,0,0,1,0,0,0,0,0,0,0,'On respawn - add npc flag 1|2'),
(20071,0,7,0,61,0,100,0,0,0,0,0,0,18,256,0,0,0,0,0,1,0,0,0,0,0,0,0,'Linked to id 6 - set unit flag 256'),
(2007100,9,0,0,0,0,100,0,0,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0.008,'Set orientation'),
(2007100,9,1,0,0,0,100,0,0,0,0,0,0,101,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Set home position'),
(2007100,9,2,0,0,0,100,0,1000,1000,0,0,0,1,1,0,0,0,0,0,7,0,0,0,0,0,0,0,'Talk 1'),
(2007100,9,3,0,0,0,100,0,3000,3000,0,0,0,19,256,0,0,0,0,0,1,0,0,0,0,0,0,0,'Remove unit flag 256'),
(2007100,9,4,0,0,0,100,0,0,0,0,0,0,2,14,0,0,0,0,0,1,0,0,0,0,0,0,0,'Set faction 14'),
(2007100,9,5,0,0,0,100,0,0,0,0,0,0,8,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'Set react state aggerssive'),
(2007100,9,6,0,0,0,100,0,0,0,0,0,0,41,180000,0,0,0,0,0,1,0,0,0,0,0,0,0,'Despawn after 3 minutes');
DELETE FROM `creature_text` WHERE `entry` IN (20071);
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(20071,0,0,'Let us hold our discussion in a more... private place. Follow me, $n.',12,0,100,1,0,0,'Wind Trader Marid to Player'),
(20071,1,0,'You didn''t really think I''d do business with you again, did you? Sometimes, the best way to return to profitability is to know when to cut your losses.',12,0,100,1,0,0,'Wind Trader Marid to Player');
DELETE FROM `waypoints` WHERE `entry` IN (20071);
INSERT INTO `waypoints` (`entry`,`pointid`,`position_x`,`position_y`,`position_z`,`point_comment`) VALUES
(20071,1,4286.12,2232.04,124.217,''),
(20071,2,4294.74,2222.18,124.217,''),
(20071,3,4295.54,2217.33,122.895,''),
(20071,4,4299.45,2212.04,119.061,''),
(20071,5,4304.91,2201.45,120.693,''),
(20071,6,4309.57,2189.28,115.873,''),
(20071,7,4311.52,2184.85,116.046,''),
(20071,8,4312.05,2169.89,118.482,''),
(20071,9,4311.13,2159.71,123.648,''),
(20071,10,4304.89,2140.55,129.738,'');
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (20071) AND `AIName` = '' AND `ScriptName` = '';
