-- These quest spells need a spell focus object near their target: Item - Place Signaling Gem DND (29279, focus 1367); Ignite Alliance Siege Engine (33067, focus 1398).
-- 1 object(s) had no usable template and 4 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject_template` WHERE `entry` IN (183110);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`) VALUES
(183110, 8, 299, 'Alliance Cannon', 1, 1398, 10);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 911605 AND 911608;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911605, 181449, 530, 1, 0, 0, 162.807, 3486.04, 61.5357, 0.335081, 0, 0, 0.166758, 0.985998, 600, 0, 1),
(911606, 183110, 530, 1, 0, 0, -303.804, 2389.89, 46.3501, 1.19826, 0, 0, 0.563924, 0.825827, 600, 0, 1),
(911607, 183122, 530, 1, 0, 0, -308.108, 2393.24, 45.728, 0.732589, 0, 0, 0.358158, 0.933661, 300, 0, 1),
(911608, 185122, 530, 1, 0, 0, -367.842, 2700.72, 34.9069, 0.657984, 0, 0, 0.323089, 0.946368, 300, 0, 1);
