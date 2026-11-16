-- Sholazar Basin: 28216 Zepik the Gorloc Hunter (gives and ends 12536 to 12539) was listed for his quests but had no
-- spawn here.
DELETE FROM `creature` WHERE `guid` BETWEEN 8320006 AND 8320006;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8320006, 28216, 571, 1, 0, 0, 0, 0, 6147.37, 4917.29, -94.094, 3.42155, 300, 0, 0, 0, 0, 0, 0, 0, 0);
