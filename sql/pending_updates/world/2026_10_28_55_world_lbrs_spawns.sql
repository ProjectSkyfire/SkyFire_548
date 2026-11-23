-- Lower Blackrock Spire: gameobject spawns that SkyFire lacks, from 4.3.4 (spawnMask 2). A 4.3.4 spawn is imported only if no SkyFire spawn of the same entry is left within 15 yards
-- (one-to-one, nearest pair first). Safe to run twice: each block deletes its reserved id range first.

DELETE FROM `gameobject` WHERE `guid` BETWEEN 910520 AND 910549;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(910520, 175571, 229, 2, 0, 0, -47.9095, -369.089, 51.5425, 1.37881, 0, 0, 0.636078, 0.771625, -1, 255, 1),
(910521, 175571, 229, 2, 0, 0, -27.8804, -385.891, 48.5067, 3.7001, 0, 0, 0.961261, -0.275638, -1, 255, 1),
(910522, 175571, 229, 2, 0, 0, -13.7275, -384.816, 48.9746, 3.68265, 0, 0, 0.96363, -0.267241, -1, 255, 1),
(910523, 175571, 229, 2, 0, 0, -12.3689, -376.475, 49.335, 5.044, 0, 0, 0.580703, -0.814115, -1, 255, 1),
(910524, 175571, 229, 2, 0, 0, -24.8824, -369.619, 49.7059, 3.4034, 0, 0, 0.991444, -0.13053, -1, 255, 1),
(910525, 175584, 229, 2, 0, 0, -14.3415, -395.729, 48.5381, 2.86233, 0, 0, 0.990267, 0.139178, -1, 255, 0),
(910526, 175571, 229, 2, 0, 0, -34.5437, -370.233, 50.3396, 5.35816, 0, 0, 0.446198, -0.894934, -1, 255, 1);
