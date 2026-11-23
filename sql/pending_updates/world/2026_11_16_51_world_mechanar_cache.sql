-- The Mechanar: the Cache of the Legion (its loot table 20530 is present) was not spawned.
-- Position and rotation from TrinityCore 3.3.5; AzerothCore has the same spot.
DELETE FROM `gameobject` WHERE `guid` = 1062210;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(1062210, 184849, 554, 6, 0, 0, 222.543, 70.6106, -0.00479339, 4.67748, 0, 0, -0.719339, 0.694659, 86400, 255, 1);
