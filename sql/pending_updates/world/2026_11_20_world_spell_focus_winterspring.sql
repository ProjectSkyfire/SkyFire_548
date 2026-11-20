-- These quest spells need a spell focus object near their target: Summon Shy-Rotam (16796, focus 783); Demon Summoning Torch (25793, focus 1350).
-- 2 object(s) had no usable template and 2 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject_template` WHERE `entry` IN (103819, 180673);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `data3`) VALUES
(103819, 8, 362, 'Stone of Shy-Rotam', 3, 783, 10, 1),
(180673, 8, 299, 'High Chief Winterfall Cave Mouth Spell Focus', 1, 1350, 10, 1);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 1061254 AND 1061255;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(1061254, 103819, 1, 1, 0, 0, 8071.34, -3843.02, 690.85, 0, 0, 0, 0, 1, 300, 0, 1),
(1061255, 180673, 1, 1, 0, 0, 6717.54, -5248.85, 779.311, 0, 0, 0, 0, 1, 180, 0, 1);
