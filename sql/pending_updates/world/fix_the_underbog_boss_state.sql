-- Ghaz'an and Swamplord Musel'ek: report the fight to the instance script
-- (field 1 = Ghaz'an, field 2 = Musel'ek, data = encounter state)
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND ((`entryorguid` = 18105 AND `id` IN (7, 8, 9)) OR (`entryorguid` = 17826 AND `id` IN (15, 16, 17)));
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(18105, 0, 7, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 34, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ghaz''an - On Aggro - Set Instance Data 1 to 1'),
(18105, 0, 8, 0, 7, 0, 100, 0, 0, 0, 0, 0, 0, 34, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ghaz''an - On Evade - Set Instance Data 1 to 0'),
(18105, 0, 9, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 34, 1, 3, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ghaz''an - On Death - Set Instance Data 1 to 3'),
(17826, 0, 15, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 34, 2, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Swamplord Musel''ek - On Aggro - Set Instance Data 2 to 1'),
(17826, 0, 16, 0, 7, 0, 100, 0, 0, 0, 0, 0, 0, 34, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Swamplord Musel''ek - On Evade - Set Instance Data 2 to 0'),
(17826, 0, 17, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 34, 2, 3, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Swamplord Musel''ek - On Death - Set Instance Data 2 to 3');
