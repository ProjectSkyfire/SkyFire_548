-- Orgrimmar: Traitor's Bait.
DELETE FROM `creature` WHERE `guid`=9257001;
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseId`,`phaseGroup`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`) VALUES
(9257001,44120,1,1,0,0,0,0,1735.44,-4284.23,101.413,0.715585,300,0,0,0,0,0,0,0,0);

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry` IN (44120,44160);
DELETE FROM `creature_text` WHERE `entry`=44120;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(44120,1,0,'Hear anything interesting, $n? Your little revelation about me is about to cost you your life.',12,0,100,1,0,0,'Sauranok the Mystic to Player'),
(44120,2,0,'The Twilight\'s Hammer has agents everywhere! Kill me and a dozen take my place!',12,0,100,0,0,0,'Sauranok the Mystic');
DELETE FROM `creature_text` WHERE `entry`=44160;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(44160,0,0,'Wait, uh - you put words in my mouth! Not fair!',12,0,100,0,0,0,'Suspicious Peon to Player');
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid` IN (44120, 44160);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(44160,0,0,1,62,0,100,0,11807,0,0,0,0,33,44132,0,0,0,0,0,7,0,0,0,0,0,0,0,'Suspicious Peon - On Gossip Option 0 Selected - Quest Credit (Sauranok\'s Guilt Revealed)'),
(44160,0,1,2,61,0,100,0,0,0,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Suspicious Peon - Linked To Id 0 - Close Gossip'),
(44160,0,2,3,61,0,100,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Suspicious Peon - Linked To Id 1 - Say Line 0'),
(44160,0,3,0,61,0,100,0,0,0,0,0,0,45,1,1,0,0,0,0,19,44120,100,0,0,0,0,0,'Suspicious Peon - Linked To Id 2 - Set Data 1 1 On Sauranok the Mystic'),
(44120,0,0,1,38,0,100,0,1,1,0,0,0,19,768,0,0,0,0,0,1,0,0,0,0,0,0,0,'Sauranok the Mystic - On Data 1 1 Set - Remove Flags Immune To Players And NPCs'),
(44120,0,1,2,61,0,100,0,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Sauranok the Mystic - Linked To Id 0 - Say Line 1'),
(44120,0,2,0,61,0,100,0,0,0,0,0,0,49,0,0,0,0,0,0,21,100,0,0,0,0,0,0,'Sauranok the Mystic - Linked To Id 1 - Start Attack'),
(44120,0,3,0,6,0,100,0,0,0,0,0,0,1,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'Sauranok the Mystic - On Just Died - Say Line 2');
