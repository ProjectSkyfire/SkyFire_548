-- Mogujia Soul-Caller SAI
UPDATE creature_template SET AIName='SmartAI' WHERE entry=59797;
DELETE FROM `smart_scripts` WHERE `entryorguid`=59797;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(59797, 0, 0, 0, 0, 0, 70, 2, 25000, 25000, 25000, 25000, 11, 129132, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Mogujia Soul-Caller - In Combat - Cast Shadow Crash - on random target'),
(59797, 0, 1, 0, 0, 0, 75, 2, 15000, 15000, 30000, 30000, 11, 9613, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mogujia Soul-Caller - In Combat - Cast Shadow Bolt'),
(59797, 0, 2, 0, 2, 0, 100, 0, 0, 30, 0, 0, 11, 84533, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mogujia Soul-Caller - At 30% HP - Cast Drain Life');
