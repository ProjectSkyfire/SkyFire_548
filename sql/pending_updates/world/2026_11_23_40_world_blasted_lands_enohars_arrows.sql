-- Enohar's Explosive Arrows (item 57119, spell 78838): the arrow is fired at Darktail Bonepickers for the 20 kills of
-- "Curtail the Darktail" (25709): in three of four hits the Bonepicker dies from it and the shooter gets the kill
-- credit (42235), as it does not die from damage. The Bonepickers had neither the spell hit rows nor did anything limit
-- the arrow to them, so it could be fired at anything and counted for nothing.
-- Rows and spell target condition are as in Legends of Azeroth 5.4.8. The existing rows (ids 0 and 1) are kept.

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` = 78838;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 78838, 0, 0, 31, 1, 3, 42235, 0, 0, 0, 0, '', 'Enohar''s Explosive Arrow - Target is a Darktail Bonepicker');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 42235 AND `id` IN (2, 3);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(42235, 0, 2, 3, 8, 0, 75, 1, 78838, 0, 0, 0, 0, 37, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darktail Bonepicker - On SpellHit Enohar''s Explosive Arrow - Die'),
(42235, 0, 3, 0, 61, 0, 100, 1, 0, 0, 0, 0, 0, 33, 42235, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Darktail Bonepicker - Linked To Id 2 - Quest Credit');
