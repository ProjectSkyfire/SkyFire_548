UPDATE `smart_scripts` SET `action_param5`=5774804 WHERE `entryorguid`=57748;

INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5774804, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 10, 508, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Qun - Play Emote 508'),
(5774804, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Qun - Play Random Sound (33643, 33645, 33646)'),
(5774804, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 3, 5, 0, 0, 0, 0, 11, 0, 15, 0, 0, 0, 0, 0, 'Instructor Qun - Set Data 3 5');

DELETE FROM `smart_scripts` WHERE `entryorguid`=53565 AND `source_type`=0 AND `id`=12;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(53565, 0, 12, 0, 38, 0, 100, 0, 3, 5, 0, 0, 0, 80, 5356501, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 3 5 - Start Script');
DELETE FROM `smart_scripts` WHERE `entryorguid`=65469 AND `source_type`=0 AND `id`=12;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(65469, 0, 12, 0, 38, 0, 100, 0, 3, 5, 0, 0, 0, 80, 5356501, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 3 5 - Start Script');
