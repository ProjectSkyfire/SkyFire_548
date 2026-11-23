-- These quest spells need a spell focus object near their target: Violet Scrying Crystal (32042, focus 1390).
-- 1 object(s) had no usable template and 2 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject_template` WHERE `entry` IN (182187);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `data3`) VALUES
(182187, 8, 2770, 'Underground Water Source Focus', 1, 1390, 25, 1);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 911558 AND 911559;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911558, 182187, 0, 1, 0, 0, -11158.1, -1917.49, -18.2718, 3.31645, 0, 0, 0.996181, -0.087317, 600, 0, 1),
(911559, 182187, 0, 1, 0, 0, -11369.2, -2165.15, 21.7162, 2.17057, 0, 0, 0.884436, 0.466661, 600, 0, 1);
