-- Nagrand: combat SmartAI for 2 creatures that had no spells and no AI here. The rows are the 5.4.8 ones; the
-- spells they cast agree with the 3.3.5 SmartAI of the same creatures.

-- Nagrand combat abilities
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (17149,17150);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(17149,0,0,0,0,0,100,0,1900,5400,5800,14900,0,11,14873,0,0,0,0,0,2,0,0,0,0,0,0,0,'Vir''aani Raider - In Combat - Cast ''Sinister Strike'' (No Repeat)'),
(17149,0,1,0,0,0,100,0,7900,9200,11300,44700,0,11,15691,0,0,0,0,0,2,0,0,0,0,0,0,0,'Vir''aani Raider - In Combat - Cast ''Eviscerate'' (No Repeat)'),
(17149,0,2,0,0,0,100,0,10400,17500,27300,34800,0,11,12540,0,0,0,0,0,2,0,0,0,0,0,0,0,'Vir''aani Raider - In Combat - Cast ''Gouge'' (No Repeat)'),
(17150,0,0,0,1,0,100,1,0,0,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vir''aani Arcanist - Out of Combat - Disable Combat Movement (No Repeat)'),
(17150,0,1,0,0,0,100,0,0,0,300000,300000,0,11,33839,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vir''aani Arcanist - In Combat - Cast ''Vir''aani Concentration'' (No Repeat)'),
(17150,0,2,3,4,0,100,1,0,0,0,0,0,11,13901,0,0,0,0,0,2,0,0,0,0,0,0,0,'Vir''aani Arcanist - On Aggro - Cast ''Arcane Bolt'' (No Repeat)'),
(17150,0,3,0,61,0,100,0,0,0,0,0,0,23,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vir''aani Arcanist - On Aggro - Increment Phase By 1 (No Repeat)'),
(17150,0,4,0,9,1,100,0,0,40,2400,3800,0,11,13901,0,0,0,0,0,2,0,0,0,0,0,0,0,'Vir''aani Arcanist - Within 0-40 Range - Cast ''Arcane Bolt'' (No Repeat)'),
(17150,0,5,6,3,1,100,1,0,15,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vir''aani Arcanist - Between 0-15% Mana - Enable Combat Movement (Phase 1) (No Repeat)'),
(17150,0,6,7,61,1,100,0,0,0,0,0,0,11,14514,1,0,0,0,0,1,0,0,0,0,0,0,0,'Vir''aani Arcanist - Between 0-15% Mana - Cast ''Blink'' (Phase 1) (No Repeat)'),
(17150,0,7,0,61,1,100,0,0,0,0,0,0,23,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vir''aani Arcanist - Between 0-15% Mana - Increment Phase By 1 (Phase 1) (No Repeat)'),
(17150,0,8,0,9,1,100,0,35,80,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vir''aani Arcanist - Within 35-80 Range - Enable Combat Movement (Phase 1) (No Repeat)'),
(17150,0,9,0,9,1,100,0,5,15,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vir''aani Arcanist - Within 5-15 Range - Disable Combat Movement (Phase 1) (No Repeat)'),
(17150,0,10,0,9,1,100,0,0,5,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vir''aani Arcanist - Within 0-5 Range - Enable Combat Movement (Phase 1) (No Repeat)'),
(17150,0,11,0,0,1,100,0,13400,36100,13400,36100,0,11,22938,1,0,0,0,0,2,0,0,0,0,0,0,0,'Vir''aani Arcanist - In Combat - Cast ''Arcane Explosion'' (Phase 1) (No Repeat)'),
(17150,0,12,0,3,2,100,0,30,100,100,100,0,23,0,1,0,0,0,0,1,0,0,0,0,0,0,0,'Vir''aani Arcanist - Between 30-100% Mana - Decrement Phase By 1 (Phase 1) (No Repeat)'),
(17150,0,13,14,2,0,100,1,0,15,0,0,0,22,3,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vir''aani Arcanist - Between 0-15% Health - Set Event Phase 3 (No Repeat)'),
(17150,0,14,15,61,0,100,0,0,0,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vir''aani Arcanist - Between 0-15% Health - Enable Combat Movement (No Repeat)'),
(17150,0,15,0,61,0,100,0,0,0,0,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vir''aani Arcanist - Between 0-15% Health - Flee For Assist (No Repeat)');
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (17149,17150) AND `AIName` = '' AND `ScriptName` = '';
