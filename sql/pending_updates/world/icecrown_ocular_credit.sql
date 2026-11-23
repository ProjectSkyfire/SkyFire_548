-- It's All Fun and Games (12887, 12892): killing The Ocular gave no credit, because its SmartAI had no scripts.
-- An upstream cleanup of 2017 deleted them with other Cataclysm leftovers. The kill credit spell (55289) is cast by
-- the killer.
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 29747;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(29747, 0, 0, 0, 0, 0, 100, 0, 1500, 1500, 4000, 4000, 0, 11, 55269, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'The Ocular - In Combat - Cast Deathly Stare'),
(29747, 0, 1, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 85, 55289, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'The Ocular - On Death - Invoker Cast The Ocular Kill Credit'),
(29747, 0, 2, 0, 25, 0, 100, 0, 0, 0, 0, 0, 0, 75, 55162, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'The Ocular - On Reset - Add Aura The Ocular: Transform');
