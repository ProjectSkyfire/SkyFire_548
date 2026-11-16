-- Icecrown Citadel: SmartAI for the Skybreaker and Kor'kron healers and ranged crew that had none
-- Ported from TrinityCore 3.3.5.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (37016,37021,37027,37030,37031,37144) AND `AIName` = '' AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (37016,37021,37027,37030,37031,37144);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(37016, 0, 0, 0, 0, 0, 100, 0, 1000, 1500, 20000, 25000, 0, 11, 69926, 0, 0, 0, 0, 0, 26, 40, 0, 0, 0, 0, 0, 0, 'Skybreaker Luminary - In Combat - Cast ''Earth Shield'''),
(37016, 0, 1, 0, 0, 0, 100, 0, 3000, 3000, 6000, 7000, 0, 11, 69958, 0, 0, 0, 0, 0, 26, 40, 0, 0, 0, 0, 0, 0, 'Skybreaker Luminary - In Combat - Cast ''Healing Wave'''),
(37016, 0, 2, 0, 0, 0, 100, 0, 4000, 6000, 6000, 7000, 0, 11, 69923, 0, 0, 0, 0, 0, 26, 40, 0, 0, 0, 0, 0, 0, 'Skybreaker Luminary - In Combat - Cast ''Chain Heal'''),
(37016, 0, 3, 0, 0, 0, 100, 0, 7000, 8000, 7000, 8000, 0, 11, 69970, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Skybreaker Luminary - In Combat - Cast ''Lightning Bolt'''),
(37021, 0, 0, 0, 0, 0, 100, 0, 7000, 7000, 5000, 10000, 0, 11, 69963, 0, 0, 0, 0, 0, 26, 40, 0, 0, 0, 0, 0, 0, 'Skybreaker Vicar - In Combat - Cast ''Greater Heal'''),
(37021, 0, 1, 0, 0, 0, 100, 0, 0, 0, 7000, 8000, 0, 11, 69967, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Skybreaker Vicar - In Combat - Cast ''Smite'''),
(37021, 0, 2, 0, 0, 0, 100, 0, 25000, 30000, 25000, 30000, 0, 11, 69910, 0, 0, 0, 0, 0, 26, 40, 0, 0, 0, 0, 0, 0, 'Skybreaker Vicar - In Combat - Cast ''Pain Suppression'''),
(37027, 0, 0, 0, 0, 0, 100, 0, 0, 0, 3000, 5000, 0, 11, 69898, 0, 0, 0, 0, 0, 26, 40, 0, 0, 0, 0, 0, 0, 'Skybreaker Hierophant - In Combat - Cast ''Rejuvenation'''),
(37027, 0, 1, 0, 0, 0, 100, 0, 7000, 7000, 18000, 24000, 0, 11, 69882, 0, 0, 0, 0, 0, 26, 40, 0, 0, 0, 0, 0, 0, 'Skybreaker Hierophant - In Combat - Cast ''Regrowth'''),
(37027, 0, 2, 0, 0, 0, 100, 0, 25000, 25000, 10000, 15000, 0, 11, 69899, 0, 0, 0, 0, 0, 26, 40, 0, 0, 0, 0, 0, 0, 'Skybreaker Hierophant - In Combat - Cast ''Healing Touch'''),
(37027, 0, 3, 0, 0, 0, 100, 0, 0, 500, 3000, 5000, 0, 11, 69968, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Skybreaker Hierophant - In Combat - Cast ''Wrath'''),
(37030, 0, 0, 0, 0, 0, 100, 0, 0, 0, 3000, 5000, 0, 11, 69898, 0, 0, 0, 0, 0, 26, 40, 0, 0, 0, 0, 0, 0, 'Kor''kron Primalist - In Combat - Cast ''Rejuvenation'''),
(37030, 0, 1, 0, 0, 0, 100, 0, 7000, 7000, 18000, 24000, 0, 11, 69882, 0, 0, 0, 0, 0, 26, 40, 0, 0, 0, 0, 0, 0, 'Kor''kron Primalist - In Combat - Cast ''Regrowth'''),
(37030, 0, 2, 0, 0, 0, 100, 0, 25000, 25000, 10000, 15000, 0, 11, 69899, 0, 0, 0, 0, 0, 26, 40, 0, 0, 0, 0, 0, 0, 'Kor''kron Primalist - In Combat - Cast ''Healing Touch'''),
(37030, 0, 3, 0, 0, 0, 100, 0, 0, 500, 3000, 5000, 0, 11, 69968, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Kor''kron Primalist - In Combat - Cast ''Wrath'''),
(37031, 0, 0, 0, 0, 0, 100, 0, 1000, 1500, 20000, 25000, 0, 11, 69926, 0, 0, 0, 0, 0, 26, 40, 0, 0, 0, 0, 0, 0, 'Kor''kron Oracle - In Combat - Cast ''Earth Shield'''),
(37031, 0, 1, 0, 0, 0, 100, 0, 3000, 3000, 6000, 7000, 0, 11, 69958, 0, 0, 0, 0, 0, 26, 40, 0, 0, 0, 0, 0, 0, 'Kor''kron Oracle - In Combat - Cast ''Healing Wave'''),
(37031, 0, 2, 0, 0, 0, 100, 0, 4000, 6000, 6000, 7000, 0, 11, 69923, 0, 0, 0, 0, 0, 26, 40, 0, 0, 0, 0, 0, 0, 'Kor''kron Oracle - In Combat - Cast ''Chain Heal'''),
(37031, 0, 3, 0, 0, 0, 100, 0, 7000, 8000, 7000, 8000, 0, 11, 69970, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Kor''kron Oracle - In Combat - Cast ''Lightning Bolt'''),
(37144, 0, 0, 0, 0, 0, 100, 0, 3000, 3000, 10000, 14000, 0, 11, 69989, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Skybreaker Marksman - In Combat - Cast ''Arcane Shot'''),
(37144, 0, 1, 0, 0, 0, 100, 0, 6000, 6000, 7000, 14000, 0, 11, 69975, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Skybreaker Marksman - In Combat - Cast ''Explosive Shot'''),
(37144, 0, 2, 0, 0, 0, 100, 0, 0, 0, 3000, 4000, 0, 11, 69974, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Skybreaker Marksman - In Combat - Cast ''Shoot''');
