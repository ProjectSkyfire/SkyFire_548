-- Uldaman (map 70): SmartAI rows for four creature types that have AIName SmartAI but no rows, so they only melee.
-- Stonevault Oracle (4852, 23 spawns), Stonevault Flameweaver (7321, 6), Stonevault Cave Lurker (4850, 14) and
-- Shadowforge Relic Hunter (4847, 5). The rows are the ones the 4.3.4 data has for them.
-- Idempotent: the rows are deleted by key before they are inserted.

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (4847,4850,4852,7321);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4847, 0, 1, 0, 0, 0, 100, 4, 8000, 11000, 12000, 16000, 0, 11, 6726, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Shadowforge Relic Hunter - In Combat - Cast ''Silence'''),
(4850, 0, 0, 0, 1, 0, 100, 5, 1000, 1000, 0, 0, 0, 11, 22766, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Stonevault Cave Lurker - Out of Combat - Cast ''Sneak'' (No Repeat) (Normal Dungeon)'),
(4852, 0, 0, 0, 2, 0, 100, 4, 0, 50, 20000, 20000, 0, 11, 11986, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Stonevault Oracle - Between 0-50% Health - Cast ''Healing Wave'' (Normal Dungeon)'),
(4852, 0, 2, 0, 0, 0, 100, 4, 8000, 11000, 48000, 55000, 0, 11, 5605, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Stonevault Oracle - In Combat - Cast ''Healing Ward'' (Normal Dungeon)'),
(4852, 0, 3, 0, 0, 0, 100, 4, 4000, 4000, 30000, 35000, 0, 11, 8264, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Stonevault Oracle - In Combat - Cast ''Lava Spout Totem'' (Normal Dungeon)'),
(4852, 0, 4, 0, 2, 0, 100, 5, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Stonevault Oracle - Between 0-15% Health - Flee For Assist (Normal Dungeon)'),
(7321, 0, 1, 0, 0, 0, 100, 4, 9000, 12000, 18000, 21000, 0, 11, 2602, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Stonevault Flameweaver - In Combat - Cast ''Fire Shield IV'' (Normal Dungeon)'),
(7321, 0, 2, 0, 0, 0, 100, 4, 15000, 18000, 15000, 21000, 0, 11, 7739, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Stonevault Flameweaver - In Combat - Cast ''Inferno Shell'' (Normal Dungeon)'),
(7321, 0, 3, 0, 2, 0, 100, 5, 0, 15, 0, 0, 0, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Stonevault Flameweaver - Between 0-15% Health - Flee For Assist (Normal Dungeon)');
