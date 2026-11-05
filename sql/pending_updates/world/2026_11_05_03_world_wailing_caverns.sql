-- Wailing Caverns: boss abilities, boss yells, moved boss spawns, entrance quest givers, missing herbs.
-- Sources: The Cataclysm Preservation Project 4.3.4 (boss and trash behaviour, texts, spawns),
-- Legends of Azeroth 5.4.8 (Mutanus the Devourer abilities).

-- Bosses and the two trash types that had no AI: use SmartAI. Event flags carry no difficulty bits so the
-- rows fire whichever difficulty numbering the core uses; the dungeon has a single mode.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (3640, 3653, 3840) AND `ScriptName` = '';

-- Lord Cobrahn, Lord Pythas, Lady Anacondra, Lord Serpentis and Mutanus keep their row 0 (instance data on death).
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (3669, 3670, 3671, 3673, 3654) AND `id` > 0;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (3653, 3674, 5775, 3640, 3840);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` IN (364000, 364001, 364002, 364003, 384000);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(3669, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lord Cobrahn - On Aggro - Set Event Phase 1'),
(3669, 0, 2, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lord Cobrahn - On Aggro - Say Line 0'),
(3669, 0, 3, 0, 0, 1, 100, 0, 1000, 1000, 1000, 1000, 0, 11, 20805, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Lord Cobrahn - In Combat - Cast ''Lightning Bolt'' (Phase 1)'),
(3669, 0, 4, 0, 0, 1, 100, 1, 20000, 20000, 0, 0, 0, 11, 8040, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Lord Cobrahn - In Combat - Cast ''Druid''s Slumber'' (Phase 1) (No Repeat)'),
(3669, 0, 5, 6, 0, 1, 100, 1, 38000, 38000, 0, 0, 0, 22, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lord Cobrahn - In Combat - Set Event Phase 2 (Phase 1) (No Repeat)'),
(3669, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 7965, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lord Cobrahn - Linked - Cast ''Cobrahn Serpent Form'''),
(3669, 0, 7, 0, 0, 2, 100, 1, 13000, 13000, 0, 0, 0, 11, 744, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Lord Cobrahn - In Combat - Cast ''Poison'' (Phase 2) (No Repeat)'),
(3670, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lord Pythas - On Aggro - Say Line 0'),
(3670, 0, 2, 0, 0, 0, 100, 0, 3000, 3000, 11000, 11000, 0, 11, 20805, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Lord Pythas - In Combat - Cast ''Lightning Bolt'''),
(3670, 0, 3, 0, 0, 0, 100, 0, 19000, 19000, 33000, 33000, 0, 11, 8147, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lord Pythas - In Combat - Cast ''Thunderclap'''),
(3670, 0, 4, 0, 0, 0, 100, 0, 14000, 14000, 31000, 31000, 0, 11, 11431, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lord Pythas - In Combat - Cast ''Healing Touch'''),
(3670, 0, 5, 0, 0, 0, 100, 0, 35000, 35000, 19000, 19000, 0, 11, 8040, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Lord Pythas - In Combat - Cast ''Druid''s Slumber'''),
(3671, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lady Anacondra - On Aggro - Say Line 0'),
(3671, 0, 2, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 28, 13236, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lady Anacondra - On Aggro - Remove Aura ''Nature Channeling'''),
(3671, 0, 3, 0, 7, 0, 100, 0, 0, 0, 0, 0, 0, 11, 13236, 34, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lady Anacondra - On Evade - Cast ''Nature Channeling'''),
(3671, 0, 4, 0, 0, 0, 100, 0, 1000, 1000, 4000, 4000, 0, 11, 20295, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Lady Anacondra - In Combat - Cast ''Lightning Bolt'''),
(3671, 0, 5, 0, 0, 0, 100, 0, 8000, 8000, 12000, 12000, 0, 11, 8040, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Lady Anacondra - In Combat - Cast ''Druid''s Slumber'''),
(3671, 0, 6, 0, 0, 0, 100, 1, 15000, 15000, 0, 0, 0, 11, 8148, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lady Anacondra - In Combat - Cast ''Thorns Aura'' (No Repeat)'),
(3671, 0, 7, 0, 0, 0, 100, 1, 49000, 49000, 0, 0, 0, 11, 23381, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lady Anacondra - In Combat - Cast ''Healing Touch'' (No Repeat)'),
(3673, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lord Serpentis - On Aggro - Say Line 0'),
(3673, 0, 2, 0, 0, 0, 100, 0, 1000, 1000, 15000, 15000, 0, 11, 12167, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Lord Serpentis - In Combat - Cast ''Lightning Bolt'''),
(3673, 0, 3, 0, 0, 0, 100, 0, 27000, 27000, 9000, 9000, 0, 11, 23381, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lord Serpentis - In Combat - Cast ''Healing Touch'''),
(3673, 0, 4, 0, 0, 0, 100, 0, 7000, 7000, 40000, 40000, 0, 11, 8040, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Lord Serpentis - In Combat - Cast ''Druid''s Slumber'''),
(3653, 0, 0, 0, 0, 0, 100, 0, 6000, 6000, 16000, 16000, 0, 11, 80362, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Kresh - In Combat - Cast ''Crushing Bite'''),
(3674, 0, 0, 0, 0, 0, 100, 0, 3000, 4000, 3000, 4000, 0, 11, 6254, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Skum - In Combat - Cast ''Chained Bolt'''),
(5775, 0, 0, 0, 0, 0, 100, 0, 20000, 35000, 20000, 35000, 0, 11, 8142, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Verdan the Everliving - In Combat - Cast ''Grasping Vines'''),
(3654, 0, 1, 0, 0, 0, 100, 0, 15000, 21000, 18000, 24000, 0, 11, 7399, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Mutanus the Devourer - In Combat - Cast ''Terrify'''),
(3654, 0, 2, 0, 0, 0, 100, 0, 6000, 11000, 25000, 36000, 0, 11, 7967, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Mutanus the Devourer - In Combat - Cast ''Naralex''s Nightmare'''),
(3654, 0, 3, 0, 0, 0, 100, 0, 10000, 14000, 10000, 16000, 0, 11, 8150, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mutanus the Devourer - In Combat - Cast ''Thundercrack'''),
(3640, 0, 0, 0, 1, 0, 100, 1, 1000, 1000, 1000, 1000, 0, 87, 364000, 364001, 364002, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Evolving Ectoplasm - Out of Combat - Run Random Script (No Repeat)'),
(3640, 0, 1, 0, 0, 0, 100, 1, 12000, 17000, 12000, 17000, 0, 87, 364000, 364001, 364002, 364003, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Evolving Ectoplasm - In Combat - Run Random Script (No Repeat)'),
(3840, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Druid of the Fang - On Aggro - Set Event Phase 1'),
(3840, 0, 1, 0, 25, 0, 100, 0, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Druid of the Fang - On Reset - Set Event Phase 0'),
(3840, 0, 2, 0, 0, 1, 100, 0, 0, 0, 3500, 4000, 0, 11, 9532, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Druid of the Fang - In Combat CMC - Cast ''Lightning Bolt'' (Phase 1)'),
(3840, 0, 3, 0, 0, 1, 100, 0, 37000, 37000, 24000, 26000, 0, 80, 384000, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Druid of the Fang - In Combat - Run Script (Phase 1)'),
(3840, 0, 4, 0, 0, 0, 100, 0, 8000, 11000, 10000, 20000, 0, 11, 8040, 33, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 'Druid of the Fang - In Combat - Cast ''Druid''s Slumber'''),
(3840, 0, 5, 0, 74, 0, 100, 0, 0, 40, 100, 12000, 18000, 11, 23381, 1, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 'Druid of the Fang - On Friendly Between 0-40% Health - Cast ''Healing Touch'''),
(364000, 9, 0, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, 28, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Evolving Ectoplasm - Script - Remove All Auras'),
(364000, 9, 1, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, 11, 7940, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Evolving Ectoplasm - Script - Cast ''immunity-frost'''),
(364000, 9, 2, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, 11, 7944, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Evolving Ectoplasm - Script - Cast ''evolving-ectoplasm-blue'''),
(364001, 9, 0, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, 28, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Evolving Ectoplasm - Script - Remove All Auras'),
(364001, 9, 1, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, 11, 7941, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Evolving Ectoplasm - Script - Cast ''immunity-nature'''),
(364001, 9, 2, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, 11, 7945, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Evolving Ectoplasm - Script - Cast ''evolving-ectoplasm-green'''),
(364002, 9, 0, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, 28, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Evolving Ectoplasm - Script - Remove All Auras'),
(364002, 9, 1, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, 11, 7942, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Evolving Ectoplasm - Script - Cast ''immunity-fire'''),
(364002, 9, 2, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, 11, 7943, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Evolving Ectoplasm - Script - Cast ''evolving-ectoplasm-red'''),
(364003, 9, 0, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, 28, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Evolving Ectoplasm - Script - Remove All Auras'),
(364003, 9, 1, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, 11, 7943, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Evolving Ectoplasm - Script - Cast ''evolving-ectoplasm-red'''),
(364003, 9, 2, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, 11, 7946, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Evolving Ectoplasm - Script - Cast ''evolving-ectoplasm-black'''),
(384000, 9, 0, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Druid of the Fang - Script - Set Event Phase 0'),
(384000, 9, 1, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, 21, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Druid of the Fang - Script - Enable Combat Movement'),
(384000, 9, 2, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, 11, 8041, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Druid of the Fang - Script - Cast ''Serpent Form'''),
(384000, 9, 3, 0, 0, 0, 100, 0, 10000, 12000, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Druid of the Fang - Script - Set Event Phase 1');

-- Aggro yells of the four fanglords (same text in both references).
DELETE FROM `creature_text` WHERE `entry` IN (3669, 3670, 3671, 3673) AND `groupid` = 0;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(3669, 0, 0, 'You will never wake the dreamer!', 14, 0, 100, 0, 0, 5785, 'Lord Cobrahn on Aggro'),
(3670, 0, 0, 'The coils of death will crush you!', 14, 0, 100, 0, 0, 5787, 'Lord Pythas on Aggro'),
(3671, 0, 0, 'None can stand against the Serpent Lords!', 14, 0, 100, 0, 0, 5786, 'Lady Anacondra on Aggro'),
(3673, 0, 0, 'I am the serpent king! I can do anything!', 14, 0, 100, 0, 0, 5788, 'Lord Serpentis on Aggro');

-- Lady Anacondra channels Nature Channeling until she is pulled; Ebru and Nalpak sit at the entrance.
UPDATE `creature_template_addon` SET `auras` = '13236' WHERE `entry` = 3671;
DELETE FROM `creature_template_addon` WHERE `entry` IN (5767, 5768);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(5767, 0, 0, 1, 1, 0, NULL),
(5768, 0, 0, 1, 1, 0, NULL);

-- Lord Pythas, Lady Anacondra and Skum stood in the wrong place (moved by guid).
UPDATE `creature` SET `position_x` = -45.4295, `position_y` = 372.129, `position_z` = -60.0352, `orientation` = 0.191986 WHERE `guid` = 87107 AND `id` = 3670;
UPDATE `creature` SET `position_x` = -55.2565, `position_y` = 274.696, `position_z` = -92.7609, `orientation` = 4.69494 WHERE `guid` = 27366 AND `id` = 3671;
UPDATE `creature` SET `position_x` = -231.456, `position_y` = -8.97287, `position_z` = -105.665, `orientation` = 1.19865, `spawndist` = 5, `MovementType` = 1 WHERE `guid` = 87131 AND `id` = 3674;

-- Nalpak and Ebru, the quest givers next to the Disciple of Naralex, were never spawned.
DELETE FROM `creature` WHERE `guid` IN (8315000, 8315001);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8315000, 5767, 43, 2, 0, 0, 0, 0, -150.298, 123.82, -75.9513, 3.22886, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8315001, 5768, 43, 2, 0, 0, 0, 0, -155.661, 123.652, -75.0228, 0.471239, 7200, 0, 0, 0, 0, 0, 0, 0, 0);

-- Three Serpentbloom plants near Skum are in both references and missing here.
DELETE FROM `gameobject` WHERE `guid` IN (911400, 911401, 911402);
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911400, 13874, 43, 2, 0, 0, -209.992, -42.5342, -104.412, 0.942477, 0, 0, 0.45399, 0.891007, 7200, 100, 1),
(911401, 13874, 43, 2, 0, 0, -257.6, -42.1874, -105.691, 0.942477, 0, 0, 0.45399, 0.891007, 7200, 100, 1),
(911402, 13874, 43, 2, 0, 0, -198.207, -29.0856, -105.853, 0.942477, 0, 0, 0.45399, 0.891007, 7200, 100, 1);

