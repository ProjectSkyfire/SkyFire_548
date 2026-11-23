-- Place Dithers' Drum (83176) needs spell focus 1679 near each of the four Felstone Farm targets, and Create Relic Bundle (18987) needs spell focus 944
-- at Caer Darrow. The objects had no template and no spawn.
-- Based on Legends of Azeroth 5.4.8.
DELETE FROM `gameobject_template` WHERE `entry` IN (301072, 177528);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`) VALUES
(301072, 8, 299, 'Felstone Farm Plague Drum Target', 1, 1679, 20);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `data3`) VALUES
(177528, 8, 327, 'Darrowshire Town Square spell focus (DND)', 1, 944, 20, 1);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 911507 AND 911511;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911507, 301072, 0, 1, 0, 0, 1752.47, -1186.84, 60.0328, 0, 0, 0, 0, 0, 300, 100, 1),
(911508, 301072, 0, 1, 0, 0, 1805.95, -1166.25, 61.4514, 0, 0, 0, 0, 0, 300, 100, 1),
(911509, 301072, 0, 1, 0, 0, 1811.29, -1223.74, 60.503, 0, 0, 0, 0, 0, 300, 100, 1),
(911510, 301072, 0, 1, 0, 0, 1785.58, -1251.32, 60.0896, 0, 0, 0, 0, 0, 300, 100, 1),
(911511, 177528, 0, 1, 0, 0, 1384.44, -1515.28, 59.12, 0, 0, 0, 0, 1, 180, 0, 1);
