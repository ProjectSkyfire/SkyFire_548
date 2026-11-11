-- Hostile vehicle creatures without an AI name get the passive vehicle AI: they never cast, never attack and never
-- evade. Give them SmartAI with their combat spells.

UPDATE `creature_template` SET `AIName` = 'SmartAI'
WHERE `entry` IN (28009, 28018, 28246, 29698, 29753, 31137, 31139, 31262, 31702, 34353) AND `AIName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (28009, 28018, 28246, 29698, 29753, 31137, 31139, 31262, 31702, 34353);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(28009, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 11, 55193, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shardhorn Rhino - On Aggro - Cast Rhino Charge'),
(28009, 0, 1, 0, 0, 0, 100, 0, 5000, 9000, 7000, 12000, 0, 11, 32019, 32, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 'Shardhorn Rhino - In Combat - Cast Gore'),
(28018, 0, 0, 0, 0, 0, 100, 0, 5000, 5000, 12000, 14000, 0, 11, 50456, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Thiassi the Lightning Bringer - In Combat - Cast Thiassi''s Stormbolt'),
(28018, 0, 1, 0, 0, 0, 100, 0, 9000, 9000, 15000, 19000, 0, 11, 15593, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thiassi the Lightning Bringer - In Combat - Cast War Stomp'),
(28246, 0, 0, 0, 0, 0, 100, 0, 5000, 10000, 10000, 15000, 0, 11, 31273, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Sky Terror - In Combat - Cast Screech'),
(29698, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 11, 54487, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Drakuru Raptor - On Aggro - Cast Jump Attack'),
(29753, 0, 0, 0, 0, 0, 100, 0, 3000, 6000, 9000, 12000, 0, 11, 57833, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Stormpeak Wyrm - In Combat - Cast Frost Spit'),
(31137, 0, 0, 0, 0, 0, 100, 0, 0, 5000, 5000, 15000, 0, 11, 60667, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Frostbrood Skytalon - In Combat - Cast Frost Breath'),
(31139, 0, 0, 0, 0, 0, 100, 0, 3000, 5000, 3000, 7000, 0, 11, 28405, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Pustulent Horror - In Combat - Cast Knock Back'),
(31139, 0, 1, 0, 0, 0, 100, 0, 4000, 6000, 5000, 9000, 0, 11, 63546, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Pustulent Horror - In Combat - Cast Stomp'),
(31262, 0, 0, 0, 0, 0, 100, 0, 0, 1000, 1000, 2000, 0, 11, 50092, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Blight Falconer - In Combat - Cast Shoot'),
(31702, 0, 0, 0, 0, 0, 100, 0, 0, 5000, 5000, 15000, 0, 11, 60667, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Frostbrood Spawn - In Combat - Cast Frost Breath'),
(34353, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Krom''gar Incinerator - On Respawn - Set React State Defensive'),
(34353, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Krom''gar Incinerator - On Aggro - Attack Start');
