UPDATE `creature` SET `id`=54587 WHERE `guid`=224359;
UPDATE `creature` SET `id`=54587 WHERE `guid`=224374;

DELETE FROM `smart_scripts` WHERE `entryorguid`=-224377;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-224377, 0, 0, 0, 1, 0, 100, 0, 5000, 5000, 5000, 5000, 87, 5458701, 5458702, 5458703, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - OOC - Run Random Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5458701;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5458701, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 5, 507, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Script - Play Emote 507'),
(5458701, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 11, 54587, 10, 0, 0, 0, 0, 0, 'Tushui Trainee - On Script - Set Data 0 1');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5458702;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5458702, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 5, 508, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Script - Play Emote 508'),
(5458702, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 2, 0, 0, 0, 0, 11, 54587, 10, 0, 0, 0, 0, 0, 'Tushui Trainee - On Script - Set Data 0 2');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5458703;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5458703, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 5, 509, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Script - Play Emote 509'),
(5458703, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 3, 0, 0, 0, 0, 11, 54587, 10, 0, 0, 0, 0, 0, 'Tushui Trainee - On Script - Set Data 0 3');
