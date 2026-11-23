-- Summon Edana Hatetalon (11840) needs spell focus 343 near the Hatetalon Stones. The focus object (144569) had a template but no spawn,
-- so the Horn of Hatetalon could not be used there.
-- Based on Legends of Azeroth 5.4.8.
DELETE FROM `gameobject` WHERE `guid` BETWEEN 910916 AND 910916;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(910916, 144569, 1, 1, 0, 0, -2755.55, 2611.56, 65.7428, 0, 0, 0, 0, 1, 300, 0, 1);
