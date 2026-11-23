-- These quest spells need a spell focus object near their target: Placing Smokeys Explosives (19250, focus 983).
-- The objects had templates and 8 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject` WHERE `guid` BETWEEN 911550 AND 911557;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911550, 177668, 0, 1, 0, 0, 3180.5, -3321, 166.007, -1, 0, 0, -0.479426, 0.877583, 180, 100, 1),
(911551, 177668, 0, 1, 0, 0, 2855.55, -3232.55, 136.483, 2.83171, 0, 0, 0.988021, 0.154322, 180, 100, 1),
(911552, 177668, 0, 1, 0, 0, 3234.96, -3095.31, 163.635, 6.22462, 0, 0, 0.029278, -0.999571, 180, 100, 1),
(911553, 177668, 0, 1, 0, 0, 2803.04, -3759.51, 126.572, 0.145643, 0, 0, 0.072757, 0.99735, 180, 100, 1),
(911554, 177668, 0, 1, 0, 0, 3006.06, -3433.58, 152.234, 1.43056, 0, 0, 0.655829, 0.75491, 180, 100, 1),
(911555, 177668, 0, 1, 0, 0, 3124.68, -3869, 139.169, 0.227, 0, 0, 0.113256, 0.993566, 180, 100, 1),
(911556, 177668, 0, 1, 0, 0, 3292.42, -3644, 149.743, 1.344, 0, 0, 0.622552, 0.782578, 180, 100, 1),
(911557, 177668, 0, 1, 0, 0, 3183.1, -3461, 168.207, 1.047, 0, 0, 0.499914, 0.866075, 180, 100, 1);
