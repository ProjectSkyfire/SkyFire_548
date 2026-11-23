-- Haunted Servitor (3875) casts nothing in heroic mode: its only row is limited to normal mode.
-- The 4.3.4 data casts Haunting Spirits in both modes and adds Drain Life in heroic mode only
-- (event flags: 2 normal, 4 heroic).
UPDATE `smart_scripts` SET `event_flags` = 0 WHERE `entryorguid` = 3875 AND `source_type` = 0 AND `id` = 0;

DELETE FROM `smart_scripts` WHERE `entryorguid` = 3875 AND `source_type` = 0 AND `id` = 1;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(3875, 0, 1, 0, 0, 0, 100, 8, 3000, 3000, 3000, 3000, 0, 11, 91291, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Haunted Servitor - In Combat - Cast Drain Life (heroic)');
