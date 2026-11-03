-- Black Temple trash a player could not select. The Bonechewer Shield Disciples and Blood Prophets and the Mutant War
-- Hounds carry the not selectable flag here, TrinityCore 3.3.5 has none on them. The Dragonmaw Sky Stalkers keep the
-- flag as in TrinityCore, which removes it when they enter combat and dismounts them on death; those two rows were
-- missing.
UPDATE `creature_template` SET `unit_flags` = 32832 WHERE `entry` IN (23236, 23237);
UPDATE `creature_template` SET `unit_flags` = 32768 WHERE `entry` = 23232;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 23030;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(23030, 0, 0, 0, 6, 0, 100, 32, 0, 0, 0, 0, 0, 43, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dragonmaw Sky Stalker - On Just Died - Dismount'),
(23030, 0, 1, 0, 0, 0, 100, 33, 0, 0, 0, 0, 0, 19, 33554432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dragonmaw Sky Stalker - In Combat - Remove Flag Not Selectable (No Repeat)'),
(23030, 0, 2, 0, 9, 0, 100, 32, 5, 41, 15800, 15800, 0, 11, 40872, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Dragonmaw Sky Stalker - At 5 - 41 Range - Cast Immolation Arrow'),
(23030, 0, 3, 0, 9, 0, 100, 32, 5, 41, 9800, 9800, 0, 11, 40873, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Dragonmaw Sky Stalker - At 5 - 41 Range - Cast Shoot');
