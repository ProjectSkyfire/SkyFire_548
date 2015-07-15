DELETE FROM `creature_model_info` WHERE `modelid` IN (901, 39637, 45862, 45863, 43717, 43718, 43720, 43724, 43725, 45106, 42250, 42352, 41667, 4558, 43717, 43718, 43721, 43910, 39641, 41281, 38459, 43722, 43723, 43724, 43725, 45124);
INSERT INTO `creature_model_info` (`modelid`, `bounding_radius`, `combat_reach`) VALUES
(901, 0.2, 1), -- 901
(39637, 0.3819442, 1.65), -- 39637
(45862, 0.347222, 1.5), -- 45862
(45863, 0.347222, 1.5), -- 45863
(43717, 1.6, 2.4), -- 43717
(43718, 1.6, 2.4), -- 43718
(43720, 1.6, 2.4), -- 43720
(43724, 1.6, 2.4), -- 43724
(43725, 1.6, 2.4), -- 43725
(45106, 0.347222, 1.5), -- 45106
(42250, 1.6, 2.4), -- 42250
(42352, 1.6, 2.4), -- 42352
(41667, 0.347222, 1.5), -- 41667
(4558, 0.208, 1.5), -- 4558
(43721, 1.6, 2.4), -- 43721
(43910, 1.6, 2.4), -- 43910
(39641, 0.3993053, 1.725), -- 39641
(41281, 0.347222, 1.5), -- 41281
(38459, 0.3, 1), -- 38459
(43722, 1.6, 2.4), -- 43722
(43723, 1.6, 2.4), -- 43723
(45124, 0.347222, 1.5); -- 45124

DELETE FROM `creature_equip_template` WHERE `entry` IN (69334);
INSERT INTO `creature_equip_template` (`entry`, `itemEntry1`, `ItemEntry2`, `ItemEntry3`) VALUES
(69334, 89897, 0, 0);

DELETE FROM `creature_template` WHERE `entry` IN (73674, 68981, 70461, 71483, 70643, 69334, 69085, 73983, 74093, 70268, 70269, 68011);
INSERT INTO `creature_template` VALUES (74093, 0, 0, 0, 0, 0, 1126, 11686, 0, 0, 'Timeless Elite Kill Credit', '', '', 0, 85, 85, 4, 0, 35, 35, 0, 1, 1.14286, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 1024, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 20173);
INSERT INTO `creature_template` VALUES (73983, 0, 0, 0, 0, 0, 1126, 11686, 0, 0, 'Timeless Rare and Rare Elite Kill Credit', '', '', 0, 85, 85, 4, 0, 35, 35, 0, 1, 1.14286, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 1024, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 20173);
INSERT INTO `creature_template` VALUES (73674, 0, 0, 0, 0, 0, 51480, 0, 0, 0, 'Blizzix Sparkshiv', 'Raid Finder Vendor', '', 0, 93, 93, 0, 0, 1735, 1735, 4224, 1, 1.14286, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 12, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 20173);
INSERT INTO `creature_template` VALUES (71483, 0, 0, 0, 0, 0, 48923, 0, 0, 0, 'Abrogar Dusthoof', '', '', 0, 90, 90, 4, 0, 1735, 1735, 3, 1, 1.14286, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 20173);
INSERT INTO `creature_template` VALUES (70643, 0, 0, 0, 0, 0, 49043, 49042, 48230, 48231, 'Big Blossom Digger', '', '', 0, 90, 90, -1, 0, 35, 35, 0, 1, 1.14286, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 6, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 20173);
INSERT INTO `creature_template` VALUES (70461, 0, 0, 0, 0, 0, 45170, 0, 0, 0, 'Milly Greenfield', 'Assistant Seed Counter', '', 0, 8, 8, 4, 0, 35, 35, 1, 1, 1.14286, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 20173);
INSERT INTO `creature_template` VALUES (70269, 0, 0, 0, 0, 0, 16925, 0, 0, 0, 'Alliance Player Credit', '', '', 0, 1, 1, 0, 0, 35, 35, 0, 1, 1.14286, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 1074790400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 20173);
INSERT INTO `creature_template` VALUES (70268, 0, 0, 0, 0, 0, 16925, 0, 0, 0, 'Horde Player Credit', '', '', 0, 1, 1, 0, 0, 35, 35, 0, 1, 1.14286, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 1074790400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 20173);
INSERT INTO `creature_template` VALUES (69334, 0, 0, 0, 0, 0, 47467, 0, 0, 0, 'Adherent Hanjun', 'Tushui Quartermaster', '', 0, 90, 90, 4, 0, 35, 35, 0, 1, 1.14286, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 20173);
INSERT INTO `creature_template` VALUES (69085, 0, 0, 0, 0, 0, 43578, 0, 0, 0, 'Shek\'zeer Bladesworn', '', '', 0, 90, 90, 4, 0, 2557, 2557, 0, 1, 1.14286, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 1048576, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 20173);
INSERT INTO `creature_template` VALUES (68981, 0, 0, 0, 0, 0, 47231, 0, 0, 0, 'Voidbinder Shadzor', 'Item Upgrade', 'reforge', 0, 100, 100, -1, 0, 35, 35, 0, 1, 1.14286, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 3, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 20173);
INSERT INTO `creature_template` VALUES (68011, 0, 0, 0, 0, 0, 46557, 0, 0, 0, 'Shieldwall Mecha-Pounder', '', '', 0, 90, 90, 4, 0, 2618, 2618, 0, 1, 1.14286, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 20173);

DELETE FROM `creature_model_info` WHERE `modelid`= 20770;
DELETE FROM `creature_model_info` WHERE `modelid`= 18035;
DELETE FROM `creature_model_info` WHERE `modelid`= 34;
DELETE FROM `creature_model_info` WHERE `modelid`= 46;
DELETE FROM `creature_model_info` WHERE `modelid`= 63;
DELETE FROM `creature_model_info` WHERE `modelid`= 116;
DELETE FROM `creature_model_info` WHERE `modelid`= 156;
DELETE FROM `creature_model_info` WHERE `modelid`= 199;
DELETE FROM `creature_model_info` WHERE `modelid`= 219;
DELETE FROM `creature_model_info` WHERE `modelid`= 259;
DELETE FROM `creature_model_info` WHERE `modelid`= 349;
DELETE FROM `creature_model_info` WHERE `modelid`= 357;
DELETE FROM `creature_model_info` WHERE `modelid`= 419;
DELETE FROM `creature_model_info` WHERE `modelid`= 444;
DELETE FROM `creature_model_info` WHERE `modelid`= 461;
DELETE FROM `creature_model_info` WHERE `modelid`= 463;
DELETE FROM `creature_model_info` WHERE `modelid`= 469;
DELETE FROM `creature_model_info` WHERE `modelid`= 595;
DELETE FROM `creature_model_info` WHERE `modelid`= 619;
DELETE FROM `creature_model_info` WHERE `modelid`= 629;
DELETE FROM `creature_model_info` WHERE `modelid`= 849;
DELETE FROM `creature_model_info` WHERE `modelid`= 891;
DELETE FROM `creature_model_info` WHERE `modelid`= 899;
DELETE FROM `creature_model_info` WHERE `modelid`= 1179;
DELETE FROM `creature_model_info` WHERE `modelid`= 2356;
DELETE FROM `creature_model_info` WHERE `modelid`= 2923;
DELETE FROM `creature_model_info` WHERE `modelid`= 2944;
DELETE FROM `creature_model_info` WHERE `modelid`= 3157;
DELETE FROM `creature_model_info` WHERE `modelid`= 3159;
DELETE FROM `creature_model_info` WHERE `modelid`= 3173;
DELETE FROM `creature_model_info` WHERE `modelid`= 4796;
DELETE FROM `creature_model_info` WHERE `modelid`= 4799;
DELETE FROM `creature_model_info` WHERE `modelid`= 4984;
DELETE FROM `creature_model_info` WHERE `modelid`= 5148;
DELETE FROM `creature_model_info` WHERE `modelid`= 5152;
DELETE FROM `creature_model_info` WHERE `modelid`= 5153;
DELETE FROM `creature_model_info` WHERE `modelid`= 5154;
DELETE FROM `creature_model_info` WHERE `modelid`= 5155;
DELETE FROM `creature_model_info` WHERE `modelid`= 5174;
DELETE FROM `creature_model_info` WHERE `modelid`= 5199;
