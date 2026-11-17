-- Heroic fixes for Sethekk Halls. The Time-Lost Shadowmage lacked Shadow Missiles (normal and heroic ids) and two heroic
-- templates had levels that disagree with the retail data.

DELETE FROM `smart_scripts` WHERE `entryorguid` = 18320 AND `source_type` = 0 AND `id` IN (2, 3);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(18320, 0, 2, 0, 0, 0, 100, 4, 6600, 18600, 9300, 21800, 0, 11, 32675, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Time-Lost Shadowmage - In Combat - Cast ''Shadow Missiles'' (Normal Dungeon)'),
(18320, 0, 3, 0, 0, 0, 100, 8, 6600, 18600, 9300, 21800, 0, 11, 38148, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Time-Lost Shadowmage - In Combat - Cast ''Shadow Missiles'' (Heroic Dungeon)');

UPDATE `creature_template` SET `minlevel` = 71, `maxlevel` = 72 WHERE `entry` = 20701; -- Sethekk Talon Lord heroic, was 72-72
UPDATE `creature_template` SET `minlevel` = 70, `maxlevel` = 70 WHERE `entry` = 20686; -- Avian Darkhawk heroic, was 69-70
