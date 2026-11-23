-- Northern Barrens: area trigger of Investigate the Wreckage and the three raptor nest credits of Into the Raptor's Den.
-- Based on Legends of Azeroth 5.4.8.
DELETE FROM `areatrigger_involvedrelation` WHERE `id`=5482;
INSERT INTO `areatrigger_involvedrelation` (`id`,`quest`) VALUES
(5482,14066);

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=34967;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid` IN (-149808, -149797, -149725);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(-149808,0,0,0,10,0,100,0,1,10,30000,30000,1,33,34962,0,0,0,0,0,7,0,0,0,0,0,0,0,'Raptor Nest Bunny - Out of Combat LoS - Quest Credit (Yellow Raptor Nest)'),
(-149797,0,0,0,10,0,100,0,1,10,30000,30000,1,33,34963,0,0,0,0,0,7,0,0,0,0,0,0,0,'Raptor Nest Bunny - Out of Combat LoS - Quest Credit (Blue Raptor Nest)'),
(-149725,0,0,0,10,0,100,0,1,10,30000,30000,1,33,34964,0,0,0,0,0,7,0,0,0,0,0,0,0,'Raptor Nest Bunny - Out of Combat LoS - Quest Credit (Red Raptor Nest)');
