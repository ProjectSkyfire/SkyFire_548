-- Quest race masks, instances, battlegrounds, other maps, quest categories and quests without a zone: quests that allow every race but are limited to one faction in the 5.4.8 data.
-- Alliance is 18875469 and Horde is 33555378. Only rows still at 0 are changed.
-- Based on Legends of Azeroth 5.4.8 (AllowableRaces).

-- (no zone)
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    7671, 7672, 7673, 7674, 7675, 7676, 7677, 7678, 8229, 14467, 25055, 25567, 25568, 25569, 25570, 25572, 25605,
    25606, 27137
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    7660, 7661, 7662, 7663, 7664, 7665, 8228, 8270, 8274, 10559, 25092, 25781, 25782, 25783, 25784, 25785, 25786,
    25787, 27138
);

-- Alterac Valley
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    7221, 7402, 7426, 7428, 8271, 24427
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    7161, 7165, 7222, 7427, 8272, 24426
);

-- Arathi Basin
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    14178, 24220, 24223, 28573
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    14181, 24221, 24226
);

-- Blackfathom Deeps
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    26881, 26882, 26883, 26884, 26885, 26897, 26898
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    26888, 26891, 26892, 26894, 26899
);

-- Blackrock Depths
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    27565, 27567, 27568, 27569, 27571, 27573, 27578, 28068, 28328, 28393, 28394, 28398, 28399, 28406
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    3802, 3911, 4122, 4123, 4133, 27579, 27581, 27582, 27585, 27589, 27591, 27593, 28069, 28457, 28458, 28459, 28461,
    28463, 28465, 28476, 28478
);

-- Blackrock Spire
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    4701, 4867, 5002, 27471
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    4768, 4769, 4788, 4981, 4983
);

-- Deepholm
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    27004, 27006, 27040, 27042, 27058, 27952
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    27005, 27008, 27041, 27043, 27059, 27953
);

-- Dire Maul
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    7483, 7484, 7485, 27124, 27125, 27129, 27131, 27133
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    7441, 7461, 7502, 27128, 27130, 27132, 27134
);

-- Eye of the Storm
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    14179
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    14182
);

-- Gnomeregan
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    4601, 4602, 26939, 26941, 26942, 26943, 26944
);

-- Hellfire Ramparts
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    9492, 9494
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    9495, 9497, 9525
);

-- Icecrown Citadel
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    24871, 24876
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    24870, 24877
);

-- Isle of Conquest
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    14163
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    14164
);

-- Kezan
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    24503, 28607
);

-- Maraudon
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    7067
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    7028
);

-- Naxxramas
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    9033
);

-- Ragefire Chasm
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    5761, 26865, 26866, 26867
);

-- Razorfen Downs
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    27063
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    3523, 3525, 27062
);

-- Scarlet Monastery
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    26972, 26973, 26974, 26976, 26982, 26983, 26984, 26985
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    26950, 26959, 26962, 26967, 26986, 26987, 26988, 26991
);

-- ScholomanceOLD
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    5382, 5515
);

-- Shadowfang Keep
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    27917, 27920, 27921, 27968
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    1014, 1098, 27974, 27988, 27996, 27998
);

-- Sort: Ahn'Qiraj War
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    8492, 8493, 8494, 8495, 8499, 8503, 8505, 8509, 8511, 8512, 8513, 8514, 8515, 8516, 8517, 8518, 8520, 8521, 8522,
    8523, 8524, 8525, 8526, 8528, 8529, 8530, 8531, 8795, 8796, 8797
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    8532, 8533, 8542, 8543, 8545, 8546, 8549, 8550, 8580, 8581, 8582, 8583, 8588, 8589, 8590, 8591, 8600, 8601, 8604,
    8605, 8607, 8608, 8609, 8610, 8611, 8612, 8613, 8614, 8615, 8616, 8617, 8618
);

-- Sort: Battlegrounds
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    13477
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    13475
);

-- Sort: Blacksmithing
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    2760, 7650, 7652, 7653, 7654, 7655, 7656, 7657, 7658, 7659
);

-- Sort: Brewfest
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    13932, 29396
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    13931
);

-- Sort: Cataclysm
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    25180, 25253, 25282, 25290, 25414, 25415, 25416, 25417, 25418, 25425, 25604, 25773, 26054, 27122, 27207, 27307,
    27473, 27540, 27546, 27566, 27741, 27749, 27767, 27780
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    25181, 25228, 25288, 25293, 25343, 25347, 25348, 25351, 25380, 25571, 25789, 26990, 27127, 27209, 27308, 27570,
    27572, 27799, 27800, 27801, 27802
);

-- Sort: Children's Week
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    10950, 10952, 10954, 10966, 29093, 29106, 29107, 29117, 29119
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    10945, 10951, 10953, 10960, 10967, 29146, 29167, 29176, 29190, 29191
);

-- Sort: Cooking
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    2178, 4161
);

-- Sort: Darkmoon Faire
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    7884, 7940
);

-- Sort: Engineering
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    3642, 3647
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    9635
);

-- Sort: Firelands Invasion
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29387, 29391
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29388, 29390
);

-- Sort: Fishing
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    11531
);

-- Sort: Hallow's End
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    8311, 8353, 8355, 8356, 8357, 11135, 11242, 11356, 11404, 12286, 12331, 12332, 12333, 12334, 12335, 12336, 12337,
    12338, 12339, 12340, 12341, 12342, 12343, 12344, 12345, 12347, 12348, 12349, 12351, 12352, 12353, 12354, 12355,
    12356, 12357, 12358, 12359, 12360, 12944, 12945, 13433, 13434, 13435, 13436, 13437, 13438, 13439, 13448, 13473,
    29074
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    8312, 8354, 8358, 8359, 8360, 11220, 11357, 11361, 11403, 11405, 12361, 12362, 12363, 12364, 12365, 12366, 12367,
    12368, 12369, 12370, 12371, 12373, 12374, 12375, 12376, 12377, 12378, 12379, 12380, 12381, 12382, 12383, 12384,
    12385, 12386, 12387, 12388, 12389, 12390, 12391, 12392, 12393, 12394, 12395, 12946, 12947, 13464, 13465, 13466,
    13467, 13468, 13469, 13470, 13471, 13474, 13501, 13548
);

-- Sort: Herbalism
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    866
);

-- Sort: Hunter
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    9288, 27266, 27344
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    27278, 27395
);

-- Sort: Invasion
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    9261, 9262
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    9263, 9264
);

-- Sort: Love is in the Air
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    24597, 24609, 24610, 24611, 24655, 24656, 24657, 24658, 24659, 24660, 24662, 24663, 24664, 24665, 24666, 24745,
    24792, 24804, 24848, 24849, 28934
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    14483, 24536, 24541, 24576, 24612, 24613, 24614, 24615, 24638, 24645, 24647, 24648, 24649, 24650, 24651, 24652,
    24793, 24805, 24850, 24851, 28935
);

-- Sort: Mage
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    27271, 27354
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    27277, 27400
);

-- Sort: Midsummer
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    9319, 11657, 11921, 11924, 29092
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    9386, 11923, 11925, 11926
);

-- Sort: Paladin
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    9679
);

-- Sort: Pilgrim's Bounty
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    14022
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    14036
);

-- Sort: Priest
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    9291, 27268, 27361
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    27331, 28164
);

-- Sort: Rogue
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    2358, 27267, 27351
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    27279, 27396
);

-- Sort: Seasonal
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    8149, 8993, 11583, 11804, 11805, 11806, 11808, 11809, 11810, 11811, 11812, 11813, 11814, 11815, 11816, 11817,
    11818, 11819, 11820, 11822, 11823, 11824, 11825, 11826, 11827, 11828, 11829, 11830, 11831, 11832, 11833, 11834,
    12346, 13485, 13486, 13487, 13488, 13489, 13490, 13491, 13492, 28911, 28914, 28917, 28918, 28919, 28920, 28922,
    28925, 28926, 28928, 28929, 28932, 28944, 28945, 28948, 28950
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    1657, 6963, 6983, 6984, 8150, 8981, 11584, 11835, 11837, 11839, 11840, 11841, 11842, 11844, 11845, 11846, 11847,
    11848, 11849, 11850, 11851, 11852, 11853, 11854, 11855, 11857, 11858, 11859, 11860, 11861, 11862, 11863, 13493,
    13494, 13495, 13496, 13497, 13498, 13499, 13500, 28910, 28912, 28913, 28915, 28916, 28921, 28923, 28924, 28927,
    28930, 28931, 28933, 28943, 28946, 28947, 28949
);

-- Sort: Shaman
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    8410, 9461, 9502, 14111
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    972, 2986, 8411, 14100, 27280, 27397
);

-- Sort: Special
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    558, 25563
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    25759
);

-- Sort: Tailoring
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    13265, 13268
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    13269, 13270
);

-- Sort: The Zandalari
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29100, 29102, 29103, 29104, 29105, 29114, 29115, 29120, 29121, 29124, 29131, 29133, 29150, 29151, 29152, 29155,
    29213, 29267
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29219, 29220, 29221, 29222, 29223, 29226, 29227, 29230, 29231, 29232, 29233, 29235, 29236, 29237, 29238, 29250,
    29253, 29268
);

-- Sort: Tournament
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    14074, 14076, 14077, 14080, 14090, 14096, 14112, 14152
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    14092, 14136, 14140, 14141, 14142, 14143, 14144, 14145
);

-- Sort: Treasure Map
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    2882
);

-- Sort: Warlock
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    27272, 27355
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    27282, 27402
);

-- Sort: Warrior
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    9289, 27225, 27337
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    27281, 27365
);

-- Stratholme
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    5213
);

-- Sunken Temple
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    1446, 3373, 8733
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    1445, 3528
);

-- The Deadmines
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    27756, 27758, 27781, 27785, 27790
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    27842, 27844, 27847, 27848, 27850
);

-- The Stockade
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    27733, 27737, 27739
);

-- Throne of the Tides
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    28834, 28836
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    28833, 28843
);

-- Tol Barad
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    28117, 28118, 28120, 28122, 28162, 28163, 28165, 28185, 28186, 28188, 28223, 28232, 28882
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    28657, 28658, 28659, 28660, 28661, 28662, 28663, 28664, 28665, 28668, 28669, 28670, 28884
);

-- Tol Barad Peninsula
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    27944, 27948, 27949, 27966, 27967, 27970, 27971, 27972, 27973, 27975, 27978, 27987, 27991, 27992, 28046, 28050,
    28059, 28063, 28065, 28130, 28137, 28275
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    28678, 28679, 28680, 28681, 28682, 28683, 28684, 28685, 28686, 28687, 28689, 28690, 28691, 28692, 28693, 28694,
    28695, 28696, 28697, 28698, 28700, 28721
);

-- Uldaman
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    27672, 27673, 27676, 27677
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    27679, 27680, 27681, 27682
);

-- Wailing Caverns
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    6981, 26878
);

-- Warsong Gulch
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    8290, 14180, 24218, 24219, 24224
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    8295, 14183, 24216, 24217, 24225
);
