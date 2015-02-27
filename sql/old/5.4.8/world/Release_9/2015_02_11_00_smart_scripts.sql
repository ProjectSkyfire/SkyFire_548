-- Dojani Enforcer SAI
SET @ENFORCER        := 65626;  -- Dojani Enforcer
SET @SPELL_SHOCKWAVE := 129018; -- Shockwave
SET @SPELL_LEAP      := 129017; -- Leap of Victory
SET @SPELL_ENRAGE    := 129016; -- Enrage
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENFORCER;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENFORCER AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENFORCER,0,0,0,0,0,100,0,21800,21900,21700,23100,11,@SPELL_SHOCKWAVE,0,0,0,0,0,2,0,0,0,0,0,0,0,"Dojani Enforcer - In Combat - Shockwave"),
(@ENFORCER,0,1,0,0,0,100,0,13300,14600,14500,15800,11,@SPELL_LEAP,0,0,0,0,0,2,0,0,0,0,0,0,0,"Dojani Enforcer - In Combat - Leap of Victory"),
(@ENFORCER,0,2,0,0,0,100,0,12400,16700,16500,21800,11,@SPELL_ENRAGE,0,0,0,0,0,2,0,0,0,0,0,0,0,"Dojani Enforcer - In Combat - Enrage");
