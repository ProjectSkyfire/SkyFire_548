-- The Eye: SmartAI for the Novice Astromancer (30 spawns, no AI)
-- Ported from TrinityCore 3.3.5.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 20044 AND `AIName` = '' AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 20044;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(20044, 0, 0, 0, 0, 0, 50, 1, 1000, 5000, 0, 0, 0, 11, 37282, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Novice Astromancer - In Combat - Cast ''Fire Shield'' (No Repeat)'),
(20044, 0, 1, 0, 0, 0, 50, 0, 5000, 10000, 10000, 15000, 0, 11, 37111, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Novice Astromancer - In Combat - Cast ''Fireball'''),
(20044, 0, 2, 0, 0, 0, 50, 0, 5000, 10000, 10000, 15000, 0, 11, 37279, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Novice Astromancer - In Combat - Cast ''Arcane Volley'''),
(20044, 0, 3, 0, 0, 0, 50, 0, 10000, 15000, 10000, 15000, 0, 11, 38728, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Novice Astromancer - In Combat - Cast ''Fire Nova''');
