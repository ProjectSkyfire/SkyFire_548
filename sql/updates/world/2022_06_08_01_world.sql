-- Wildscale Herbalist SAI
UPDATE creature_template SET AIName='SmartAI' WHERE entry=58216;
DELETE FROM `smart_scripts` WHERE `entryorguid`=58216;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(58216, 0, 0, 0, 0, 0, 100, 0, 0, 0, 3300, 5100, 11, 119577, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Wildscale Herbalist - In Combat - Cast Wrath'),
(58216, 0, 1, 0, 14, 0, 80, 1, 0, 40, 0, 0, 11, 119575, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Wildscale Herbalist - On Friendly Unit At 0 - 40% Health - Cast Healing Wave');
