-- Duskwood: the Nightbane Shadow Weaver (533, "The Rotting Orchard" 26689) and the Skeletal Healer (787) use SmartAI but
-- had no rows, so they never cast. Both cast Shadow Bolt in combat, as in Legends of Azeroth 5.4.8.

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (533, 787);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(533, 0, 0, 0, 0, 0, 100, 0, 0, 0, 2800, 3800, 0, 11, 9613, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Nightbane Shadow Weaver - In Combat CMC - Cast ''Shadow Bolt'''),
(787, 0, 0, 0, 0, 0, 100, 0, 0, 0, 3500, 3700, 0, 11, 9613, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Skeletal Healer - In Combat CMC - Cast ''Shadow Bolt''');
