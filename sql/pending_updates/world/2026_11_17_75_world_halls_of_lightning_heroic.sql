-- Heroic fixes for Halls of Lightning. The Titanium Thunderer never threw: Throw and Deadly Throw, with their heroic
-- variants, are the TrinityCore rows.

DELETE FROM `smart_scripts` WHERE `entryorguid` = 28965 AND `source_type` = 0 AND `id` IN (4, 5, 6, 7);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(28965, 0, 4, 0, 13, 0, 100, 5, 45000, 60000, 0, 0, 0, 11, 52885, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Titanium Thunderer - On Target Casting - Cast Deadly Throw'),
(28965, 0, 5, 0, 13, 0, 100, 9, 45000, 60000, 0, 0, 0, 11, 59180, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Titanium Thunderer - On Target Casting - Cast Deadly Throw (Heroic)'),
(28965, 0, 6, 0, 0, 0, 100, 4, 0, 5000, 7000, 15000, 0, 11, 52904, 2, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Titanium Thunderer - IC - Cast Throw'),
(28965, 0, 7, 0, 0, 0, 100, 8, 0, 5000, 7000, 15000, 0, 11, 59179, 2, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Titanium Thunderer - IC - Cast Throw (Heroic)');
