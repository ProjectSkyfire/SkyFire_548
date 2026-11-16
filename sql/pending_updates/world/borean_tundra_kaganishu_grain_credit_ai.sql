-- Borean Tundra: quest credit for Kaganishu and Take No Chances, given by a spell hit on a creature.
-- The creatures had no SmartAI here, so the quests could not be completed.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (25425,26161) AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (25425,26161);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(25425, 0, 0, 0, 8, 0, 100, 0, 45607, 0, 0, 0, 0, 33, 25425, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Farseer Grimwalker''s Spirit - On Spellhit ''Kaganishu''s Fetish Trigger'' - Quest Credit ''Kaganishu'' (No Repeat)'),
(26161, 0, 0, 0, 8, 0, 100, 0, 46574, 0, 0, 0, 0, 33, 26161, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Farshire Grain Credit - On Spellhit ''Burn Grain'' - Quest Credit ''Take No Chances''');
