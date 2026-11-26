-- Spawns for 38 pet battle tamers that have a creature_template but no spawn.
-- Position check: Wowhead MoP Classic / warcraft.wiki.gg map coordinates,
-- converted with the 5.4.8 client WorldMapArea.dbc. Rows with orientation 0 are
-- those converted coordinates, z from the terrain height of the extracted client
-- maps (Morulu The Elder: the floor height of Lower City). The other rows come
-- from older community spawn data and lie within 11 yards of the public
-- coordinates, except Zunta (25 yards) and Obalis (about 72 yards, not
-- confirmed in game). Steven Lisbane's height is not confirmed in game either.

SET @CGUID := 9000101;

DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID AND @CGUID+37;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`) VALUES
(@CGUID+0, 64330, 0, 1, -9876.05, 89.4845, 32.2, 1.63207, 300),                      -- Julia Stevens, Elwynn Forest
(@CGUID+1, 65648, 0, 1, -9831.59, 886.783, 27.1075, 3.86978, 300),                   -- Old MacDonald, Westfall
(@CGUID+2, 65651, 0, 1, -9408.66, -2336.01, 58.05, 2.538, 300),                      -- Lindsay, Redridge Mountains
(@CGUID+3, 65655, 0, 1, -10518.2, 296.56, 31.22, 3.204, 300),                        -- Eric Davidson, Duskwood
(@CGUID+4, 65656, 0, 1, -14448, 76.2, 4.21, 0, 300),                                 -- Bill Buckler, Cape of Stranglethorn
(@CGUID+5, 66478, 0, 1, 65.27, -3992.8, 135.12, 0, 300),                             -- David Kosse, The Hinterlands
(@CGUID+6, 66512, 0, 1, 2296.83, -4985.47, 73.76, 5.821, 300),                       -- Deiza Plaguehorn, Eastern Plaguelands
(@CGUID+7, 66515, 0, 1, -6511.35, -1103.46, 313.05, 5.171, 300),                     -- Kortas Darkhammer, Searing Gorge
(@CGUID+8, 66518, 0, 1, -10231.1, -4011.38, 23.27, 0.64, 300),                       -- Everessa, Swamp of Sorrows
(@CGUID+9, 66520, 0, 1, -7974.75, -1263.76, 133.05, 3.427, 300),                     -- Durin Darkhammer, Burning Steppes
(@CGUID+10, 66522, 0, 1, -11148.4, -1836.59, 71.9, 6.153, 300),                      -- Lydia Accoste, Deadwind Pass
(@CGUID+11, 66822, 0, 1, -4153.28, -5410.54, 16.87, 3.866, 300),                     -- Goz Banefury, Twilight Highlands
(@CGUID+12, 66126, 1, 1, 783.398, -4270.26, 18.966, 3.29585, 300),                   -- Zunta, Durotar
(@CGUID+13, 66135, 1, 1, -220.1, -3165, 181.92, 0, 300),                             -- Dagra the Fierce, Northern Barrens
(@CGUID+14, 66136, 1, 1, 3539, 535.1, 5.85, 0, 300),                                 -- Analynn, Ashenvale
(@CGUID+15, 66137, 1, 1, 627.2, 350.3, 53.52, 0, 300),                               -- Zonya the Sadist, Stonetalon Mountains
(@CGUID+16, 66352, 1, 1, -4666.74, 1288.99, 96.2586, 1.97081, 300),                  -- Traitor Gluk, Feralas
(@CGUID+17, 66372, 1, 1, -917.524, 1665.68, 59.9484, 3.80191, 300),                  -- Merda Stronghoof, Desolace
(@CGUID+18, 66412, 1, 1, 7563.3, -2445.4, 452.83, 0, 300),                           -- Elena Flutterfly, Moonglade
(@CGUID+19, 66422, 1, 1, -3699.7, -1579.1, 92, 0, 300),                              -- Cassandra Kaboom, Southern Barrens
(@CGUID+20, 66436, 1, 1, -4654.8, -3804.8, 44, 0, 300),                              -- Grazzle the Great, Dustwallow Marsh
(@CGUID+21, 66442, 1, 1, 4951.1, -624.1, 293.16, 0, 300),                            -- Zoltan, Felwood
(@CGUID+22, 66452, 1, 1, -4931.7, -1836.9, 80.02, 0, 300),                           -- Kela Grimtotem, Thousand Needles
(@CGUID+23, 66466, 1, 1, 6149.2, -5026.1, 789.65, 0, 300),                           -- Stone Cold Trixxy, Winterspring
(@CGUID+24, 66819, 1, 1, 5269.79, -3534.61, 1595.53, 3.22234, 300),                  -- Brok, Mount Hyjal
(@CGUID+25, 66550, 530, 1, -216.5, 2218.8, 68.3, 0, 300),                            -- Nicki Tinytech, Hellfire Peninsula
(@CGUID+26, 66551, 530, 1, 242.6, 8605.3, 22.25, 0, 300),                            -- Ras'an, Zangarmarsh
(@CGUID+27, 66552, 530, 1, -1777.9, 6925.6, -36.36, 0, 300),                         -- Narrok, Nagrand
(@CGUID+28, 66553, 530, 1, -2083.5, 5377.6, -31.5, 0, 300),                          -- Morulu The Elder, Shattrath City
(@CGUID+29, 66557, 530, 1, -3487.9, 2553, 61.6, 0, 300),                             -- Bloodknight Antari, Shadowmoon Valley
(@CGUID+30, 66635, 571, 1, 1750.1, -3127, 156.89, 0, 300),                           -- Beegle Blastfuse, Howling Fjord
(@CGUID+31, 66636, 571, 1, 5431.5, 79.6, 148.66, 0, 300),                            -- Nearly Headless Jacob, Crystalsong Forest
(@CGUID+32, 66638, 571, 1, 2695.5, 318.2, 53.71, 0, 300),                            -- Okrut Dragonwaste, Dragonblight
(@CGUID+33, 66639, 571, 1, 5444.9, -1259.2, 248.75, 0, 300),                         -- Gutretch, Zul'Drak
(@CGUID+34, 66675, 571, 1, 8607.6, 590.1, 550.09, 0, 300),                           -- Major Payne, Icecrown
(@CGUID+35, 66815, 646, 1, 856.13, 508.818, -62.5991, 4.73275, 300),                 -- Bordin Steadyfist, Deepholm
(@CGUID+36, 63194, 0, 1, -12123.67, -142.25, 32.0, 0, 300),                          -- Steven Lisbane, Northern Stranglethorn
(@CGUID+37, 66824, 1, 1, -9733.54, -983.063, 57.1865, 3.40234, 300);                 -- Obalis, Vir'naal Dam
