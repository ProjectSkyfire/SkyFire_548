-- Lupello SAI
SET @ENTRY := 56357;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,9,0,100,0,5,40,1700,3800,11,129496,0,0,0,0,0,2,0,0,0,0,0,0,0,"Lupello - Within 5-40 Range - Cast 'Prey Pounce'"),
(@ENTRY,0,1,0,0,0,100,0,10000,10500,10500,15500,11,129502,0,0,0,0,0,2,0,0,0,0,0,0,0,"Lupello - In Combat - Cast 'Fearsome Howl'"),
(@ENTRY,0,2,0,0,0,100,0,5000,10000,10000,15000,11,129497,0,0,0,0,0,2,0,0,0,0,0,0,0,"Lupello - In Combat - Cast 'Pounced'");

-- Lupello Spawn
SET @CGUID := 2572;
DELETE FROM `creature` WHERE `guid`=@CGUID;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(@CGUID, @ENTRY, 870, 1, 1, 79.69495, -548.7769, 204.871, 2.686525, 120, 0, 0);