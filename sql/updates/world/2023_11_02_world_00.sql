-- Riverblade Slayer SAI
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=58273;
DELETE FROM `smart_scripts` WHERE `entryorguid`=58273;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(58273, 0, 1, 0, 0, 0, 100, 0, 1000, 2000, 9000, 10000, 11, 119561, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Riverblade Slayer - In Combat - Cast Bloodletting'),
(58273, 0, 2, 0, 0, 0, 100, 0, 2000, 3000, 10000, 11000, 11, 119569, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Riverblade Slayer - In Combat - Cast Savage Strikes');
