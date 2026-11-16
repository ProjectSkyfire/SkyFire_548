-- Sholazar Basin: 12734 "Rejek: First Blood" gives credit when Blood Rejek's Sword hits a Sapphire Hive Wasp, Mistwhisper Warrior or Oracle.
-- The three creatures had no SmartAI here.
-- SmartAI of 28086, 28109, 28110
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (28086,28109,28110) AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (28086,28109,28110);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(28086, 0, 0, 0, 2, 0, 100, 0, 0, 30, 7000, 9000, 0, 11, 34392, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sapphire Hive Wasp - Between 0-30% Health - Cast ''Stinger Rage'''),
(28086, 0, 1, 2, 8, 0, 100, 1, 52992, 0, 0, 0, 0, 33, 28040, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Sapphire Hive Wasp - On Spellhit ''Blood Rejek''s Sword'' - Quest Credit ''Rejek: First Blood'' (No Repeat)'),
(28086, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sapphire Hive Wasp - On Spellhit ''Blood Rejek''s Sword'' - Despawn Instant (No Repeat)'),
(28109, 0, 0, 0, 0, 0, 100, 0, 2000, 6000, 4000, 7000, 0, 11, 50533, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mistwhisper Warrior - In Combat - Cast ''Flip Attack'''),
(28109, 0, 1, 2, 8, 0, 100, 1, 52992, 0, 0, 0, 0, 33, 29043, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mistwhisper Warrior - On Spellhit ''Blood Rejek''s Sword'' - Quest Credit ''Rejek: First Blood'' (No Repeat)'),
(28109, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mistwhisper Warrior - On Spellhit ''Blood Rejek''s Sword'' - Despawn Instant (No Repeat)'),
(28110, 0, 0, 0, 0, 0, 100, 0, 4000, 7000, 11000, 19000, 0, 11, 54921, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mistwhisper Oracle - In Combat - Cast ''Lightning Cloud'' (No Repeat)'),
(28110, 0, 1, 2, 8, 0, 100, 1, 52992, 0, 0, 0, 0, 33, 29043, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mistwhisper Oracle - On Spellhit ''Blood Rejek''s Sword'' - Quest Credit ''Rejek: First Blood'' (No Repeat)'),
(28110, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mistwhisper Oracle - On Spellhit ''Blood Rejek''s Sword'' - Despawn Instant (No Repeat)');
