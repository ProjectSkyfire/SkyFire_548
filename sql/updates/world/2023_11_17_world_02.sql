SET @CGUID := 120768;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID-1 AND @CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID-0, 53714, 860, 1, 594, 0, 0, 0, 1493.68, 3418.64, 171.051, 2.77507, 30, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-1, 57873, 860, 1, 594, 0, 0, 0, 1467.26, 3424.37, 171.087, 5.68977, 30, 0, 0, 0, 0, 0, 0, 0, 0);
