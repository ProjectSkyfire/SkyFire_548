-- Quest race masks for Pandaria and the 5.x quests of older places: quests that allow every race but are limited to one
-- faction in the 5.4.8 data. Only rows still at 0 are changed. Alliance is 18875469 and Horde is 33555378.
-- Based on Legends of Azeroth 5.4.8 (AllowableRaces).

-- Bloodmyst Isle
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29681
);

-- Dalaran
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29608
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29609
);

-- Dread Wastes
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    31390
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    31391
);

-- Dun Morogh
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    31137
);

-- Durotar
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    31158, 31159, 31160, 31161, 31162, 31163
);

-- End Time
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    30095
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    30094
);

-- Gate of the Setting Sun
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32003
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    31524
);

-- Hellfire Ramparts
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29528, 29529, 29543, 29594
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29527, 29530, 29542, 29593
);

-- Isle of Thunder
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32301, 32485, 32525, 32526, 32527, 32528, 32529, 32530, 32531, 32532, 32533, 32535, 32536, 32537, 32538, 32539,
    32540, 32541, 32542, 32543, 32544, 32545, 32546, 32547, 32548, 32549, 32550, 32551, 32552, 32553, 32554, 32555,
    32556, 32557, 32559, 32560, 32567, 32568, 32571, 32572, 32573, 32574, 32575, 32576, 32577, 32578, 32579, 32580,
    32581, 32582, 32583, 32584, 32585, 32586, 32587, 32588, 32606, 32607, 32608, 32627, 32628, 32631, 32632, 32633,
    32634, 32635, 32636, 32637, 32638, 32639, 32640, 32644, 32652, 32654, 32655, 32656, 32679, 32681, 32706, 32724,
    32731, 32732, 32733
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32200, 32201, 32204, 32206, 32207, 32208, 32209, 32212, 32213, 32215, 32216, 32217, 32218, 32219, 32220, 32224,
    32225, 32226, 32227, 32228, 32230, 32232, 32233, 32234, 32252, 32254, 32255, 32262, 32264, 32265, 32266, 32268,
    32269, 32274, 32275, 32276, 32277, 32278, 32279, 32282, 32283, 32285, 32287, 32288, 32292, 32293, 32294, 32297,
    32298, 32299, 32300, 32302, 32303, 32304, 32305, 32489, 32491, 32493, 32494, 32495, 32506, 32507, 32517, 32520,
    32521, 32522, 32523, 32524, 32561, 32562, 32563, 32564, 32565, 32605, 32641, 32676, 32677, 32678, 32680, 32709,
    32722, 32728, 32729, 32730
);

-- Kun-Lai Summit
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    30506, 30507, 30508, 30512, 30514, 30569, 30575, 30583, 30593, 30619, 30650, 30651, 30652, 30660, 30662, 31252,
    31380, 31392, 31394, 31452, 31455, 31456, 31460, 31512
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    30509, 30510, 30511, 30513, 30515, 30570, 30594, 30620, 30655, 30656, 30657, 30661, 30663, 31251, 31253, 31256,
    31381, 31393, 31395, 31451, 31453, 31457, 31459, 31511
);

-- Mana-Tombs
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29583
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29582
);

-- Mogu'shan Palace
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32002
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    31523
);

-- Molten Front
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29453
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29452
);

-- Orgrimmar
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29401
);

-- Ragefire Chasm
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    30995, 30996, 30997, 30998
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    30969, 30983, 31034, 31036, 31037
);

-- Scarlet Halls
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32006
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    31527
);

-- Scarlet Monastery
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32005
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    31526
);

-- Scholomance
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32007
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    31528
);

-- Shado-Pan Monastery
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32001
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    30757, 31522
);

-- Siege of Niuzao Temple
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32004
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    31525
);

-- Sort: Battle Pets
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    31308, 31309, 31316, 31548, 31549, 31550, 31551, 31552, 31553, 31555, 31556, 31568, 31569, 31582, 31583, 31584,
    31591, 31592, 31593, 31693, 31724, 31725, 31726, 31780, 31781, 31785, 31821, 31822, 31825, 31826, 31832, 31850,
    31851, 31852, 31889, 31910, 31911, 31912, 31913, 31914, 31917, 31919, 31927, 31930, 31966, 31975, 31976, 31981,
    31984, 31985, 32008
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    31570, 31571, 31572, 31573, 31574, 31575, 31576, 31577, 31578, 31579, 31580, 31581, 31585, 31586, 31587, 31588,
    31589, 31590, 31812, 31818, 31819, 31823, 31824, 31827, 31828, 31830, 31831, 31854, 31862, 31871, 31872, 31891,
    31903, 31904, 31905, 31906, 31907, 31908, 31918, 31921, 31929, 31952, 31967, 31977, 31980, 31982, 31983, 31986,
    32009
);

-- Sort: Elemental Bonds
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29439
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29440
);

-- Sort: Hallow's End
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32033, 32042, 32049, 32052
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29400, 32020, 32022, 32028, 32040, 32047, 32050
);

-- Sort: Landfall
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32115, 32116, 32119, 32121, 32122, 32142, 32143, 32144, 32145, 32146, 32148, 32149, 32150, 32151, 32152, 32153,
    32154, 32156, 32157, 32158, 32164, 32165, 32166, 32170, 32171, 32172, 32184, 32185, 32186, 32346, 32347, 32446,
    32451, 32452
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32118, 32120, 32123, 32126, 32127, 32128, 32130, 32131, 32132, 32133, 32134, 32135, 32136, 32137, 32138, 32139,
    32140, 32141, 32161, 32162, 32163, 32167, 32168, 32169, 32181, 32182, 32183, 32197, 32199, 32214, 32221, 32222,
    32223, 32235, 32236, 32237, 32238, 32342, 32343, 32344, 32345, 32348, 32449, 32450
);

-- Sort: Legendary
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32373, 32388, 32389, 32390, 32474
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32427, 32430, 32431, 32432, 32476
);

-- Sort: Monk
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    31142, 31173
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    31146, 31147
);

-- Sort: Pandaren Campaign
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32109, 32193, 32194, 32243, 32246, 32247, 32248, 32315, 32316, 32321, 32331, 32332, 32333, 32334, 32335, 32336,
    32337, 32338, 32355, 32370, 32371, 32377, 32380, 32381, 32382, 32383, 32393, 32394, 32397, 32400, 32401, 32414,
    32416, 32423, 32426, 32455, 32460
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32108, 32190, 32191, 32192, 32242, 32244, 32249, 32250, 32251, 32256, 32257, 32284, 32318, 32319, 32320, 32326,
    32327, 32328, 32329, 32330, 32351, 32352, 32363, 32368, 32372, 32376, 32378, 32379, 32384, 32391, 32392, 32398,
    32399, 32402, 32403, 32404, 32405, 32406, 32408, 32409, 32410, 32412, 32413, 32448
);

-- Sort: Proving Grounds
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    33248
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    33249
);

-- Sort: Riding
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32618, 32661, 32662, 32663, 32664, 32665, 32675
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32667, 32668, 32669, 32670, 32671, 32672, 32673, 32674
);

-- Sort: Scenario
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32900
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32901
);

-- Sort: Seasonal
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32496, 32510
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32503, 32509
);

-- Sort: Warlock
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32310
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32309
);

-- Stormstout Brewery
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32000
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    31519
);

-- Stormwind City
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29412
);

-- Temple of the Jade Serpent
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    31998
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    31520
);

-- The Blood Furnace
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29538, 29539, 29540
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29535, 29536, 29537
);

-- The Jade Forest
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29552, 29553, 29555, 29556, 29558, 29559, 29560, 29562, 29725, 29726, 29727, 29733, 29759, 29762, 29883, 29885,
    29887, 29888, 29889, 29890, 29891, 29892, 29893, 29894, 29898, 29899, 29900, 29901, 29903, 29904, 29905, 29906,
    29922, 30069, 30070, 30498, 30565, 30568, 31130, 31319, 31362, 31373, 31376, 31734, 31735, 31736, 31737, 31738,
    31739, 31740, 31741, 31742, 31743, 31744, 31745
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29694, 29730, 29731, 29743, 29765, 29804, 29815, 29821, 29822, 29823, 29824, 29827, 29879, 29924, 29933, 29935,
    29936, 29937, 29939, 29941, 29942, 29943, 29966, 29967, 29968, 29971, 30015, 30466, 30484, 30485, 30499, 30504,
    31112, 31121, 31132, 31134, 31152, 31167, 31239, 31241, 31261, 31375, 31377, 31768, 31770, 31771, 31772, 31773,
    31774, 31775, 31776, 31777, 31778, 31779, 31978, 31999
);

-- The Shattered Halls
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29649, 29652, 29654, 29656, 29689
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29650, 29651, 29653, 29655, 29688
);

-- The Veiled Stair
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    31254
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    31255
);

-- Throne of Thunder
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32599
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    32589
);

-- Timeless Isle
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    33160, 33231
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    33156, 33232
);

-- Townlong Steppes
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    31382, 31386, 31387
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    31383, 31388, 31389
);

-- Utgarde Keep
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29763, 29764, 29803
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    30112
);

-- Vale of Eternal Blossoms
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    30631, 31367, 31384, 32683, 33229
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    31368, 31385, 32726, 33230
);

-- Valley of the Four Winds
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    30360, 30624, 31372, 32645, 32646, 32944, 32945
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    30241, 30653, 31374, 32642, 32643, 32942, 32943
);

-- Zul'Gurub
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    29153, 29154
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    8189, 29251, 29252
);
