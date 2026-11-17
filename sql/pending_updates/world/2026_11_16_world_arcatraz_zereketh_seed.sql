-- Zereketh the Unbound never cast Seed of Corruption. The heroic spell is chosen through spelldifficulty_dbc.

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 20870 AND `id` = 9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(20870, 0, 9, 0, 0, 0, 100, 12, 5000, 10000, 15000, 20000, 0, 11, 36123, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Zereketh The Unbound - Combat - Cast Seed of Corruption');
