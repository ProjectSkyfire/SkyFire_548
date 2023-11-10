UPDATE `creature_template` SET `AIName`='SmartAI' WHERE  `entry`=61411;

DELETE FROM `smart_scripts` WHERE `entryorguid`=61411;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(61411, 0, 0, 0, 1, 0, 100, 0, 5000, 5000, 5000, 5000, 87, 6141101, 6141102, 6141103, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Zhi - OOC - Run Random Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=6141101;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(6141101, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 5, 507, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Zhi - Play Emote 507'),
(6141101, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 45, 0, 1, 0, 0, 0, 0, 11, 54587, 10, 0, 0, 0, 0, 0, 'Instructor Zhi - Set Data 0 1'),
(6141101, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 11, 65471, 10, 0, 0, 0, 0, 0, 'Instructor Zhi - Set Data 0 1');

DELETE FROM `smart_scripts` WHERE `entryorguid`=6141102;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(6141102, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 5, 508, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Zhi - Play Emote 508'),
(6141102, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 45, 0, 2, 0, 0, 0, 0, 11, 54587, 10, 0, 0, 0, 0, 0, 'Instructor Zhi - Set Data 0 2'),
(6141102, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 45, 0, 2, 0, 0, 0, 0, 11, 65471, 10, 0, 0, 0, 0, 0, 'Instructor Zhi - Set Data 0 2');

DELETE FROM `smart_scripts` WHERE `entryorguid`=6141103;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(6141103, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 5, 509, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Zhi - Play Emote 509'),
(6141103, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 45, 0, 3, 0, 0, 0, 0, 11, 54587, 10, 0, 0, 0, 0, 0, 'Instructor Zhi - Set Data 0 3'),
(6141103, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 45, 0, 3, 0, 0, 0, 0, 11, 65471, 10, 0, 0, 0, 0, 0, 'Instructor Zhi - Set Data 0 3');
