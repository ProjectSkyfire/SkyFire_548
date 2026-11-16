-- Sholazar Basin: 28107 Lightningcaller Soo-met (ends 12705, 12761, 12762) was listed for his quests but had no
-- spawn here.
DELETE FROM `creature` WHERE `guid` BETWEEN 8320004 AND 8320004;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8320004, 28107, 571, 1, 0, 0, 0, 0, 5115.86, 5471.11, -91.842, 1.5625, 600, 0, 0, 0, 0, 0, 0, 0, 0);
