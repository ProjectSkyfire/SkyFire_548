-- Ported from TrinityCore 3.3.5.
UPDATE `creature_template` SET `ScriptName` = 'npc_andorov', `npcflag` = `npcflag` | 1 WHERE `entry` = 15471;
UPDATE `creature_template` SET `ScriptName` = 'npc_kaldorei_elite' WHERE `entry` = 15473;

DELETE FROM `creature` WHERE `guid` BETWEEN 9219900 AND 9219904;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9219900, 15471, 509, 8, 0, 0, 0, 0, -8624.47, 1471.77, 32.0758, 2.33195, 300, 0, 0, 10000, 0, 0, 0, 0, 0),
(9219901, 15473, 509, 8, 0, 0, 0, 0, -8622.47, 1471.77, 32.0758, 2.33195, 300, 0, 0, 10000, 0, 0, 0, 0, 0),
(9219902, 15473, 509, 8, 0, 0, 0, 0, -8626.47, 1471.77, 32.0758, 2.33195, 300, 0, 0, 10000, 0, 0, 0, 0, 0),
(9219903, 15473, 509, 8, 0, 0, 0, 0, -8624.47, 1473.77, 32.0758, 2.33195, 300, 0, 0, 10000, 0, 0, 0, 0, 0),
(9219904, 15473, 509, 8, 0, 0, 0, 0, -8624.47, 1469.77, 32.0758, 2.33195, 300, 0, 0, 10000, 0, 0, 0, 0, 0);
