-- Swift Discipline (9066): the Disciplinary Rod (27907) is used on Apprentice Ralen (15941) and Apprentice Meledor
-- (15945), who are to give the credit, say their line and be left alone for 15 seconds. Ralen had no script and Meledor
-- only a fight script, so neither gave credit and the quest could not be finished; their lines were also missing.
-- The rows and lines are those of Legends of Azeroth 5.4.8: a spell hit starts a timed action list that gives the
-- credit and 3 seconds later the text. Meledor's fight script stays.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 15941 AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE (`entryorguid` = 15941 AND `source_type` = 0) OR (`entryorguid` = 15945 AND `source_type` = 0 AND `id` = 14) OR (`entryorguid` IN (1594100, 1594500) AND `source_type` = 9);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(15941, 0, 0, 0, 8, 0, 100, 0, 27907, 0, 15000, 15000, 0, 80, 1594100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Timed action list on spell hit'),
(15945, 0, 14, 0, 8, 0, 100, 0, 27907, 0, 15000, 15000, 0, 80, 1594500, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Timed action list on spell hit'),
(1594100, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 15941, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kill credit for quest 9066'),
(1594100, 9, 1, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Text on spell hit'),
(1594500, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 15945, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kill credit for quest 9066'),
(1594500, 9, 1, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Text on spell hit');

DELETE FROM `creature_text` WHERE `entry` IN (15941, 15945);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(15941, 0, 0, 'What?  Oh, not this again!', 12, 0, 100, 0, 0, 0, 'Apprentice Ralen - line 0'),
(15945, 0, 0, 'You can''t do this to me!  We had a deal!', 12, 0, 100, 0, 0, 0, 'Apprentice Meledor - line 0');
