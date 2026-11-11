-- Twilight Stormwaker (40573) is a vehicle creature without an AI name, so it got the passive vehicle AI and never cast.
-- Give it SmartAI with its combat spell.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 40573 AND `AIName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 40573;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(40573, 0, 0, 0, 0, 0, 100, 0, 0, 0, 3400, 4700, 0, 11, 65060, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Twilight Stormwaker - In Combat - Cast Twilight Bolt');
