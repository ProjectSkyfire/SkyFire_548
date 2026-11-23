-- Azshara quest fixes (Horde levelling content). Data reference: 5.4.8 for the spawns, scripts and loot of quest
-- objects that the 4.3.4 data does not have at all. Safe to run twice: every block deletes its own keys first or sets
-- absolute values.

-- Operation Fishgut can only be started from the Wrenchmen Recruitment Posters, and none of them was spawned, so the
-- whole Bilgewater Harbor chain behind it (Rapid Deployment, There Are Many Like It, Mop Up) could not be reached.
-- Positions are the 5.4.8 spawns.
DELETE FROM `gameobject` WHERE `guid` BETWEEN 930000 AND 930009;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(930000, 201578, 1, 1, 0, 0, 3606.95, -6463.52, 21.9795, -2.35619, 0, 0, -0.923879, 0.382686, 300, 255, 1),
(930001, 201578, 1, 1, 0, 0, 3470.76, -6413.2, 13.9499, -0.820302, 0, 0, -0.398748, 0.91706, 300, 255, 1),
(930002, 201578, 1, 1, 0, 0, 3539.4, -6515.19, 45.645, 3.12415, 0, 0, 0.999962, 0.00873456, 300, 255, 1),
(930003, 203755, 1, 1, 0, 0, 3531.47, -6257.24, 11.6376, -0.052359, 0, 0, -0.0261765, 0.999657, 300, 255, 1);

DELETE FROM `creature` WHERE `guid` BETWEEN 8330000 AND 8330199;
-- Kill targets that were never spawned: Talrendis Sniper (Up a Tree) and Talrendis Raider (Defend the Gates!), the
-- Polymorphed Cockroaches (Trouble Under Foot), Bingham Gadgetspring (The Terrible Tinkers of the Ruined Tower), Lord
-- Kassarus (Head of the Snake), the Ectoplasmic Exhaust (Gunk in the Trunk) and the Mutant Goblin (Nine's Plan), plus the
-- trial bunnies of the Archmage Xylem quests.
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8330000, 35149, 1, 1, 0, 0, 0, 0, 2632.28, -4727.66, 143.165, -2.24886, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330001, 35149, 1, 1, 0, 0, 0, 0, 2629.76, -4726.85, 143.165, 2.82829, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330002, 35149, 1, 1, 0, 0, 0, 0, 2632.03, -4727.83, 143.087, 3.57783, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330003, 35149, 1, 1, 0, 0, 0, 0, 2641.5, -4668.36, 144.426, 4.09536, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330004, 35149, 1, 1, 0, 0, 0, 0, 2637.15, -4672.49, 144.58, 4.32032, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330005, 35149, 1, 1, 0, 0, 0, 0, 2636.69, -4674.97, 144.51, -2.12438, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330006, 35149, 1, 1, 0, 0, 0, 0, 2596.39, -4711.31, 141.899, 3.41241, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330007, 35149, 1, 1, 0, 0, 0, 0, 2668.84, -4642.42, 148.217, -0.179746, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330008, 35149, 1, 1, 0, 0, 0, 0, 2603.8, -5070.88, 120.557, 5.47687, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330009, 35177, 1, 1, 0, 0, 0, 0, 2573.32, -4834.31, 145.832, 2.26687, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330010, 35177, 1, 1, 0, 0, 0, 0, 2572.04, -4827.91, 146.159, 0.257529, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330011, 35177, 1, 1, 0, 0, 0, 0, 2560.83, -4874.65, 142.417, 0.473575, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330012, 35177, 1, 1, 0, 0, 0, 0, 2564.08, -4876.47, 141.88, -2.61532, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330013, 35177, 1, 1, 0, 0, 0, 0, 2562.12, -4874.77, 142.492, -1.87958, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330014, 35177, 1, 1, 0, 0, 0, 0, 2570.41, -4840.53, 146.582, 0.779263, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330015, 35177, 1, 1, 0, 0, 0, 0, 2559.1, -4875.13, 143.045, -1.02454, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330016, 35177, 1, 1, 0, 0, 0, 0, 2559.84, -4874.78, 142.907, -1.2465, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330017, 35177, 1, 1, 0, 0, 0, 0, 2557.52, -4877.38, 143.542, 4.07279, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330018, 35177, 1, 1, 0, 0, 0, 0, 2558.18, -4875.94, 143.292, -0.685932, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330019, 35177, 1, 1, 0, 0, 0, 0, 2558.58, -4875.55, 143.042, 0.992015, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330020, 35177, 1, 1, 0, 0, 0, 0, 2572.24, -4828.56, 145.931, 4.14702, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330021, 35177, 1, 1, 0, 0, 0, 0, 2568.99, -4831.89, 146.741, -1.30079, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330022, 35177, 1, 1, 0, 0, 0, 0, 2579.17, -4827.08, 144.866, 3.11376, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330023, 35177, 1, 1, 0, 0, 0, 0, 2559.65, -4874.85, 142.667, 3.18453, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330024, 35177, 1, 1, 0, 0, 0, 0, 2571.45, -4823.36, 146.616, -0.547015, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330025, 35177, 1, 1, 0, 0, 0, 0, 2572.22, -4824.88, 146.175, 3.33133, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330026, 35177, 1, 1, 0, 0, 0, 0, 2578.65, -4828.86, 144.741, 4.42617, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330027, 35177, 1, 1, 0, 0, 0, 0, 2631.81, -4804.04, 135.669, 4.05084, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330028, 35177, 1, 1, 0, 0, 0, 0, 2639.06, -4797.85, 134.904, 2.85394, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330029, 35177, 1, 1, 0, 0, 0, 0, 2626.42, -4798.72, 137.907, 3.94444, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330030, 35177, 1, 1, 0, 0, 0, 0, 2631.43, -4799.99, 136.542, 4.08407, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330031, 35177, 1, 1, 0, 0, 0, 0, 2631.93, -4813.53, 133.797, 3.26297, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330032, 35177, 1, 1, 0, 0, 0, 0, 2656.61, -4819.29, 130.493, 3.50811, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330033, 35177, 1, 1, 0, 0, 0, 0, 2640.62, -4810.14, 133.076, 3.4901, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330034, 35177, 1, 1, 0, 0, 0, 0, 2629.92, -4787.86, 138.323, 5.04036, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330035, 35177, 1, 1, 0, 0, 0, 0, 2622.94, -4874.31, 129.481, 2.98451, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330036, 35177, 1, 1, 0, 0, 0, 0, 2631.15, -4872.52, 129.506, 3.02552, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330037, 35177, 1, 1, 0, 0, 0, 0, 2638.63, -4793.9, 135.714, 4.55942, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330038, 35177, 1, 1, 0, 0, 0, 0, 2663.43, -4811.43, 131.379, 3.49066, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330039, 35177, 1, 1, 0, 0, 0, 0, 2628.51, -4880.05, 127.921, 3.081, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330040, 35177, 1, 1, 0, 0, 0, 0, 2626.02, -4866.51, 129.734, 2.98451, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330041, 35177, 1, 1, 0, 0, 0, 0, 2641.92, -4864.63, 127.939, 3.33358, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330042, 35177, 1, 1, 0, 0, 0, 0, 2631.95, -4850.37, 128.46, 2.98451, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330043, 35177, 1, 1, 0, 0, 0, 0, 2640.64, -4877.22, 127.613, 3.21141, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330044, 35177, 1, 1, 0, 0, 0, 0, 2637.06, -4856.95, 127.844, 4.53849, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330045, 35177, 1, 1, 0, 0, 0, 0, 2651.14, -4804.98, 131.973, 3.78447, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330046, 35177, 1, 1, 0, 0, 0, 0, 2645.83, -4818.91, 131.877, 3.03201, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330047, 35177, 1, 1, 0, 0, 0, 0, 2625.36, -4815.77, 134.242, 3.50287, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330048, 35177, 1, 1, 0, 0, 0, 0, 2654.77, -4823.58, 130.655, 4.09333, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330049, 35177, 1, 1, 0, 0, 0, 0, 2637.66, -4800.18, 134.847, 4.5593, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330050, 35177, 1, 1, 0, 0, 0, 0, 2648.53, -4819.05, 131.479, 3.1123, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330051, 35177, 1, 1, 0, 0, 0, 0, 2637.18, -4803.27, 134.546, 4.5593, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330052, 35177, 1, 1, 0, 0, 0, 0, 2645.06, -4839.47, 130.003, 4.16045, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330053, 35177, 1, 1, 0, 0, 0, 0, 2614.14, -4801.32, 139.871, 2.60476, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330054, 35177, 1, 1, 0, 0, 0, 0, 2617.01, -4819.73, 135.672, 3.69462, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330055, 35177, 1, 1, 0, 0, 0, 0, 2615.87, -4793.92, 140.493, 3.34402, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330056, 35177, 1, 1, 0, 0, 0, 0, 2632.33, -4817.42, 132.953, 3.02911, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330057, 35177, 1, 1, 0, 0, 0, 0, 2620.09, -4815.06, 135.604, 3.72752, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330058, 35177, 1, 1, 0, 0, 0, 0, 2615.99, -4801.31, 139.602, 3.38525, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330059, 35177, 1, 1, 0, 0, 0, 0, 2604.22, -4826.35, 140.075, 3.6358, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330060, 35177, 1, 1, 0, 0, 0, 0, 2624.25, -4803.78, 137.487, 3.62707, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330061, 35177, 1, 1, 0, 0, 0, 0, 2612.21, -4820.43, 137.297, 3.53642, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330062, 35177, 1, 1, 0, 0, 0, 0, 2600.94, -4796.98, 143.124, 3.34071, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330063, 35177, 1, 1, 0, 0, 0, 0, 2655.46, -4801.74, 132.57, 3.63029, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330064, 35628, 1, 1, 0, 0, 0, 0, 4570.56, -6296.96, 110.767, 1.82977, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330065, 35628, 1, 1, 0, 0, 0, 0, 4558.79, -6341.69, 121.348, 6.12611, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330066, 35628, 1, 1, 0, 0, 0, 0, 4505.16, -6363.71, 127.426, 0.925025, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330067, 35628, 1, 1, 0, 0, 0, 0, 4428.85, -6397.15, 119.658, 2.56563, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330068, 35628, 1, 1, 0, 0, 0, 0, 4398.92, -6408.8, 115.154, 1.91998, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330069, 35628, 1, 1, 0, 0, 0, 0, 4407.77, -6443.56, 118.799, 3.47321, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330070, 35628, 1, 1, 0, 0, 0, 0, 4456.58, -6419.73, 138.383, 5.2709, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330071, 35628, 1, 1, 0, 0, 0, 0, 4483.93, -6404.81, 134.668, 1.15192, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330072, 35628, 1, 1, 0, 0, 0, 0, 4511.39, -6305.05, 118.303, 2.67095, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330073, 35628, 1, 1, 0, 0, 0, 0, 4509.21, -6397.27, 127.421, 3.19395, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330074, 35628, 1, 1, 0, 0, 0, 0, 4487.44, -6396.89, 126.186, 0.488692, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330075, 35628, 1, 1, 0, 0, 0, 0, 4477.67, -6376.43, 124.023, 3.36848, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330076, 36407, 1, 1, 0, 0, 0, 0, 2266.56, -5569.23, 21.2302, 5.20108, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330077, 36822, 1, 1, 0, 0, 0, 0, 3548.06, -5357.79, 107.565, 1.69297, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330078, 35558, 1, 1, 0, 0, 0, 0, 2900.81, -5032.61, 127.262, 3.25852, 300, 3, 0, 0, 0, 1, 0, 0, 0),
(8330079, 35558, 1, 1, 0, 0, 0, 0, 2910.25, -5032.42, 126.952, 0, 300, 3, 0, 0, 0, 1, 0, 0, 0),
(8330080, 36156, 1, 1, 0, 0, 0, 0, 2581.89, -5907.86, 96.6035, 6.21426, 300, 3, 0, 0, 0, 1, 0, 0, 0),
(8330081, 36156, 1, 1, 0, 0, 0, 0, 2572.44, -5907.58, 97.039, 6.19592, 300, 3, 0, 0, 0, 1, 0, 0, 0),
(8330082, 34773, 1, 1, 0, 0, 0, 0, 4534.98, -5235.04, 304.29, 5.72468, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330083, 34773, 1, 1, 0, 0, 0, 0, 4514.17, -5232.38, 304.303, 5.72468, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330084, 34773, 1, 1, 0, 0, 0, 0, 4504.55, -5220.82, 304.287, 5.72468, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330085, 34773, 1, 1, 0, 0, 0, 0, 4526.21, -5223.18, 304.287, 5.72468, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330086, 34773, 1, 1, 0, 0, 0, 0, 4503.49, -5231.25, 304.29, 5.72468, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330087, 34773, 1, 1, 0, 0, 0, 0, 4505.58, -5210.48, 304.287, 5.72468, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330088, 34773, 1, 1, 0, 0, 0, 0, 4516.49, -5211.53, 304.287, 5.72468, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330089, 34773, 1, 1, 0, 0, 0, 0, 4536.45, -5224.38, 304.289, 5.72468, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330090, 34773, 1, 1, 0, 0, 0, 0, 4517.83, -5201.04, 304.288, 5.72468, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330091, 34773, 1, 1, 0, 0, 0, 0, 4528.85, -5202.38, 304.289, 5.72468, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330092, 34773, 1, 1, 0, 0, 0, 0, 4539.15, -5203.7, 304.287, 5.72468, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330093, 34773, 1, 1, 0, 0, 0, 0, 4524.71, -5233.67, 304.303, 5.72468, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330094, 34773, 1, 1, 0, 0, 0, 0, 4515.34, -5221.93, 304.287, 5.72468, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330095, 34773, 1, 1, 0, 0, 0, 0, 4537.77, -5213.72, 304.287, 5.72468, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330096, 34773, 1, 1, 0, 0, 0, 0, 4527.5, -5212.69, 304.287, 5.72468, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330097, 35076, 1, 1, 0, 0, 0, 0, 4635.67, -6784.81, 215.548, 0, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330098, 35076, 1, 1, 0, 0, 0, 0, 4650.46, -6788.63, 218.397, 0, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330099, 35076, 1, 1, 0, 0, 0, 0, 4619.61, -6774.81, 215.372, 0, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330100, 36345, 1, 1, 0, 0, 0, 0, 4330.53, -5053.82, 335.543, 5.79449, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330101, 36345, 1, 1, 0, 0, 0, 0, 4331.03, -5086.92, 335.595, 0.366519, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330102, 36345, 1, 1, 0, 0, 0, 0, 4386.87, -5113.11, 335.595, 1.81514, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330103, 36345, 1, 1, 0, 0, 0, 0, 4366.58, -5120, 335.604, 1.62316, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330104, 36345, 1, 1, 0, 0, 0, 0, 4336.19, -5102.63, 335.595, 0.715585, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330105, 36345, 1, 1, 0, 0, 0, 0, 4322.61, -5069.79, 336.026, 0, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330106, 36345, 1, 1, 0, 0, 0, 0, 4350.35, -5114.65, 335.758, 1.02974, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330107, 36345, 1, 1, 0, 0, 0, 0, 4345, -5039.45, 335.599, 5.58505, 60, 3, 0, 0, 0, 1, 0, 0, 0);

-- Quest givers, enders and credit NPCs that were never spawned: the Images of Archmage Xylem (The Pinnacle of Learning,
-- Watch Your Step, the three trials), Spirit of Azuregos and Azuregos, Subject Nine (My Favorite Subject, Nine's Plan,
-- Raptor Raptor Rocket), Captain Tork, Bombardier Captain Smooks, Sergeant Hort, Friz Groundspin, Archmage Selwyn and
-- the Spirit of Kalytha (Memories of the Dead), Ungarl (The Blackmaw Doublecross) and Gormungan (Befriending Giants).
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8330108, 36334, 1, 1, 0, 0, 0, 0, 4838.26, -6444.98, 19.7808, 4.95674, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330109, 36335, 1, 1, 0, 0, 0, 0, 4515.12, -5190.95, 305.154, 5.02655, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330110, 36336, 1, 1, 0, 0, 0, 0, 4606.32, -6802.91, 211.973, 0.959931, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330111, 36337, 1, 1, 0, 0, 0, 0, 4396.75, -5085.69, 335.518, 3.63029, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330112, 36361, 1, 1, 0, 0, 0, 0, 4934.08, -6458.29, 160.713, 0, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330113, 36436, 1, 1, 0, 0, 0, 0, 3911.53, -4912.29, 149.46, 1.11701, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330114, 36676, 1, 1, 0, 0, 0, 0, 4629.98, -6953.75, 206.121, 5.24625, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330115, 36500, 1, 1, 0, 0, 0, 0, 2583.3, -5701.69, 110.488, 1.39626, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330116, 36745, 1, 1, 0, 0, 0, 0, 3730.14, -5265.1, 84.2507, 4.2237, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330117, 36785, 1, 1, 0, 0, 0, 0, 2602.42, -4170.15, 158.716, 2.11185, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330118, 36921, 1, 1, 0, 0, 0, 0, 3397.98, -5655.15, 7.33072, 0.942478, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330119, 50367, 1, 1, 0, 0, 0, 0, 2663.84, -6168.66, 138.185, 3.90954, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330120, 35595, 1, 1, 0, 0, 0, 0, 2641.2, -5441.82, 115.505, 0.610865, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330121, 35567, 1, 1, 0, 0, 0, 0, 2720.8, -5377.83, 108.148, 1.49594, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330122, 36618, 1, 1, 0, 0, 0, 0, 4278.36, -5081.08, 133.064, 5.25344, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330123, 36297, 1, 1, 0, 0, 0, 0, 2604.24, -6005.88, 101.413, 6.07375, 300, 0, 0, 0, 0, 0, 0, 0, 0);

-- Templates that were placeholders (level 1, friendly faction, no loot id) for mobs the quests kill or loot, and quest
-- NPCs that lacked their gossip flag, menu or script binding. Values follow the 5.4.8 data.
UPDATE `creature_template` SET `minlevel` = 12, `maxlevel` = 12, `faction_A` = 14, `faction_H` = 14, `lootid` = 35558, `type` = 15 WHERE `entry` = 35558;
UPDATE `creature_template` SET `minlevel` = 17, `maxlevel` = 17, `faction_A` = 14, `faction_H` = 14, `unit_flags` = 32768, `lootid` = 36130 WHERE `entry` = 36130;
UPDATE `creature_template` SET `minlevel` = 14, `maxlevel` = 14, `faction_A` = 14, `faction_H` = 14, `unit_flags` = 32832, `lootid` = 36156 WHERE `entry` = 36156;
UPDATE `creature_template` SET `faction_A` = 188, `faction_H` = 188, `npcflag` = 16777216 WHERE `entry` = 35628;
UPDATE `creature_template` SET `AIName` = 'SmartAI', `npcflag` = 1, `gossip_menu_id` = 10658 WHERE `entry` = 35595;
UPDATE `creature_template` SET `AIName` = 'SmartAI', `npcflag` = 1, `gossip_menu_id` = 10659 WHERE `entry` = 35567;
UPDATE `creature_template` SET `AIName` = 'SmartAI', `npcflag` = 1, `gossip_menu_id` = 10828 WHERE `entry` = 36618;
UPDATE `creature_template` SET `AIName` = 'SmartAI', `npcflag` = 1, `gossip_menu_id` = 10801 WHERE `entry` = 36472;
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (36297, 36437, 36077, 35526, 36025, 36061, 36922, 36951, 36952, 36953);
UPDATE `creature_template` SET `AIName` = 'SmartAI', `npcflag` = 16777216 WHERE `entry` = 36756;
UPDATE `creature_template` SET `AIName` = 'SmartAI', `minlevel` = 15, `maxlevel` = 15, `faction_A` = 2159, `faction_H` = 2159 WHERE `entry` = 36937;

-- Polymorphed Cockroach is clicked to be squashed (Trouble Under Foot).
-- Dead Soldier is clicked to take his military supplies (Hand-me-downs).
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (35628, 36756);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(35628, 67822, 1, 0),
(36756, 69261, 2, 0);

-- Hostile quest mobs with no damage at all (placeholder rows): Talrendis Sniper and Raider, Sable Drake, Lord
-- Kassarus and Bingham Gadgetspring. The numbers follow the other quest mobs of the same level in this zone, they are
-- not retail values.
UPDATE `creature_template` SET `mindmg` = 22, `maxdmg` = 40, `attackpower` = 35, `baseattacktime` = 2000 WHERE `entry` = 35149;
UPDATE `creature_template` SET `mindmg` = 24, `maxdmg` = 44, `attackpower` = 42, `baseattacktime` = 2000 WHERE `entry` = 35177;
UPDATE `creature_template` SET `mindmg` = 34, `maxdmg` = 61, `attackpower` = 67, `baseattacktime` = 2000 WHERE `entry` = 36640;
UPDATE `creature_template` SET `mindmg` = 34, `maxdmg` = 59, `attackpower` = 66, `baseattacktime` = 2000 WHERE `entry` = 36822;
UPDATE `creature_template` SET `mindmg` = 27, `maxdmg` = 51, `attackpower` = 54, `baseattacktime` = 2000 WHERE `entry` = 36407;

-- Quest items nothing dropped: Pristine Thunderhead Feather (Shear Will), Ectosplatter Sample (Gunk in the Trunk),
-- Globe of Boiling Water (Full of Hot Water), Secret Rocket Plans (Nine's Plan) and Azsharite Sample (Mysterious
-- Azsharite, from the Azsharite Formations). Chances are the 5.4.8 ones.
DELETE FROM `creature_loot_template` WHERE `entry` IN (35558, 36130, 36156) OR (`entry` IN (6375, 6377) AND `item` = 49068);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(6375, 49068, -81.2801, 1, 0, 5, 12),
(6377, 49068, -68.2685, 1, 0, 5, 12),
(35558, 48780, -100, 1, 0, 1, 1),
(36130, 49174, -100, 1, 0, 1, 1),
(36156, 49204, -100, 1, 0, 1, 1);

DELETE FROM `gameobject_loot_template` WHERE `entry` IN (9819, 9820, 9676) AND `item` = 49207;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(9819, 49207, -100, 1, 0, 1, 1),
(9820, 49207, -100, 1, 0, 1, 1),
(9676, 49207, -100, 1, 0, 1, 1);

-- Gossip: Ungarl's menu had no option, and the options of Archmage Selwyn, Gormungan and the Door Buzzer showed
-- without the conditions that tie them to their quest.
DELETE FROM `gossip_menu_option` WHERE `MenuID` = 10828 AND `OptionID` = 0;
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcflag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(10828, 0, 0, 'What''s that smell? Is that you? Seriously.', 36563, 1, 1, 0, 0, 0, 0, NULL, 0, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` IN (10658, 10780, 10801, 10828);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 10658, 0, 0, 0, 9, 0, 14215, 0, 0, 0, 0, 0, '', 'Show gossip if player have quest 14215'),
(15, 10780, 0, 0, 0, 2, 0, 49211, 1, 0, 0, 0, 0, '', 'Only show gossip if player has item'),
(15, 10780, 0, 0, 0, 9, 0, 14377, 0, 0, 0, 0, 0, '', 'Only show gossip if player has quest'),
(15, 10780, 1, 0, 0, 2, 0, 49230, 1, 0, 0, 0, 0, '', 'Only show gossip if player has item'),
(15, 10780, 1, 0, 0, 9, 0, 14385, 0, 0, 0, 0, 0, '', 'Only show gossip if player has quest'),
(15, 10780, 2, 0, 0, 9, 0, 14388, 0, 0, 0, 0, 0, '', 'Only show gossip if player has quest'),
(15, 10801, 0, 0, 0, 9, 0, 14408, 0, 0, 0, 0, 0, '', 'Show gossip_menu_option only if quest accepted but not completed'),
(15, 10828, 0, 0, 0, 1, 0, 69059, 0, 0, 0, 0, 0, '', 'Show gossip menu option if player has aura');

-- Credit scripts of the Azshara quest NPCs (5.4.8): Archmage Selwyn (Memories of the Dead), Spirit of Kalytha, Ungarl
-- (The Blackmaw Doublecross), Door Buzzer (Nine's Plan) and Gormungan with the Rocketway Rats (Befriending Giants,
-- Azsharite Experiment Number One and Two).
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (35595, 35567, 36618, 36472, 36297, 36437);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(35567, 0, 0, 1, 62, 0, 100, 0, 10659, 0, 0, 0, 0, 11, 67704, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Spirit of Kalytha - On Gossip Select - Cast Memories of the Dead'),
(35567, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Spirit of Kalytha - Linked To Id 0 - Close Gossip'),
(35567, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Spirit of Kalytha - Linked To Id 1 - Say Line 0'),
(35595, 0, 0, 1, 62, 0, 100, 0, 10658, 0, 0, 0, 0, 33, 35593, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Archmage Selwyn - On Gossip Select - Quest Credit'),
(35595, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 28, 67704, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Archmage Selwyn - Linked To Id 0 - Remove Aura Memories of the Dead'),
(35595, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Archmage Selwyn - Linked To Id 1 - Close Gossip'),
(36297, 0, 0, 1, 62, 0, 100, 0, 10780, 0, 0, 0, 0, 57, 49211, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Gormungan - On Gossip Select - Remove Item Mound o'' Meat'),
(36297, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Gormungan Linked To Id 0 - Close Gossip'),
(36297, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 36333, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Gormungan - Linked To Id 1 - Quest Credit'),
(36297, 0, 3, 4, 62, 0, 100, 0, 10780, 1, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Gormungan - On Gossip Select - Close Gossip'),
(36297, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 36420, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Gormungan - Linked To Id 3 - Quest Credit'),
(36297, 0, 5, 6, 62, 0, 100, 0, 10780, 2, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Gormungan - On Gossip Select - Store Target List'),
(36297, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Gormungan - Linked To Id 5 - Close Gossip'),
(36297, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 3000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gormungan - Linked To Id 6 - Say Line 0'),
(36297, 0, 8, 9, 61, 0, 100, 0, 0, 0, 0, 0, 0, 85, 68747, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Gormungan - Linked To Id 7 - Cast Giant Forcecast'),
(36297, 0, 9, 10, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 68746, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Gormungan - Linked To Id 8 - Cast Giant Stomp'),
(36297, 0, 10, 11, 61, 0, 100, 0, 0, 0, 0, 0, 0, 85, 68749, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Gormungan - Linked To Id 9 - Cast Stun'),
(36297, 0, 11, 12, 61, 0, 100, 0, 0, 0, 0, 0, 0, 28, 68710, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Gormungan - Linked To Id 10 - Remove Aura Shrunkified'),
(36297, 0, 12, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 19, 36437, 5, 0, 0, 0, 0, 0, 'Gormungan - Linked To Id 11 - Set Data 0 1'),
(36297, 0, 13, 0, 52, 0, 100, 0, 0, 36297, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Gormungan - On Text Over 0 - Say Line 1'),
(36437, 0, 0, 1, 38, 0, 100, 0, 0, 1, 0, 0, 0, 28, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rocketway Rat - On Data Set 0 1 - Remove All Auras'),
(36437, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rocketway Rat - Linked To Id 0 - Despawn'),
(36472, 0, 0, 1, 62, 0, 100, 0, 10801, 0, 0, 0, 0, 85, 68752, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Door Buzzer - On Gossip Select - Invoker Cast Ring Buzzer'),
(36472, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Door Buzzer - Linked To Id 0 - Close Gossip'),
(36618, 0, 0, 1, 62, 0, 100, 0, 10828, 0, 0, 0, 0, 33, 36625, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Ungarl'),
(36618, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Ungarl'),
(36618, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 28, 69059, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Ungarl');

-- Runaway Shredder, Reprogrammed Shredder and Backup Shredder (Runaway Shredder!, The Captain's Logs, Up a Tree, Defend
-- the Gates!, Arborcide): the shredder is beaten down to 1 health, becomes friendly and is clicked to pilot a vehicle.
-- The 4.3.4 version only made it a mob that dies. This replaces the 4.3.4 rows of the Runaway Shredder.
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (35111, 35129, 35196);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(35111, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 42, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Runaway Shredder - On Respawn - Set Invincibility Hp 1'),
(35111, 0, 1, 2, 2, 0, 100, 1, 0, 1, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Runaway Shredder - On Between 0-1% Health - Set Faction 35'),
(35111, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Runaway Shredder - Linked To Id 1 - Say Line 0'),
(35111, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Runaway Shredder - Linked To Id 2 - Combat Stop'),
(35111, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 60000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Runaway Shredder - Linked To Id 3 - Despawn after 60 sec'),
(35111, 0, 5, 6, 73, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Runaway Shredder - On Spellclick - Store Target List'),
(35111, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 67, 35111, 2000, 2000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Runaway Shredder - Linked To Id 5 - Create Timed Event'),
(35111, 0, 7, 8, 59, 0, 100, 0, 35111, 0, 0, 0, 0, 86, 66787, 0, 12, 1, 0, 0, 19, 35129, 5, 0, 0, 0, 0, 0, 'Runaway Shredder - On Timed Event Triggered - Cross Cast Pilot a Shredder'),
(35111, 0, 8, 9, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 35111, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Runaway Shredder - Linked To Id 7 - Quest Credit'),
(35111, 0, 9, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Runaway Shredder - Linked To Id 8 - Despawn'),
(35129, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Reprogrammed Shredde - On Summoned - Set Run 1'),
(35129, 0, 1, 0, 7, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Reprogrammed Shredde - On Evade - Despawn'),
(35196, 0, 0, 1, 73, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Backup Shredder - On Spellclick - Store Target List'),
(35196, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 67, 35196, 2000, 2000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Backup Shredder - Linked To Id 0 - Create Timed Event'),
(35196, 0, 2, 3, 59, 0, 100, 0, 35196, 0, 0, 0, 0, 86, 66787, 0, 12, 1, 0, 0, 19, 35129, 5, 0, 0, 0, 0, 0, 'Backup Shredder - On Timed Event Triggered - Cross Cast Pilot a Shredder'),
(35196, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Backup Shredder - Linked To Id 2 - Despawn');

-- Quest credit scripts of the other Azshara quests: Secret Lab Fires and Research Intern with their timed lists
-- (When Science Attacks), Assistant Greely's shrinking potion (Befriending Giants), Clubnik's Dozer (Dozercism), the
-- Wounded Soldier and Airborne Priest (Halo Drops), the three Real Estate Bunnies (First Come, First Served) and the
-- Dead Soldier (Hand-me-downs).
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (36025, 36061, 36077, 35526, 36922, 36937, 36951, 36952, 36953, 36756);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(35526, 0, 0, 0, 8, 0, 100, 0, 68007, 0, 0, 0, 0, 33, 35526, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Clubnik''s Dozer'),
(36025, 0, 0, 1, 11, 0, 100, 0, 0, 0, 0, 0, 0, 75, 52855, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Secret Lab Fires - Cast Low Poly Fire'),
(36025, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 75, 65734, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Secret Lab Fires - Cast Low Poly Fire'),
(36025, 0, 2, 0, 8, 0, 100, 0, 68283, 0, 2000, 2000, 0, 80, 3602500, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Secret Lab Fires - Run Script'),
(36061, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 75, 68294, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Research Intern - Cast Low Poly Fire'),
(36061, 0, 1, 0, 8, 0, 100, 0, 68283, 0, 1000, 1000, 0, 80, 3606100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Research Intern - Run Script'),
(36077, 0, 0, 1, 62, 0, 100, 0, 10797, 0, 0, 0, 0, 85, 68710, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Assistant Greely - On Gossip Select - Invoker Cast Shrunkified'),
(36077, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Assistant Greely Linked To Id 0 - Close Gossip'),
(36077, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Assistant Greely - Linked To Id 1 - Say Line 0'),
(36756, 0, 0, 0, 73, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dead Soldier'),
(36922, 0, 0, 1, 8, 0, 100, 0, 69640, 0, 0, 0, 0, 28, 114371, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Soldier - On SpellHit Resurrection - Remove Aura Permanent Feign Death'),
(36922, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 36922, 0, 0, 0, 0, 0, 18, 30, 0, 0, 0, 0, 0, 0, 'Wounded Soldier - Linked To Id 0 - Quest Credit'),
(36922, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Soldier - Linked To Id 1 - Despawn after 5 sec'),
(36937, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Airborne Priest - On Summoned - Store Target List'),
(36937, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 19, 36922, 10, 0, 0, 0, 0, 0, 'Airborne Priest - Linked To Id 0 - Set Orientation'),
(36937, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 3000, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Airborne Priest - Linked To Id 1 - Say Line 0'),
(36937, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 69640, 0, 0, 0, 0, 0, 19, 36922, 10, 0, 0, 0, 0, 0, 'Airborne Priest - Linked To Id 2 - Cast Resurrection'),
(36937, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 8000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Airborne Priest - Linked To Id 3 - Despawn after 8 sec'),
(36937, 0, 5, 0, 52, 0, 20, 0, 0, 36937, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Airborne Priest - On Text Over 0 - Say Line 1'),
(36951, 0, 0, 0, 8, 0, 100, 0, 69648, 0, 0, 0, 0, 33, 36951, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Northern Vista claimed'),
(36952, 0, 0, 0, 8, 0, 100, 0, 69648, 0, 0, 0, 0, 33, 36952, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Big ol'' Tower claimed'),
(36953, 0, 0, 0, 8, 0, 100, 0, 69648, 0, 0, 0, 0, 33, 36953, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Southern Pagoda claimed');

-- Timed lists of the Secret Lab Fires and the Research Intern.
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` IN (3602500, 3606100);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(3602500, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 36025, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Secret Lab Fires - Give Kill Credit'),
(3602500, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 28, 52855, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Secret Lab Fires - Forced Despawn'),
(3602500, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 28, 65734, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Secret Lab Fires - Forced Despawn'),
(3602500, 9, 4, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Secret Lab Fires - Forced Despawn'),
(3606100, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 36061, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Research Intern - Give Kill Credit'),
(3606100, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 28, 68294, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Research Intern - Forced Despawn'),
(3606100, 9, 3, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Research Intern - Forced Despawn');

-- Texts for those scripts (the Spirit of Kalytha line is an emote here).
DELETE FROM `creature_text` WHERE `entry` IN (36297, 35567, 36937);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(35567, 0, 0, 'You see the world through Kalytha''s eyes...', 16, 0, 100, 0, 0, 0, 'Spirit of Kalytha'),
(36297, 0, 0, 'EeeeEEEEEEKK!', 12, 0, 100, 0, 0, 0, ''),
(36297, 1, 0, 'Oh, $n, it was just you. Please don''t scare me like that.', 12, 0, 100, 0, 0, 0, ''),
(36937, 0, 0, 'Your salvation has arrived... and it''s affordably priced!', 12, 0, 100, 0, 0, 0, 'Airborne Priest'),
(36937, 0, 1, 'Have I got a heal for you!', 12, 0, 100, 0, 0, 0, 'Airborne Priest'),
(36937, 0, 2, 'I''ve got the best heals, anywhere!', 12, 0, 100, 0, 0, 0, 'Airborne Priest'),
(36937, 0, 3, 'Health from above!', 12, 0, 100, 0, 0, 0, 'Airborne Priest'),
(36937, 0, 4, 'I love the smell of salvation in the morning.', 12, 0, 100, 0, 0, 0, 'Airborne Priest'),
(36937, 0, 5, 'I''ve got what you need!', 12, 0, 100, 0, 0, 0, 'Airborne Priest'),
(36937, 0, 6, 'Back to work!', 12, 0, 100, 0, 0, 0, 'Airborne Priest'),
(36937, 0, 7, 'You didn''t even make it 20 more feet that time.', 12, 0, 100, 0, 0, 0, 'Airborne Priest'),
(36937, 1, 0, 'I''ll take my fee out of your paycheck.', 12, 0, 100, 0, 0, 0, 'Airborne Priest'),
(36937, 1, 1, 'Light is money, friend.', 12, 0, 100, 0, 0, 0, 'Airborne Priest'),
(36937, 1, 2, 'Rez incoming!', 12, 0, 100, 0, 0, 0, 'Airborne Priest'),
(36937, 1, 3, 'No more lying down on the job.', 12, 0, 100, 0, 0, 0, 'Airborne Priest'),
(36937, 1, 4, 'Not getting out of your debts that easy.', 12, 0, 100, 0, 0, 0, 'Airborne Priest'),
(36937, 1, 5, 'Walk it off.', 12, 0, 100, 0, 0, 0, 'Airborne Priest'),
(36937, 1, 6, 'That''s 3 for you, buddy. One more and my fee goes up.', 12, 0, 100, 0, 0, 0, 'Airborne Priest'),
(36937, 1, 7, 'I''m charging you for hazard pay on this one, pal.', 12, 0, 100, 0, 0, 0, 'Airborne Priest');

-- Shredder vehicles. The 4.3.4 templates for the Reprogrammed and Backup Shredder are placeholders with no vehicle id,
-- spells or click data. Values from the 5.4.8 data (vehicle 460).
UPDATE `creature_template` SET `speed_run` = 0.992063, `flags_extra` = 2, `mechanic_immune_mask` = 16384 WHERE `entry` = 35111;
UPDATE `creature_template` SET `minlevel` = 12, `maxlevel` = 12, `faction_A` = 1801, `faction_H` = 1801, `npcflag` = 16777216, `unit_flags` = 33280, `speed_run` = 1.14286, `spell1` = 66811, `spell2` = 66868, `spell3` = 66795, `spell4` = 66893, `spell6` = 66937, `VehicleId` = 460, `AIName` = 'SmartAI', `mechanic_immune_mask` = 16384 WHERE `entry` = 35129;
UPDATE `creature_template` SET `minlevel` = 12, `maxlevel` = 12, `faction_A` = 35, `faction_H` = 35, `npcflag` = 16777216, `unit_flags` = 32768, `speed_run` = 0.992063, `AIName` = 'SmartAI', `mechanic_immune_mask` = 16384 WHERE `entry` = 35196;

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (35111, 35129, 35196);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(35111, 66778, 1, 0),
(35129, 66787, 1, 0),
(35196, 66778, 1, 0);
