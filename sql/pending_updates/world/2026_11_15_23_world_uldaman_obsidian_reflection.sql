-- Uldaman (map 70): the Obsidian Sentinel (7023) and its Obsidian Shards (7209) raise Obsidian Reflection (9941), a shield
-- with a 20 percent chance to reflect the next spell cast at them (the 5.4.8 journal lists it for both). The aura has no
-- duration, so it is renewed whenever it is missing, checked every 8 to 10 seconds in combat. The Shard had no AI.
-- Idempotent: rows deleted by key before they are inserted, absolute UPDATE.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 7209 AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 7023 AND `id` = 8;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 7209;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(7023, 0, 8, 0, 0, 0, 100, 0, 2000, 2000, 8000, 10000, 0, 11, 9941, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Obsidian Sentinel - In Combat - Cast ''Obsidian Reflection'''),
(7209, 0, 0, 0, 0, 0, 100, 0, 1000, 1000, 8000, 10000, 0, 11, 9941, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Obsidian Shard - In Combat - Cast ''Obsidian Reflection''');
