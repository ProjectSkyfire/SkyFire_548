-- These quest spells need a spell focus object near their target: Call Rexxar (39223, focus 1453); Call Sablemane (39224, focus 1453).
-- 1 object(s) had no usable template and 1 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject_template` WHERE `entry` IN (185323);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `data3`) VALUES
(185323, 8, 327, '[DND]Goc Altar Focus', 1, 1453, 20, 1);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 911609 AND 911609;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911609, 185323, 530, 1, 0, 0, 3741.3, 5363.73, -7.42, 0, 0, 0, 0, 0, 120, 100, 1);

DELETE FROM `event_scripts` WHERE `id` IN (14525);
INSERT INTO `event_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES
(14525, 0, 10, 22473, 600000, 0, 3747.3, 5357.36, -8.02653, 2.26842),
(14525, 1, 10, 20555, 180000, 0, 3705.15, 5386.27, -4.06, 5.11);
