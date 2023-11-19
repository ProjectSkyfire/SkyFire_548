SET @ENTRY := -80370;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -80367;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -80365;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -80364;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -80363;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');
