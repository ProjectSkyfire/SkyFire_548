-- Tavarok: report the fight to the instance script (field 1 = Tavarok, data = encounter state)
DELETE FROM `smart_scripts` WHERE `entryorguid` = 18343 AND `source_type` = 0 AND `id` IN (4, 5, 6);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(18343, 0, 4, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 34, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tavarok - On Aggro - Set Instance Data 1 to 1'),
(18343, 0, 5, 0, 7, 0, 100, 0, 0, 0, 0, 0, 0, 34, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tavarok - On Evade - Set Instance Data 1 to 0'),
(18343, 0, 6, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 34, 1, 3, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tavarok - On Death - Set Instance Data 1 to 3');
