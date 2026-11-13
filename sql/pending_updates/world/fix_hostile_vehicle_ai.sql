-- Hostile vehicle creatures without an AI name get the passive vehicle AI: they never cast, never attack and never
-- evade. Give them SmartAI with their combat spells.

UPDATE `creature_template` SET `AIName` = 'SmartAI'
WHERE `entry` IN (28009, 31139, 31262, 34353) AND `AIName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (28009, 31139, 31262, 34353);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(28009, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 11, 55193, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shardhorn Rhino - On Aggro - Cast Rhino Charge'),
(28009, 0, 1, 0, 0, 0, 100, 0, 5000, 9000, 7000, 12000, 0, 11, 32019, 32, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 'Shardhorn Rhino - In Combat - Cast Gore'),
(31139, 0, 0, 0, 0, 0, 100, 0, 3000, 5000, 3000, 7000, 0, 11, 28405, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Pustulent Horror - In Combat - Cast Knock Back'),
(31139, 0, 1, 0, 0, 0, 100, 0, 4000, 6000, 5000, 9000, 0, 11, 63546, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Pustulent Horror - In Combat - Cast Stomp'),
(31262, 0, 0, 0, 0, 0, 100, 0, 0, 1000, 1000, 2000, 0, 11, 50092, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Blight Falconer - In Combat - Cast Shoot'),
(34353, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Krom''gar Incinerator - On Respawn - Set React State Defensive'),
(34353, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Krom''gar Incinerator - On Aggro - Attack Start');
