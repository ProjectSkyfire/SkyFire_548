-- These quest spells need a spell focus object near their target: Orb of the Stars (89314, focus 1690).
-- 1 object(s) had no usable template and 4 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject_template` WHERE `entry` IN (301083);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`) VALUES
(301083, 8, 299, 'Ancient Mechanism', 1, 1690, 20);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 1061250 AND 1061253;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(1061250, 301083, 1, 1, 0, 0, -9217.01, -1512.75, -169.26, 3.89926, 0, 0, 0.929097, -0.369837, 120, 255, 1),
(1061251, 301083, 1, 1, 0, 0, -9305.14, -1591.71, -169.249, 0.875475, 0, 0, 0.423892, 0.905713, 120, 255, 1),
(1061252, 301083, 1, 1, 0, 0, -9305.8, -1511.99, -169.261, 5.04201, 0, 0, 0.581512, -0.813538, 120, 255, 1),
(1061253, 301083, 1, 1, 0, 0, -9216.73, -1592.49, -169.254, 2.01823, 0, 0, 0.846361, 0.532609, 120, 255, 1);
