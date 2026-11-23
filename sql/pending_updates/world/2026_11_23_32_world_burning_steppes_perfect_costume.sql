-- A Perfect Costume (28202 to 28205, 28428 to 28431): the disguise is handed out by Thelaron Direneedle, who gives the
-- credit (47910) and the Blackrock Disguise (63357) when the player chooses "I brought you some hides, mud, and thread."
-- and speaks about his masterpiece. Thelaron had no SmartAI and no speech, so the option did nothing and the quests
-- could not be finished.
-- The rows and the speech are as in Legends of Azeroth 5.4.8. The menu has the option twice in this database (ids 0
-- and 2; the source has only the first), so both get the same rows. The source also
-- removes Thelaron's gossip flag after the first use and gives it back through a data event that nothing in the source
-- sets, which would shut the option for every player until a restart; those rows are left out.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 48001 AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 48001;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(48001, 0, 0, 1, 62, 0, 100, 0, 12376, 0, 0, 0, 0, 33, 47910, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Thelaron Direneedle - On Gossip Select - Quest Credit Costume'),
(48001, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 56, 63357, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Thelaron Direneedle - On Gossip Select - Add Item Blackrock Disguise'),
(48001, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Thelaron Direneedle - On Gossip Select - Say Line 0'),
(48001, 0, 3, 4, 62, 0, 100, 0, 12376, 2, 0, 0, 0, 33, 47910, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Thelaron Direneedle - On Gossip Select - Quest Credit Costume'),
(48001, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 56, 63357, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Thelaron Direneedle - On Gossip Select - Add Item Blackrock Disguise'),
(48001, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Thelaron Direneedle - On Gossip Select - Say Line 0');

DELETE FROM `creature_text` WHERE `entry` IN (48001);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(48001, 0, 0, 'This... this is perfect!  This disguise shall be my masterpiece!', 12, 0, 100, 0, 0, 0, '');
