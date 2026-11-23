-- Axis of Awful (26868) cannot be finished. The Potent Murloc Pheromones (spell 82799) pick their target by entry, but
-- no condition names the Mosshide Representative (44262), and the representative has no AI to give the kill credit
-- (44266) when the pheromones hit him. Both rows are as in Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 44262;

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 1 AND `SourceEntry` = 82799;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 82799, 0, 0, 31, 0, 3, 44262, 0, 0, 0, 0, '', 'Potent Murloc Pheromones - Mosshide Representative');

DELETE FROM `smart_scripts` WHERE `entryorguid` = 44262 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(44262, 0, 0, 0, 8, 0, 100, 0, 82799, 0, 0, 0, 0, 33, 44266, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mosshide Representative - On Spell Hit Potent Murloc Pheromones - Quest Credit Mosshide Representative Credit');
