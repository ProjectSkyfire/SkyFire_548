-- The Kodocaller's Horn (28252, 28435): the Horn of the Callers (item 63356, spell 89477) is blown near the Blackrock War
-- Kodos and each one that hears it should give the credit (48112) and walk back to camp. The kodos had no reaction and
-- nothing limited the spell to them, so the horn did nothing and the quest could not be finished.
-- The spell hit row, the script (4811101) and the spell target condition are as in Legends of Azeroth 5.4.8. The source
-- also has rows that run the same ending when a kodo falls below half health; they use an action this core does not have
-- and are left out. The source calls the script through a random list of two scripts of which only one exists, so it is
-- called directly. The existing Kodo Stomp row (id 0) is kept.

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 48111 AND `id` IN (1, 2);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(48111, 0, 1, 2, 8, 0, 100, 0, 89477, 0, 120000, 120000, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Blackrock War Kodo - On SpellHit Horn of the Callers - Store Target'),
(48111, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4811101, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackrock War Kodo - Linked To Id 1 - Run Script');

DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` = 4811101;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4811101, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 48112, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Blackrock War Kodo - On Script - Quest Credit'),
(4811101, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, -8131.91, -1895.78, 133.214, 0, 'Blackrock War Kodo - On Script - Move To Pos'),
(4811101, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackrock War Kodo - On Script - Despawn after 5 sec');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 89477;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 89477, 0, 0, 31, 0, 3, 48111, 0, 0, 0, 0, '', 'Horn of the Callers target limit to Blackrock War Kodo');
