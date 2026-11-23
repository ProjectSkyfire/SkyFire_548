-- Redridge Mountains: the Redridge Mystic (430), the Murloc Tidecaller (545), the Murloc Minor Tidecaller (548), the
-- Shadowhide Warrior (568) and the Shadowhide Assassin (579) use SmartAI but had no rows, so they only fought in melee.
-- Rows are as in Legends of Azeroth 5.4.8: the Mystic's Lightning Bolt, the Tidecallers' Frost Nova and Frostbolt, the
-- Warrior's Defensive Stance, the Assassin's Stealth and Poison, and the flee at low health. The Mystic and the Warrior
-- say their lines (creature_text of both exists) when they aggro.

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (430, 545, 548, 568, 579);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(430, 0, 0, 0, 4, 0, 10, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Redridge Mystic - On Aggro - Say Line 0'),
(430, 0, 1, 0, 0, 0, 100, 0, 0, 0, 600, 1800, 0, 11, 20802, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Redridge Mystic - In Combat CMC - Cast ''Lightning Bolt'''),
(430, 0, 2, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Redridge Mystic - Between 0-15% Health - Flee For Assist (No Repeat)'),
(545, 0, 0, 0, 0, 0, 100, 0, 6200, 12600, 25000, 30000, 0, 11, 122, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Murloc Tidecaller - In Combat - Cast ''Frost Nova'''),
(545, 0, 2, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Murloc Tidecaller - Between 0-15% Health - Flee For Assist'),
(548, 0, 0, 0, 0, 0, 100, 0, 0, 0, 3500, 6400, 0, 11, 9672, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Murloc Minor Tidecaller - In Combat CMC - Cast ''Frostbolt'''),
(548, 0, 2, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Murloc Minor Tidecaller - Between 0-15% Health - Flee For Assist (No Repeat)'),
(568, 0, 0, 1, 4, 0, 10, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadowhide Warrior - On Aggro - Say Line 0 (No Repeat)'),
(568, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 7164, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadowhide Warrior - On Aggro - Cast ''Defensive Stance'' (No Repeat)'),
(568, 0, 3, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadowhide Warrior - Between 0-15% Health - Flee For Assist (No Repeat)'),
(579, 0, 0, 0, 1, 0, 100, 1, 1000, 1000, 0, 0, 0, 11, 30831, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadowhide Assassin - Out of Combat - Cast ''Stealth'' (No Repeat)'),
(579, 0, 2, 0, 0, 0, 100, 0, 16900, 29700, 31300, 34500, 0, 11, 744, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Shadowhide Assassin - In Combat - Cast ''Poison'' (No Repeat)'),
(579, 0, 3, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Shadowhide Assassin - Between 0-15% Health - Flee For Assist (No Repeat)');
