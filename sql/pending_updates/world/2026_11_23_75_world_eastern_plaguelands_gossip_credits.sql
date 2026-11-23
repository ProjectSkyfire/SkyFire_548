-- Traveling Companions (27381) and Heroes of Darrowshire (27388) cannot be finished. Choosing the caravan question
-- from Argus Highbeacon at the Crown Guard Tower never gave the companion credit, and Carlin Redpath never handed
-- over the Extended Annals of Darrowshire, because neither had a script. Both options are also shown only
-- while the quest is taken (and, for Carlin, while the player does not already carry the book).
-- Rows and conditions are as in Legends of Azeroth 5.4.8, adapted to the follow up menus the options already open
-- here (so Argus's close gossip row is left out). The gossip option numbers are the ones of the existing options.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (11063, 45451) AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (11063, 45451);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(11063, 0, 0, 0, 62, 0, 100, 0, 3864, 1, 0, 0, 0, 56, 13202, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Carlin Redpath - On Gossip Select - Add Extended Annals of Darrowshire'),
(45451, 0, 0, 0, 62, 0, 100, 0, 12034, 0, 0, 0, 0, 33, 45501, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Argus Highbeacon - On Gossip Select - Quest Credit');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND ((`SourceGroup` = 3864 AND `SourceEntry` = 1) OR (`SourceGroup` = 12034 AND `SourceEntry` = 0));
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 3864, 1, 0, 0, 2, 0, 13202, 1, 0, 1, 0, 0, '', 'Show option only if the player has no Extended Annals of Darrowshire'),
(15, 3864, 1, 0, 0, 9, 0, 27388, 0, 0, 0, 0, 0, '', 'Show option only if quest 27388 is taken'),
(15, 12034, 0, 0, 0, 9, 0, 27381, 0, 0, 0, 0, 0, '', 'Show option only if quest 27381 is taken');
