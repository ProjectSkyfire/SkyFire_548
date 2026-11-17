-- Swamplord Musel'ek never cast Raptor Strike or Deterrence, both listed in the client journal. Deterrence is cast
-- at 70% and 30% health with a 50% chance, as in the reference script.

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 17826 AND `id` IN (18, 19, 20);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(17826, 0, 18, 0, 0, 0, 100, 12, 10000, 20000, 10000, 20000, 0, 11, 31566, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Swamplord Musel''ek - In Combat - Cast Raptor Strike'),
(17826, 0, 19, 0, 2, 0, 50, 13, 0, 70, 0, 0, 0, 11, 31567, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Swamplord Musel''ek - Between 0-70% Health - Cast Deterrence'),
(17826, 0, 20, 0, 2, 0, 50, 13, 0, 30, 0, 0, 0, 11, 31567, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Swamplord Musel''ek - Between 0-30% Health - Cast Deterrence');
