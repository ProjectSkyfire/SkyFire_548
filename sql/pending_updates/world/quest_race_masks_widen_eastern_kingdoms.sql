-- Quest race masks, Eastern Kingdoms: quests whose race mask is narrower than in the 5.4.8 data
-- (mostly missing the Worgen, Goblin and Pandaren races). A row is only changed from the mask it has now.
-- Alliance is 18875469 and Horde is 33555378. Based on Legends of Azeroth 5.4.8 (AllowableRaces).

-- Arathi Highlands
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    639, 640, 641, 643, 644, 645, 646, 655, 671, 672, 673, 674, 675, 677, 678, 679, 680, 688, 701, 702, 847, 8438
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    653, 682, 684, 685, 691, 693, 694, 695, 696, 697, 8384
);

-- Badlands
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    728, 782, 793, 795, 1419, 1420, 2202, 2203, 2258, 2339, 2340, 2341, 2342, 3821, 4062, 9439
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    17, 704, 706, 708, 717, 718, 719, 720, 721, 722, 723, 724, 725, 726, 727, 732, 733, 735, 738, 739, 762, 779, 1139,
    1360, 2198, 2199, 2200, 2201, 2204, 2240, 2279, 2361, 2398
);

-- Blackrock Mountain
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    3906, 9032
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    4262, 8960
);

-- Blasted Lands
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    2621, 2784
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    2783
);

-- Burning Steppes
UPDATE `quest_template` SET `RequiredRaces` = 16777221 WHERE `RequiredRaces` = 4 AND `Id` IN (
    28202
);
UPDATE `quest_template` SET `RequiredRaces` = 33554450 WHERE `RequiredRaces` = 18 AND `Id` IN (
    28428
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    3822, 4061
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    3823, 3824, 3825, 4182, 4183, 4184, 4185, 4186, 4223, 4224, 4283
);

-- Dun Morogh
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 68 AND `Id` IN (
    6387, 6388, 6391, 6392
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    2841, 2842, 2843, 2949
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    170, 179, 182, 183, 218, 233, 234, 287, 291, 308, 310, 311, 312, 313, 314, 315, 317, 318, 319, 320, 384, 400, 412,
    413, 414, 415, 417, 419, 420, 432, 433, 466, 467, 2160, 2922, 2923, 2924, 2925, 2926, 2927, 2928, 2929, 2930,
    2931, 2947, 2962, 3361, 3364, 3365, 5541, 5841
);

-- Duskwood
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    1372, 1383, 1388, 1391
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    5, 56, 57, 58, 66, 67, 68, 69, 70, 72, 74, 75, 78, 79, 80, 90, 93, 95, 97, 98, 101, 133, 134, 148, 149, 154, 156,
    157, 158, 159, 160, 163, 164, 165, 173, 174, 175, 177, 181, 221, 222, 225, 226, 227, 229, 230, 231, 240, 245, 251,
    252, 253, 262, 265, 266, 268, 269, 270, 323, 324, 337, 401, 453, 526, 1395
);

-- Eastern Plaguelands
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    5901, 6042, 6136, 6145, 6146, 6147, 8930, 9665, 14350
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    5246, 5247, 5248, 5903, 6185, 6187, 8929, 9664
);

-- Elwynn Forest
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1 AND `Id` IN (
    5805, 28817, 28818, 28819, 28820, 28821, 28822, 28823
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    6, 7, 11, 15, 16, 18, 21, 33, 35, 37, 39, 40, 45, 46, 47, 52, 54, 59, 60, 61, 62, 71, 76, 83, 84, 85, 86, 87, 88,
    106, 107, 109, 111, 112, 114, 123, 147, 176, 239, 783, 2158, 3903, 3904, 3905, 5261, 5545, 28780, 28784, 28785,
    28786, 28788, 28789, 28791, 28792, 28793, 28794, 28796, 28797
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 2097153 AND `Id` IN (
    28787, 28795
);

-- Hillsbrad Foothills
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    494, 496, 498, 499, 501, 502, 503, 507, 509, 513, 517, 518, 519, 520, 521, 524, 527, 528, 529, 532, 533, 539, 541,
    544, 545, 546, 547, 549, 550, 552, 553, 556, 557, 566, 567, 676, 8387, 14351
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    500, 504, 505, 510, 511, 512, 514, 522, 523, 525, 536, 537, 551, 554, 555, 559, 560, 562, 563, 564, 565, 659, 661,
    8375, 8383, 9435
);

-- Ironforge
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 8 AND `Id` IN (
    6721
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    637, 683, 686, 689, 700, 707, 1019, 1453, 2039, 2041, 2439, 2946, 2948, 2963, 2964, 2977, 3201, 3371, 3448, 3449,
    3450, 3451, 3461, 3483, 3701, 3702, 3790, 4512, 4513, 5090, 7342, 7802, 7803, 7804, 7805, 7806, 7807, 7808, 7809,
    7811, 7812, 7905, 8275, 8905, 8906, 8907, 8908, 8909, 8910, 8911, 8912, 8926, 8931, 8932, 8933, 8934, 8935, 8936,
    8937, 8951, 8952, 8953, 8954, 8955, 8956, 8958, 8977, 8997, 8999, 9000, 9001, 9002, 9003, 9004, 9005, 9006, 10492,
    10494, 10496, 10498
);

-- Loch Modan
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    161, 199, 217, 224, 237, 250, 255, 256, 257, 258, 263, 267, 271, 273, 274, 278, 280, 283, 297, 298, 301, 302, 307,
    309, 385, 416, 418, 436, 454, 531, 1338, 1339, 2038, 2500, 2501, 3182
);

-- Redridge Mountains
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    19, 20, 34, 89, 91, 92, 94, 115, 116, 120, 121, 125, 127, 128, 129, 130, 131, 143, 144, 145, 146, 150, 169, 178,
    180, 219, 244, 246, 248, 249
);

-- Searing Gorge
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    3181, 3367, 3368, 3372, 3377, 3378, 3566
);

-- Silverpine Forest
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    439, 9189
);

-- Stormwind City
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    212, 332, 333, 334, 335, 336, 343, 344, 345, 346, 347, 350, 353, 373, 389, 392, 393, 394, 395, 396, 397, 399, 434,
    538, 540, 542, 543, 579, 690, 1015, 1097, 1241, 1243, 1244, 1245, 1246, 1247, 1248, 1249, 1250, 1264, 1265, 1266,
    1267, 1274, 1301, 1324, 1363, 1364, 1447, 1448, 1449, 1477, 2745, 2746, 3765, 3789, 5048, 5066, 6182, 6183, 6186,
    6402, 6403, 6501, 7495, 7496, 7497, 7781, 7782, 7791, 7793, 7794, 7795, 7796
);

-- Stranglethorn Vale
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    568, 569, 570, 571, 572, 573, 581, 582, 584, 585, 586, 588, 589, 591, 592, 593, 596, 598, 629, 638, 9436, 9457
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    198, 200, 202, 203, 204, 205, 206, 207, 210, 215, 328, 329, 330, 331, 574, 578, 601, 602, 603, 610, 611, 616, 622,
    623, 627
);

-- Swamp of Sorrows
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    698, 699, 1418, 1422, 1424, 1426, 1427, 1428, 1429, 1430, 2622, 2623, 9440
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    1396, 1398, 1421, 1423, 1425, 9609, 9610
);

-- The Hinterlands
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    1444, 2742, 2782, 2933, 2934, 2935, 2937, 2938, 2995, 7815, 7816, 7828, 7829, 7830, 7839, 7840, 7841, 7842, 7843,
    7844, 7845, 7846, 7847, 7849, 7850, 7861, 7862
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    1450, 1451, 1452, 1469, 2877, 2988, 2989, 2990, 2992, 2993, 2994, 9469, 9471, 9475, 9476
);

-- Tirisfal Glades
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 16 AND `Id` IN (
    363, 364, 367, 368, 369, 492, 3901, 5847, 6321, 6322, 6323, 6324
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    8, 354, 355, 356, 358, 359, 360, 361, 362, 365, 366, 370, 371, 372, 374, 375, 376, 380, 381, 382, 383, 398, 404,
    405, 407, 408, 409, 410, 411, 426, 427, 431, 445, 590, 3902, 5481, 5482, 5902, 6395
);

-- Undercity
UPDATE `quest_template` SET `RequiredRaces` = 33554866 WHERE `RequiredRaces` = 178 AND `Id` IN (
    9812
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    232, 238, 357, 495, 736, 864, 1004, 1164, 3568, 3569, 3784, 4293, 4294, 4561, 4642, 4661, 5049, 5961, 7813, 7814,
    7817, 7818, 7819, 13267
);

-- Western Plaguelands
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 690 AND `Id` IN (
    105, 838, 964, 4987, 5023, 5093, 5094, 5095, 5096, 5098, 5228, 5229, 5230, 5231, 5232, 5233, 5234, 5235, 5236,
    5237, 5406, 5511, 5514, 5804, 6390, 9443, 9444, 10374
);
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    211, 4986, 5022, 5092, 5097, 5215, 5216, 5217, 5219, 5220, 5222, 5223, 5225, 5226, 5238, 5344, 5404, 5505, 5533,
    5537, 5538, 5803, 5904, 6184, 6389
);

-- Westfall
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    9, 12, 13, 14, 22, 36, 38, 48, 49, 50, 51, 53, 64, 65, 102, 117, 132, 135, 141, 142, 151, 153, 155, 184, 6181,
    6261, 6281, 6285
);

-- Wetlands
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 1101 AND `Id` IN (
    275, 276, 277, 279, 281, 284, 285, 286, 288, 289, 290, 292, 293, 294, 295, 296, 299, 303, 304, 305, 306, 455, 463,
    464, 465, 468, 469, 471, 473, 474, 484, 631, 632, 633, 634, 647, 1132, 1302
);
