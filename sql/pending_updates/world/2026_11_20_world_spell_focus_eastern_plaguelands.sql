-- Four quest spells of the Eastern Plaguelands need a spell focus object near their target: Rayne's Seed (84964, focus 1684), Create Relic Bundle
-- (18987, 944), Trial of the Crypt (85308, 1685) and Destroy Thresh'jin's Body (85327, 1686). None of the objects had a template or a spawn.
-- Based on Legends of Azeroth 5.4.8.
DELETE FROM `gameobject_template` WHERE `entry` IN (301077, 177528, 301078, 301079);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`) VALUES
(301077, 8, 299, 'Fungal Vale Necropolis', 1, 1684, 20),
(301078, 8, 299, 'Light''s Hope Chapel Crypt', 1, 1685, 20),
(301079, 8, 299, 'Zul''Mashar Ziggurat Bonfire', 1, 1686, 20);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `data3`) VALUES
(177528, 8, 327, 'Darrowshire Town Square spell focus (DND)', 1, 944, 20, 1);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 911500 AND 911505;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911500, 301077, 0, 1, 0, 0, 2406.45, -3801.06, 185.472, 0.499034, 0, 0, 0.246936, 0.969032, 300, 255, 1),
(911501, 301077, 0, 1, 0, 0, 2562.52, -3810.19, 185.058, 0.499034, 0, 0, 0.246936, 0.969032, 300, 255, 1),
(911502, 301077, 0, 1, 0, 0, 2509.77, -3645.18, 186.609, 0.499034, 0, 0, 0.246936, 0.969032, 300, 255, 1),
(911503, 177528, 0, 1, 0, 0, 1442.67, -3702.75, 77.3847, 2.51397, 0, 0, 0.951164, 0.308686, 180, 0, 1),
(911504, 301078, 0, 1, 0, 0, 2323.19, -5385.99, 76.6035, 4.76732, 0, 0, 0.687421, -0.726259, 300, 100, 1),
(911505, 301079, 0, 1, 0, 0, 3459.95, -5001.16, 200.323, 5.51941, 0, 0, 0.372673, -0.927963, 300, 100, 1);
