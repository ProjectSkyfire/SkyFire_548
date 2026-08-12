-- Grik'nir the Cold
DELETE FROM `creature_text` WHERE `entry`=808;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(808, 0, 0, 'First da fire, and now da dwarf! You gonna die!', 12, 0, 100, 0, 0, 0, 'Grik\'nir the Cold - SAY_AGGRO');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=808;

DELETE FROM `smart_scripts` WHERE `entryorguid`=808;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(808, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grik\'nir the Cold - On Aggro - Say 0'),
(808, 0, 1, 0, 0, 0, 100, 1, 4000, 5000, 9000, 10000, 0, 11, 6957, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grik\'nir the Cold - In Combat - Cast Frostmane Strength'),
(808, 0, 2, 0, 0, 0, 100, 0, 2000, 4000, 7000, 9000, 0, 11, 79895, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Grik\'nir the Cold - In Combat - Cast Frost Strike');
