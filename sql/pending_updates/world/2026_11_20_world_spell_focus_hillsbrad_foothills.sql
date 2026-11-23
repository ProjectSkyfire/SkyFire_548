-- These quest spells need a spell focus object near their target: Place Goblin Pocket-Nuke (91085, focus 1704).
-- 1 object(s) had no usable template and 4 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject_template` WHERE `entry` IN (207214);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`) VALUES
(207214, 8, 0, 'Four Fingers Spell Focus', 2, 1704, 10);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 911519 AND 911522;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911519, 207214, 0, 1, 0, 0, 235.464, -850.531, 146.945, 0, 0, 0, 0, 1, 120, 255, 0),
(911520, 207214, 0, 1, 0, 0, -26.272, -550.632, 151.307, 0, 0, 0, 0, 1, 120, 255, 0),
(911521, 207214, 0, 1, 0, 0, -104.95, -287.162, 143.865, 0, 0, 0, 0, 1, 120, 255, 0),
(911522, 207214, 0, 1, 0, 0, -104.992, -69.866, 147.112, 0, 0, 0, 0, 1, 120, 255, 0);
