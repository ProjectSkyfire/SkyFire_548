-- Crossroads Caravan Pickup: Halga Bloodeye's gossip gives the caravan credit.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (34258) AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (34258);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(34258, 0, 0, 1, 62, 0, 100, 0, 10514, 0, 0, 0, 0, 33, 53613, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Halga Bloodeye - On Gossip Select - Give Quest Credit'),
(34258, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 15, 13949, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Halga Bloodeye - On Gossip Select - Give Quest Credit'),
(34258, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Halga Bloodeye - Gossip Closed');
-- Grol'dom Net: a netted Razormane Pillager is snared for 8 seconds and stops fighting; it keeps its flee at low health.
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 34503;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(34503, 0, 0, 1, 8, 0, 100, 0, 65580, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Razormane Pillager - On SpellHit Grol''dom Net - Say Line 0'),
(34503, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 65581, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Razormane Pillager - Linked To Id 0 - Cast Snared in Net'),
(34503, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Razormane Pillager - Linked To Id 1 - Set Faction 35'),
(34503, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Razormane Pillager - Linked To Id 2 - Combat Stop'),
(34503, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 67, 34503, 8000, 8000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Razormane Pillager - Linked To Id 3 - Create Timed Event'),
(34503, 0, 5, 6, 59, 0, 100, 0, 34503, 0, 0, 0, 0, 28, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Razormane Pillager - On Timed Event Triggered - Remove All Auras'),
(34503, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 0, 2, 109, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Razormane Pillager - Linked To Id 5 - Set Faction 109'),
(34503, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 0, 5, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Razormane Pillager - Linked To Id 6 - Play Emote 35'),
(34503, 0, 8, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Razormane Pillager - Linked To Id 7 - Say Line 1'),
(34503, 0, 9, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Razormane Pillager - Between 0-15% Health - Flee For Assist (No Repeat)');
DELETE FROM `creature_text` WHERE `entry` = 34503;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(34503, 0, 0, '%s is ensnared!', 16, 0, 100, 0, 0, 0, 'Razormane Pillager - netted'),
(34503, 1, 0, '%s breaks out of the net.', 16, 0, 100, 0, 0, 0, 'Razormane Pillager - net broken');
