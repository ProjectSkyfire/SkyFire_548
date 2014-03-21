-- Master Shang Xi
SET @CGUID :=258000;
DELETE FROM `creature` WHERE `guid` = @CGUID;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(@CGUID, 54786, 860, 1, 1, 960.0434, 3606.05, 196.414, 6.230825, 120, 0, 0);