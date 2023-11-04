-- Thunderfist Gorilla SAI
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=59742;
DELETE FROM `smart_scripts` WHERE `entryorguid`=59785;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(59742, 0, 1, 0, 0, 0, 100, 0, 4000, 5000, 8000, 9000, 11, 116013, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Thunderfist Gorilla - In Combat - Cast Thunderfist Rage'),
(59742, 0, 2, 0, 0, 0, 100, 0, 8000, 9000, 12000, 13000, 11, 116007, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Thunderfist Gorilla - In Combat - Cast Thunderfist');
