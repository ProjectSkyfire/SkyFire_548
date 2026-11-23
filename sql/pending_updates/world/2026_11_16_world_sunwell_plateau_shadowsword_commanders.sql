-- Sunwell Plateau: the two Shadowsword Commanders (25837) that had a template and SmartAI but no spawns
-- Ported from TrinityCore 3.3.5. TrinityCore's 25 player bit is moved to this core's bit 16; the second Commander patrols there and stands still here.

DELETE FROM `creature` WHERE `guid` BETWEEN 8319800 AND 8319801;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8319800, 25837, 580, 16, 0, 0, 0, 1, 1682.33, 537.624, 85.3339, 2.07694, 604800, 0, 0, 175934, 0, 0, 0, 0, 0),
(8319801, 25837, 580, 16, 0, 0, 0, 1, 1754.95, 567.301, 85.1341, 0.759888, 604800, 0, 0, 175934, 0, 0, 0, 0, 0);
