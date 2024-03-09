SET @CGUID := 120771;
DELETE FROM `creature` WHERE `guid` IN (120662, @CGUID-0, @CGUID-1);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID-0, 53714, 860, 1, 594, 0, 0, 0, 1471.21, 3399.65, 171.064, 1.51844, 30, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-1, 53714, 860, 1, 594, 0, 0, 0, 1468.5, 3399.89, 171.056, 1.50098, 30, 0, 0, 0, 0, 0, 0, 0, 0);
