-- Howling Fjord: the quest NPCs that hand out quest items through gossip had no SmartAI here.
-- Captain Adams and Chieftain Ashtotem create the Skorn insignia and tokens.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (23749,24129) AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (23749,24129);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(23749, 0, 0, 1, 62, 0, 100, 0, 8852, 0, 0, 0, 0, 85, 43122, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captain Adams - On Gossip Option 0 Selected - Invoker cast Operation: Skornful Wrath: Create Insignia'),
(23749, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captain Adams - Link - Close Gossip'),
(24129, 0, 0, 1, 62, 0, 100, 0, 8898, 0, 0, 0, 0, 85, 43123, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Chieftain Ashtotem - On Gossip Option 0 Selected - Invoker cast Skorn Must Fall!: Create Token'),
(24129, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Chieftain Ashtotem - Link - Close Gossip');
