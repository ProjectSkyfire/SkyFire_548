-- QUEST: Sauranok Will Point The Way (28909). Spawn Questender Sauranok (42637).
SET @CGUID := 1348;
DELETE FROM `creature` WHERE `guid`=@CGUID;
DELETE FROM `creature` WHERE `id`=42637;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `PhaseId`, `PhaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 42637, 1, 1, 0, 0, 32981, 0, 1663.12, -4340.15, 26.4407, 4.34587, 300, 0, 0, 96744, 8726, 0, 0, 0, 0);
