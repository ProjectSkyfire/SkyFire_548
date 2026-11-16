-- Icecrown: quest credit given by a spell hit on creatures that had no SmartAI here.
-- 29392 Ravenous Jaws (12810) and the four Summoning Altars (13119).
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (29392,30742,30744,30745,30950) AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (29392,30742,30744,30745,30950);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(29392, 0, 0, 0, 0, 0, 100, 0, 2000, 5000, 4000, 7000, 0, 11, 3391, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ravenous Jaws - In Combat - Cast ''Thrash'''),
(29392, 0, 1, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 11, 47172, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ravenous Jaws - On Just Died - Cast ''Cosmetic - Underwater Blood (no sound)'''),
(29392, 0, 2, 3, 8, 0, 100, 0, 6509, 0, 0, 0, 0, 28, 47172, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ravenous Jaws - On Spellhit ''Gore Bladder'' - Remove Aura ''Cosmetic - Underwater Blood (no sound)'''),
(29392, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 29391, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Ravenous Jaws - On Spellhit ''Gore Bladder'' - Quest Credit ''Blood in the Water'''),
(30742, 0, 0, 0, 8, 0, 100, 0, 57852, 0, 0, 0, 0, 33, 30742, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'First Summoning Altar - On Spellhit ''Destroy Altar'' - Quest Credit ''Destroying the Altars'' (Phase 1)'),
(30744, 0, 0, 0, 8, 0, 100, 0, 57852, 0, 0, 0, 0, 33, 30744, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Second Summoning Altar - On Spellhit ''Destroy Altar'' - Quest Credit ''Destroying the Altars'' (Phase 1)'),
(30745, 0, 0, 0, 8, 0, 100, 0, 57852, 0, 0, 0, 0, 33, 30745, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Third Summoning Altar - On Spellhit ''Destroy Altar'' - Quest Credit ''Destroying the Altars'' (Phase 1)'),
(30950, 0, 0, 0, 8, 0, 100, 0, 57852, 0, 0, 0, 0, 33, 30950, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Fourth Summoning Altar - On Spellhit ''Destroy Altar'' - Quest Credit ''Destroying the Altars''');
