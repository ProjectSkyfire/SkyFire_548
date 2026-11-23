-- Who Needs Cauldrons? (26978): the objectives are the four Who Needs Cauldrons credit bunnies (44622 to 44625), one
-- beside each place that gets a Dithers' Plague Drum (object 205069). The bunnies had no script, so placing a drum
-- never gave the credit. As in the source, each bunny gives the credit to players within 10 yards when a drum is
-- within 8 yards of it. This core has no "gameobject in range" smart event, so the check is an out of combat update
-- every second with the same distance test as a smart event condition.
-- Rows are as in Legends of Azeroth 5.4.8 with the event changed.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (44622, 44623, 44624, 44625) AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (44622, 44623, 44624, 44625) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(44622, 0, 0, 0, 1, 0, 100, 0, 1000, 1000, 1000, 1000, 0, 33, 44622, 0, 0, 0, 0, 0, 18, 10, 0, 0, 0, 0, 0, 0, 'Who Needs Cauldrons: Windmill Credit - Out of Combat - Quest Credit'),
(44623, 0, 0, 0, 1, 0, 100, 0, 1000, 1000, 1000, 1000, 0, 33, 44623, 0, 0, 0, 0, 0, 18, 10, 0, 0, 0, 0, 0, 0, 'Who Needs Cauldrons: Silo Credit - Out of Combat - Quest Credit'),
(44624, 0, 0, 0, 1, 0, 100, 0, 1000, 1000, 1000, 1000, 0, 33, 44624, 0, 0, 0, 0, 0, 18, 10, 0, 0, 0, 0, 0, 0, 'Who Needs Cauldrons: Ranch House Credit - Out of Combat - Quest Credit'),
(44625, 0, 0, 0, 1, 0, 100, 0, 1000, 1000, 1000, 1000, 0, 33, 44625, 0, 0, 0, 0, 0, 18, 10, 0, 0, 0, 0, 0, 0, 'Who Needs Cauldrons: Barn Credit - Out of Combat - Quest Credit');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 1 AND `SourceEntry` IN (44622, 44623, 44624, 44625) AND `SourceId` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 1, 44622, 0, 0, 30, 1, 205069, 8, 0, 0, 0, 0, '', 'Credit bunny gives credit only if a Dithers'' Plague Drum is within 8 yards'),
(22, 1, 44623, 0, 0, 30, 1, 205069, 8, 0, 0, 0, 0, '', 'Credit bunny gives credit only if a Dithers'' Plague Drum is within 8 yards'),
(22, 1, 44624, 0, 0, 30, 1, 205069, 8, 0, 0, 0, 0, '', 'Credit bunny gives credit only if a Dithers'' Plague Drum is within 8 yards'),
(22, 1, 44625, 0, 0, 30, 1, 205069, 8, 0, 0, 0, 0, '', 'Credit bunny gives credit only if a Dithers'' Plague Drum is within 8 yards');
