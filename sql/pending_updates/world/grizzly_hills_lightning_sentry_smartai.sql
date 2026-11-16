-- Lightning Sentry (26407) and Depleted War Golem (27017), quest 12138 "...Or Maybe We Don't": the ScriptName
-- npc_lightning_sentry has no script in the core any more (upstream replaced it by SmartAI in 2013, but this database
-- never got the rows). A Sentry that dies tells the nearest golem, which casts the credit spell 47797.
UPDATE `creature_template` SET `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` IN (26407, 27017);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (26407, 27017);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(26407, 0, 0, 0, 0, 0, 100, 0, 10000, 12000, 10000, 12000, 0, 11, 52703, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lightning Sentry - In Combat - Cast ''Charged Sentry Totem'''),
(26407, 0, 1, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 19, 27017, 25, 0, 0, 0, 0, 0, 'Lightning Sentry - On Just Died - Set Data 0 1'),
(27017, 0, 0, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 11, 47797, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 'Depleted War Golem - On Data Set 0 1 - Cast ''War Golem Charge Credit''');
