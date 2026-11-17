-- Heroic fixes for The Mechanar. The heroic variants of the trash abilities (spell ids from the 5.4.8 client data and
-- the TrinityCore rows) were missing or cast with the normal id in both modes, and three heroic templates had levels that
-- disagree with the retail data.

UPDATE `smart_scripts` SET `event_flags` = 2 WHERE `entryorguid` = 19166 AND `source_type` = 0 AND `id` = 0 AND `action_param1` = 35012;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 19166 AND `source_type` = 0 AND `id` = 2;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(19166, 0, 2, 0, 0, 0, 100, 8, 2000, 2000, 9000, 11000, 0, 11, 38941, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Tempest-Forge Patroller - In Combat - Cast Charged Arcane Missile (Heroic Dungeon)');

DELETE FROM `smart_scripts` WHERE `entryorguid` = 19231 AND `source_type` = 0 AND `id` IN (1, 2);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(19231, 0, 1, 0, 0, 0, 100, 4, 11100, 19300, 22900, 28900, 0, 11, 35056, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mechanar Crusher - In Combat - Cast ''Glob of Machine Fluid'' (Normal Dungeon)'),
(19231, 0, 2, 0, 0, 0, 100, 8, 11100, 19300, 22900, 28900, 0, 11, 38923, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mechanar Crusher - In Combat - Cast ''Glob of Machine Fluid'' (Heroic Dungeon)');

DELETE FROM `smart_scripts` WHERE `entryorguid` = 19712 AND `source_type` = 0 AND `id` IN (3, 4);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(19712, 0, 3, 0, 0, 0, 100, 4, 13300, 37400, 21700, 33800, 0, 11, 35056, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mechanar Driller - In Combat - Cast ''Glob of Machine Fluid'' (Normal Dungeon)'),
(19712, 0, 4, 0, 0, 0, 100, 8, 13300, 37400, 21700, 33800, 0, 11, 38923, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mechanar Driller - In Combat - Cast ''Glob of Machine Fluid'' (Heroic Dungeon)');

DELETE FROM `smart_scripts` WHERE `entryorguid` = 19713 AND `source_type` = 0 AND `id` IN (1, 2);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(19713, 0, 1, 0, 0, 0, 100, 4, 15200, 27700, 21700, 36800, 0, 11, 35056, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mechanar Wrecker - In Combat - Cast ''Glob of Machine Fluid'' (Normal Dungeon)'),
(19713, 0, 2, 0, 0, 0, 100, 8, 15200, 27700, 21700, 36800, 0, 11, 38923, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mechanar Wrecker - In Combat - Cast ''Glob of Machine Fluid'' (Heroic Dungeon)');

DELETE FROM `smart_scripts` WHERE `entryorguid` = 20059 AND `source_type` = 0 AND `id` IN (3, 4);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(20059, 0, 3, 0, 0, 0, 100, 4, 6100, 15600, 12100, 22500, 0, 11, 35243, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Sunseeker Netherbinder - In Combat - Cast ''Starfire'' (Normal Dungeon)'),
(20059, 0, 4, 0, 0, 0, 100, 8, 6100, 15600, 12100, 22500, 0, 11, 38935, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Sunseeker Netherbinder - In Combat - Cast ''Starfire'' (Heroic Dungeon)');

UPDATE `creature_template` SET `minlevel` = 70, `maxlevel` = 70 WHERE `entry` = 21539; -- Sunseeker Astromage heroic, was 70-71
UPDATE `creature_template` SET `minlevel` = 71, `maxlevel` = 71 WHERE `entry` = 21522; -- Bloodwarder Centurion heroic, was 70-71
UPDATE `creature_template` SET `minlevel` = 70, `maxlevel` = 70 WHERE `entry` = 21523; -- Bloodwarder Physician heroic, was 69-70
