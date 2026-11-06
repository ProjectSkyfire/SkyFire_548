-- Ashenvale quest fixes. Reference data: 4.3.4 for the quest data and 5.4.8 for the spawns, loot rows and quest
-- scripts that the 4.3.4 data does not have. Safe to run twice: every block deletes its own keys first or sets
-- absolute values.

-- Race masks. These Horde and Alliance quests are restricted to the old 6 or 7 classic races each, so Goblin, Worgen
-- and Pandaren players could not take them. 4.3.4 has no race limit on them; 5.4.8 has the faction races with the
-- new races added (33555378 for the Horde, 18875469 for the Alliance).
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `Id` IN (2, 23, 24, 25, 216, 824, 6441, 6442, 6462, 6482, 6503, 6544, 6546, 6621, 6641, 7866, 7867, 7868);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `Id` IN (7863, 7864, 7865);

-- The three rare-drop quests (Sharptalon's Claw, Ursangous's Paw, Shadumbra's Head) need "The Ashenvale Hunt" (6383)
-- first, which nobody gives in 4.3.4 or 5.4.8, so the quests the rare items start could never be taken. The 5.4.8
-- data has no previous quest on them.
UPDATE `quest_template` SET `PrevQuestId` = 0 WHERE `Id` IN (2, 23, 24);

-- Quest NPCs and kill targets that were never spawned, so their quests could not be started, finished or
-- completed: Kadrak, Krokk, Durak and Stikwad at Splintertree Post and Hellscream's Watch (givers and enders of
-- 13712, 13798, 13803, 13805, 13808, 13815, 13848, 13865, 13879, 13888), Brutusk (To the Rescue!), Avrus the
-- Redeemed (All's Well), the Kaldorei Assassin (Security!), the Ashenvale Assassins (They're Out There!) and the
-- Astranaar Sentinels and Throwers (Blastranaar!). Positions are the 5.4.8 spawns; the 4.3.4 data has none of them.
DELETE FROM `creature` WHERE `guid` BETWEEN 8330200 AND 8330292;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8330200, 33837, 1, 1, 0, 0, 0, 0, 2284.79, -2544.7, 98.4122, 3.35103, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Kadrak (Splintertree Post)
(8330201, 33889, 1, 1, 0, 0, 0, 0, 2323.65, -2520.54, 102.991, 4.52771, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Krokk
(8330202, 33977, 1, 1, 0, 0, 0, 0, 2456.87, -2463.13, 109.959, 5.70723, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Durak
(8330203, 34512, 1, 1, 0, 0, 0, 0, 2501.03, -1303.75, 135.117, 0.244346, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Stikwad
(8330204, 34007, 1, 1, 0, 0, 0, 0, 2267.41, -2524.76, 95.9884, 3.97007, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Brutusk
(8330205, 34358, 1, 1, 0, 0, 0, 0, 2368.18, -1705.86, 152.96, 0, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Avrus the Redeemed
(8330206, 34206, 1, 1, 0, 0, 0, 0, 2380.18, -3521.19, 92.307, 4.77617, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Kaldorei Assassin
(8330207, 34603, 1, 1, 0, 0, 0, 0, 2237.89, -840.702, 102.659, 5.01187, 60, 3, 0, 0, 0, 1, 0, 0, 0), -- Ashenvale Assassin
(8330208, 34603, 1, 1, 0, 0, 0, 0, 2207.77, -877.51, 98.7236, 5.02929, 60, 3, 0, 0, 0, 1, 0, 0, 0), -- Ashenvale Assassin
(8330209, 34603, 1, 1, 0, 0, 0, 0, 2248.4, -870.296, 99.1508, 3.48847, 60, 3, 0, 0, 0, 1, 0, 0, 0), -- Ashenvale Assassin
(8330210, 34603, 1, 1, 0, 0, 0, 0, 2219, -963.753, 90.9684, 5.64341, 60, 3, 0, 0, 0, 1, 0, 0, 0), -- Ashenvale Assassin
(8330211, 34603, 1, 1, 0, 0, 0, 0, 2261.51, -906.164, 91.4727, 3.36966, 60, 3, 0, 0, 0, 1, 0, 0, 0), -- Ashenvale Assassin
(8330212, 34603, 1, 1, 0, 0, 0, 0, 2300.35, -921.393, 90.6451, 3.3841, 60, 3, 0, 0, 0, 1, 0, 0, 0), -- Ashenvale Assassin
(8330213, 34603, 1, 1, 0, 0, 0, 0, 2287.64, -954.652, 87.2387, 2.50899, 60, 3, 0, 0, 0, 1, 0, 0, 0), -- Ashenvale Assassin
(8330214, 34603, 1, 1, 0, 0, 0, 0, 2205.46, -921.818, 93.8268, 6.12231, 60, 3, 0, 0, 0, 1, 0, 0, 0), -- Ashenvale Assassin
(8330215, 34603, 1, 1, 0, 0, 0, 0, 2245.12, -991.093, 94.4737, 4.4134, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Ashenvale Assassin
(8330216, 34603, 1, 1, 0, 0, 0, 0, 2204.86, -1003.74, 98.5109, 3.48596, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Ashenvale Assassin
(8330217, 34603, 1, 1, 0, 0, 0, 0, 2157.32, -854.32, 99.6546, 4.37243, 60, 3, 0, 0, 0, 1, 0, 0, 0), -- Ashenvale Assassin
(8330218, 34603, 1, 1, 0, 0, 0, 0, 2178.27, -985.608, 97.0333, 4.29432, 60, 3, 0, 0, 0, 1, 0, 0, 0), -- Ashenvale Assassin
(8330219, 34603, 1, 1, 0, 0, 0, 0, 2184.65, -901.938, 97.0302, 3.87356, 60, 3, 0, 0, 0, 1, 0, 0, 0), -- Ashenvale Assassin
(8330220, 34603, 1, 1, 0, 0, 0, 0, 2196.23, -795.445, 105.203, 1.47157, 60, 3, 0, 0, 0, 1, 0, 0, 0), -- Ashenvale Assassin
(8330221, 34603, 1, 1, 0, 0, 0, 0, 2251.95, -802.193, 108.475, 1.28368, 60, 3, 0, 0, 0, 1, 0, 0, 0), -- Ashenvale Assassin
(8330222, 34603, 1, 1, 0, 0, 0, 0, 2220.83, -970.834, 90.2555, 4.15388, 60, 3, 0, 0, 0, 1, 0, 0, 0), -- Ashenvale Assassin
(8330223, 34603, 1, 1, 0, 0, 0, 0, 2184.09, -895.857, 98.2631, 5.90283, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Ashenvale Assassin
(8330224, 34603, 1, 1, 0, 0, 0, 0, 2197.44, -800.063, 105.992, 0, 60, 3, 0, 0, 0, 1, 0, 0, 0), -- Ashenvale Assassin
(8330225, 34603, 1, 1, 0, 0, 0, 0, 2247.93, -800.047, 108.564, 0, 60, 3, 0, 0, 0, 1, 0, 0, 0), -- Ashenvale Assassin
(8330226, 34494, 1, 1, 0, 0, 0, 0, 2760.2, -290.121, 120.38, 5.84685, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330227, 34494, 1, 1, 0, 0, 0, 0, 2703.34, -348.799, 110.796, 5.84685, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330228, 34494, 1, 1, 0, 0, 0, 0, 2700.34, -332.255, 110.81, 5.84685, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330229, 34494, 1, 1, 0, 0, 0, 0, 2757.85, -407.819, 111.554, 2.67035, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330230, 34494, 1, 1, 0, 0, 0, 0, 2747.75, -436.903, 111.556, 5.84685, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330231, 34494, 1, 1, 0, 0, 0, 0, 2654.65, -382.385, 107.719, 4.92183, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330232, 34494, 1, 1, 0, 0, 0, 0, 2759.81, -290.326, 111.192, 5.84685, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330233, 34494, 1, 1, 0, 0, 0, 0, 2763.71, -409.31, 111.477, 3.82491, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330234, 34494, 1, 1, 0, 0, 0, 0, 2680.27, -428.901, 108.058, 4.46752, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330235, 34494, 1, 1, 0, 0, 0, 0, 2672.59, -440.729, 108.619, 1.8326, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330236, 34494, 1, 1, 0, 0, 0, 0, 2719.46, -370.438, 107.837, 5.91667, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330237, 34494, 1, 1, 0, 0, 0, 0, 2729.36, -356.615, 108.539, 5.84685, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330238, 34494, 1, 1, 0, 0, 0, 0, 2743.13, -377.7, 108.339, 3.2696, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330239, 34494, 1, 1, 0, 0, 0, 0, 2729.46, -440.431, 110.034, 2.67035, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330240, 34494, 1, 1, 0, 0, 0, 0, 2763.04, -305.045, 120.581, 4.45059, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330241, 34494, 1, 1, 0, 0, 0, 0, 2672.1, -396.119, 108.624, 6.25344, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330242, 34494, 1, 1, 0, 0, 0, 0, 2785.17, -343.188, 107.969, 0.247757, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330243, 34494, 1, 1, 0, 0, 0, 0, 2798.77, -262.549, 107.325, 0.977384, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330244, 34494, 1, 1, 0, 0, 0, 0, 2682.7, -401.734, 107.747, 5.2709, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330245, 34494, 1, 1, 0, 0, 0, 0, 2743.53, -341.737, 107.608, 5.37803, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330246, 34494, 1, 1, 0, 0, 0, 0, 2790.14, -343.814, 108.019, 5.0799, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330247, 34494, 1, 1, 0, 0, 0, 0, 2723.64, -391.942, 107.214, 4.94298, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330248, 34494, 1, 1, 0, 0, 0, 0, 2736.17, -401.783, 108.652, 5.30736, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330249, 34494, 1, 1, 0, 0, 0, 0, 2735.38, -456.814, 109.186, 3.47321, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330250, 34494, 1, 1, 0, 0, 0, 0, 2720.88, -462.379, 108.66, 5.87235, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330251, 34494, 1, 1, 0, 0, 0, 0, 2709.97, -488.91, 107.413, 5.53269, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330252, 34494, 1, 1, 0, 0, 0, 0, 2817.49, -278.311, 107.479, 0.837758, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330253, 34494, 1, 1, 0, 0, 0, 0, 2780.21, -303.125, 107.214, 4.11036, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330254, 34494, 1, 1, 0, 0, 0, 0, 2766.28, -321.543, 108.011, 4.59022, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330255, 34494, 1, 1, 0, 0, 0, 0, 2694.91, -405.241, 107.214, 3.93245, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330256, 34494, 1, 1, 0, 0, 0, 0, 2702.62, -446.645, 108.764, 4.08434, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330257, 34494, 1, 1, 0, 0, 0, 0, 2720.05, -454.418, 108.517, 5.42608, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330258, 34494, 1, 1, 0, 0, 0, 0, 2643.48, -433.481, 108.29, 0.418879, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330259, 34494, 1, 1, 0, 0, 0, 0, 2649.38, -452.106, 108.126, 1.11701, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330260, 34494, 1, 1, 0, 0, 0, 0, 2656.75, -423.087, 107.907, 1.34451, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330261, 34494, 1, 1, 0, 0, 0, 0, 2688.37, -398.774, 107.596, 4.08547, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330262, 34494, 1, 1, 0, 0, 0, 0, 2748.39, -338.863, 107.377, 1.13373, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330263, 34494, 1, 1, 0, 0, 0, 0, 2761.99, -309.198, 111.192, 4.4855, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330264, 34494, 1, 1, 0, 0, 0, 0, 2730.68, -356.344, 108.357, 0.612364, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330265, 34494, 1, 1, 0, 0, 0, 0, 2755.36, -322.165, 107.997, 4.88692, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330266, 34494, 1, 1, 0, 0, 0, 0, 2787.7, -303.589, 107.214, 0.317037, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330267, 34494, 1, 1, 0, 0, 0, 0, 2777.69, -311.478, 107.204, 4.21903, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330268, 34494, 1, 1, 0, 0, 0, 0, 2770.98, -353.232, 108.835, 0.33589, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330269, 34494, 1, 1, 0, 0, 0, 0, 2725.57, -411.321, 108.642, 4.59278, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330270, 34494, 1, 1, 0, 0, 0, 0, 2780.03, -338.77, 107.506, 1.0569, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330271, 34494, 1, 1, 0, 0, 0, 0, 2761.17, -359.367, 108.498, 0.659145, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330272, 34494, 1, 1, 0, 0, 0, 0, 2737.12, -405.262, 109.196, 2.67035, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330273, 34494, 1, 1, 0, 0, 0, 0, 2695.99, -369.445, 109.391, 2.26439, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330274, 34494, 1, 1, 0, 0, 0, 0, 2785.97, -328.495, 108.18, 5.84685, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330275, 34494, 1, 1, 0, 0, 0, 0, 2763.51, -359.825, 108.435, 5.84685, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330276, 34494, 1, 1, 0, 0, 0, 0, 2775.98, -348.385, 108.02, 5.84685, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330277, 34494, 1, 1, 0, 0, 0, 0, 2756.2, -362.701, 108.927, 5.84685, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330278, 34494, 1, 1, 0, 0, 0, 0, 2742.37, -376.724, 108.174, 5.84685, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330279, 34494, 1, 1, 0, 0, 0, 0, 2713.63, -453.819, 108.16, 5.84685, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330280, 34494, 1, 1, 0, 0, 0, 0, 2701.56, -452.163, 108.924, 5.84685, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330281, 34494, 1, 1, 0, 0, 0, 0, 2782.25, -338.627, 107.876, 5.84685, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330282, 34494, 1, 1, 0, 0, 0, 0, 2720.54, -410.74, 108.532, 5.84685, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330283, 34494, 1, 1, 0, 0, 0, 0, 2732.67, -383.802, 107.475, 5.84685, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330284, 34494, 1, 1, 0, 0, 0, 0, 2653.47, -438.932, 107.185, 5.84685, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Sentinel
(8330285, 34492, 1, 1, 0, 0, 0, 0, 2782.63, -377.885, 107.601, 5.16617, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Thrower
(8330286, 34492, 1, 1, 0, 0, 0, 0, 2701.32, -463.438, 108.798, 0.0872665, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Thrower
(8330287, 34492, 1, 1, 0, 0, 0, 0, 2638.9, -425.036, 109.674, 3.22886, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Thrower
(8330288, 34492, 1, 1, 0, 0, 0, 0, 2754.14, -471.146, 108.572, 4.71239, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Thrower
(8330289, 34492, 1, 1, 0, 0, 0, 0, 2743.25, -325.896, 108.919, 5.16617, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Thrower
(8330290, 34492, 1, 1, 0, 0, 0, 0, 2731.38, -413.885, 109.974, 3.10669, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Thrower
(8330291, 34492, 1, 1, 0, 0, 0, 0, 2790.78, -275.021, 107.714, 1.95477, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Astranaar Thrower
(8330292, 34492, 1, 1, 0, 0, 0, 0, 2691.39, -384.193, 108.93, 0.0349066, 60, 0, 0, 0, 0, 0, 0, 0, 0); -- Astranaar Thrower

-- The Kaldorei Assassin was an empty level 1 template with a neutral faction, so Security! could not be completed.
-- Level, faction and the quest drop (Kaldorei Assassin's Head) are the 5.4.8 values; damage follows the other
-- level 22 hostile mobs of the zone.
UPDATE `creature_template` SET `minlevel` = 22, `maxlevel` = 22, `faction_A` = 124, `faction_H` = 124, `mindmg` = 38, `maxdmg` = 53, `attackpower` = 86, `baseattacktime` = 2000, `lootid` = 34206 WHERE `entry` = 34206 AND `minlevel` = 1;

-- Hostile mobs that deal no damage (Ashenvale Assassin, Astranaar Sentinel, Astranaar Thrower). Values follow the
-- other hostile mobs of the same level.
UPDATE `creature_template` SET `mindmg` = 41, `maxdmg` = 54, `attackpower` = 100, `baseattacktime` = 2000 WHERE `entry` = 34603 AND `mindmg` = 0;
UPDATE `creature_template` SET `mindmg` = 56, `maxdmg` = 74, `attackpower` = 135, `baseattacktime` = 2000 WHERE `entry` = 34494 AND `mindmg` = 0;
UPDATE `creature_template` SET `mindmg` = 38, `maxdmg` = 53, `attackpower` = 86, `baseattacktime` = 2000 WHERE `entry` = 34492 AND `mindmg` = 0;

-- Quest drops that the 4.3.4 data does not have: Natural Oil (Needs a Little Lubrication, Rotting Slime), Venison
-- Steak (Before You Go..., Wild Buck), Furbolg Ear (Thinning the... Herd?), Gorgannon's Flaming Blade and the
-- Seeker's Fel Spear (Never Again!), Kaldorei Assassin's Head (Security!). Chances are the 5.4.8 ones.
DELETE FROM `creature_loot_template` WHERE (`entry`, `item`) IN ((3928, 45071), (3816, 46392), (3743, 46741), (3745, 46741), (3750, 46741), (6072, 45476), (17300, 45477), (34206, 46147));
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(3928, 45071, -32.5354, 1, 0, 1, 1), -- Natural Oil
(3816, 46392, -30.9739, 1, 0, 1, 1), -- Venison Steak
(3743, 46741, -89.65, 1, 0, 1, 1), -- Furbolg Ear
(3745, 46741, -89.212, 1, 0, 1, 1), -- Furbolg Ear
(3750, 46741, -89.5858, 1, 0, 1, 1), -- Furbolg Ear
(6072, 45476, -100, 1, 0, 1, 1), -- Seeker's Fel Spear
(17300, 45477, -100, 1, 0, 1, 1), -- Gorgannon's Flaming Blade
(34206, 46147, -100, 1, 0, 1, 1); -- Kaldorei Assassin's Head

-- Demon Duty needs 12 Ritual Gems but none were spawned. Ten of the 5.4.8 spawns are used (they respawn after 5
-- minutes).
DELETE FROM `gameobject` WHERE `guid` BETWEEN 930010 AND 930019;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(930010, 194612, 1, 1, 0, 0, 2907.04, -2777.71, 213.628, 0, 0, 0, 0, 1, 300, 255, 1),
(930011, 194612, 1, 1, 0, 0, 2937.22, -2809.41, 213.062, 0, 0, 0, 0, 1, 300, 255, 1),
(930012, 194612, 1, 1, 0, 0, 2861.03, -2888.64, 204.255, 0, 0, 0, 0, 1, 300, 255, 1),
(930013, 194612, 1, 1, 0, 0, 2852.97, -2826.84, 200.121, 0, 0, 0, 0, 1, 300, 255, 1),
(930014, 194612, 1, 1, 0, 0, 2729.7, -2979.86, 141.134, 0, 0, 0, 0, 1, 300, 255, 1),
(930015, 194612, 1, 1, 0, 0, 2896.68, -2828.13, 208.82, 0, 0, 0, 0, 1, 300, 255, 1),
(930016, 194612, 1, 1, 0, 0, 2885.29, -2856.79, 204.18, 0, 0, 0, 0, 1, 300, 255, 1),
(930017, 194612, 1, 1, 0, 0, 2739.5, -3077.67, 157.817, 0, 0, 0, 0, 1, 300, 255, 1),
(930018, 194612, 1, 1, 0, 0, 2701.5, -2963.55, 140.799, 0, 0, 0, 0, 1, 300, 255, 1),
(930019, 194612, 1, 1, 0, 0, 2770.35, -2970.1, 145.768, 0, 0, 0, 0, 1, 300, 255, 1);

-- Scripts. Gorka (Crisis at Splintertree) gives the credit for talking to him through his gossip option. The Forest
-- Heart Bunny (Pierce Their Heart!) and the Broken-down Wagon Bunny (Small Hands, Short Fuse) give the credit when the
-- quest item's spell hits them; the wagon also casts spell 65550 on itself. The gossip menu, the option and the spells exist.
UPDATE `creature_template` SET `AIName` = 'SmartAI', `npcflag` = 3, `gossip_menu_id` = 10342 WHERE `entry` = 33421;
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (33847, 34424);

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (33421, 33847, 34424);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(33421, 0, 0, 1, 62, 0, 100, 0, 10342, 0, 0, 0, 0, 33, 33507, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Gorka - On Gossip Select - Quest Credit'),
(33421, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Gorka - Linked To Id 0 - Close Gossip'),
(33847, 0, 0, 0, 8, 0, 100, 0, 63797, 0, 0, 0, 0, 33, 33847, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Forest Heart Bunny (Horde) - On Spellhit - Quest Credit'),
(34424, 0, 0, 1, 8, 0, 100, 0, 65433, 0, 0, 0, 0, 33, 34424, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Broken-down Wagon Bunny - On Spellhit - Quest Credit'),
(34424, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 65550, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Broken-down Wagon Bunny - Linked To Id 0 - Cast Spell 65550');
