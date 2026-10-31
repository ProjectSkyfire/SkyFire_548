-- Terrace of Endless Spring: a 10-player-only copy of every creature and object stood on top of the all-sizes set,
-- with a stray Tsulong among the Protectors. Legends of Azeroth 5.4.8 has only the all-sizes set (67 creatures).
DELETE FROM `creature` WHERE `map` = 996 AND `spawnMask` = 8 AND `guid` BETWEEN 200259 AND 200325;

-- Objects: drop the second entrance portal, spawn the six vortex walls in every size.
DELETE FROM `gameobject` WHERE `guid` = 200014 AND `id` = 214525;
UPDATE `gameobject` SET `spawnMask` = 248 WHERE `map` = 996 AND `guid` IN (200006, 200007, 200009, 200010, 200012, 200013);

-- The vortex walls are buttons in Legends of Azeroth, with names and sizes.
UPDATE `gameobject_template` SET `type` = 1, `name` = 'Sha of Fear Vortex Wall', `size` = 2.31817 WHERE `entry` = 214849;
UPDATE `gameobject_template` SET `type` = 1, `name` = 'Sha of Fear Vortex', `size` = 3.01007 WHERE `entry` = 214850;
UPDATE `gameobject_template` SET `type` = 1, `name` = 'Lei Shi Vortex Wall', `size` = 1.23444 WHERE `entry` = 214851;
UPDATE `gameobject_template` SET `type` = 1, `name` = 'Lei Shi Vortex', `size` = 1.95501 WHERE `entry` = 214852;
UPDATE `gameobject_template` SET `type` = 1, `name` = 'Jinyu Council Vortex', `size` = 1.95501 WHERE `entry` = 214853;
UPDATE `gameobject_template` SET `type` = 1, `name` = 'Jinyu Council Vortex Wall', `size` = 1.23444 WHERE `entry` = 214854;

-- Tsulong's chests (LFR, normal, heroic) did not exist; the instance script picks one when he is defeated.
DELETE FROM `gameobject_template` WHERE `entry` IN (215355, 215356, 215357);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `data3`, `data15`) VALUES
(215355, 3, 12438, 'Cache of Tsulong', 2.25, 1634, 44078, 1, 1),
(215356, 3, 12438, 'Cache of Tsulong', 2.25, 1634, 43579, 1, 1),
(215357, 3, 12438, 'Cache of Tsulong', 2.25, 1634, 44088, 1, 1);

-- The chests of Tsulong and Lei Shi, despawned until the encounter is done.
DELETE FROM `gameobject` WHERE `guid` BETWEEN 911200 AND 911204;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911200, 215355, 996, 248, -1018.02, -2992.92, 12.3084, 4.735, 0, 0, 0, 1, -604800, 255, 1),
(911201, 215356, 996, 248, -1018.02, -2992.92, 12.3084, 4.735, 0, 0, 0, 1, -604800, 255, 1),
(911202, 215357, 996, 248, -1018.02, -2992.92, 12.3084, 4.735, 0, 0, 0, 1, -604800, 255, 1),
(911203, 213075, 996, 248, -1018.17, -2885.5, 19.6325, 4.688, 0, 0, 0, 1, -604800, 255, 1),
(911204, 213076, 996, 248, -1018.17, -2885.5, 19.6325, 4.688, 0, 0, 0, 1, -604800, 255, 1);

-- Levels Legends of Azeroth gives the helpers that were still at level 1.
UPDATE `creature_template` SET `minlevel` = 90, `maxlevel` = 90 WHERE `entry` IN (65736, 64846);
UPDATE `creature_template` SET `npcflag` = 16777216 WHERE `entry` = 65736;
UPDATE `creature_template` SET `minlevel` = 92, `maxlevel` = 92 WHERE `entry` IN (64443, 62962);
