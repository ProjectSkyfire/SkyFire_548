-- Hellfire Peninsula: Testing the Antidote is completed by the Administer Antidote spell hitting a Hulking Helboar.
-- The Helboar had no SmartAI, so the quest could not be finished.

-- Testing the Antidote
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (16880);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(16880,0,0,0,4,0,100,1,0,0,0,0,0,11,33909,0,0,0,0,0,1,0,0,0,0,0,0,0,'Hulking Helboar - On Aggro - Cast ''Hulk'' (No Repeat)'),
(16880,0,1,0,8,0,100,1,34665,0,0,0,0,33,16992,0,0,0,0,0,7,0,0,0,0,0,0,0,'Hulking Helboar - On Spellhit ''Administer Antidote'' - Quest Credit ''Testing the Antidote'' (No Repeat)');
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (16880) AND `AIName` = '' AND `ScriptName` = '';
