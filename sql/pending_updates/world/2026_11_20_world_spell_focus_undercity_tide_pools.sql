-- These quest spells need a spell focus object near their target: Getting Tide Pool Sample #1 (12802, focus 443); Getting Tide Pool Sample #2 (12805, focus 444); Getting Tide Pool Sample #3 (12806, focus 445); Getting Tide Pool Sample #4 (12808, focus 446).
-- 4 object(s) had no usable template and 4 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject_template` WHERE `entry` IN (152598, 152604, 152605, 152606);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `data3`) VALUES
(152598, 8, 299, 'the First Tide Pool', 1, 443, 20, 1),
(152604, 8, 299, 'the Second Tide Pool', 1, 444, 15, 1),
(152605, 8, 299, 'the Third Tide Pool', 1, 445, 13, 1),
(152606, 8, 299, 'the Fourth Tide Pool', 1, 446, 20, 1);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 1061246 AND 1061249;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(1061246, 152598, 1, 1, 0, 0, 3278.15, -5710.01, -1.73, 0, 0, 0, 0, 1, 300, 0, 1),
(1061247, 152604, 1, 1, 0, 0, 3604.16, -5703.09, -3.95, 0, 0, 0, 0, 1, 300, 0, 1),
(1061248, 152605, 1, 1, 0, 0, 3701.84, -5743.96, -0.76, 0, 0, 0, 0, 1, 300, 0, 1),
(1061249, 152606, 1, 1, 0, 0, 3779.38, -5680.15, -6.13, 0, 0, 0, 0, 1, 300, 0, 1);
