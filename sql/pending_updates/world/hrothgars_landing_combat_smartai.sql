-- Hrothgar's Landing: combat SmartAI for 6 creatures that cast no spell here and had no AI.
-- Rows from 5.4.8 (Legends of Azeroth), taken only where every event, action and target type is numbered the same in this core,
-- every spell is in the client data and 3.3.5 casts at least half of the same spells. Quest, summon and waypoint scripts are not included.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (34838,34839,35070,35090,35098,35102) AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (34838,34839,35070,35090,35098,35102);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(34838, 0, 0, 0, 0, 0, 100, 0, 3000, 6000, 12000, 15000, 0, 11, 49922, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Kvaldir Reaver - In Combat - Cast ''Wave Crash'''),
(34839, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 11, 45658, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kvaldir Mist Binder - Out of Combat - Cast ''Call of the Mist'''),
(34839, 0, 1, 0, 0, 0, 100, 0, 6000, 9000, 12000, 17000, 0, 11, 49816, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Kvaldir Mist Binder - In Combat - Cast ''Mist of Strangulation'''),
(35070, 0, 0, 0, 0, 0, 100, 0, 0, 0, 2300, 3900, 0, 11, 6660, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Firehawk Mariner - In Combat CMC - Cast ''Shoot'''),
(35070, 0, 1, 0, 0, 0, 100, 0, 8000, 10000, 7000, 9000, 0, 11, 17174, 1, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Firehawk Mariner - In Combat - Cast ''Concussive Shot'''),
(35070, 0, 2, 0, 9, 0, 100, 0, 5, 30, 8000, 11000, 0, 11, 14443, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Firehawk Mariner - Within 5-30 Range - Cast ''Multi-Shot'''),
(35090, 0, 0, 0, 0, 0, 100, 0, 0, 0, 2300, 3900, 0, 11, 6660, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Captain Aerthas Firehawk - In Combat CMC - Cast ''Shoot'''),
(35090, 0, 1, 0, 9, 0, 100, 0, 5, 30, 8000, 11000, 0, 11, 14443, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Captain Aerthas Firehawk - Within 5-30 Range - Cast ''Multi-Shot'''),
(35090, 0, 2, 0, 9, 0, 100, 0, 0, 5, 4000, 7000, 0, 11, 29426, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Captain Aerthas Firehawk - Within 0-5 Range - Cast ''Heroic Strike'''),
(35098, 0, 0, 0, 0, 0, 100, 0, 0, 0, 2300, 3900, 0, 11, 6660, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Wavecrest Mariner - In Combat CMC - Cast ''Shoot'''),
(35098, 0, 1, 0, 0, 0, 100, 0, 8000, 10000, 7000, 9000, 0, 11, 17174, 1, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Wavecrest Mariner - In Combat - Cast ''Concussive Shot'' (Phase 1)'),
(35098, 0, 2, 0, 9, 0, 100, 0, 5, 30, 8000, 11000, 0, 11, 14443, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Wavecrest Mariner - Within 5-30 Range - Cast ''Multi-Shot'' (Phase 1)'),
(35102, 0, 0, 0, 0, 0, 100, 0, 0, 0, 2300, 3900, 0, 11, 6660, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Captain Elleane Wavecrest - In Combat CMC - Cast ''Shoot'''),
(35102, 0, 1, 0, 9, 0, 100, 0, 5, 30, 8000, 11000, 0, 11, 14443, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Captain Elleane Wavecrest - Within 5-30 Range - Cast ''Multi-Shot'''),
(35102, 0, 2, 0, 9, 0, 100, 0, 0, 5, 4000, 7000, 0, 11, 29426, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Captain Elleane Wavecrest - Within 0-5 Range - Cast ''Heroic Strike''');
