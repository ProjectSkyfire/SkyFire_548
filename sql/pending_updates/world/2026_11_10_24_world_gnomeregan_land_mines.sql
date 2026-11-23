-- Gnomeregan (map 90): the six Dark Iron Land Mines of the 5.4.8 data. Their SmartAI detonates them within 8 yards, but
-- no mine was spawned. Normal mode only. Idempotent: the block's spawns are deleted before they are inserted.

DELETE FROM `creature` WHERE `guid` BETWEEN 9252000 AND 9252005;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9252000, 8035, 90, 2, 0, 0, 0, 0, -732.939, 711.568, -318.06, 2.49655, 7200, 0, 0, 171, 0, 0, 0, 0, 0),
(9252001, 8035, 90, 2, 0, 0, 0, 0, -768.261, 601.513, -291.06, 1.37257, 7200, 0, 0, 171, 0, 0, 0, 0, 0),
(9252002, 8035, 90, 2, 0, 0, 0, 0, -652.372, 711.294, -327.06, 2.20214, 7200, 0, 0, 171, 0, 0, 0, 0, 0),
(9252003, 8035, 90, 2, 0, 0, 0, 0, -757.635, 645.506, -306.135, 2.78222, 7200, 0, 0, 171, 0, 0, 0, 0, 0),
(9252004, 8035, 90, 2, 0, 0, 0, 0, -738.205, 541.665, -291.06, 2.83474, 7200, 0, 0, 171, 0, 0, 0, 0, 0),
(9252005, 8035, 90, 2, 0, 0, 0, 0, -622.084, 529.586, -272.979, 6.21004, 7200, 0, 0, 171, 0, 0, 0, 0, 0);
