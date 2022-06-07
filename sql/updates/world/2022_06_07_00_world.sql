-- Shadowmancer Selenora SAI
UPDATE creature_template SET AIName='SmartAI' WHERE entry=68009;
DELETE FROM `smart_scripts` WHERE `entryorguid`=68009;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(68009, 0, 0, 0, 0, 0, 70, 2, 25000, 25000, 25000, 25000, 11, 14875, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Shadowmancer Selenora - In Combat - Cast Curse of Agony'),
(68009, 0, 1, 0, 0, 0, 75, 2, 15000, 15000, 30000, 30000, 11, 12471, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Shadowmancer Selenora - In Combat - Cast Fel Immolate');
