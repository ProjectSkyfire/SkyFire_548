-- Mortbreath Snapper SAI
SET @SNAPPER         := 60201; -- Mortbreath Snapper
SET @SPELL_CHARGE    := 87930; -- Charge
SET @SPELL_JAW_SNAP  := 118990; -- Jaw Snap
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@SNAPPER;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@SNAPPER AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@SNAPPER,0,0,0,9,0,100,0,5,40,1000,1500,11,@SPELL_CHARGE,0,0,0,0,0,2,0,0,0,0,0,0,0,"Mortbreath Snapper - Within 5-40 Range - Charge"),
(@SNAPPER,0,1,0,0,0,100,0,12000,12900,12500,13400,11,@SPELL_JAW_SNAP,0,0,0,0,0,2,0,0,0,0,0,0,0,"Mortbreath Snapper - In Combat - Jaw Snap");
