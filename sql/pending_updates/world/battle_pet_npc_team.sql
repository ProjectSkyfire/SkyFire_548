-- Tamer pet teams for `.battlepet npc` and the tamer battle option.
-- Every species exists in the 5.4.8 client BattlePetSpecies.db2. Levels,
-- qualities and breeds come from 5.4.8-era tamer data; the quality 4 and 5 pets
-- match what Wowhead MoP Classic comments say (Pandaria grand masters epic,
-- Pandaren spirits legendary). Brok, Bordin and Goz stay rare although one 5.2
-- comment says epic, as does Major Payne. Slot order comes from Wowhead MoP
-- Classic tamer pages (player comments; Wowhead shows no structured team).
-- Tamers without a source keep species id order. Slot 0 is the front pet.
-- Slot order source per tamer:
--   63194 Steven Lisbane: Wowhead MoP Classic, one comment
--   64330 Julia Stevens: no source, species id order
--   65648 Old MacDonald: no source, species id order
--   65651 Lindsay: no source, species id order
--   65655 Eric Davidson: Wowhead MoP Classic, one comment
--   65656 Bill Buckler: Wowhead MoP Classic, one comment
--   66126 Zunta: Wowhead MoP Classic comments, several agree
--   66135 Dagra the Fierce: Wowhead MoP Classic, one comment
--   66136 Analynn: no source, species id order
--   66137 Zonya the Sadist: Wowhead MoP Classic, one comment
--   66352 Traitor Gluk: Wowhead MoP Classic, one comment
--   66372 Merda Stronghoof: no source, species id order
--   66412 Elena Flutterfly: Wowhead MoP Classic comments, several agree
--   66422 Cassandra Kaboom: Wowhead MoP Classic, one comment
--   66436 Grazzle the Great: Wowhead MoP Classic, one comment
--   66442 Zoltan: no source, species id order
--   66452 Kela Grimtotem: Wowhead MoP Classic, one comment
--   66466 Stone Cold Trixxy: Wowhead MoP Classic comments, several agree
--   66478 David Kosse: no source, species id order
--   66512 Deiza Plaguehorn: Wowhead MoP Classic comments, several agree
--   66515 Kortas Darkhammer: no source, species id order
--   66518 Everessa: no source, species id order
--   66520 Durin Darkhammer: Wowhead MoP Classic comments, several agree
--   66522 Lydia Accoste: no source, species id order
--   66550 Nicki Tinytech: Wowhead MoP Classic comments, several agree
--   66551 Ras'an: Wowhead MoP Classic comments, several agree
--   66552 Narrok: Wowhead MoP Classic comments, several agree
--   66553 Morulu The Elder: no source, species id order
--   66557 Bloodknight Antari: Wowhead MoP Classic comments, several agree
--   66635 Beegle Blastfuse: Wowhead MoP Classic comments, several agree
--   66636 Nearly Headless Jacob: Wowhead MoP Classic comments, several agree
--   66638 Okrut Dragonwaste: Wowhead MoP Classic comments, several agree
--   66639 Gutretch: Wowhead MoP Classic comments, several agree
--   66675 Major Payne: Wowhead MoP Classic comments, several agree
--   66730 Hyuna of the Shrines: Wowhead MoP Classic comments, several agree
--   66733 Mo'ruk: Wowhead MoP Classic comments, several agree
--   66734 Farmer Nishi: Wowhead MoP Classic comments, several agree
--   66738 Courageous Yon: Wowhead MoP Classic comments, several agree
--   66739 Wastewalker Shu: Wowhead MoP Classic comments, several agree
--   66741 Aki the Chosen: Wowhead MoP Classic comments, several agree
--   66815 Bordin Steadyfist: Wowhead MoP Classic comments, several agree
--   66819 Brok: Wowhead MoP Classic comments, several agree
--   66822 Goz Banefury: Wowhead MoP Classic comments, several agree
--   66824 Obalis: Wowhead MoP Classic comments, several agree
--   66918 Seeker Zusshi: Wowhead MoP Classic comments, several agree
--   67370 Jeremy Feasel: Wowhead MoP Classic comments, several agree
--   68462 Flowing Pandaren Spirit: Wowhead MoP Classic comments, several agree
--   68463 Burning Pandaren Spirit: Wowhead MoP Classic comments, several agree
--   68464 Whispering Pandaren Spirit: Wowhead MoP Classic comments, several agree
--   68465 Thundering Pandaren Spirit: no source, species id order
--   73626 Little Tommy Newcomer: no source, species id order

CREATE TABLE IF NOT EXISTS `battle_pet_npc_team` (
  `entry` int unsigned NOT NULL COMMENT 'creature_template.entry of the tamer',
  `slot` tinyint unsigned NOT NULL COMMENT '0 = front pet, at most 3 slots',
  `species` smallint unsigned NOT NULL COMMENT 'BattlePetSpecies.db2 id',
  `level` tinyint unsigned NOT NULL,
  `quality` tinyint unsigned NOT NULL COMMENT '0 poor to 5 legendary',
  `breed` tinyint unsigned NOT NULL,
  PRIMARY KEY (`entry`, `slot`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 COMMENT='Pet team of each pet battle tamer';

DELETE FROM `battle_pet_npc_team`;
INSERT INTO `battle_pet_npc_team` (`entry`, `slot`, `species`, `level`, `quality`, `breed`) VALUES
(63194, 0, 885, 9, 1, 9),
(63194, 1, 884, 9, 1, 9),
(63194, 2, 883, 9, 1, 7),
(64330, 0, 872, 2, 1, 6),
(64330, 1, 873, 2, 1, 6),
(65648, 0, 874, 3, 1, 6),
(65648, 1, 875, 3, 1, 9),
(65648, 2, 876, 3, 1, 8),
(65651, 0, 877, 5, 1, 12),
(65651, 1, 878, 5, 1, 6),
(65651, 2, 879, 5, 1, 8),
(65655, 0, 881, 7, 1, 9),
(65655, 1, 882, 7, 1, 12),
(65655, 2, 880, 7, 1, 8),
(65656, 0, 888, 11, 2, 6),
(65656, 1, 887, 11, 2, 9),
(65656, 2, 886, 11, 2, 12),
(66126, 0, 890, 2, 1, 6),
(66126, 1, 889, 2, 1, 6),
(66135, 0, 893, 3, 1, 6),
(66135, 1, 892, 3, 1, 9),
(66135, 2, 891, 3, 1, 5),
(66136, 0, 894, 5, 1, 5),
(66136, 1, 895, 5, 1, 9),
(66136, 2, 896, 5, 1, 7),
(66137, 0, 899, 7, 1, 9),
(66137, 1, 898, 7, 1, 6),
(66137, 2, 897, 7, 1, 12),
(66352, 0, 906, 13, 2, 5),
(66352, 1, 905, 13, 2, 9),
(66352, 2, 904, 13, 2, 12),
(66372, 0, 900, 9, 1, 7),
(66372, 1, 901, 9, 1, 7),
(66372, 2, 902, 9, 1, 12),
(66412, 0, 926, 17, 2, 9),
(66412, 1, 925, 17, 2, 8),
(66412, 2, 924, 17, 2, 12),
(66422, 0, 909, 11, 2, 5),
(66422, 1, 908, 11, 2, 9),
(66422, 2, 907, 11, 2, 12),
(66436, 0, 913, 14, 2, 9),
(66436, 1, 912, 14, 2, 4),
(66436, 2, 911, 14, 2, 8),
(66442, 0, 921, 16, 2, 8),
(66442, 1, 922, 16, 2, 4),
(66442, 2, 923, 16, 2, 7),
(66452, 0, 917, 15, 2, 7),
(66452, 1, 916, 15, 2, 9),
(66452, 2, 915, 15, 2, 8),
(66466, 0, 929, 19, 2, 12),
(66466, 1, 928, 19, 2, 9),
(66466, 2, 927, 19, 2, 5),
(66478, 0, 931, 13, 2, 12),
(66478, 1, 932, 13, 2, 7),
(66478, 2, 933, 13, 2, 5),
(66512, 0, 936, 14, 2, 6),
(66512, 1, 935, 14, 2, 9),
(66512, 2, 934, 14, 2, 7),
(66515, 0, 937, 15, 2, 9),
(66515, 1, 938, 15, 2, 12),
(66515, 2, 939, 15, 2, 8),
(66518, 0, 941, 16, 2, 9),
(66518, 1, 942, 16, 2, 7),
(66518, 2, 943, 16, 2, 12),
(66520, 0, 946, 17, 2, 5),
(66520, 1, 945, 17, 2, 8),
(66520, 2, 944, 17, 2, 7),
(66522, 0, 947, 19, 2, 12),
(66522, 1, 948, 19, 2, 5),
(66522, 2, 949, 19, 2, 9),
(66550, 0, 952, 20, 2, 8),
(66550, 1, 951, 20, 2, 8),
(66550, 2, 950, 20, 2, 7),
(66551, 0, 955, 21, 2, 5),
(66551, 1, 954, 21, 2, 6),
(66551, 2, 953, 21, 2, 7),
(66552, 0, 958, 22, 2, 6),
(66552, 1, 957, 22, 2, 9),
(66552, 2, 956, 22, 2, 7),
(66553, 0, 959, 23, 2, 9),
(66553, 1, 960, 23, 2, 4),
(66553, 2, 961, 23, 2, 4),
(66557, 0, 964, 24, 3, 6),
(66557, 1, 963, 24, 3, 8),
(66557, 2, 962, 24, 3, 9),
(66635, 0, 967, 25, 3, 9),
(66635, 1, 966, 25, 3, 12),
(66635, 2, 965, 25, 3, 6),
(66636, 0, 970, 25, 3, 9),
(66636, 1, 969, 25, 3, 12),
(66636, 2, 968, 25, 3, 8),
(66638, 0, 973, 25, 3, 9),
(66638, 1, 972, 25, 3, 12),
(66638, 2, 971, 25, 3, 8),
(66639, 0, 976, 25, 3, 8),
(66639, 1, 975, 25, 3, 7),
(66639, 2, 974, 25, 3, 9),
(66675, 0, 979, 25, 3, 7),
(66675, 1, 978, 25, 3, 9),
(66675, 2, 977, 25, 3, 12),
(66730, 0, 994, 25, 4, 9),
(66730, 1, 993, 25, 4, 5),
(66730, 2, 992, 25, 4, 7),
(66733, 0, 998, 25, 4, 12),
(66733, 1, 1000, 25, 4, 9),
(66733, 2, 999, 25, 4, 7),
(66734, 0, 997, 25, 4, 8),
(66734, 1, 996, 25, 4, 7),
(66734, 2, 995, 25, 4, 12),
(66738, 0, 1003, 25, 4, 12),
(66738, 1, 1002, 25, 4, 12),
(66738, 2, 1001, 25, 4, 9),
(66739, 0, 1009, 25, 4, 8),
(66739, 1, 1008, 25, 4, 7),
(66739, 2, 1007, 25, 4, 4),
(66741, 0, 1012, 25, 5, 5),
(66741, 1, 1011, 25, 5, 8),
(66741, 2, 1010, 25, 5, 9),
(66815, 0, 985, 25, 3, 8),
(66815, 1, 984, 25, 3, 12),
(66815, 2, 983, 25, 3, 7),
(66819, 0, 982, 25, 3, 5),
(66819, 1, 981, 25, 3, 9),
(66819, 2, 980, 25, 3, 7),
(66822, 0, 988, 25, 3, 9),
(66822, 1, 987, 25, 3, 12),
(66822, 2, 986, 25, 3, 4),
(66824, 0, 991, 25, 4, 9),
(66824, 1, 990, 25, 4, 5),
(66824, 2, 989, 25, 4, 12),
(66918, 0, 1006, 25, 4, 7),
(66918, 1, 1005, 25, 4, 12),
(66918, 2, 1004, 25, 4, 12),
(67370, 0, 1065, 25, 4, 9),
(67370, 1, 1067, 25, 4, 8),
(67370, 2, 1066, 25, 4, 10),
(68462, 0, 1132, 25, 5, 5),
(68462, 1, 1133, 25, 5, 10),
(68462, 2, 1138, 25, 5, 12),
(68463, 0, 1130, 25, 5, 9),
(68463, 1, 1139, 25, 5, 9),
(68463, 2, 1131, 25, 5, 11),
(68464, 0, 1135, 25, 5, 3),
(68464, 1, 1136, 25, 5, 6),
(68464, 2, 1140, 25, 5, 6),
(68465, 0, 1134, 25, 5, 12),
(68465, 1, 1137, 25, 5, 7),
(68465, 2, 1141, 25, 5, 8),
(73626, 0, 1339, 25, 5, 6);
