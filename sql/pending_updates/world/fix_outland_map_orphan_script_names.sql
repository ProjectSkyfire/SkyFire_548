-- Blood Knight Stillblade 17768 (quest 9685 "Redeeming the Dead"), Budd 23559 (quest 11171) and Invis Legion Teleporter
-- 21807 are bound to ScriptNames that have no script in the core. Their behaviour is SmartAI in the other 3.3.5 and
-- 5.4.8 databases, and the teleporter is an area trigger script (at_legion_teleporter, 4560).

-- Stillblade: the Shimmering Vessel (spell 31225) revives him, gives the quest credit, he thanks the player and lies
-- down again after two minutes.
UPDATE `creature_template` SET `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` = 17768;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 17768;
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` = 1776800;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(17768, 0, 0, 0, 8, 0, 100, 0, 31225, 0, 0, 0, 0, 80, 1776800, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blood Knight Stillblade - On Spellhit ''Shimmering Vessel'' - Run Script'),
(1776800, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 17768, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Blood Knight Stillblade - On Script - Quest Credit ''Redeeming the Dead'''),
(1776800, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 32343, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blood Knight Stillblade - On Script - Cast ''Revive Self'''),
(1776800, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 91, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blood Knight Stillblade - On Script - Remove Flag Standstate Dead'),
(1776800, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 96, 40, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blood Knight Stillblade - On Script - Remove Dynamic Flags Tapped By Player & Dead'),
(1776800, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blood Knight Stillblade - On Script - Say Line 0'),
(1776800, 9, 5, 0, 0, 0, 100, 0, 120000, 120000, 0, 0, 0, 94, 40, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blood Knight Stillblade - On Script - Set Dynamic Flags Tapped By Player & Dead'),
(1776800, 9, 6, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 90, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blood Knight Stillblade - On Script - Set Flag Standstate Dead');

-- Budd: his gossip option 0 (menu 8775) closes the window and gives the player the disguise (spell 42540).
UPDATE `creature_template` SET `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` = 23559;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 23559;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(23559, 0, 0, 1, 62, 0, 100, 0, 8775, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Budd - On Gossip Option 0 Selected - Close Gossip'),
(23559, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 42540, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Budd - On Gossip Option 0 Selected - Cast ''Zul''''aman Disguise Creation''');

-- Invis Legion Teleporter: only a marker, the teleport is the area trigger script.
UPDATE `creature_template` SET `ScriptName` = '' WHERE `entry` = 21807;
