-- Heroic fixes for Auchenai Crypts. The Auchenai Necromancer lacked Seed of Corruption (normal and heroic ids) and the
-- heroic Auchenai Monk had a level that disagrees with the retail data.

DELETE FROM `smart_scripts` WHERE `entryorguid` = 18702 AND `source_type` = 0 AND `id` IN (3, 4);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(18702, 0, 3, 0, 0, 0, 100, 4, 12100, 29000, 22900, 35000, 0, 11, 32863, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Auchenai Necromancer - In Combat - Cast ''Seed of Corruption'' (Normal Dungeon)'),
(18702, 0, 4, 0, 0, 0, 100, 8, 12100, 17500, 4800, 9600, 0, 11, 38252, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Auchenai Necromancer - In Combat - Cast ''Seed of Corruption'' (Heroic Dungeon)');

UPDATE `creature_template` SET `minlevel` = 70, `maxlevel` = 71 WHERE `entry` = 20299; -- Auchenai Monk heroic, was 69-71
