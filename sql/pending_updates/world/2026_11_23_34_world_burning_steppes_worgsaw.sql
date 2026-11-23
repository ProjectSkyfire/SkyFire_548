-- A Heap of Delicious Worg (28313, 28445): the Worgsaw (item 63427, spell 89776) cuts the best pieces of meat from a
-- dead Giant Ember Worg and makes the Worg Cutlets (item 63424) itself, so the cutlets have no loot rows. The spell had no
-- target limit, so it could be used on any creature, and the carcass stayed after being cut.
-- The two target conditions (a Giant Ember Worg, and dead) and the removal of the carcass are as in Legends of Azeroth
-- 5.4.8. The source has a third condition, "not in phase 1", of a condition type this core does not have, and sets that
-- phase in the same script; the carcass is removed one second after the cut instead.

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` = 89776;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 89776, 0, 0, 31, 1, 3, 9697, 0, 0, 0, 0, '', 'Worgsaw target limit to Giant Ember Worg'),
(17, 0, 89776, 0, 0, 36, 1, 0, 0, 0, 1, 0, 0, '', 'Worgsaw can be casted only if target is dead');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 9697 AND `id` = 1;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(9697, 0, 1, 0, 8, 0, 100, 0, 89776, 0, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Giant Ember Worg - On SpellHit Worgsaw - Despawn');
