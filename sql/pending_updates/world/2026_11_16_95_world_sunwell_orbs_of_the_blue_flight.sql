-- Sunwell Plateau: the four Orbs of the Blue Flight (188415) belong to the Kil'jaeden encounter, whose instance
-- script registers them and whose players use them for Vengeance of the Blue Flight (45839) against Darkness of a
-- Thousand Souls. They were not spawned in the Sunwell. Positions from Legends of Azeroth 5.4.8.

DELETE FROM `gameobject` WHERE `guid` BETWEEN 1062211 AND 1062214;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(1062211, 188415, 580, 16, 0, 0, 1703.91, 581.981, 28.132, 1.761, 0, 0, 0.771057, 0.636766, 180, 255, 1),
(1062212, 188415, 580, 16, 0, 0, 1694.32, 676.565, 28.05, 4.808, 0, 0, 0.672508, -0.740090, 180, 255, 1),
(1062213, 188415, 580, 16, 0, 0, 1747.59, 621.556, 28.05, 3.111, 0, 0, 0.999883, 0.015296, 180, 255, 1),
(1062214, 188415, 580, 16, 0, 0, 1651.39, 635.547, 28.142, 6.163, 0, 0, 0.060056, -0.998195, 180, 255, 1);
