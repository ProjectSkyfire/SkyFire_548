-- Howling Fjord: the quest NPCs of the Alpha Worg quests had no SmartAI here.
-- Ulfang and Watcher Moonleaf run the Alpha Worg quests.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (24261,24273) AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (24261,24273);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(24261, 0, 0, 0, 19, 0, 100, 0, 11326, 0, 0, 0, 0, 85, 50102, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Ulfang - On Quest Accept Alpha Worg - Invoker Cast Ulfang: Force Cast Player Eyes of the Eagle'),
(24261, 0, 1, 0, 19, 0, 100, 0, 11324, 0, 0, 0, 0, 85, 50102, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Ulfang - On Quest Accept Alpha Worg - Invoker cast Ulfang: Force Cast Player Eyes of the Eagle'),
(24273, 0, 0, 1, 20, 0, 100, 0, 11326, 0, 0, 0, 0, 28, 50102, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Watcher Moonleaf - On Quest Reward Alpha Worg - Remove Aura Ulfang: Force Cast Player Eyes of the Eagle'),
(24273, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 28, 43060, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Watcher Moonleaf - Linked with Previous Event - Remove Aura Eyes of the Eagle'),
(24273, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 28, 43369, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Watcher Moonleaf - Linked with Previous Event - Remove Aura Worg Disguise'),
(24273, 0, 3, 4, 62, 0, 100, 0, 8918, 0, 0, 0, 0, 85, 43379, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Watcher Moonleaf - On Gossip Option Select - Invoker Cast The Cleansing: Create Worg Disguise'),
(24273, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Watcher Moonleaf - Linked with Previous Event - Close Gossip');
