-- The Shattered Halls: the two adamantite chests and the two Fel Braziers of the wall, from the
-- TrinityCore 3.3.5 sniffs (build 14545 for the chests, Fel Braziers also in AzerothCore).
-- SkyFire had no chest in the instance.
DELETE FROM `gameobject` WHERE `guid` BETWEEN 1062200 AND 1062203;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(1062200, 184941, 540, 6, 0, 0, 473.462, 144.087, 0.594992, 0.0698117, 0, 0, 0.992546, 0.121868, 86400, 100, 1),
(1062201, 184940, 540, 6, 0, 0, 565.684, 230.211, 0.8906, 2.75282, 0, 0, 0.976296, 0.21644, 86400, 100, 1),
(1062202, 181679, 540, 6, 0, 0, 168.66, 291.89, -6.93, 0, 0, 0, 0, 1, 180, 0, 1),
(1062203, 181679, 540, 6, 0, 0, 187.77, 292.46, -7.33, 0, 0, 0, 0, 1, 180, 0, 1);
