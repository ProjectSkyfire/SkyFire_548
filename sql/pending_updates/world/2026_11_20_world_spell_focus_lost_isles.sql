-- Rockin' Powder Infused Rocket Boots (72971) need spell focus 1648 near Coach Crosscheck. The focus object (301042) had no template and no spawn.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject_template` WHERE `entry` IN (301042);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`) VALUES
(301042, 8, 299, 'Coach Crosscheck', 1, 1648, 20);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 911720 AND 911720;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911720, 301042, 648, 1, 0, 0, 1463.41, 2044.82, 225.827, 0.562344, 0, 0, 0.277482, 0.960731, 300, 100, 1);
