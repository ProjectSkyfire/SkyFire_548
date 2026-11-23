-- Three low level quest items need a spell focus object near their target: the Ironforge Banners (focus 1671, Dun Morogh), the Westfall target (1672)
-- and the Mirror Lake waterwall (123, Elwynn). Add the spawns and the template of the Westfall object (301065 was an empty placeholder of the wrong type).
-- Based on Legends of Azeroth 5.4.8.
DELETE FROM `gameobject_template` WHERE `entry` IN (301065);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`) VALUES
(301065, 8, 299, 'you to be at the end of the Jangolode Mine.', 1, 1672, 20);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 911512 AND 911518;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911512, 203452, 0, 1, 0, 0, -4997.68, -1613.2, 507.656, 2.84489, 0, 0, 0.989016, 0.147811, 300, 255, 1),
(911513, 203452, 0, 1, 0, 0, -4946.31, -1613.89, 508.701, 2.84489, 0, 0, 0.989016, 0.147811, 300, 255, 1),
(911514, 203452, 0, 1, 0, 0, -4973.82, -1612.05, 509.448, 2.84489, 0, 0, 0.989016, 0.147811, 300, 255, 1),
(911515, 203452, 0, 1, 0, 0, -4935.85, -1679.71, 505.481, 2.84489, 0, 0, 0.989016, 0.147811, 300, 255, 1),
(911516, 301065, 0, 1, 0, 0, -9977.34, 1009.86, 31.4688, 0.321414, 0.219706, -0.685921, 0.160016, 0.675009, 300, 255, 1),
(911517, 301065, 0, 1, 0, 0, -9848.62, 1390.5, 38.0731, 3.92641, 0, 0, 0.92399, -0.382416, 300, 255, 1),
(911518, 174794, 0, 1, 0, 0, -9359.18, 545.729, 48.6228, 0, 0, 0, 0, 1, 600, 100, 1);
