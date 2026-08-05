-- Spawn Cannoneer Smythe for The Guns of Northwatch.

UPDATE `creature_template`
SET `npcflag` = `npcflag` | 3
WHERE `entry` = 3454;

SET @CGUID := 269115;

DELETE FROM `creature`
WHERE `guid` = @CGUID;

INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID, 3454, 1, 1, 0, 0, 1863, 0, -1993.4, -3730.63, 5.3686, 3.05433, 300, 0, 0, 1146, 0, 0, 0, 0, 0);
