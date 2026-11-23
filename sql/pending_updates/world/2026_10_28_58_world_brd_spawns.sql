-- Blackrock Depths: gameobject spawns that SkyFire lacks, from 4.3.4 (spawnMask 2). A 4.3.4 spawn is imported only if no SkyFire spawn of the same entry is left within 15 yards
-- (one-to-one, nearest pair first). Safe to run twice: each block deletes its reserved id range first.

DELETE FROM `gameobject` WHERE `guid` BETWEEN 910550 AND 910569;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(910550, 165554, 230, 2, 0, 0, 802.907, -356.401, -48.9423, 0, 0, 0, 0, 1, 7200, 100, 1),
(910551, 165738, 230, 2, 0, 0, 891.99, -181.77, -42.811, 4.98, 0, 0, 0.606454, -0.795119, 300, 255, 1),
(910552, 165738, 230, 2, 0, 0, 891.36, -182.133, -42.811, 3.6782, 0, 0, 0.964222, -0.265096, 300, 255, 1),
(910553, 165738, 230, 2, 0, 0, 891.299, -183.168, -42.811, 4.55, 0, 0, 0.762127, -0.647427, 300, 255, 1),
(910554, 165738, 230, 2, 0, 0, 891.788, -182.847, -42.811, 0.597, 0, 0, 0.294087, 0.955779, 300, 255, 1),
(910555, 165738, 230, 2, 0, 0, 891.7, -184.356, -42.811, 4.673, 0, 0, 0.720895, -0.693044, 300, 255, 1);
