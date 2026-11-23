-- The dungeon journal and the current spell list give Mekgineer Thermaplugg Steam Blast, Pound and Welding Beam next
-- to the Walking Bombs, and no Knock Away. His SmartAI only had Knock Away (rows 2 and 3). The new rows take no
-- difficulty flag: Gnomeregan has one difficulty.
DELETE FROM `smart_scripts` WHERE `entryorguid` = 7800 AND `source_type` = 0 AND `id` IN (2, 3, 8, 9, 10);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(7800, 0, 8, 0, 0, 0, 100, 0, 12000, 16000, 20000, 25000, 0, 11, 50375, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mekgineer Thermaplugg - In Combat - Cast Steam Blast'),
(7800, 0, 9, 0, 0, 0, 100, 0, 8000, 12000, 15000, 20000, 0, 11, 74720, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mekgineer Thermaplugg - In Combat - Cast Pound'),
(7800, 0, 10, 0, 0, 0, 100, 0, 20000, 25000, 30000, 35000, 0, 11, 52778, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mekgineer Thermaplugg - In Combat - Cast Welding Beam');
