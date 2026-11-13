-- Azjol-Nerub: Anub'ar Darters, Prime Guard and Webspinner AI, a heroic Anub'arak loot row.

-- Anub'ar Darter spawns (none existed here)
DELETE FROM `creature` WHERE `guid` BETWEEN 8306000 AND 8306011;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(8306000, 29349, 601, 6, 542.719, 517.971, 336.968, 4.97886, 7200, 10, 1),
(8306001, 29349, 601, 6, 535.318, 527.104, 325.288, 3.81776, 7200, 10, 1),
(8306002, 29349, 601, 6, 520.652, 456.035, 324.844, 1.56603, 7200, 10, 1),
(8306003, 29349, 601, 6, 562.511, 559.846, 330.12, 0.860635, 7200, 10, 1),
(8306004, 29349, 601, 6, 526.822, 538.034, 325.25, 3.61099, 7200, 10, 1),
(8306005, 29349, 601, 6, 503.233, 532.791, 318.974, 3.10973, 7200, 10, 1),
(8306006, 29349, 601, 6, 510.697, 551.998, 319.843, 2.29444, 7200, 10, 1),
(8306007, 29349, 601, 6, 534.447, 480.184, 320.819, 3.07491, 7200, 10, 1),
(8306008, 29349, 601, 6, 536.271, 519.256, 316.44, 4.21134, 7200, 10, 1),
(8306009, 29349, 601, 6, 570.606, 497.176, 334.092, 5.50029, 7200, 10, 1),
(8306010, 29349, 601, 6, 555.247, 519.084, 328.507, 1.21519, 7200, 10, 1),
(8306011, 29349, 601, 6, 550.508, 568.013, 331.526, 1.16499, 7200, 10, 1);

-- Anub'ar Prime Guard and Anub'ar Webspinner SmartAI (TrinityCore 3.3.5; no combat-move cast flag here)
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (29128, 29335) AND `AIName` = '' AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (29128, 29335) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(29128, 0, 0, 0, 0, 0, 100, 2, 0, 0, 10000, 13000, 0, 11, 54309, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Anub\'ar Prime Guard - Combat CMC - Cast \'Mark of Darkness\' (Normal Dungeon)'),
(29128, 0, 1, 0, 0, 0, 100, 4, 0, 0, 10000, 13000, 0, 11, 59352, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Anub\'ar Prime Guard - Combat CMC - Cast \'Mark of Darkness\' (Heroic Dungeon)'),
(29128, 0, 2, 0, 9, 0, 100, 7, 0, 5, 0, 0, 0, 11, 54314, 33, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Anub\'ar Prime Guard - 0-5 Range - Cast \'Drain Power\' (No Repeat) (Dungeon)'),
(29335, 0, 0, 0, 0, 0, 100, 2, 4000, 7000, 9000, 12000, 0, 11, 54290, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Anub\'ar Webspinner - In Combat - Cast \'Web Shot\' (Phase 1) (No Repeat)'),
(29335, 0, 1, 0, 0, 0, 100, 4, 4000, 7000, 9000, 12000, 0, 11, 59362, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Anub\'ar Webspinner - In Combat - Cast \'Web Shot\' (Phase 1) (No Repeat)'),
(29335, 0, 2, 0, 0, 0, 100, 6, 10000, 13000, 19000, 22000, 0, 11, 52086, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Anub\'ar Webspinner - In Combat - Cast \'Web Wrap\' (Phase 1) (No Repeat)');

-- Heroic Anub'arak: Design: Glinting Twilight Opal
DELETE FROM `creature_loot_template` WHERE `entry` = 31610 AND `item` = 41796;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(31610, 41796, 17, 1, 1, 1, 1);
