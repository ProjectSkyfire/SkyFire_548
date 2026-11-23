-- Netherstorm: the credit scripts of Captain Tyralius and Teleport This! (the Gehenna teleporters and the
-- mind-controlled forgelords).

-- Captain Tyralius
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (20787);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(20787,0,0,0,11,0,100,0,0,0,0,0,0,67,20787,100,100,0,0,0,1,0,0,0,0,0,0,0,'Captain Tyralius - On Respawn - Create Timed Event'),
(20787,0,1,2,59,0,100,0,20787,0,0,0,0,33,20787,0,0,0,0,0,12,1,0,0,0,0,0,0,'Captain Tyralius - On Timed Event Triggered'),
(20787,0,2,3,61,0,100,0,0,0,0,0,0,1,0,0,0,0,0,0,12,1,0,0,0,0,0,0,'Captain Tyralius - Linked To Id 1 - Say Line 0'),
(20787,0,3,0,61,0,100,0,0,0,0,0,0,41,10000,0,0,0,0,0,1,0,0,0,0,0,0,0,'Captain Tyralius - Linked To Id 2 - Despawn after 10 sec');
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (20787) AND `AIName` = '' AND `ScriptName` = '';

-- Teleport This!
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (16943,20928,22348,22350,22351);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(16943,0,0,0,9,0,100,0,0,8,19000,23000,0,11,36255,0,0,0,0,0,1,0,0,0,0,0,0,0,'Cyber-Rage Forgelord - Within 0-8 Range - Cast ''Chaos Nova'''),
(16943,0,1,2,2,0,100,1,0,30,0,0,0,11,8599,1,0,0,0,0,1,0,0,0,0,0,0,0,'Cyber-Rage Forgelord - Between 0-30% Health - Cast ''Enrage'' (No Repeat)'),
(16943,0,2,0,61,0,100,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Cyber-Rage Forgelord - Between 0-30% Health - Say Line 0 (No Repeat)'),
(16943,0,3,0,8,0,100,0,38915,0,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,'Cyber-Rage Forgelord - On SpellHit Mental Interference - Store Target List'),
(16943,0,4,0,38,0,100,0,0,1,0,0,0,33,22348,0,0,0,0,0,12,1,0,0,0,0,0,0,'Cyber-Rage Forgelord - On Data Set 0 1 - Quest Credit'),
(16943,0,5,0,38,0,100,0,0,2,0,0,0,33,22350,0,0,0,0,0,12,1,0,0,0,0,0,0,'Cyber-Rage Forgelord - On Data Set 0 2 - Quest Credit'),
(16943,0,6,0,38,0,100,0,0,3,0,0,0,33,22351,0,0,0,0,0,12,1,0,0,0,0,0,0,'Cyber-Rage Forgelord - On Data Set 0 3 - Quest Credit'),
(20928,0,0,0,8,0,100,0,38915,0,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,'Ironspine Forgelord - On SpellHit Mental Interference - Store Target List'),
(20928,0,1,0,38,0,100,0,0,1,0,0,0,33,22348,0,0,0,0,0,12,1,0,0,0,0,0,0,'Ironspine Forgelord - On Data Set 0 1 - Quest Credit'),
(20928,0,2,0,38,0,100,0,0,2,0,0,0,33,22350,0,0,0,0,0,12,1,0,0,0,0,0,0,'Ironspine Forgelord - On Data Set 0 2 - Quest Credit'),
(20928,0,3,0,38,0,100,0,0,3,0,0,0,33,22351,0,0,0,0,0,12,1,0,0,0,0,0,0,'Ironspine Forgelord - On Data Set 0 1 - Quest Credit'),
(22348,0,0,0,8,0,100,0,38920,0,15000,15000,0,45,0,1,0,0,0,0,7,0,0,0,0,0,0,0,'Western Gehenna Teleporter Credit - On SpellHit Detonate Teleporter - Set Data 0 1'),
(22350,0,0,0,8,0,100,0,38920,0,15000,15000,0,45,0,2,0,0,0,0,7,0,0,0,0,0,0,0,'Central Gehenna Teleporter Credit - On SpellHit Detonate Teleporter - Set Data 0 2'),
(22351,0,0,0,8,0,100,0,38920,0,15000,15000,0,45,0,3,0,0,0,0,7,0,0,0,0,0,0,0,'Eastern Gehenna Teleporter Credit - On SpellHit Detonate Teleporter - Set Data 0 3');
DELETE FROM `creature_text` WHERE `entry` IN (16943);
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(16943,0,0,'%s goes into a frenzy!',16,0,100,0,0,0,'Cyber-Rage Forgelord');
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (16943,20928,22348,22350,22351) AND `AIName` = '' AND `ScriptName` = '';
