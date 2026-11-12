-- Quest race masks, instances, battlegrounds, other maps, quest categories and quests without a zone: quests whose race mask is narrower than in the 5.4.8 data
-- (mostly missing the Worgen, Goblin and Pandaren races). A row is only changed from the mask it has now.
-- Alliance is 18875469 and Horde is 33555378. Based on Legends of Azeroth 5.4.8 (AllowableRaces).

-- (no zone)
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    3482
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    1782, 8404, 8405, 8406, 8408
);

-- Alterac Valley
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    5893, 6741, 6801, 6825, 6826, 6827, 6847, 6861, 6901, 6985, 7001, 7002, 7082, 7101, 7123, 7124, 7142, 7163, 7164,
    7166, 7167, 7181, 7224, 7241, 7281, 7302, 7361, 7362, 7363, 7368, 7381, 7385, 7401, 7421, 7422, 7423, 8369, 11340,
    13428
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    5892, 6781, 6846, 6848, 6862, 6881, 6941, 6942, 6943, 6982, 7026, 7027, 7081, 7102, 7121, 7122, 7141, 7162, 7168,
    7169, 7170, 7171, 7172, 7202, 7223, 7261, 7282, 7301, 7364, 7365, 7366, 7367, 7382, 7386, 7424, 7425, 11336, 13427
);

-- Arathi Basin
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    8120, 8121, 8122, 8123, 8124, 8160, 8161, 8162, 8163, 8164, 8165, 8169, 8170, 8171, 8263, 8264, 8265, 8299, 8300,
    8370, 8390, 8436, 8437, 8439, 8440, 8441, 8442, 8443, 11339
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    8080, 8081, 8105, 8114, 8115, 8154, 8155, 8156, 8157, 8158, 8159, 8166, 8167, 8168, 8260, 8261, 8262, 8297, 8298,
    8374, 8391, 8392, 8393, 8394, 8395, 8396, 8397, 8398, 11335
);

-- Auchenai Crypts
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    10167
);

-- Blackfathom Deeps
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    6561, 6562, 6563, 6564, 6565, 6921, 6922
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    971, 1198, 1199, 1200, 1275
);

-- Blackrock Depths
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    3907, 3981, 3982, 4001, 4002, 4003, 4004, 4063, 4081, 4082, 4121, 4132, 7201
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    4126, 4128, 4241, 4242, 4263, 4264, 4282, 4286, 4322, 4341, 4342, 4361, 4362, 4363
);

-- Blackrock Spire
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    4724, 4903, 4974, 4982, 6569, 6602
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    4764, 4765, 4766, 5001, 5081, 5089, 5102, 6502
);

-- Deeprun Tram
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    6661, 6662
);

-- Dire Maul
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    7481, 7489, 7492, 7877
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    7462, 7482, 7488, 7494
);

-- Eye of the Storm
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    11341
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    11337
);

-- Halls of Reflection
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    24561, 24713, 24802
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    24480, 24500, 24711
);

-- Hellfire Ramparts
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    9496, 9572, 9590, 9608
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    9493, 9575, 9589, 9607
);

-- Maraudon
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    7029, 7064, 7068
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    7041, 7065, 7070
);

-- Pit of Saron
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    24507, 24559, 24682, 24712
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    24461, 24498, 24683, 24710
);

-- Ragefire Chasm
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    5722, 5723, 5724, 5725, 5728, 14356
);

-- Razorfen Downs
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    3341, 6521, 6522, 14352, 14353
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    3636
);

-- Scarlet Monastery
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 674 AND `Id` IN (
    1049
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    1048, 1051, 1113, 1160, 14355
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    1050, 1053
);

-- ScholomanceOLD
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    5341
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    5343
);

-- Shadowfang Keep
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    1013
);

-- Sort: Ahn'Qiraj War
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    8792, 8793, 8794, 8851, 8852, 8853, 8854, 8855, 10500
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    8500, 8504, 8506, 8510, 8527, 8846, 8847, 8848, 8849, 8850, 10501
);

-- Sort: Alchemy
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    1581
);

-- Sort: Battlegrounds
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    8367, 8388, 13476
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    8371, 8385, 13478
);

-- Sort: Blacksmithing
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    2751, 2752, 2753, 2754, 2755, 2756, 2757, 5301, 5302, 10892
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    1578, 1618, 2758, 2759, 5283, 5284, 10891
);

-- Sort: Brewfest
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    11120, 11407, 11408, 11409, 11412, 11413, 11419, 11431, 11446, 11447, 11487, 12191, 12192, 12194, 12306, 12421,
    12492
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    11117, 11122, 11293, 11294, 11318, 11321, 11400, 11441, 11442, 11486, 12020, 12022, 12193, 12278, 12420, 12491
);

-- Sort: Children's Week
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    172, 5502, 10942, 10963, 11975
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    171, 1468, 10943, 10956, 10962, 10968
);

-- Sort: Cooking
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    862, 6611, 7321, 13089, 13090, 13112, 13113, 13114, 13115, 13116
);
UPDATE `quest_template` SET `RequiredRaces` = 33555250 WHERE `RequiredRaces` = 818 AND `Id` IN (
    26233
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 946 AND `Id` IN (
    26220, 26226, 26227, 26235, 29315, 29332, 29333, 29334, 29358, 29360, 29362, 29363, 29364, 29365
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    6612, 13087, 13088, 13100, 13101, 13102, 13103, 13107
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 2098253 AND `Id` IN (
    26153, 26177, 26183, 26190, 26192, 29313, 29314, 29316, 29318, 29351, 29352, 29353, 29355, 29356, 29357
);

-- Sort: Death Knight
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    13189
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    13188
);

-- Sort: Druid
UPDATE `quest_template` SET `RequiredRaces` = 1101 WHERE `RequiredRaces` = 8 AND `Id` IN (
    26
);
UPDATE `quest_template` SET `RequiredRaces` = 2097160 WHERE `RequiredRaces` = 8 AND `Id` IN (
    3120
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 8 AND `Id` IN (
    29, 272, 5061, 5921, 5924, 5925, 5929, 5931, 6001
);
UPDATE `quest_template` SET `RequiredRaces` = 160 WHERE `RequiredRaces` = 32 AND `Id` IN (
    3094
);
UPDATE `quest_template` SET `RequiredRaces` = 690 WHERE `RequiredRaces` = 32 AND `Id` IN (
    27
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 32 AND `Id` IN (
    28, 30, 31, 5922, 5927, 5928, 5930, 5932, 6002
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    6126, 6127, 6128, 6129, 6130
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    6121, 6122, 6123, 6124, 6125
);

-- Sort: Engineering
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    3526, 3635, 3637
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    3629, 3630, 3632, 3634, 3640, 3641, 4181, 9636
);

-- Sort: First Aid
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    6622, 6623
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    6624, 6625
);

-- Sort: Fishing
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    6608
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 946 AND `Id` IN (
    26543, 26556, 26557, 26572, 26588, 29317, 29319, 29320, 29322, 29345, 29346, 29348, 29349, 29354, 29361
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    1579, 1580, 6609
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 2098253 AND `Id` IN (
    26414, 26420, 26442, 26488, 26536, 29321, 29323, 29324, 29325, 29342, 29343, 29344, 29347, 29350, 29359
);

-- Sort: Hallow's End
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    11219, 12139, 12155
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    11131, 12133, 12135
);

-- Sort: Hunter
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 2 AND `Id` IN (
    3087
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 4 AND `Id` IN (
    3108
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 128 AND `Id` IN (
    3082
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 512 AND `Id` IN (
    9617
);

-- Sort: Invasion
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    9265, 9310, 12816
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    9260, 9292, 12817
);

-- Sort: Jewelcrafting
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    12952, 13002
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 946 AND `Id` IN (
    25158, 25159, 25160, 25161, 25162
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    12918, 13004
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 2098253 AND `Id` IN (
    25105, 25154, 25155, 25156, 25157
);

-- Sort: Leatherworking
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    2854, 2855, 2856, 2857, 2858, 2859, 2860, 5145, 5146, 5148
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    1582, 2847, 2848, 2849, 2850, 2851, 2852, 2853, 5141, 5143, 5144
);

-- Sort: Mage
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1 AND `Id` IN (
    1861
);
UPDATE `quest_template` SET `RequiredRaces` = 1032 WHERE `RequiredRaces` = 8 AND `Id` IN (
    26841
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 64 AND `Id` IN (
    3114
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 128 AND `Id` IN (
    3086
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    1881, 1882, 1883, 1884, 1943, 1944, 1945, 1946, 1959, 1960, 1961, 1962, 9402, 9403, 9404, 12173
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1024 AND `Id` IN (
    9290
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    1860, 1879, 1880, 1919, 1920, 1921, 1938, 1939, 1940, 1941, 1942, 9595, 12172
);

-- Sort: Midsummer
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    9330, 9331, 9332, 9339, 9368, 11915, 11922, 11933, 11966, 11971
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    9324, 9325, 9326, 9365, 9367, 11731, 11882, 11935, 11964, 11970
);

-- Sort: Noblegarden
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    13479, 13483, 13503
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    13480, 13484, 13502
);

-- Sort: Paladin
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 4 AND `Id` IN (
    3107
);
UPDATE `quest_template` SET `RequiredRaces` = 544 WHERE `RequiredRaces` = 32 AND `Id` IN (
    27015, 27023
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1024 AND `Id` IN (
    9287
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1029 AND `Id` IN (
    7637, 7638, 7639, 7640, 7641, 7642, 7643, 7644, 7645, 7646, 7647, 7670
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    1442, 1649, 1650, 1651, 1652, 1653, 1654, 1655, 1661, 1793, 1794, 1806, 4485, 4486, 7648, 7666, 8414, 8415, 8416,
    8418
);

-- Sort: Pilgrim's Bounty
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    14037, 14040, 14041, 14043, 14044, 14047, 14058, 14059, 14060, 14061, 14062, 14065
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    14023, 14024, 14028, 14030, 14033, 14035, 14048, 14051, 14053, 14054, 14055, 14064
);

-- Sort: Priest
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 4 AND `Id` IN (
    3110
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 8 AND `Id` IN (
    5621, 5622
);
UPDATE `quest_template` SET `RequiredRaces` = 160 WHERE `RequiredRaces` = 32 AND `Id` IN (
    27066
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 128 AND `Id` IN (
    3085
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    5648, 5650, 5651, 9489
);

-- Sort: Reputation
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    8815, 8816, 8817, 8818, 8823, 8824, 8825, 8826, 8832, 8833, 8840, 8841, 8842, 8843, 8844, 8845, 10693, 10694,
    10697, 10698
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    8811, 8812, 8813, 8814, 8819, 8820, 8821, 8822, 8830, 8831, 8834, 8835, 8836, 8837, 8838, 8839, 10695, 10696,
    10699, 10700
);

-- Sort: Rogue
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 2 AND `Id` IN (
    3088
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    1858, 1859, 1885, 1886, 1898, 1899, 1963, 1978, 1998, 1999, 2378, 2379, 2380, 2381, 2382, 2458, 2460, 2478, 2479,
    2480, 9491, 10372, 10548, 10794, 14418, 14419, 14420, 14421
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    2206, 2241, 2242, 2259, 2260, 2281, 2282, 2300, 2359, 2360, 2607, 2608, 2609
);

-- Sort: Seasonal
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    6961, 6962, 6964, 7021, 7024, 7061, 8322, 8409, 8746, 8799, 8828, 8861, 8900, 8901, 8902, 8979, 8980, 8982, 8983,
    8984, 11558, 11732, 11734, 11735, 11736, 11737, 11738, 11739, 11740, 11741, 11742, 11743, 11744, 11745, 11746,
    11747, 11748, 11749, 11750, 11751, 11752, 11753, 11754, 11755, 11756, 11757, 11758, 11759, 11760, 11761, 11762,
    11763, 11836, 11838, 11843, 11856, 13440, 13442, 13443, 13444, 13445, 13446, 13447, 13449
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    1658, 7022, 7023, 7025, 7042, 7043, 7045, 7062, 7063, 8762, 8763, 8827, 8860, 8897, 8898, 8899, 8903, 9024, 9025,
    9026, 9027, 9028, 11764, 11765, 11766, 11767, 11768, 11769, 11770, 11771, 11772, 11773, 11774, 11775, 11776,
    11777, 11778, 11779, 11780, 11781, 11782, 11783, 11784, 11785, 11786, 11787, 11799, 11800, 11801, 11802, 11803,
    11807, 13441, 13450, 13451, 13453, 13454, 13455, 13457, 13458
);

-- Sort: Shaman
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 2 AND `Id` IN (
    3089
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    63, 96, 100, 220, 1103, 1462, 1463, 1464, 1523, 1524, 1525, 1526, 1527, 1528, 1529, 1530, 1531, 1532, 1534, 1535,
    1536, 2983, 2984, 7667, 8258
);
UPDATE `quest_template` SET `RequiredRaces` = 1101 WHERE `RequiredRaces` = 1024 AND `Id` IN (
    9508
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1024 AND `Id` IN (
    9500, 9501, 9503, 9504, 9509, 9547, 9551, 9552, 9553, 9554, 10490, 10491
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    9421, 9449, 9450, 9451, 9462, 9464, 9465, 9467, 9468, 9555
);

-- Sort: Special
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1 AND `Id` IN (
    12774
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 8 AND `Id` IN (
    12773
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 16 AND `Id` IN (
    12787
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 32 AND `Id` IN (
    12786
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 512 AND `Id` IN (
    12788
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    910, 911, 925, 1800
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1024 AND `Id` IN (
    12776
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    1479, 1558, 1687, 4822
);

-- Sort: Tournament
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1 AND `Id` IN (
    13684
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 2 AND `Id` IN (
    13691
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 4 AND `Id` IN (
    13685
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 16 AND `Id` IN (
    13695
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 77 AND `Id` IN (
    13705
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 178 AND `Id` IN (
    13711
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 512 AND `Id` IN (
    13696
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 562 AND `Id` IN (
    13708
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 658 AND `Id` IN (
    13709
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 674 AND `Id` IN (
    13710
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 688 AND `Id` IN (
    13707
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    13634, 13668, 13673, 13674, 13675, 13676, 13677, 13678, 13680, 13809, 13810, 13811, 13812, 13813, 13814, 13829,
    13838, 13839, 13862, 13863
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1024 AND `Id` IN (
    13690
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1037 AND `Id` IN (
    13704
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1093 AND `Id` IN (
    13706
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1097 AND `Id` IN (
    13703
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1100 AND `Id` IN (
    13593
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    13625, 13633, 13666, 13667, 13669, 13670, 13671, 13672, 13679, 13682, 13788, 13789, 13790, 13791, 13793, 13828,
    13835, 13837, 13861, 13864
);

-- Sort: Warlock
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 512 AND `Id` IN (
    9529, 9619
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    1474, 1476, 1515, 1801, 1803, 1805, 2996, 4737, 4739, 4969
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    1688, 1689, 1716, 1717, 1738, 1739, 1758, 1798, 1802, 1804, 4736, 4738, 4965
);

-- Sort: Warrior
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 2 AND `Id` IN (
    2383
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 128 AND `Id` IN (
    3065
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    1502, 1503, 1820, 1821, 1822, 1823, 1824, 1825, 1838, 1839, 1840, 1842, 1843, 1844, 1845, 1846, 1847, 1848
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    1638, 1639, 1640, 1665, 1666, 1667, 1681, 1682, 1684, 1686, 1692, 1693, 1698, 1699, 1700, 1701, 1702, 1703, 1704,
    1705, 1706, 1708, 1709, 1710, 1711, 9582, 10350
);

-- Strand of the Ancients
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    13407
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    13405
);

-- Stratholme
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    6163
);

-- Sunken Temple
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    3380, 4146
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    1475
);

-- Sunwell Plateau
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    24553, 24595
);

-- The Deadmines
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    166, 167, 168, 214, 2040
);

-- The Forge of Souls
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    24506, 24511
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    24499, 24510
);

-- The Nexus
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    13095
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    13094
);

-- The Stockade
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    377, 378, 386, 387, 388, 391
);

-- Uldaman
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    2280
);

-- Utgarde Keep
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    11262, 11272, 13206
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    11252, 13205
);

-- Wailing Caverns
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    914, 962, 1489, 1490
);

-- Warsong Gulch
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    7789, 7874, 7875, 7876, 7922, 7923, 7924, 7925, 8293, 8294, 8368, 8389, 8426, 8427, 8428, 8429, 8430, 8431, 8432,
    8433, 8434, 8435, 11342
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    7788, 7871, 7872, 7873, 7886, 7887, 7888, 7921, 8291, 8292, 8372, 8386, 8399, 8400, 8401, 8402, 8403, 8407, 11338
);
