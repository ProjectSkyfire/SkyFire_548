-- The quest spells of Dustwallow Marsh need a spell focus object near their target: Raptor Bait (42338, focus 1459), Wreath (42390, 1461),
-- Stonemaul Banner (42425, 1463), Witchbane Torch (42521, 1464) and Salvage Wreckage (42287, 1458). The four focus objects had no
-- template and no spawn, and the debris had only 4 of its 10 spawns, so the spells failed with "Requires" at the quest sites.
-- Based on Legends of Azeroth 5.4.8.
DELETE FROM `gameobject_template` WHERE `entry` IN (186299, 186324, 186336, 186424);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `data3`) VALUES
(186299, 8, 0, 'Blackhoof Village Windmill', 1, 1459, 15, 1),
(186324, 8, 0, 'Hyal Family Monument', 1, 1461, 10, 1),
(186336, 8, 0, 'Entrance to Onyxia''s Lair', 1, 1463, 20, 1),
(186424, 8, 0, 'Swamplight Dock', 1, 1464, 10, 1);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 910905 AND 910915;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(910905, 186283, 1, 1, 0, 0, -2761.82, -4496.42, 0, -1.37881, 0, 0, -0.636078, 0.771625, 300, 255, 1),
(910906, 186283, 1, 1, 0, 0, -2656.52, -4466.58, 0, 0.663223, 0, 0, 0.325567, 0.945519, 300, 255, 1),
(910907, 186283, 1, 1, 0, 0, -2893.16, -4418.93, 0, -1.15192, 0, 0, -0.544639, 0.838671, 300, 255, 1),
(910908, 186283, 1, 1, 0, 0, -2595.31, -4487.07, 0, 2.3911, 0, 0, 0.930417, 0.366502, 300, 255, 1),
(910909, 186283, 1, 1, 0, 0, -2738.53, -4376.88, 0, 2.18166, 0, 0, 0.887011, 0.461749, 300, 255, 1),
(910910, 186283, 1, 1, 0, 0, -2738.74, -4420.66, 0, 2.63544, 0, 0, 0.968147, 0.250381, 300, 255, 1),
(910911, 186283, 1, 1, 0, 0, -2613.55, -4411.73, 0, -0.890115, 0, 0, -0.43051, 0.902586, 300, 255, 1),
(910912, 186299, 1, 1, 0, 0, -2446.01, -3153.87, 33.3341, 4.25555, 0, 0, 0.848856, -0.528624, 600, 0, 1),
(910913, 186324, 1, 1, 0, 0, -3532.88, -4317.78, 7.06083, 5.98722, 0, 0, 0.147443, -0.989071, 180, 0, 1),
(910914, 186336, 1, 1, 0, 0, -4698.88, -3721.16, 50.8914, 3.76424, 0, 0, 0.951929, -0.306319, 180, 0, 1),
(910915, 186424, 1, 1, 0, 0, -2967.28, -3871.99, 33.06, 0, 0, 0, 0, 1, 180, 0, 0);
