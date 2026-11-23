-- Azshara: Wasn't It Obvious? and Ready the Ground Troops.
DELETE FROM `spell_target_position` WHERE `id`=68744;
INSERT INTO `spell_target_position` (`id`,`effIndex`,`target_map`,`target_position_x`,`target_position_y`,`target_position_z`,`target_orientation`) VALUES
(68744,0,1,3922.08,-4891.25,150.864,5.39);

DELETE FROM `creature` WHERE `guid`=9257000;
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseId`,`phaseGroup`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`) VALUES
(9257000,37040,1,1,0,0,0,0,3925.43,-4895.41,150.803,3.28122,300,0,0,0,0,0,0,0,0);

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=37040;

DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid` IN (37040);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(37040,0,0,0,10,0,100,0,1,40,5000,5000,0,15,14389,0,0,0,0,0,7,0,0,0,0,0,0,0,'Anara - Out of Combat LoS - Area Explored Or Event Happens (Wasn\'t It Obvious?)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=42646;
DELETE FROM `creature_text` WHERE `entry`=42646;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(42646,0,0,'$gRight away, sir:Right away ma\'am;!',12,0,100,2,0,0,'AWOL Grunt to Player'),
(42646,1,0,'Please don\'t tell the Warlord...',12,0,100,2,0,0,'AWOL Grunt to Player'),
(42646,3,0,'It won\'t happen again, $gsir:ma\'am;.',12,0,100,16,0,0,'AWOL Grunt to Player');
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid` IN (42646);
DELETE FROM `smart_scripts` WHERE `source_type`=9 AND `entryorguid` IN (4264601, 4264602, 4264603);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(42646,0,0,0,73,0,100,0,0,0,0,0,0,87,4264601,4264602,0,0,0,0,1,0,0,0,0,0,0,0,'AWOL Grunt - On Spellclick - Run Random Actionlist'),
(42646,0,1,0,2,0,100,1,0,30,0,0,0,80,4264603,2,0,0,0,0,1,0,0,0,0,0,0,0,'AWOL Grunt - Between 0-30% Health - Run Actionlist'),
(4264601,9,0,0,0,0,100,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'AWOL Grunt - On Script - Say Line 0'),
(4264601,9,1,0,0,0,100,0,0,0,0,0,0,33,42893,0,0,0,0,0,7,0,0,0,0,0,0,0,'AWOL Grunt - On Script - Quest Credit (AWOL Grunt Disciplined)'),
(4264601,9,2,0,0,0,100,0,0,0,0,0,0,83,16777216,0,0,0,0,0,1,0,0,0,0,0,0,0,'AWOL Grunt - On Script - Remove Spellclick Flag'),
(4264601,9,3,0,0,0,100,0,2000,2000,0,0,0,46,15,0,0,0,0,0,1,0,0,0,0,0,0,0,'AWOL Grunt - On Script - Move Forward'),
(4264601,9,4,0,0,0,100,0,4000,4000,0,0,0,41,1000,0,0,0,0,0,1,0,0,0,0,0,0,0,'AWOL Grunt - On Script - Despawn'),
(4264602,9,0,0,0,0,100,0,0,0,0,0,0,2,16,0,0,0,0,0,1,0,0,0,0,0,0,0,'AWOL Grunt - On Script - Set Faction Hostile'),
(4264602,9,1,0,0,0,100,0,0,0,0,0,0,83,16777216,0,0,0,0,0,1,0,0,0,0,0,0,0,'AWOL Grunt - On Script - Remove Spellclick Flag'),
(4264602,9,2,0,0,0,100,0,0,0,0,0,0,49,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'AWOL Grunt - On Script - Start Attack'),
(4264603,9,0,0,0,0,100,0,0,0,0,0,0,2,125,0,0,0,0,0,1,0,0,0,0,0,0,0,'AWOL Grunt - On Script - Set Faction Friendly'),
(4264603,9,1,0,0,0,100,0,0,0,0,0,0,27,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'AWOL Grunt - On Script - Combat Stop'),
(4264603,9,2,0,0,0,100,0,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'AWOL Grunt - On Script - Say Line 1'),
(4264603,9,3,0,0,0,100,0,0,0,0,0,0,33,42893,0,0,0,0,0,21,60,0,0,0,0,0,0,'AWOL Grunt - On Script - Quest Credit (AWOL Grunt Disciplined)'),
(4264603,9,4,0,0,0,100,0,3000,3000,0,0,0,1,3,0,0,0,0,0,1,0,0,0,0,0,0,0,'AWOL Grunt - On Script - Say Line 3'),
(4264603,9,5,0,0,0,100,0,0,0,0,0,0,46,15,0,0,0,0,0,1,0,0,0,0,0,0,0,'AWOL Grunt - On Script - Move Forward'),
(4264603,9,6,0,0,0,100,0,5000,5000,0,0,0,41,1000,0,0,0,0,0,1,0,0,0,0,0,0,0,'AWOL Grunt - On Script - Despawn');
