-- Loch Modan: Magosh (1399), the Stonesplinter Shaman (1197) and Kubb (1425) have SmartAI but no rows, so Magosh, a shaman,
-- fought in melee only and none of them spoke or fled at low health. The rows are as in Legends of Azeroth 5.4.8: Magosh speaks
-- and casts Lightning Bolt and Shock, the shaman speaks, and the shaman and Kubb flee for assistance at low health.
-- Their speech exists already.

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (1197, 1399, 1425);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1197, 0, 0, 0, 4, 0, 10, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Stonesplinter Shaman - On Aggro - Say Line 0'),
(1197, 0, 2, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Stonesplinter Shaman - Between 0-15% Health - Flee For Assist'),
(1399, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Magosh - On Aggro - Say Line 0'),
(1399, 0, 1, 0, 0, 0, 100, 0, 0, 40, 1200, 3400, 0, 11, 9532, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Magosh - In Combat CMC - Cast ''Lightning Bolt'''),
(1399, 0, 3, 0, 0, 0, 100, 0, 1000, 3200, 20300, 35000, 0, 11, 2606, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Magosh - In Combat - Cast ''Shock'''),
(1425, 0, 1, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kubb - Between 0-15% Health - Flee For Assist');
