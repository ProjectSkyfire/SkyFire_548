-- Soul-Fed Construct SAI
SET @ENTRY := 70308;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,11,0,100,6,0,0,0,0,113,33727360,6,0,0,0,0,1,0,0,0,0,0,0,0,"Soul-Fed Construct - On Respawn - Set Health Value = 33,727,360 in 10 N and 10 HC. "),
(@ENTRY,0,1,0,0,0,100,20,0,0,0,0,113,101182080,6,0,0,0,0,0,0,0,0,0,0,0,0,"Soul-Fed Construct - On Respawn - Set Health Value = 101,182,080 in 25 N and 25 HC. "),
(@ENTRY,0,2,0,0,0,100,0,5000,7000,9000,11000,11,33661,0,0,0,0,0,5,0,0,0,0,0,0,0,"Soul-Fed Construct - In Combat - Cast Crush Armor."),
(@ENTRY,0,3,0,0,0,100,0,10000,10000,15000,22000,11,139895,1,0,0,0,0,5,0,0,0,0,0,0,0,"Soul-Fed Construct - In Combat - Cast Spiritfire Beam.");
