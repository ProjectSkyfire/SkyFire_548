-- Borean Tundra: quest credit for Leading the Ancestors Home, Hampering Their Escape and Emergency Protocol,
-- given by a spell hit on an NPC or bunny. The creatures had no SmartAI here, so the quests could not be completed.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (25397,25398,25399,25441,25442,25443,25845,25846,25847) AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (25397,25398,25399,25441,25442,25443,25845,25846,25847);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(25397, 0, 0, 0, 8, 0, 100, 0, 45536, 0, 0, 0, 0, 33, 25397, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Elder Kesuk - On Spellhit ''Complete Ancestor Ritual'' - Quest Credit ''Leading the Ancestors Home'''),
(25398, 0, 0, 0, 8, 0, 100, 0, 45536, 0, 0, 0, 0, 33, 25398, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Elder Sagani - On Spellhit ''Complete Ancestor Ritual'' - Quest Credit ''Leading the Ancestors Home'''),
(25399, 0, 0, 0, 8, 0, 100, 0, 45536, 0, 0, 0, 0, 33, 25399, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Elder Takret - On Spellhit ''Complete Ancestor Ritual'' - Quest Credit ''Leading the Ancestors Home'''),
(25441, 0, 0, 0, 8, 0, 100, 0, 45583, 0, 0, 0, 0, 33, 25441, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'North Platform - On Spellhit ''Throw Gnomish Grenade'' - Quest Credit ''Hampering Their Escape'' (No Repeat)'),
(25442, 0, 0, 0, 8, 0, 100, 0, 45583, 0, 0, 0, 0, 33, 25442, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'East Platform - On Spellhit ''Throw Gnomish Grenade'' - Quest Credit ''Hampering Their Escape'' (No Repeat)'),
(25443, 0, 0, 0, 8, 0, 100, 0, 45583, 0, 0, 0, 0, 33, 25443, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'West Platform - On Spellhit ''Throw Gnomish Grenade'' - Quest Credit ''Hampering Their Escape'' (No Repeat)'),
(25845, 0, 0, 0, 8, 0, 100, 0, 46171, 0, 0, 0, 0, 33, 25845, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Northwest Crash - On Spellhit - Kill Credit'),
(25846, 0, 0, 0, 8, 0, 100, 0, 46171, 0, 0, 0, 0, 33, 25846, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'South Crash - On Spellhit - Kill Credit'),
(25847, 0, 0, 0, 8, 0, 100, 0, 46171, 0, 0, 0, 0, 33, 25847, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'East Crash - On Spellhit - Kill Credit');
