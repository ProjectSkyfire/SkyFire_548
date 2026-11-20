-- These quest spells need a spell focus object near their target: Horn of Fecundity (53020, focus 1561); Didgeridoo of Contemplation (53038, focus 1562).
-- 1 object(s) had no usable template and 4 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject_template` WHERE `entry` IN (191141);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `data3`) VALUES
(191141, 8, 2770, 'Sholazar Pillar Focus', 1, 1562, 20, 1);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 911927 AND 911930;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911927, 191136, 571, 1, 0, 0, 6131.45, 5785.1, 73.7117, -1.37881, 0, 0, 0, 0, 300, 100, 1),
(911928, 191136, 571, 1, 0, 0, 6208.92, 5788.1, 51.1155, 1.43117, 0, 0, 0, 0, 300, 100, 1),
(911929, 191136, 571, 1, 0, 0, 6237.64, 5847.45, 50.2631, 0.506145, 0, 0, 0, 0, 300, 100, 1),
(911930, 191141, 571, 1, 0, 0, 6200.74, 4767.16, 222.515, 4.48069, 0, 0, 0.784103, -0.620631, 300, 0, 1);
