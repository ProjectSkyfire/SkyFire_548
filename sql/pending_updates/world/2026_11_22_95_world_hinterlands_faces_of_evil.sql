-- Faces of Evil (26357 and 26521) cannot be finished. The quest item, the Tiki Torch (58209, spell 79513), is used near a
-- Jintha'Alor tiki mask, where the Tiki Mask Fire Bunny (42704) gives the credit and disappears. The bunny had no
-- SmartAI, so using the torch did nothing, and the spell had no limit to its target and place. The spell works only near
-- a bunny (10 yards) and only on a bunny. Script and conditions are as in Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 42704 AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `entryorguid` = 42704 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(42704, 0, 0, 1, 8, 0, 100, 0, 79513, 0, 0, 0, 0, 33, 42704, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Jintha''Alor Tiki Mask Fire Bunny - On SpellHit Tiki Torch - Quest Credit'),
(42704, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Jintha''Alor Tiki Mask Fire Bunny - Linked To Id 0 - Despawn');

DELETE FROM `conditions` WHERE `SourceEntry` = 79513 AND `SourceTypeOrReferenceId` IN (13, 17);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 79513, 0, 0, 31, 0, 3, 42704, 0, 0, 0, 0, '', 'Tiki Torch - Target is a Jintha''Alor Tiki Mask Fire Bunny'),
(17, 0, 79513, 0, 0, 29, 0, 42704, 10, 0, 0, 0, 0, '', 'Tiki Torch - Only near a Jintha''Alor Tiki Mask Fire Bunny');
