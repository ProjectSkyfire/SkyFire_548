-- Kurak and Wyneth were never spawned, so King of Centaur Mountain (13995) could not be done, and Verog the Dervish
-- (851) came only from the small chance on the Kolkar. Wyneth calls Verog when she dies, next to her, where the Kolkar
-- also summon him.
DELETE FROM `creature` WHERE `guid` IN (8311010, 8311011);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8311010, 34635, 1, 1, 0, 0, 0, 0, -1158.43, -2954.45, 93.689, 2.61691, 60, 0, 0, 0, 0, 2, 0, 0, 0), -- Kurak
(8311011, 34846, 1, 1, 0, 0, 0, 0, -1211, -2724.12, 106.772, 4.81711, 300, 0, 0, 0, 0, 0, 0, 0, 0); -- Wyneth

DELETE FROM `creature_addon` WHERE `guid` = 8311010;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(8311010, 83110100, 0, 0, 1, 0, '');

DELETE FROM `waypoint_data` WHERE `id` = 83110100;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) VALUES
(83110100, 1, -1154.81, -2956.84, 93.5935, 0, 0, 0, 0, 100, 0),
(83110100, 2, -1155.51, -2956.14, 93.5935, 0, 0, 0, 0, 100, 0),
(83110100, 3, -1161.93, -2952.42, 93.8032, 0, 0, 0, 0, 100, 0),
(83110100, 4, -1167.45, -2948.45, 94.4094, 0, 0, 0, 0, 100, 0),
(83110100, 5, -1171.65, -2944, 96.2222, 0, 0, 0, 0, 100, 0),
(83110100, 6, -1173.75, -2940.28, 98.7914, 0, 0, 0, 0, 100, 0),
(83110100, 7, -1175.85, -2936.99, 101.859, 0, 0, 0, 0, 100, 0),
(83110100, 8, -1179.95, -2931.88, 104.695, 0, 0, 0, 0, 100, 0);

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (34635, 34846);

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (34635, 34846);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(34635, 0, 0, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Kurak - Between 0-15% Health - Flee For Assist (No Repeat)'),
(34846, 0, 0, 0, 0, 0, 100, 0, 0, 0, 3400, 4700, 0, 11, 9532, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Wyneth - In Combat - Cast Lightning Bolt'),
(34846, 0, 1, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Wyneth - Between 0-15% Health - Flee For Assist (No Repeat)'),
(34846, 0, 2, 0, 0, 0, 100, 0, 5000, 7000, 18000, 26000, 0, 11, 6535, 1, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 'Wyneth - In Combat - Cast Lightning Cloud'),
(34846, 0, 3, 4, 6, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wyneth - On Death - Say Line 0'),
(34846, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 3395, 4, 60000, 0, 0, 0, 8, 0, 0, 0, -1209.65, -2738.38, 102.646, 4.99352, 'Wyneth - On Death - Summon Verog the Dervish');

DELETE FROM `creature_text` WHERE `entry` = 34846;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(34846, 0, 0, '%s calls out for her mate!', 16, 0, 100, 0, 0, 0, 'Wyneth');
