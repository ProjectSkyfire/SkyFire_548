-- Summon Eagle Spirit (69340) needs spell focus 1640 next to Dyami Windsoar. The focus object (301035) had no template and no spawn, so the Water of Vision could not be used.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject_template` WHERE `entry` IN (301035);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`) VALUES
(301035, 8, 299, 'you to be next to Dyami Windsoar', 1, 1640, 20);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 910984 AND 910985;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(910984, 301035, 1, 1, 0, 0, -2945.58, 11.4951, 190.589, 1.56396, 0, 0, 0.704686, 0.709519, 300, 255, 1),
(910985, 301035, 1, 1, 0, 0, -2933.04, -42.8545, 189.676, 1.76031, 0, 0, 0.770838, 0.637031, 300, 255, 1);
