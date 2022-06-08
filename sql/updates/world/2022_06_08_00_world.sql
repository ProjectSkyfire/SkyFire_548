-- Bataari Flamecaller SAI
UPDATE creature_template SET AIName='SmartAI' WHERE entry=59148;
DELETE FROM `smart_scripts` WHERE `entryorguid`=59148;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(59148, 0, 0, 0, 0, 0, 100, 0, 7000, 10000, 10000, 15000, 11, 11970, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Bataari Flamecaller - In Combat - Cast Fire Nova'),
(59148, 0, 1, 0, 0, 0, 100, 0, 9000, 12000, 8000, 12000, 11, 15536, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Bataari Flamecaller - In Combat - Cast Fire Ball');
