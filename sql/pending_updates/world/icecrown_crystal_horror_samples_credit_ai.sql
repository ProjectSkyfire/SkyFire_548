-- Icecrown: quest credit given by a spell hit or a link on creatures that had no SmartAI here.
-- Containment Crystal (13171), Hulking Horror (13259) and the three sample bunnies (13320, 13355).
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (31131,31411,32242,32244,32245) AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (31131,31411,32242,32244,32245);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(31131, 0, 0, 1, 8, 0, 100, 0, 58662, 0, 0, 0, 0, 33, 31131, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Containment Crystal - On Spellhit ''Banish Scourge Crystal Trigger'' - Quest Credit ''From Whence They Came'''),
(31131, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Containment Crystal - On Spellhit ''Banish Scourge Crystal Trigger'' - Despawn Instant'),
(31411, 0, 0, 0, 0, 0, 100, 0, 8000, 16000, 24000, 32000, 0, 11, 49861, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Hulking Horror - In Combat - Cast ''Infected Bite'' (Phase 1) (No Repeat)'),
(31411, 0, 1, 0, 0, 0, 100, 0, 4000, 9000, 14000, 19000, 0, 11, 10966, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Hulking Horror - In Combat - Cast ''Uppercut'' (Phase 1) (No Repeat)'),
(31411, 0, 2, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 33, 31413, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Hulking Horror - On Just Died - Quest Credit ''Establishing Superiority'' (Phase 1) (No Repeat)'),
(32242, 0, 0, 0, 8, 0, 100, 0, 60256, 0, 0, 0, 0, 33, 32242, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Blue Sample KC Bunny - On Spellhit ''Collect Sample'' - Quest Credit ''Cannot Reproduce'''),
(32242, 0, 1, 2, 8, 0, 100, 0, 60310, 0, 0, 0, 0, 33, 32266, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Blue Sample KC Bunny - On Spellhit ''Throw Writhing Mass'' - Quest Credit ''Retest Now'''),
(32242, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 60505, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Blue Sample KC Bunny - On Spellhit ''Fire SGM-3'' - Cast ''Fury of the Fallen Cultist'''),
(32244, 0, 0, 0, 8, 0, 100, 0, 60256, 0, 0, 0, 0, 33, 32244, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Green Sample KC Bunny - On Spellhit ''Collect Sample'' - Quest Credit ''Cannot Reproduce'''),
(32244, 0, 1, 2, 8, 0, 100, 0, 60310, 0, 0, 0, 0, 33, 32266, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Green Sample KC Bunny - On Spellhit ''Throw Writhing Mass'' - Quest Credit ''Retest Now'''),
(32244, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 60506, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Green Sample KC Bunny - On Spellhit ''Throw Writhing Mass'' - Cast ''Blood of the Fallen Cultist'''),
(32245, 0, 0, 0, 8, 0, 100, 0, 60256, 0, 0, 0, 0, 33, 32245, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dark Sample KC Bunny - On Spellhit ''Collect Sample'' - Quest Credit ''Cannot Reproduce'''),
(32245, 0, 1, 2, 8, 0, 100, 0, 60310, 0, 0, 0, 0, 33, 32266, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dark Sample KC Bunny - On Spellhit ''Throw Writhing Mass'' - Quest Credit ''Retest Now'''),
(32245, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 60504, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dark Sample KC Bunny - On Spellhit ''Throw Writhing Mass'' - Cast ''Agony of the Fallen Cultist''');
