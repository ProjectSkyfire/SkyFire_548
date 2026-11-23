-- These quest spells need a spell focus object near their target: Destroy Bijou (24377, focus 1344).
-- 1 object(s) had no usable template and 1 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject_template` WHERE `entry` IN (180375);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `data3`) VALUES
(180375, 8, 299, 'Altar of Zanza Spell Focus', 1, 1344, 5, 1);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 911565 AND 911565;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911565, 180375, 0, 1, 0, 0, -11809.9, 1366.92, 0.11, 0, 0, 0, 0, 1, 180, 100, 1);
