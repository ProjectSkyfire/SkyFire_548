-- Elwynn Forest: the Northshire Guard (1642) did not cast Invisibility and Stealth Detection when it respawned, and Kira Songshine
-- (3937) had no AI or speech, so she neither spoke nor called for help. AIName, rows and speech are as in Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 3937 AND `ScriptName` = '';

DELETE FROM `creature_text` WHERE `entry` = 3937;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(3937, 0, 0, 'Fresh bread for sale!', 12, 7, 100, 0, 0, 0, 'Kira Songshine'),
(3937, 0, 1, 'Fresh bread, baked this very morning.', 12, 7, 100, 0, 0, 0, 'Kira Songshine'),
(3937, 0, 2, 'Come get yer fresh bread!', 12, 7, 100, 0, 0, 0, 'Kira Songshine'),
(3937, 1, 0, 'Guards! Help me!', 12, 0, 100, 0, 0, 0, 'Kira Songshine');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (1642, 3937);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1642, 0, 0, 0, 11, 0, 100, 1, 0, 0, 0, 0, 0, 11, 18950, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Northshire Guard - On Respawn - Cast ''Invisibility and Stealth Detection'''),
(3937, 0, 0, 0, 1, 0, 100, 0, 5000, 10000, 30000, 45000, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kira Songshine - Out of Combat - Say Text Line 0'),
(3937, 0, 1, 2, 4, 0, 100, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kira Songshine - On Aggro - Say Text Line 1'),
(3937, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 39, 60, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kira Songshine - On Aggro (Link) - Call For Help');
