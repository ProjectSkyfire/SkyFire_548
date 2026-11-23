-- Arcatraz: the Warden's Cage prisoners tell the instance when they die, so Warden Mellichar
-- releases the next one and finally Harbinger Skyriss
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry` IN (20905,20906,20908,20909,20910,20911) AND `ScriptName`='';
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (20905,20906,20908,20909,20910,20911) AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(20905,0,0,0,6,0,100,0,0,0,0,0,0,34,5,3,0,0,0,0,1,0,0,0,0,0,0,0,'Blazing Trickster - On Just Died - Set Instance Data 5 to 3'),
(20906,0,0,0,6,0,100,0,0,0,0,0,0,34,5,3,0,0,0,0,1,0,0,0,0,0,0,0,'Phase-Hunter - On Just Died - Set Instance Data 5 to 3'),
(20908,0,0,0,6,0,100,0,0,0,0,0,0,34,7,3,0,0,0,0,1,0,0,0,0,0,0,0,'Akkiris Lightning-Waker - On Just Died - Set Instance Data 7 to 3'),
(20909,0,0,0,6,0,100,0,0,0,0,0,0,34,7,3,0,0,0,0,1,0,0,0,0,0,0,0,'Sulfuron Magma-Thrower - On Just Died - Set Instance Data 7 to 3'),
(20910,0,0,0,6,0,100,0,0,0,0,0,0,34,8,3,0,0,0,0,1,0,0,0,0,0,0,0,'Twilight Drakonaar - On Just Died - Set Instance Data 8 to 3'),
(20911,0,0,0,6,0,100,0,0,0,0,0,0,34,8,3,0,0,0,0,1,0,0,0,0,0,0,0,'Blackwing Drakonaar - On Just Died - Set Instance Data 8 to 3');
