-- Azshara: the Runaway Shredders, Valormok Grunts and Talrendis Defenders had no AI, so they only stood or meleed.
-- SmartAI and texts from The Cataclysm Preservation Project 4.3.4.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (35111, 36815, 36816) AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (35111, 36815, 36816);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(35111, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 0, 11, 32735, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Runaway Shredder - On Aggro - Cast Saw Blade (No Repeat)'),
(35111, 0, 1, 0, 6, 0, 100, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Runaway Shredder - On Just Died - Say Line 0 (No Repeat)'),
(36815, 0, 0, 0, 0, 0, 100, 0, 0, 0, 2300, 3900, 0, 11, 95826, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Valormok Grunt - In Combat - Cast Shoot'),
(36815, 0, 1, 0, 9, 0, 100, 0, 0, 5, 9000, 11000, 0, 11, 40505, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Valormok Grunt - Within 0-5 Range - Cast Cleave'),
(36815, 0, 2, 3, 2, 0, 100, 1, 0, 30, 0, 0, 0, 11, 8599, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Valormok Grunt - Between 0-30% Health - Cast Enrage (No Repeat)'),
(36815, 0, 3, 0, 61, 0, 100, 1, 0, 30, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Valormok Grunt - Between 0-30% Health - Say Line 0 (No Repeat)'),
(36816, 0, 0, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Talrendis Defender - Between 0-15% Health - Flee For Assist (No Repeat)'),
(36816, 0, 1, 0, 4, 0, 100, 1, 0, 0, 0, 0, 0, 11, 22120, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Talrendis Defender - On Aggro - Cast Charge (No Repeat)'),
(36816, 0, 2, 0, 13, 0, 100, 0, 2000, 4500, 20000, 30000, 0, 11, 11972, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Talrendis Defender - On Victim Casting - Cast Shield Bash');

DELETE FROM `creature_text` WHERE `entry` IN (35111, 36815);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(35111, 0, 0, 'Syntax Error Line 14,Sequence:Flee for Life. Entering Passive Mode.*Click!*', 12, 0, 100, 0, 0, 0, 'Runaway Shredder - on death'),
(36815, 0, 0, '%s becomes enraged!', 16, 0, 100, 0, 0, 0, 'Valormok Grunt - enrage');
