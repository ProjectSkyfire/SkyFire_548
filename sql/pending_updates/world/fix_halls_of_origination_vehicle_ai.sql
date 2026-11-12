-- Spatial Anomaly (40170) and Living Vine (40668) are vehicle creatures without an AI name, so they got the passive
-- vehicle AI and never cast. Give them SmartAI with their combat spells.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (40170, 40668) AND `AIName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (40170, 40668);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(40170, 0, 0, 0, 9, 0, 100, 0, 0, 10, 14000, 14500, 0, 11, 74888, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Spatial Anomaly - Within 0-10 Range - Cast Arcane Burst'),
(40668, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Living Vine - On Aggro - Stop Moving'),
(40668, 0, 1, 0, 0, 0, 100, 0, 2000, 4500, 10000, 13000, 0, 11, 75153, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 'Living Vine - In Combat - Cast Spore Blast');
