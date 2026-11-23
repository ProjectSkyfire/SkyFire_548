-- Darkshore: area triggers of Consumed and A Lost Companion and the totem credit of Soothing the Elements.
-- Based on Legends of Azeroth 5.4.8.
DELETE FROM `areatrigger_involvedrelation` WHERE `id` IN (5449,5972);
INSERT INTO `areatrigger_involvedrelation` (`id`,`quest`) VALUES
(5449,13881),
(5972,13564);

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry` IN (34367,34371);
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid` IN (34367, 34371);
DELETE FROM `smart_scripts` WHERE `source_type`=9 AND `entryorguid` IN (3436700);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(34367,0,0,1,1,0,100,1,1,1,0,0,0,103,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Energized Soothing Totem - Out of Combat - Root'),
(34367,0,1,2,61,0,100,0,0,0,0,0,0,12,34371,1,60000,0,0,0,1,0,0,0,0,0,0,0,'Energized Soothing Totem - Linked To Id 0 - Summon Energized Soothing Totem Credit'),
(34367,0,2,0,61,0,100,0,0,0,0,0,0,37,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Energized Soothing Totem - Linked To Id 1 - Die'),
(34371,0,0,0,54,0,100,0,0,0,0,0,0,80,3436700,2,0,0,0,0,1,0,0,0,0,0,0,0,'Energized Soothing Totem Credit - On Summoned - Run Script'),
(3436700,9,0,0,0,0,100,0,2000,2000,0,0,0,45,1,0,0,0,0,0,9,24042,0,25,0,0,0,0,'Energized Soothing Totem Credit - On Script - Set Data 1 On Generic Trigger'),
(3436700,9,1,0,0,0,100,0,10000,10000,0,0,0,45,1,0,0,0,0,0,9,24042,0,25,0,0,0,0,'Energized Soothing Totem Credit - On Script - Set Data 1 On Generic Trigger'),
(3436700,9,2,0,0,0,100,0,10000,10000,0,0,0,45,2,0,0,0,0,0,19,24042,25,0,0,0,0,0,'Energized Soothing Totem Credit - On Script - Set Data 2 On Generic Trigger'),
(3436700,9,3,0,0,0,100,0,10000,10000,0,0,0,33,34371,0,0,0,0,0,17,0,50,0,0,0,0,0,'Energized Soothing Totem Credit - On Script - Quest Credit'),
(3436700,9,4,0,0,0,100,0,0,0,0,0,0,41,2000,0,0,0,0,0,1,0,0,0,0,0,0,0,'Energized Soothing Totem Credit - On Script - Despawn');
