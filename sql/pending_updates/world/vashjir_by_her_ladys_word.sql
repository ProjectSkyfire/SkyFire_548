-- Vashj'ir, "By Her Lady's Word" (25858): the credits are "Speak to" Fathom-Lord Zin'jatar, Lady Sira'kess and Overseer
-- Idra'kess. The quest credit spells 77633, 77635 and 77634 credit the targeted player, and nothing cast them. Overseer
-- Idra'kess has the gossip option of menu 11516; the other two have no gossip menu, so they give the credit when the
-- player talks to them. Stolen Property, Setting An Example and The Culmination of Our Efforts follow the quest.

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (41049, 41455, 41456);
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (41049, 41455, 41456);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41049, 0, 0, 0, 64, 0, 100, 0, 0, 0, 0, 0, 0, 11, 77633, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Gossip Hello - Cast Quest Credit Zin''jatar on Invoker'),
(41455, 0, 0, 1, 62, 0, 100, 0, 11516, 0, 0, 0, 0, 11, 77634, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Gossip Option 0 Selected - Cast Quest Credit Idra''kess on Invoker'),
(41455, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Link - Close Gossip'),
(41456, 0, 0, 0, 64, 0, 100, 0, 0, 0, 0, 0, 0, 11, 77635, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Gossip Hello - Cast Quest Credit Sira''kess on Invoker');

UPDATE `quest_template` SET `PrevQuestId` = 25858 WHERE `Id` = 25862;
UPDATE `quest_template` SET `PrevQuestId` = 25859 WHERE `Id` IN (25861, 26191);
