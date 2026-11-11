-- Quest race masks, Kalimdor: quests whose race mask is narrower than in the 5.4.8 data
-- (mostly missing the Worgen, Goblin and Pandaren races). A row is only changed from the mask it has now.
-- Alliance is 18875469 and Horde is 33555378. Based on Legends of Azeroth 5.4.8 (AllowableRaces).

-- Ashenvale
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    247, 908, 909, 1918, 6383, 6504, 6543, 6545, 6547, 6571, 6581, 9534, 9535, 9536
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1024 AND `Id` IN (
    9432
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    976, 990, 991, 1007, 1008, 1009, 1010, 1011, 1012, 1016, 1017, 1020, 1021, 1022, 1023, 1024, 1025, 1026, 1027,
    1028, 1029, 1030, 1031, 1032, 1033, 1034, 1035, 1037, 1038, 1039, 1040, 1041, 1042, 1043, 1044, 1045, 1046, 1054,
    1055, 1056, 1070, 1085, 1134, 4581, 9516, 9517, 9518, 9519, 9520, 9521, 9522, 9526, 9533, 10752
);

-- Azshara
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    3562
);

-- Darkshore
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    1133, 4722, 4723, 4725, 4727, 4728, 4730, 4731, 4732, 4733, 4811, 4812
);

-- Darnassus
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    1047, 2519, 2520, 3763, 3764, 3781, 3785, 3803, 4510, 5091, 6761, 6762, 7799, 7800, 7801, 10352, 10354
);

-- Desolace
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    1361, 1362, 1365, 1366, 1367, 1368, 1480, 1481, 1482, 1484, 1488, 5381, 5386, 5421, 5581, 5763, 6142, 6143, 6568
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    261, 1052, 1382, 1384, 1385, 1387, 1437, 1438, 1439, 1440, 1454, 1455, 1456, 1457, 1458, 1459, 1465, 1466, 1467,
    6141
);

-- Durotar
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 130 AND `Id` IN (
    5843
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    784, 786, 788, 789, 791, 792, 794, 805, 806, 808, 809, 812, 813, 815, 816, 817, 818, 823, 825, 826, 827, 828, 829,
    830, 831, 832, 837, 840, 842, 924, 926, 2161, 4641
);

-- Dustwallow Marsh
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    1166, 1168, 1169, 1170, 1171, 1172, 1173, 1201, 1202, 1205, 1238, 1239, 1240, 1251, 1261, 1262, 1268, 1269, 1273,
    1276, 1321, 1322, 1323, 6570, 6582, 6583, 6584, 6585, 6601, 7541, 9437, 11124, 11186, 11200, 11201, 11203, 11204,
    11205, 11206, 11215, 11225
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    1135, 1204, 1219, 1220, 1222, 1252, 1253, 1258, 1259, 1260, 1271, 1282, 1284, 1285, 1286, 1287, 1288, 1289, 1319,
    1320, 11123, 11126, 11128, 11133, 11134, 11143, 11144, 11145, 11146, 11147, 11148, 11149, 11150, 11151, 11152,
    11185
);

-- Felwood
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    4102, 4505, 4506, 4521, 4721, 4741, 5882, 5883, 5884, 5885, 5886, 5887, 5888, 5889, 5890, 5891, 6162, 8485
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    939, 4101, 4103, 4104, 4105, 4106, 4107, 4108, 4109, 4110, 4111, 4112, 4261, 4421, 4441, 4442, 4906, 5249, 8484
);

-- Feralas
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    2822, 2862, 2863, 2902, 2903, 2973, 2974, 2975, 2976, 2978, 2979, 2980, 2987, 3002, 3062, 3063, 3121, 3122, 3123,
    3124, 3125, 3126, 3127, 3128, 3129, 4120, 7730, 7731, 7732, 7734, 7738
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    2821, 2844, 2845, 2866, 2867, 2869, 2870, 2871, 2879, 2939, 2940, 2941, 2942, 2943, 2944, 2969, 2970, 2972, 2982,
    3130, 3445, 3788, 3791, 3792, 3841, 3842, 3843, 4124, 4125, 4127, 4129, 4130, 4131, 4135, 4265, 4266, 4267, 4281,
    4297, 4298, 7733, 7735
);

-- Mulgore
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 32 AND `Id` IN (
    754, 758, 760, 854, 5844, 6361, 6362, 6363, 6364
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    744, 747, 776, 861, 11129, 24857
);

-- Northern Barrens
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    819, 821, 822, 843, 845, 846, 848, 849, 850, 851, 852, 853, 855, 857, 860, 867, 868, 870, 873, 874, 875, 876, 877,
    878, 879, 880, 881, 882, 883, 884, 885, 886, 889, 893, 897, 898, 899, 903, 905, 906, 907, 913, 1060, 1109, 1145,
    1153, 1492, 3261, 3281, 3301, 3369, 3513, 3514, 3921, 3922, 3923, 3924, 4021, 4921, 5042, 5043, 5044, 5045, 5046,
    5052, 6382, 6541
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    1101, 3370
);

-- Orgrimmar
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    235, 936, 1018, 1146, 1433, 1434, 1435, 1436, 2981, 4511, 4941, 5726, 5727, 5729, 5730, 6566, 6567, 7490, 7491,
    7493, 7783, 7784, 7824, 7826, 7827, 7831, 7832, 7833, 7834, 7835, 7836, 7837, 8276, 8913, 8914, 8915, 8916, 8917,
    8918, 8919, 8920, 8927, 8938, 8939, 8940, 8941, 8942, 8943, 8944, 8957, 8978, 8998, 9007, 9008, 9009, 9010, 9011,
    9012, 9013, 9014, 9016, 9017, 9018, 9019, 9020, 9021, 9022, 10493, 10495, 10497, 10499, 13266
);

-- Silithus
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    8731, 8732, 8785, 8786, 8787, 9416
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    8507, 8508, 8778, 8780, 8781, 9415, 9419
);

-- Stonetalon Mountains
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    1058, 1061, 1062, 1063, 1068, 1087, 1088, 1089, 5881, 6282, 6283, 6284, 6301, 6381, 6393, 6401, 6421, 6461, 6481,
    6523, 6542, 6548, 6629
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    1057, 1059, 1071, 1072, 1073, 1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1091
);

-- Tanaris
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    32, 243, 379, 654, 841, 2936, 4509, 5802, 8923
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    162, 2991, 3022, 4508, 5801, 8922
);

-- Teldrassil
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 8 AND `Id` IN (
    5842
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    475, 930, 941, 978, 979, 2159, 2518, 3661, 5250
);

-- Thousand Needles
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    1147, 1148, 1149, 1150, 1151, 1152, 1154, 1159, 1184, 1394, 4542, 4767, 4770, 4821, 4841, 4865, 4881, 4904, 4966,
    5062, 5064, 5088, 5147, 5151, 5361, 6627, 6628, 9431, 9433, 9434
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    1100, 1179, 2769
);

-- Thunder Bluff
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    264, 742, 768, 769, 1000, 1064, 1066, 1067, 1086, 1123, 1130, 1131, 1136, 1195, 1196, 1197, 2440, 2965, 2966,
    2967, 2968, 3761, 3762, 3782, 3786, 3804, 7820, 7821, 7822, 7823, 7825
);

-- Un'Goro Crater
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    4145, 4147, 4148, 13850
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    4141, 4142, 4143, 4144
);

-- Winterspring
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    4882, 4883, 6029, 6605
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    4861, 4863, 4864, 4901, 4902, 4970, 5201, 5244, 5245, 5252, 5253, 5981, 6028, 6604
);
