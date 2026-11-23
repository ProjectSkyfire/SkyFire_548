-- Rescue Krennan (68219) needs spell focus 1630 near Krennan Aranas at the Gilneas City gate. The focus object (301027) had no template and no spawn, so the cast failed with a requirement error. The object uses the phase of Krennan Aranas (171).
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject_template` WHERE `entry` IN (301027);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`) VALUES
(301027, 8, 299, 'you to be closer to Krennan Aranas.', 1, 1630, 20);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 1068000 AND 1068000;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(1068000, 301027, 654, 1, 171, 0, -1674.46, 1344.95, 15.1352, 0, 0, 0, 0, 0, 300, 0, 1);
