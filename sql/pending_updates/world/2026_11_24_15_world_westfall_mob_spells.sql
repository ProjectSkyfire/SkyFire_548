-- Westfall: the Murloc Oracle (517), the Riverpaw Shaman (1065) and the Master Digger (1424) have SmartAI but no rows, so the
-- two casters fought in melee only and none of them spoke or fled at low health. The rows are as in Legends of Azeroth 5.4.8: the
-- Oracle casts Holy Smite, the shaman Lightning Bolt, the digger Heroic Strike, the shaman speaks, and all three flee for
-- assistance at low health. The speech of the shaman exists already.

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (517, 1065, 1424);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(517, 0, 0, 0, 0, 0, 100, 0, 2900, 3600, 2900, 3600, 0, 11, 9734, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Murloc Oracle - In Combat - Cast ''Holy Smite'' (No Repeat)'),
(517, 0, 2, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Murloc Oracle - Between 0-15% Health - Flee For Assist (No Repeat)'),
(1065, 0, 0, 0, 4, 0, 15, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Riverpaw Shaman - On Aggro - Say Line 0'),
(1065, 0, 1, 0, 0, 0, 100, 0, 0, 0, 3500, 4800, 0, 11, 9532, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Riverpaw Shaman - In Combat CMC - Cast ''Lightning Bolt'''),
(1065, 0, 3, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Riverpaw Shaman - Between 0-15% Health - Flee For Assist (No Repeat)'),
(1424, 0, 1, 0, 0, 0, 100, 0, 9800, 12200, 12200, 15800, 0, 11, 25710, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Master Digger - In Combat - Cast ''Heroic Strike'' (No Repeat)'),
(1424, 0, 2, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Master Digger - Between 0-15% Health - Flee For Assist (No Repeat)');
