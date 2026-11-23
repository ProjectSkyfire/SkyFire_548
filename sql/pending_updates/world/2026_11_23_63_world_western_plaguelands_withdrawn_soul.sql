-- Those That Couldn't Let Go (27168): the Holy Thurible (spell 84375) could be used on any target and the Withdrawn
-- Souls did not react to it, so the 12 released souls were never counted. The spell is limited to Withdrawn Souls,
-- and a soul hit by it gives the credit and fades away. The existing Spirit Burst row is kept.
-- Rows and condition are as in Legends of Azeroth 5.4.8; the credit is given before the despawn.

DELETE FROM `smart_scripts` WHERE `entryorguid` = 45166 AND `source_type` = 0 AND `id` IN (1, 2);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(45166, 0, 1, 2, 8, 0, 100, 1, 84375, 0, 0, 0, 0, 33, 45166, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Withdrawn Soul - On Spellhit Holy Thurible - Quest Credit'),
(45166, 0, 2, 0, 61, 0, 100, 1, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Withdrawn Soul - Linked To Id 1 - Despawn');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` = 84375;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 84375, 0, 0, 31, 1, 3, 45166, 0, 0, 0, 0, '', 'Holy Thurible can hit Withdrawn Soul');
