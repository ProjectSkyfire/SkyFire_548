-- Cast Fishing Net (29866) needs spell focus 1369 at a School of Red Snapper. The object (181616) had a template but two of its spawns were missing.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject` WHERE `guid` BETWEEN 911603 AND 911604;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911603, 181616, 530, 1, 0, 0, -4314.31, -13044.1, -0.002122, 3.05126, 0, 0, 0.99898, 0.045151, 180, 100, 1),
(911604, 181616, 530, 1, 0, 0, -4366.22, -13018.1, -0.033625, 0.27487, 0, 0, 0.137003, 0.990571, 180, 100, 1);
