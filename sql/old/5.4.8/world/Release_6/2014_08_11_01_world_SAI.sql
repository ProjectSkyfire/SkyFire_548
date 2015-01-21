-- Enraged Blacksmith SAI
SET @ENTRY := 61130;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,0,4900,14400,12500,21700,11,121445,0,0,0,0,0,2,0,0,0,0,0,0,0,"Enraged Blacksmith - In Combat - Cast Furious Blow");

-- Enraged Blacksmith Creature Template Addon
DELETE FROM `creature_template_addon` WHERE entry=61130;
INSERT INTO `creature_template_addon` (`entry`, `mount`, `bytes1`, `bytes2`, `auras`) VALUES
(61130, 0, 0x0, 0x1, '118521'); -- Possessed by Rage