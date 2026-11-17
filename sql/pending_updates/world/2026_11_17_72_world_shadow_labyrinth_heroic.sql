-- Heroic fixes for Shadow Labyrinth. The Cabal Warlock lacked Seed of Corruption (normal and heroic ids) and six heroic
-- templates had levels that disagree with the retail data.

DELETE FROM `smart_scripts` WHERE `entryorguid` = 18640 AND `source_type` = 0 AND `id` IN (14, 15);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(18640, 0, 14, 0, 0, 0, 100, 4, 7800, 24100, 19300, 36200, 0, 11, 32863, 32, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Cabal Warlock - In Combat - Cast ''Seed of Corruption'' (Normal Dungeon)'),
(18640, 0, 15, 0, 0, 0, 100, 8, 7800, 24100, 19300, 36200, 0, 11, 38252, 32, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Cabal Warlock - In Combat - Cast ''Seed of Corruption'' (Heroic Dungeon)');

UPDATE `creature_template` SET `minlevel` = 71, `maxlevel` = 71 WHERE `entry` = 20642; -- Cabal Executioner heroic, was 70-71
UPDATE `creature_template` SET `minlevel` = 70, `maxlevel` = 70 WHERE `entry` = 20638; -- Cabal Acolyte heroic, was 70-71
UPDATE `creature_template` SET `minlevel` = 70, `maxlevel` = 71 WHERE `entry` = 20641; -- Cabal Deathsworn heroic, was 69-70
UPDATE `creature_template` SET `minlevel` = 70, `maxlevel` = 70 WHERE `entry` = 20639; -- Cabal Assassin heroic, was 70-71
UPDATE `creature_template` SET `minlevel` = 70, `maxlevel` = 71 WHERE `entry` = 20645; -- Cabal Ritualist heroic, was 69-71
UPDATE `creature_template` SET `minlevel` = 70, `maxlevel` = 70 WHERE `entry` = 20644; -- Cabal Fanatic heroic, was 70-71
