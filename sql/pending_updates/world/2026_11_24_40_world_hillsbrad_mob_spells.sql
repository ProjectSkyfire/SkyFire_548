-- Hillsbrad Foothills: combat spells and flee rows of the kill targets of Preemptive Strike (28356), Terrible Little
-- Creatures (28355) and the Dun Garok and Torn Fin mobs. The creatures have the SmartAI name but no rows, so they only
-- melee. Added as in Legends of Azeroth 5.4.8: the Dun Garok Mountaineer (Holy Strike), the Daggerspine Siren (Lightning
-- Bolt, Enveloping Winds), the Mudsnout Shaman (Lightning Bolt), the Torn Fin Oracle (Lightning Bolt) and, for all of
-- them and the Torn Fin Tidehunter, fleeing for help at low health.

DELETE FROM `smart_scripts` WHERE `entryorguid` = 2344 AND `source_type` = 0 AND `id` IN (1, 2);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2371 AND `source_type` = 0 AND `id` IN (0, 2, 3);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2373 AND `source_type` = 0 AND `id` IN (0, 2);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2376 AND `source_type` = 0 AND `id` IN (0, 2);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2377 AND `source_type` = 0 AND `id` IN (1);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2344, 0, 1, 0, 9, 0, 100, 0, 0, 5, 9000, 15000, 0, 11, 13953, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Dun Garok Mountaineer - Within 0-5 Range - Cast ''Holy Strike'' (No Repeat)'),
(2344, 0, 2, 0, 2, 0, 100, 1, 0, 30, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dun Garok Mountaineer - Between 0-30% Health - Flee For Assist (No Repeat)'),
(2371, 0, 0, 0, 0, 0, 100, 0, 0, 0, 3400, 4800, 0, 11, 9532, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Daggerspine Siren - In Combat CMC - Cast ''Lightning Bolt'''),
(2371, 0, 2, 0, 0, 0, 100, 0, 11000, 15000, 17000, 21000, 0, 11, 6728, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 'Daggerspine Siren - In Combat - Cast ''Enveloping Winds'''),
(2371, 0, 3, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Daggerspine Siren - Between 0-15% Health - Flee For Assist (No Repeat)'),
(2373, 0, 0, 0, 0, 0, 100, 0, 0, 0, 3400, 4800, 0, 11, 20805, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mudsnout Shaman - In Combat CMC - Cast ''Lightning Bolt'''),
(2373, 0, 2, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mudsnout Shaman - Between 0-15% Health - Flee For Assist'),
(2376, 0, 0, 0, 0, 0, 100, 0, 0, 0, 3400, 4800, 0, 11, 9532, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Torn Fin Oracle - In Combat CMC - Cast ''Lightning Bolt'''),
(2376, 0, 2, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Torn Fin Oracle - Between 0-15% Health - Flee For Assist (No Repeat)'),
(2377, 0, 1, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Torn Fin Tidehunter - Between 0-15% Health - Flee For Assist (No Repeat)');
