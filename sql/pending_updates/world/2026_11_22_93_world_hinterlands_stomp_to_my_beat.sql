-- Stomp To My Beat (26240) cannot be finished. The quest item, the Revantusk War Drums (57920, spell 79402), is used on
-- Revantusk Stalkers and Hexxers, and each use should give the credit (42597) and make the troll enraged. Neither
-- creature reacted to the spell, and the spell could be used on any target. The credit and enrage rows are added to the
-- existing combat rows, and the drums work on a troll only once at a time (Recently Enraged, 95966).
-- Rows and conditions are as in Legends of Azeroth 5.4.8.

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND ((`entryorguid` = 42536 AND `id` IN (3, 4, 5)) OR (`entryorguid` = 42555 AND `id` IN (4, 5, 6)));
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(42536, 0, 3, 4, 8, 0, 100, 0, 79402, 0, 0, 0, 0, 33, 42597, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Revantusk Stalker - On SpellHit Revantusk Drums - Quest Credit'),
(42536, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 95966, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Revantusk Stalker - Linked To Id 3 - Cast Recently Enraged'),
(42536, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 56646, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Revantusk Stalker - Linked To Id 4 - Cast Enrage'),
(42555, 0, 4, 5, 8, 0, 100, 0, 79402, 0, 0, 0, 0, 33, 42597, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Revantusk Hexxer - On SpellHit Revantusk Drums - Quest Credit'),
(42555, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 95966, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Revantusk Hexxer - Linked To Id 4 - Cast Recently Enraged'),
(42555, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 56646, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Revantusk Hexxer - Linked To Id 5 - Cast Enrage');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` = 79402;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 79402, 0, 0, 1, 1, 95966, 0, 0, 1, 0, 0, '', 'Revantusk War Drums - Target without Recently Enraged'),
(17, 0, 79402, 0, 0, 31, 1, 3, 42555, 0, 0, 0, 0, '', 'Revantusk War Drums - Target is a Revantusk Hexxer'),
(17, 0, 79402, 0, 1, 1, 1, 95966, 0, 0, 1, 0, 0, '', 'Revantusk War Drums - Target without Recently Enraged'),
(17, 0, 79402, 0, 1, 31, 1, 3, 42536, 0, 0, 0, 0, '', 'Revantusk War Drums - Target is a Revantusk Stalker');
