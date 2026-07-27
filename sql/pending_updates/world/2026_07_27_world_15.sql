-- Milo's Gyro SAI
DELETE FROM `smart_scripts` WHERE `entryorguid`=37198;
DELETE FROM `smart_scripts` WHERE `entryorguid`=3719800;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(37198, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 80, 3719800, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Milo\'s Gyro - On Spawn - Run Script'),
(37198, 0, 1, 0, 40, 0, 100, 0, 24, 0, 0, 0, 0, 41, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Milo\'s Gyro - On Reach WP - Despawn'),
(3719800, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 53, 1, 37198, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Milo\'s Gyro - On Script - Start Waypoint Movement');
