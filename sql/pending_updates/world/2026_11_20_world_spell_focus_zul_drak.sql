-- These quest spells need a spell focus object near their target: Dig For Treasure (51333, focus 1550); Place Charges (52315, focus 1554).
-- The objects had templates and 6 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject` WHERE `guid` BETWEEN 911921 AND 911926;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911921, 190550, 571, 1, 0, 0, 4837.82, -2961.74, 290.602, 2.56563, 0, 0, 0, 0, 300, 0, 1),
(911922, 190550, 571, 1, 0, 0, 4865.54, -2879.29, 291.099, 0.837757, 0, 0, 0, 0, 300, 0, 1),
(911923, 190550, 571, 1, 0, 0, 4971.44, -2966.86, 290.252, -1.76278, 0, 0, 0, 0, 300, 0, 1),
(911924, 190550, 571, 1, 0, 0, 4900.33, -2870.36, 290.404, 1.43117, 0, 0, 0, 0, 300, 0, 1),
(911925, 190731, 571, 1, 0, 0, 6153.66, -2076.12, 235.115, -2.23402, 0, 0, 0, 0, 300, 0, 1),
(911926, 190731, 571, 1, 0, 0, 6058.73, -2026.15, 235.44, 2.89725, 0, 0, 0, 0, 300, 0, 1);
