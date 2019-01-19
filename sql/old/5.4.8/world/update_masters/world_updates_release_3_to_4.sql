DELETE FROM `creature_addon` WHERE `guid` IN (48459, 48460, 48461, 48462, 48463, 48464, 48465, 48466, 48467, 48468, 48469, 48477, 48563, 48585, 48761, 48765,
48768, 48769, 48772, 48774, 48775, 48780, 48781, 48784, 48785, 48787, 48789, 48790, 48792, 48794, 48805, 48806, 48807, 48809, 48812, 48816, 48818, 48827, 48832,
48836, 48837, 48846, 48850, 48854, 48857, 48862, 48863, 48864, 48867, 48870, 48898, 48906, 48909, 48910, 48911, 48912, 48913, 48914, 48915, 48916, 48917, 48918,
48919, 48920, 48921, 48922, 48923, 48924, 48925, 48926, 48937, 48946, 48948, 48949, 48950, 48951, 48952, 48953, 48954, 48955, 48956, 48957, 48958, 48963, 48964,
48965, 48967, 48968, 48969, 48970, 48972, 48973, 48974, 48975, 48983, 48987, 48993, 48999, 49002, 49003, 49005, 49007, 49010, 49012, 49013, 49014, 49015, 49016,
49017, 49018, 49019, 49020, 49022, 91312, 91313, 91314, 91315, 91316, 91317, 91318, 91319, 91320, 91321, 91322, 91327, 91328, 91329, 91332, 91333, 91336, 91337,
91339, 91349, 91350, 91351, 91352, 91353, 91354, 91355, 91356, 91357, 91358, 91359, 91360, 91361, 91362, 91370, 91371, 91372, 91374, 91375, 91376, 91377, 91381,
91383, 91384, 91385, 91386, 91387, 91388, 91389, 91390, 91391, 91392, 91394, 91395, 91396, 91397, 91398, 91399, 91400, 91401, 91402, 91403, 91404, 91405, 91406,
91407, 91411, 91412, 91415, 91419, 91420, 91421, 91422, 91423, 91424, 91425, 91426, 91427, 52542, 23467, 23466, 23130, 7690, 14459, 14458, 14456, 14449, 14448);

DELETE FROM `creature_model_info` WHERE `modelid` IN (18035, 20770);
INSERT INTO `creature_model_info` VALUES
(18035,0.7,6,2,0),
(20770,1.05,9,2,0);-- Jinho the Wind Breaker (SPAWN) -- GUID GAPS TAKEN FROM LATEST SKYFIREDB
-- Mifan <Food and Drink>
-- Nadina Stargem <Flexible Vendor>
-- Ki'agnuu <Raid Vendor>
-- Erin
-- Sadi
-- Armorer Relna
-- Dirki Tanboshi
-- Wounded Defender
-- Warpweaver Shafiee <Transmogrifier>
-- Thaumaturge Anjool <Arcane Reforger>
-- Vaultkeeper Meelad <Void Storage>
-- Voidbinder Shadzor <Item Upgrade>
DELETE FROM creature WHERE guid IN (1225, 1226, 1293, 1294, 1295, 1296, 1342, 1344, 1345, 1346, 1347, 1360, 1361, 1362, 1363, 1372);
INSERT INTO creature (guid, id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, MovementType) VALUES
(1225, 66998, 870, 1, 1, 1673.726, 943.642, 471.1779, 0.4085974, 120, 0, 0), -- Jinho the Wind Breaker
(1226, 64041, 870, 1, 1, 1713.882, 896.3594, 471.0092, 4.778744, 120, 0, 0), -- Mifan <Food and Drink>
(1293, 74010, 870, 1, 1, 1677.979, 948.7813, 489.5686, 1.898429, 120, 0, 0), -- Nadina Stargem <Flexible Vendor>
(1294, 74012, 870, 1, 1, 1673.382, 948.5573, 489.5636, 1.686976, 120, 0, 0), -- Ki'agnuu <Raid Vendor>
(1295, 64106, 870, 1, 1, 1679.936, 868.1858, 471.0073, 2.01043, 120, 0, 0), -- Erin
(1296, 64107, 870, 1, 1, 1677.481, 867.7882, 471.0073, 0, 120, 0, 0), -- Sadi
(1342, 64588, 870, 1, 1, 1683.776, 953.2205, 489.5803, 3.500753, 120, 0, 0), -- Armorer Relna
(1344, 64119, 870, 1, 1, 1666.602, 967.9636, 489.438, 3.174507, 120, 0, 0), -- Dirki Tanboshi
(1345, 64118, 870, 1, 1, 1664.686, 967.8108, 489.4384, 6.139359, 120, 0, 0), -- Erni Tanboshi
(1346, 63753, 870, 1, 1, 1662.311, 976.3195, 471.0073, 4.523802, 120, 0, 0), -- Wounded Defender
(1347, 63753, 870, 1, 1, 1668.399, 973.0191, 471.0073, 5.395439, 120, 0, 0), -- Wounded Defender
(1360, 63753, 870, 1, 1, 1661.023, 972.8333, 471.0073, 2.463727, 120, 0, 0), -- Wounded Defender
(1361, 67014, 870, 1, 1, 1777.021, 910.5746, 489.2955, 3.286788, 120, 0, 0), -- Warpweaver Shafiee <Transmogrifier>
(1362, 64044, 870, 1, 1, 1778.918, 897.6563, 487.9105, 3.353462, 120, 0, 0), -- Thaumaturge Anjool <Arcane Reforger>
(1363, 67013, 870, 1, 1, 1777.833, 903.6163, 487.331, 3.252547, 120, 0, 0), -- Vaultkeeper Meelad <Void Storage>
(1372, 68981, 870, 1, 1, 1770.705, 917.967, 487.5727, 3.783183, 120, 0, 0); -- Voidbinder Shadzor <Item Upgrade>DELETE FROM creature_equip_template WHERE entry IN (67014, 64044, 68981, 67013);
INSERT INTO creature_equip_template (entry, itemEntry1, itemEntry2, itemEntry3) VALUES
(67014, 71739, 71741, 0), -- Warpweaver Shafiee <Transmogrifier>
(64044, 71732, 0, 0), -- Thaumaturge Anjool <Arcane Reforger>
(68981, 29417, 71741, 0), -- Voidbinder Shadzor <Item Upgrade>
(67013, 0, 71736, 0); -- Vaultkeeper Meelad <Void Storage>-- Misc human start area quest updates.
DELETE FROM `creature_queststarter` WHERE `id` = 49874 AND `quest` IN (28759, 28769, 28770, 28771, 28772, 28773, 28774, 29079);
DELETE FROM `creature_queststarter` WHERE `id` = 50047 AND `quest` IN (28806, 28808, 28809, 28810, 28811, 28812, 28813, 29082);
DELETE FROM guild_rewards;
ALTER TABLE guild_rewards DROP COLUMN achievement;
ALTER TABLE guild_rewards ADD COLUMN achievements VARCHAR(256) NOT NULL DEFAULT "" AFTER price;

INSERT INTO guild_rewards (entry, standing, racemask, price, achievements) VALUES
(69209, 4, -1, 1250000, ""),
(69210, 5, -1, 2500000, ""),
(63353, 5, 33555378, 1500000, "4989"),
(63352, 5, 18875469, 1500000, "4989"),
(63207, 5, 33555378, 3000000, "4945"),
(63206, 5, 18875469, 3000000, "4945"),
(65274, 6, 33555378, 5000000, "5035"),
(65360, 6, 18875469, 5000000, "5035"),
(65362, 5, 33555378, 3000000, "5179"),
(65364, 6, 33555378, 5000000, "5201"),
(65361, 5, 18875469, 3000000, "5031"),
(65363, 6, 18875469, 5000000, "5201"),
(63138, 7, -1, 3000000, "5812 5892"),
(63398, 6, 0, 3000000, "5144"),
(63359, 5, 18875469, 1500000, "4860"),
(64398, 5, 18875469, 2000000, "5143"),
(64399, 5, 18875469, 3000000, "5422"),
(64400, 5, 33555378, 1500000, "4860"),
(64401, 5, 33555378, 2000000, "5143"),
(64402, 5, 33555378, 3000000, "5422"),
(62800, 5, 0, 1500000, "5036"),
(62799, 5, 0, 1500000, "5467"),
(65435, 5, 0, 1500000, "5465"),
(65498, 5, 0, 1500000, "5024"),
(62286, 4, 0, 100000000, "4943 5196"),
(68136, 6, 18875469, 200000000, "5152 4949"),
(62287, 6, 33555378, 200000000, "5158 4949"),
(69892, 5, 0, 12000000, "4944"),
(62038, 5, 0, 12000000, "4944"),
(62039, 5, 0, 12000000, "4944"),
(62040, 5, 0, 12000000, "4944"),
(69887, 5, 0, 15000000, "4946"),
(61958, 5, 0, 15000000, "4946"),
(61942, 5, 0, 15000000, "4946"),
(61937, 5, 0, 15000000, "4946"),
(61936, 5, 0, 15000000, "4946"),
(61935, 5, 0, 15000000, "4946"),
(61931, 5, 0, 15000000, "4946"),
(67107, 7, 33555378, 15000000, "5492"),
(62298, 7, 18875469, 15000000, "4912"),
(63125, 7, 0, 30000000, "4988"),
(71033, 7, 0, 15000000, "5840"),
(69888, 5, -1, 17500000, "6626"),
(62029, 5, -1, 17500000, "6626"),
(62027, 5, -1, 17500000, "6626"),
(62026, 5, -1, 17500000, "6626"),
(62025, 5, -1, 17500000, "6626"),
(62024, 5, -1, 17500000, "6626"),
(62023, 5, -1, 17500000, "6626"),
(85508, 5, -1, 1000000, "7448 7449"),
(85509, 5, -1, 1000000, "7448 7449"),
(85510, 5, -1, 1000000, "7448 7449"),
(89190, 5, -1, 1500000, "6681"),
(89192, 5, -1, 1500000, "6681"),
(89194, 5, -1, 1500000, "6681"),
(89191, 7, -1, 2000000, "6644 6664"),
(89193, 7, -1, 2000000, "6644 6664"),
(89195, 7, -1, 2000000, "6644 6664"),
(85513, 6, -1, 3000000, "6634"),
(85666, 7, -1, 30000000, "6682");
-- Defeat Garrosh Hellscream in Siege of Orgrimmar on Normal or Heroic difficulty.
-- A: <Name>, Conqueror of Orgrimmar
-- H: <Name>, Liberator of Orgrimmar
DELETE FROM player_factionchange_titles WHERE alliance_id=382 OR horde_id=383;
INSERT INTO player_factionchange_titles (alliance_id, horde_id) VALUES
(382, 383);SET @TinRef := 13005;
SET @CATA1 := @TinRef+1;
SET @CATA2 := @TinRef+2;
SET @MIST1 := @TinRef+3;
SET @MIST2 := @TinRef+4;

-- CATA
SET @Obsidium := 53038;
SET @Elementium := 52185;
SET @Pyrite := 52183;
-- MIST
SET @Ghost := 72092;
SET @Kyparite := 72093;
SET @BTrillium := 72094;
SET @WTrillium := 72103;

-- Reference Loot Templates
DELETE FROM `reference_loot_template` WHERE `entry` BETWEEN @TinRef+1 AND @TinRef+4;
INSERT INTO `reference_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
-- CATA Greens
(@CATA1,52177,0,1,1,1,1), -- Carnelian
(@CATA1,52178,0,1,1,1,1), -- Zephyrite
(@CATA1,52179,0,1,1,1,1), -- Alicite
(@CATA1,52180,0,1,1,1,1), -- Nightstone
(@CATA1,52181,0,1,1,1,1), -- Hessonite
(@CATA1,52182,0,1,1,1,1), -- Jasper
-- CATA Blues
(@CATA2,52190,0,1,1,1,1), -- Inferno Ruby
(@CATA2,52191,0,1,1,1,1), -- Ocean Sapphire
(@CATA2,52192,0,1,1,1,1), -- Dream Emerald
(@CATA2,52193,0,1,1,1,1), -- Ember Topaz
(@CATA2,52194,0,1,1,1,1), -- Demonseye
(@CATA2,52195,0,1,1,1,1), -- Amberjewel
-- MIST Greens
(@MIST1,76130,0,1,1,1,1), -- Tiger Opal
(@MIST1,76133,0,1,1,1,1), -- Lapis Lazuli
(@MIST1,76134,0,1,1,1,1), -- Sunstone
(@MIST1,76135,0,1,1,1,1), -- Roguestone
(@MIST1,76136,0,1,1,1,1), -- Pandarian Garnet
(@MIST1,76137,0,1,1,1,1), -- Alexandrite
-- MIST Blues
(@MIST2,76131,0,1,1,1,1), -- Primordial Ruby
(@MIST2,76138,0,1,1,1,1), -- River's Hearth
(@MIST2,76139,0,1,1,1,1), -- Wild Jade
(@MIST2,76140,0,1,1,1,1), -- Vermilion Onyx
(@MIST2,76141,0,1,1,1,1), -- Imperial Amethyst
(@MIST2,76142,0,1,1,1,1); -- Sun's Radiance

-- --------------------------------------------------------
-- Prospecting Loot Templates
DELETE FROM `prospecting_loot_template` WHERE `entry` IN (@Obsidium,@Elementium,@Pyrite,@Ghost,@Kyparite,@BTrillium,@WTrillium);
INSERT INTO `prospecting_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
-- Obsidium Ore
(@Obsidium,1,95,1,1,-@CATA1,1), -- One from CATA Greens
(@Obsidium,2,05,1,1,-@CATA2,1), -- One from CATA Blues
-- Elementium Ore
(@Elementium,1,85,1,1,-@CATA1,2), -- Two from CATA Greens
(@Elementium,2,15,1,1,-@CATA2,1), -- One from CATA Blues
-- Pyrite Ore
(@Pyrite,52327,100,1,0,1,3), -- Volatile Earth
(@Pyrite,1,75,1,1,-@CATA1,2), -- Two from CATA Greens
(@Pyrite,2,25,1,1,-@CATA2,1), -- Two from CATA Blues
-- Ghost Iron Ore
(@Ghost,90407,80,1,0,1,2), -- Sparkling Shard
(@Ghost,1,95,1,1,-@MIST1,1), -- One from MIST Greens
(@Ghost,2,05,1,1,-@MIST2,1), -- One from MIST Blues
-- Kyparit
(@Kyparite,90407,100,1,0,1,2), -- Sparkling Shard
(@Kyparite,1,95,1,1,-@MIST1,1), -- One from MIST Greens
(@Kyparite,2,05,1,1,-@MIST2,1), -- One from MIST Blues
-- Black Trillium Ore
(@BTrillium,90407,100,1,0,1,2), -- Sparkling Shard
(@BTrillium,1,75,1,1,-@MIST1,2), -- Two from MIST Greens
(@BTrillium,2,25,1,1,-@MIST2,1), -- One from MIST Blues
-- White Trillium Ore
(@WTrillium,90407,100,1,0,1,2), -- Sparkling Shard
(@WTrillium,1,75,1,1,-@MIST1,2), -- Two from MIST Greens
(@WTrillium,2,25,1,1,-@MIST2,1); -- One from MIST Blues

-- Krogo Darkhide <Leatherworking & Skinning Supplies> (SPAWN) -- GUID GAPS TAKEN FROM LATEST SKYFIREDB
-- Stephen Wong <Cooking Supplies>
-- Tixit Brightfuse <Engineering Supplies>
-- Vaultkeeper Goldpenny <Banker>
-- Hafuna Landwalker <Herbalism Supplies>
-- Vaultkeeper Kan <Banker>
-- Vaultkeeper Shifen <Banker>
-- Reeler Uko <Fishing Supplies>
-- Gentle Dari <First Aid Supplies>
-- Esha the Loommaiden <Tailoring Supplies>
-- Challenger Wuli <Challenge Dungeons>
-- Sang-Bo <The Shado-Pan>
-- Lena Stonebrush <The Lorewalkers>
-- Training Dummy
DELETE FROM creature WHERE guid IN (1379, 1380, 1381, 1382, 1383, 1384, 1385, 1386, 1387, 1388, 1389, 1390, 1391, 1392, 1393);
INSERT INTO creature (guid, id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, MovementType) VALUES
(1379, 64054, 870, 1, 1, 1703.592, 973.9566, 471.0091, 1.926135, 120, 0, 0), -- Krogo Darkhide <Leatherworking & Skinning Supplies>
(1380, 64126, 870, 1, 1, 1674.898, 985.6649, 471.0073, 4.84544, 120, 0, 0), -- Stephen Wong <Cooking Supplies>
(1381, 64060, 870, 1, 1, 1723.431, 978.9305, 471.0098, 2.603233, 120, 0, 0), -- Tixit Brightfuse <Engineering Supplies>
(1382, 63965, 870, 1, 1, 1701.424, 1006.589, 489.0875, 4.32208, 120, 0, 0), -- Vaultkeeper Goldpenny <Banker>
(1383, 64056, 870, 1, 1, 1721.516, 996.2864, 471.0098, 4.141436, 120, 0, 0), -- Hafuna Landwalker <Herbalism Supplies>
(1384, 63964, 870, 1, 1, 1714.049, 1005.793, 489.0882, 4.893499, 120, 0, 0), -- Vaultkeeper Kan <Banker>
(1385, 63966, 870, 1, 1, 1725.307, 1009.708, 489.0875, 5.437492, 120, 0, 0), -- Vaultkeeper Shifen <Banker>
(1386, 64585, 870, 1, 1, 1764.873, 978.4774, 487.3397, 1.22025, 120, 0, 0), -- Reeler Uko <Fishing Supplies>
(1387, 64062, 870, 1, 1, 1660.983, 969.7656, 470.9239, 3.896879, 120, 0, 0), -- Gentle Dari <First Aid Supplies>
(1388, 64051, 870, 1, 1, 1692.047, 971.316, 471.0088, 1.641564, 120, 0, 0), -- Esha the Loommaiden <Tailoring Supplies>
(1389, 63994, 870, 1, 1, 1596.967, 933.7761, 424.0688, 3.311165, 120, 0, 0), -- Challenger Wuli <Challenge Dungeons>
(1390, 64002, 870, 1, 1, 1591.049, 879.2604, 424.0684, 1.681486, 120, 0, 0), -- Sang-Bo <The Shado-Pan>
(1391, 64531, 870, 1, 1, 1582.462, 901.6042, 424.0684, 1.758687, 120, 0, 0), -- Lena Stonebrush <The Lorewalkers>
(1392, 67127, 870, 1, 1, 1610.755, 996.9445, 470.7093, 4.849982, 120, 0, 0), -- Training Dummy
(1393, 67127, 870, 1, 1, 1599.734, 995.4358, 470.7093, 4.938851, 120, 0, 0); -- Training Dummy-- Gentle Dari
DELETE FROM npc_vendor WHERE entry=64062;
INSERT INTO npc_vendor (entry, slot, item, maxcount, incrtime, ExtendedCost, type) VALUES
(64062, 1, 44693, 0, 0, 0, 1), -- Wound Dressing
(64062, 2, 44694, 0, 0, 0, 1); -- Antiseptic-Soaked Dressing

-- Stephen Wong
DELETE FROM npc_vendor WHERE entry=64126;
INSERT INTO npc_vendor (entry, slot, item, maxcount, incrtime, ExtendedCost, type) VALUES
(64126, 1, 74659, 0, 0, 0, 1), -- Farm Chicken
(64126, 2, 74660, 0, 0, 0, 1), -- Pandaren Peach
(64126, 3, 74832, 0, 0, 0, 1), -- Barley
(64126, 4, 74845, 0, 0, 0, 1), -- Ginseng
(64126, 5, 74851, 0, 0, 0, 1), -- Rice
(64126, 6, 74852, 0, 0, 0, 1), -- Yak Milk
(64126, 7, 74854, 0, 0, 0, 1), -- Instant Noodles
(64126, 8, 85583, 0, 0, 0, 1), -- Needle Mushrooms
(64126, 9, 85584, 0, 0, 0, 1), -- Silkworm Pupa
(64126, 10, 85585, 0, 0, 0, 1), -- Red Beans
(64126, 11, 21219, 0, 0, 0, 1), -- Recipe: Sagefish Delight
(64126, 12, 21099, 0, 0, 0, 1), -- Recipe: Smoked Sagefish
(64126, 13, 2678, 0, 0, 0, 1), -- Mild Spices
(64126, 14, 30817, 0, 0, 0, 1), -- Simple Flour
(64126, 15, 159, 0, 0, 0, 1); -- Refreshing Spring Water

-- Jinho the Wind Breaker
DELETE FROM npc_vendor WHERE entry=66998;
INSERT INTO npc_vendor (entry, slot, item, maxcount, incrtime, ExtendedCost, type) VALUES
(66998, 1, 81414, 0, 0, 0, 1), -- Pearl Milk Tea
(66998, 2, 81415, 0, 0, 0, 1), -- Pandaren Plum Wine
(66998, 3, 74636, 0, 0, 0, 1), -- Golden Carp Consomme
(66998, 4, 81406, 0, 0, 0, 1), -- Roasted Barley Tea
(66998, 5, 81407, 0, 0, 0, 1); -- Four Wind Soju

-- Reeler Uko <Fishing Supplies>
DELETE FROM npc_vendor WHERE entry=64585;
INSERT INTO npc_vendor (entry, slot, item, maxcount, incrtime, ExtendedCost, type) VALUES
(64585, 1, 6532, 0, 0, 0, 1), -- Bright Baubles
(64585, 2, 6533, 0, 0, 0, 1), -- Aquadynamic Fish Attractor
(64585, 3, 6530, 0, 0, 0, 1), -- Nightcrawlers
(64585, 4, 6365, 1, 7200, 0, 1), -- Strong Fishing Pole
(64585, 5, 6529, 0, 0, 0, 1), -- Shiny Bauble
(64585, 6, 6256, 0, 0, 0, 1); -- Fishing Pole-- Silk Farmer
-- Wing Nga <Flight Master>
-- Messenger Grummle
-- War Serpent
-- Wild Shaghorn
-- Tina Wang <Barmaid>
-- Krik'thik Needler
-- Tania Summerbreeze <Flightmaster>
-- Barleyflower <Cooking Supplies>
DELETE FROM creature WHERE guid BETWEEN 1394 AND 1447;
INSERT INTO creature (guid, id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, MovementType) VALUES
(1394, 57406, 870, 1, 1, -367.2083, 254.9965, 167.1384, 4.849351, 120, 0, 0), -- Silk Farmer
(1395, 57406, 870, 1, 1, -495.2188, 267.6424, 155.1008, 1.553343, 120, 0, 0), -- Silk Farmer
(1396, 57406, 870, 1, 1, -501.9566, 273.8021, 154.3454, 4.712389, 120, 0, 0), -- Silk Farmer
(1397, 57406, 870, 1, 1, -510.1245, 267.2229, 154.383, 2.590982, 120, 0, 0), -- Silk Farmer
(1398, 57406, 870, 1, 1, -352.4436, 222.8909, 167.0531, 4.9045, 120, 0, 0), -- Silk Farmer
(1399, 57406, 870, 1, 1, -376.693, 267.0203, 166.9409, 6.058611, 120, 0, 0), -- Silk Farmer
(1400, 57406, 870, 1, 1, -364.4028, 303.6701, 170.7308, 6.161012, 120, 0, 0), -- Silk Farmer
(1401, 60231, 870, 1, 1, -221.9236, 461.6528, 185.5253, 1.83272, 120, 0, 0), -- Wing Nga <Flight Master>
(1402, 63778, 870, 1, 1, -216.4792, 507.7517, 190.8452, 4.929741, 120, 0, 0), -- Messenger Grummle
(1403, 65336, 870, 1, 1, 832.4965, 2087.599, 315.9681, 4.021967, 120, 0, 0), -- War Serpent
(1404, 58892, 870, 1, 1, -288.708, 1595.31, 221.77, 3.11328, 120, 0, 0), -- Wild Shaghorn
(1405, 58892, 870, 1, 1, -280.212, 1693.93, 199.488, 1.31213, 120, 0, 0), -- Wild Shaghorn
(1406, 58892, 870, 1, 1, -254.877, 1710.28, 200.856, 4.74561, 120, 0, 0), -- Wild Shaghorn
(1407, 58892, 870, 1, 1, -205.741, 1736.45, 212.121, 2.02389, 120, 0, 0), -- Wild Shaghorn
(1408, 58892, 870, 1, 1, -120.933, 1863.54, 209.35, 1.66291, 120, 0, 0), -- Wild Shaghorn
(1409, 58892, 870, 1, 1, -168.841, 1714.25, 242.608, 4.2159, 120, 0, 0), -- Wild Shaghorn
(1410, 58892, 870, 1, 1, -183.916, 1794.56, 198.994, 0.959511, 120, 0, 0), -- Wild Shaghorn
(1411, 58892, 870, 1, 1, -113.054, 1808.07, 221.502, 4.17372, 120, 0, 0), -- Wild Shaghorn
(1412, 58892, 870, 1, 1, -41.3125, 1840.55, 201.089, 2.61196, 120, 0, 0), -- Wild Shaghorn
(1413, 58892, 870, 1, 1, 136.372, 1801.37, 201.976, 3.19438, 120, 0, 0), -- Wild Shaghorn
(1414, 58892, 870, 1, 1, 140.074, 1803.39, 202.295, 3.15331, 120, 0, 0), -- Wild Shaghorn
(1415, 58892, 870, 1, 1, 135.261, 1805.35, 201.58, 3.11913, 120, 0, 0), -- Wild Shaghorn
(1416, 58892, 870, 1, 1, 127.234, 1840.54, 184.895, 2.92372, 120, 0, 0), -- Wild Shaghorn
(1417, 58892, 870, 1, 1, 123.923, 1842.19, 184.652, 5.3889, 120, 0, 0), -- Wild Shaghorn
(1418, 58892, 870, 1, 1, 122.847, 1839.9, 185.329, 2.98947, 120, 0, 0), -- Wild Shaghorn
(1419, 58892, 870, 1, 1, 124.722, 1804.52, 197.853, 3.33114, 120, 0, 0), -- Wild Shaghorn
(1420, 58892, 870, 1, 1, 2.093, 1783.93, 236.855, 0.783328, 120, 0, 0), -- Wild Shaghorn
(1421, 58892, 870, 1, 1, 8.91582, 1792.21, 228.071, 4.05169, 120, 0, 0), -- Wild Shaghorn
(1422, 58892, 870, 1, 1, 10.275, 1796.99, 224.438, 4.12709, 120, 0, 0), -- Wild Shaghorn
(1423, 58892, 870, 1, 1, 13.3336, 1794.68, 225.127, 3.98364, 120, 0, 0), -- Wild Shaghorn
(1424, 58892, 870, 1, 1, 22.4596, 1808.89, 220.916, 5.81604, 120, 0, 0), -- Wild Shaghorn
(1425, 58892, 870, 1, 1, 360.227, 1991.84, 200.393, 4.76713, 120, 0, 0), -- Wild Shaghorn
(1426, 58892, 870, 1, 1, 367.252, 2091.22, 220.963, 2.83195, 120, 0, 0), -- Wild Shaghorn
(1427, 64040, 870, 1, 1, 1727.42, 888.635, 470.926, 3.44506, 120, 5, 1), -- Tina Wang <Barmaid>
(1428, 63976, 870, 1, 1, 974.628, 2124.5, 296.19, 5.33891, 120, 0, 0), -- Krik'thik Needler
(1429, 63976, 870, 1, 1, 732.873, 2297.68, 388.088, 0.418514, 120, 0, 0), -- Krik'thik Needler
(1430, 63976, 870, 1, 1, 786.179, 2307.85, 381.562, 3.13921, 120, 0, 0), -- Krik'thik Needler
(1431, 63976, 870, 1, 1, 827.302, 2325.71, 381.958, 4.7712, 120, 0, 0), -- Krik'thik Needler
(1432, 63976, 870, 1, 1, 952.93, 2114.18, 296.19, 4.87291, 120, 0, 0), -- Krik'thik Needler
(1433, 63976, 870, 1, 1, 928.913, 2184.52, 363.145, 1.39546, 120, 0, 0), -- Krik'thik Needler
(1434, 63976, 870, 1, 1, 929.644, 2169.67, 363.145, 1.74833, 120, 0, 0), -- Krik'thik Needler
(1435, 63976, 870, 1, 1, 1214.49, 2299.59, 388.122, 0.419279, 120, 0, 0), -- Krik'thik Needler
(1436, 63976, 870, 1, 1, 1182.45, 2305.37, 388.141, 4.62268, 120, 0, 0), -- Krik'thik Needler
(1437, 63976, 870, 1, 1, 1183.6, 2303.73, 388.202, 3.13187, 120, 0, 0), -- Krik'thik Needler
(1438, 63976, 870, 1, 1, 1181.55, 2304.04, 388.19, 0.318344, 120, 0, 0), -- Krik'thik Needler
(1439, 63976, 870, 1, 1, 1176.26, 2303.8, 388.152, 6.27346, 120, 0, 0), -- Krik'thik Needler
(1440, 63976, 870, 1, 1, 958.337, 2152.53, 305.044, 1.68697, 120, 0, 0), -- Krik'thik Needler
(1441, 63976, 870, 1, 1, 983.924, 2197.35, 362.821, 6.09042, 120, 0, 0), -- Krik'thik Needler
(1442, 63976, 870, 1, 1, 966.408, 2132.74, 296.189, 4.83683, 120, 0, 0), -- Krik'thik Needler
(1443, 63976, 870, 1, 1, 941.823, 2124.38, 296.189, 3.93831, 120, 0, 0), -- Krik'thik Needler
(1444, 63976, 870, 1, 1, 963.729, 2114.39, 296.19, 4.51739, 120, 0, 0), -- Krik'thik Needler
(1445, 63976, 870, 1, 1, 731.169, 2300.65, 388.034, 1.68133, 120, 0, 0), -- Krik'thik Needler
(1446, 61122, 870, 1, 1, 1580.09, 888.84, 473.665, 1.65728, 120, 0, 0), -- Tania Summerbreeze <Flightmaster>
(1447, 64067, 870, 1, 1, 1695.06, 883.037, 489.306, 6.14079, 120, 5, 1); -- Barleyflower <Cooking Supplies>



ALTER TABLE `item_template` ADD COLUMN `Flags3` int(10) unsigned NOT NULL DEFAULT '0' AFTER `FlagsExtra`;
UPDATE `creature_template` SET `modelid1`=45045 WHERE `entry`=18281; -- Boglash
UPDATE `creature_template` SET `modelid1`=45045 WHERE `entry`=18680; -- MarticarUPDATE `creature_template_addon` SET `auras`='29266 28559' WHERE `entry`=55464; -- NPC: Hozen Corpse; AURA: Permanent Feign Death, Untrackable;
UPDATE `creature_template_addon` SET `auras`='29266 28559' WHERE `entry`=58533; -- NPC: Dustback Mushan; AURA: Permanent Feign Death, Untrackable;
UPDATE `creature_template_addon` SET `auras`='29266 28559' WHERE `entry`=61806; -- NPC: Liu of the Thousand Blows; AURA: Permanent Feign Death, Untrackable;
UPDATE `creature_template_addon` SET `auras`='29266 28559' WHERE `entry`=61808; -- NPC: Shado-Master Zhiyao; AURA: Permanent Feign Death, Untrackable;
UPDATE `creature_template_addon` SET `auras`='29266 28559' WHERE `entry`=61810; -- NPC: Shiya Boldblade; AURA: Permanent Feign Death, Untrackable;
UPDATE `creature_template_addon` SET `auras`='131531 8279' WHERE `entry`=66928; -- NPC: Sha Shooter; AURA: Sha Shooter Spawn [DNT], Stealth Detection;DELETE FROM disables WHERE sourceType=1 AND entry=4734;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 4734, 'Disable Quest 4734: Egg Freezing');

DELETE FROM disables WHERE sourceType=1 AND entry=2969;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 2969, 'Disable Quest 2969: Freedom for All Creatures');

DELETE FROM disables WHERE sourceType=1 AND entry=5383;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 5383, 'Disable Quest 5383: Krastinov''s Bag of Horrors');

DELETE FROM disables WHERE sourceType=1 AND entry=30520;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 30520, 'Disable Quest 30520: Tummy Trouble');

DELETE FROM disables WHERE sourceType=1 AND entry=9942;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 9942, 'Disable Quest 9942: Tracking Down the Culprits');

DELETE FROM disables WHERE sourceType=1 AND entry=9657;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 9657, 'Disable Quest 9657: Deprecated: Offensive Channeling');

DELETE FROM disables WHERE sourceType=1 AND entry=29259;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 29259, 'Disable Quest 29259: [DEPRECATED] Hide and Seek');

DELETE FROM disables WHERE sourceType=1 AND entry=328;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 328, 'Disable Quest 328: The Hidden Key');

DELETE FROM disables WHERE sourceType=1 AND entry=7962;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 7962, 'Disable Quest 7962: [DEPRECATED] Wabbit Pelts');

DELETE FROM disables WHERE sourceType=1 AND entry=25076;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 25076, 'Disable Quest 25076: Taming the Beast');

DELETE FROM disables WHERE sourceType=1 AND entry=30965;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 30965, 'Disable Quest 30965: Herding Cats');

DELETE FROM disables WHERE sourceType=1 AND entry=498;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 498, 'Disable Quest 498: [DEPRECATED] The Rescue');

DELETE FROM disables WHERE sourceType=1 AND entry=503;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 503, 'Disable Quest 503: [DEPRECATED] Gol''dir');

DELETE FROM disables WHERE sourceType=1 AND entry=613;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 613, 'Disable Quest 613: Cracking Maury''s Foot');

DELETE FROM disables WHERE sourceType=1 AND entry=9655;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 9655, 'Disable Quest 9655: Deprecated: Healing Amplification');

DELETE FROM disables WHERE sourceType=1 AND entry=9347;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 9347, 'Disable Quest 9347: Voidwalkers Gone Wild');

DELETE FROM disables WHERE sourceType=1 AND entry=9656;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 9656, 'Disable Quest 9656: Deprecated: Lethality First');

DELETE FROM disables WHERE sourceType=1 AND entry=14000;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 14000, 'Disable Quest 14000: Hunt or Be Hunted');

DELETE FROM disables WHERE sourceType=1 AND entry=14427;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 14427, 'Disable Quest 14427: Item Quest');

DELETE FROM disables WHERE sourceType=1 AND entry=11934;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 11934, 'Disable Quest 11934: Unlocking Your Potential [PH]');

DELETE FROM disables WHERE sourceType=1 AND entry=9411;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 9411, 'Disable Quest 9411: <UNUSED>Infused Bandages');

DELETE FROM disables WHERE sourceType=1 AND entry=5511;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 5511, 'Disable Quest 5511: The Key to Scholomance');

DELETE FROM disables WHERE sourceType=1 AND entry=9481;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 9481, 'Disable Quest 9481: <UNUSED>Light Consumable');

DELETE FROM disables WHERE sourceType=1 AND entry=31099;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 31099, 'Disable Quest 31099: Klaxx Attack');

DELETE FROM disables WHERE sourceType=1 AND entry=3825;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 3825, 'Disable Quest 3825: Ogre Head On A Stick = Party');

DELETE FROM disables WHERE sourceType=1 AND entry=9614;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 9614, 'Disable Quest 9614: Deprecated: Reforge a Short Blade');

DELETE FROM disables WHERE sourceType=1 AND entry=29046;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 29046, 'Disable Quest 29046: Chef''s Challenge: Steelback');

DELETE FROM disables WHERE sourceType=1 AND entry=2359;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 2359, 'Disable Quest 2359: Klaven''s Tower');

DELETE FROM disables WHERE sourceType=1 AND entry=2841;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 2841, 'Disable Quest 2841: Rig Wars');

DELETE FROM disables WHERE sourceType=1 AND entry=2059;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 2059, 'Disable Quest 2059: <UNUSED>');

DELETE FROM disables WHERE sourceType=1 AND entry=3201;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 3201, 'Disable Quest 3201: At Last!');

DELETE FROM disables WHERE sourceType=1 AND entry=3628;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 3628, 'Disable Quest 3628: You Are Rakh''likh, Demon');

DELETE FROM disables WHERE sourceType=1 AND entry=3802;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 3802, 'Disable Quest 3802: Dark Iron Legacy');

DELETE FROM disables WHERE sourceType=1 AND entry=9976;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 9976, 'Disable Quest 9976: Primal Magic');

DELETE FROM disables WHERE sourceType=1 AND entry=5505;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 5505, 'Disable Quest 5505: The Key to Scholomance');

DELETE FROM disables WHERE sourceType=1 AND entry=26151;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 26151, 'Disable Quest 26151: Dark Letter');
DELETE FROM disables WHERE sourceType=1 AND entry=7521;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 7521, 'Disable Quest 7521: Thunderaan the Windseeker');

DELETE FROM disables WHERE sourceType=1 AND entry=7522;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 7522, 'Disable Quest 7522: Examine the Vessel');

DELETE FROM disables WHERE sourceType=1 AND entry=9459;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 9459, 'Disable Quest 9459: <UNUSED>Anti-crit Potion');

DELETE FROM disables WHERE sourceType=1 AND entry=9482;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 9482, 'Disable Quest 9482: <UNUSED>Nature Resist Potion');

DELETE FROM disables WHERE sourceType=1 AND entry=9478;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 9478, 'Disable Quest 9478: <UNUSED>Portable Healing Font');

DELETE FROM disables WHERE sourceType=1 AND entry=29048;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 29048, 'Disable Quest 29048: Chef''s Challenge: Grumpfin');

DELETE FROM disables WHERE sourceType=1 AND entry=10029;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 10029, 'Disable Quest 10029: The Spirits Are Calling');

DELETE FROM disables WHERE sourceType=1 AND entry=12001;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 12001, 'Disable Quest 12001: zzOLDHunting Tarot: Lesser Beasts');

DELETE FROM disables WHERE sourceType=1 AND entry=10244;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 10244, 'Disable Quest 10244: R.T.F.R.C.M.');

DELETE FROM disables WHERE sourceType=1 AND entry=9975;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 9975, 'Disable Quest 9975: Primal Magic');

DELETE FROM disables WHERE sourceType=1 AND entry=32375;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 32375, 'Disable Quest 32375: A Dash of That');

DELETE FROM disables WHERE sourceType=1 AND entry=10133;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 10133, 'Disable Quest 10133: Mission: Kill the Messenger');

DELETE FROM disables WHERE sourceType=1 AND entry=10135;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 10135, 'Disable Quest 10135: Mission: Be the Messenger');

DELETE FROM disables WHERE sourceType=1 AND entry=10147;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 10147, 'Disable Quest 10147: Mission: Kill the Messenger');

DELETE FROM disables WHERE sourceType=1 AND entry=10148;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 10148, 'Disable Quest 10148: Mission: Be the Messenger');

DELETE FROM disables WHERE sourceType=1 AND entry=9650;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 9650, 'Disable Quest 9650: Deprecated: Magical Augmentation');

DELETE FROM disables WHERE sourceType=1 AND entry=9654;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 9654, 'Disable Quest 9654: Deprecated: A Well Rounded Defense');

DELETE FROM disables WHERE sourceType=1 AND entry=9479;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 9479, 'Disable Quest 9479: <UNUSED>Glorious Standard of the Alliance');

DELETE FROM disables WHERE sourceType=1 AND entry=9308;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 9308, 'Disable Quest 9308: <DEPRECATED>Blood Elf Remains?');

DELETE FROM disables WHERE sourceType=1 AND entry=9660;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 9660, 'Disable Quest 9660: Deprecated: Lethality First');

DELETE FROM disables WHERE sourceType=1 AND entry=14441;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 14441, 'Disable Quest 14441: Garrosh''s Autograph');

DELETE FROM disables WHERE sourceType=1 AND entry=260;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 260, 'Disable Quest 260: <UNUSED>');

DELETE FROM disables WHERE sourceType=1 AND entry=316;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 316, 'Disable Quest 316: <UNUSED 1>');

DELETE FROM disables WHERE sourceType=1 AND entry=6201;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 6201, 'Disable Quest 6201: <UNUSED> The Legacy of the Ashbringer');

DELETE FROM disables WHERE sourceType=1 AND entry=12626;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 12626, 'Disable Quest 12626: Company of the Damned');

DELETE FROM disables WHERE sourceType=1 AND entry=9231;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 9231, 'Disable Quest 9231: reuse');

DELETE FROM disables WHERE sourceType=1 AND entry=912;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 912, 'Disable Quest 912: Stonesplinter Trogg Disguise');

DELETE FROM disables WHERE sourceType=1 AND entry=2058;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 2058, 'Disable Quest 2058: <UNUSED>');

DELETE FROM disables WHERE sourceType=1 AND entry=3111;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 3111, 'Disable Quest 3111: <UNUSED> [DEPRECATED]');

DELETE FROM disables WHERE sourceType=1 AND entry=29550;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 29550, 'Disable Quest 29550: Saving Snowpuff');

DELETE FROM disables WHERE sourceType=1 AND entry=4299;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 4299, 'Disable Quest 4299: <NYI> <TXT>The Tomb of the Seven');

DELETE FROM disables WHERE sourceType=1 AND entry=7561;
INSERT INTO disables (sourceType, entry, comment) VALUES (1, 7561, 'Disable Quest 7561: Rise, Thunderfury!');DELETE FROM trinity_string WHERE entry IN (882, 883, 884, 885);
INSERT INTO trinity_string (entry, content_default) VALUES
(882, '* is reforger (%u)'),
(883, '* is transmogrifier (%u)'),
(884, '* is vaultkeeper (%u)');
-- New 5 Man Instance Template
DELETE FROM instance_template WHERE map IN (961, 994, 959, 962, 1011, 960);
INSERT INTO instance_template (map, parent) VALUES
(961, 870), -- Stormstout Brewery
(994, 870), -- Mogushan Palace
(959, 870), -- Shado-Pan Monastery
(962, 870), -- Gate of the Setting Sun
(1011, 870), -- Siege of Niuzao Temple
(960, 870); -- Temple of the Jade Serpent-- New AreaTrigger Teleport Locations To 5 Man Dungeons.
DELETE FROM areatrigger_teleport WHERE id IN (8134, 7705, 7694, 7726, 8315, 7854, 8135, 7755, 7699, 7725, 8312, 7687);
INSERT INTO areatrigger_teleport (id, name, target_map, target_position_x, target_position_y, target_position_z, target_orientation) VALUES
-- ENTER INSTANCES
(8134, 'Mogushan Palace - Entrance Target', 994, -3969.67, -2542.71, 26.7537, 4.71239), -- Mogushan Palace
(7705, 'Stormstout Brewery - Entrance Target', 961, -732.115, 1266.13, 116.108, 1.81208), -- Stormstout Brewery
(7694, 'Shado-Pan Monastery - Entrance Target', 959, 3657.29, 2551.92, 766.966, 0.436332), -- Shado-Pan Monastery
(7726, 'Gate of the Setting Sun - Entrance Target', 962, 722.097, 2108.08, 402.978, 1.59264), -- Gate of the Setting Sun
(8315, 'Siege of Niuzao Temple - Entrance Target', 1011, 1455.33, 5100.01, 149.415, 0.959931), -- Siege of Niuzao Temple
(7854, 'Temple of the Jade Serpent - Entrance Target', 960, 953.37, -2487.5, 180.431, 4.36908), -- Temple of the Jade Serpent

-- EXIT INSTANCES
(8135, 'Mogushan Palace - Exit Target', 870, 1391.83, 437.694, 478.941, 2.31752), -- Mogushan Palace
(7755, 'Stormstout Brewery - Exit Target', 870, -713.356, 1263.42, 136.223, 0.268626), -- Stormstout Brewery
(7699, 'Shado-Pan Monastery - Exit Target', 870, 3631.29, 2538.41, 769.912, 3.61283), -- Shado-Pan Monastery
(7725, 'Gate of the Setting Sun - Exit Target', 870, 692.365, 2080.17, 374.694, 3.18706), -- Gate of the Setting Sun
(8312, 'Siege of Niuzao Temple - Exit Target', 870, 1433.27, 5085.39, 133.807, 3.57792), -- Temple of the Jade Serpent
(7687, 'Temple of the Jade Serpent - Exit Target', 870, 957.884, -2474.01, 180.505, 1.29244); -- Temple of the Jade Serpent-- Gilneas Default Terrain Map Swap
DELETE FROM phase_definitions WHERE zoneId=4755;
INSERT INTO phase_definitions (zoneId, entry, phasemask, phaseId, terrainswapmap, flags, comment) VALUES
(4755, 1, 0, 170, 638, 0, 'Gilneas: Default Terrainswap');
-- Remove old instance templates for scarlet halls and scholomance
DELETE FROM instance_template WHERE map IN (289, 189);

-- Remove creatures from old scarlet halls and scholomance
DELETE FROM creature WHERE map IN (189, 289);

-- Remove gameobjects from old scarlet halls and scholomance
DELETE FROM gameobject WHERE map IN (189, 289);

-- Remove access_requirement from old scarlet halls and scholomance
DELETE FROM access_requirement WHERE mapId IN (189, 289);

DELETE FROM instance_template WHERE map IN (1001, 1004, 1007);
INSERT INTO instance_template (map, parent) VALUES
(1001, 0), -- Scarlet Halls
(1004, 0), -- Scarlet Monastery
(1007, 0); -- Scholomance

DELETE FROM areatrigger_teleport WHERE id IN (45, 614, 2567, 7801, 7802, 7539);
INSERT INTO areatrigger_teleport VALUES 
-- ENTER INSTANCE
(614, 'Scarlet Halls - Entrance Target', 1001, 820.743, 607.812, 13.6389, 0), -- Scarlet Halls
(45, 'Scarlet Monastery - Entrance Target', 1004, 1124.64, 512.467, 0.989549, 1.5708), -- Scarlet Monastery
(2567, 'Scholomance - Entrance Target', 1007, 199.876, 125.346, 138.43, 4.67748), -- Scholomance

-- EXIT INSTANCE
(7801, 'Scarlet Halls - Exit Target', 0, 2869.32, -820.818, 160.333, 0.349066), -- Scarlet Halls
(7802, 'Scarlet Monastery - Exit Target', 0, 2915.34, -801.58, 160.333, 3.49066), -- Scarlet Monastery
(7539, 'Scholomance - Exit Target', 0, 1273.91, -2553.09, 90.6353, 3.57792); -- Scholomance

DELETE FROM access_requirement WHERE mapId IN (961, 994, 959, 962, 1011, 960, 1001, 1004, 1007);
INSERT INTO `access_requirement` (`mapId`, `difficulty`, `level_min`, `level_max`, `item`, `item2`, `quest_done_A`, `quest_done_H`, `completed_achievement`, `quest_failed_text`, `comment`) VALUES
(961, 0, 85, 0, 0, 0, 0, 0, 0, NULL, 'Stormstout Brewery - Entrance'),
(961, 1, 90, 0, 0, 0, 0, 0, 0, NULL, 'Stormstout Brewery (H) - Entrance'),
(994, 0, 87, 0, 0, 0, 0, 0, 0, NULL, 'Mogushan Palace - Entrance'),
(994, 1, 90, 0, 0, 0, 0, 0, 0, NULL, 'Mogushan Palace (H) - Entrance'),
(959, 0, 87, 0, 0, 0, 30752, 30752, 0, NULL, 'Shado-Pan Monastery - Entrance'),
(959, 1, 90, 0, 0, 0, 0, 0, 0, NULL, 'Shado-Pan Monastery (H) - Entrance'),
(962, 0, 88, 0, 0, 0, 0, 0, 0, NULL, 'Gate of the Setting Sun - Entrance'),
(962, 1, 90, 0, 0, 0, 0, 0, 0, NULL, 'Gate of the Setting Sun (H) - Entrance'),
(1011, 0, 90, 0, 0, 0, 0, 0, 0, NULL, 'Siege of Niuzao Temple - Entrance'),
(1011, 1, 90, 0, 0, 0, 0, 0, 0, NULL, 'Siege of Niuzao Temple (H) - Entrance'),
(960, 0, 85, 0, 0, 0, 0, 0, 0, NULL, 'Temple of the Jade Serpent - Entrance'),
(960, 1, 90, 0, 0, 0, 0, 0, 0, NULL, 'Temple of the Jade Serpent (H) - Entrance'),
(1001, 0, 21, 0, 0, 0, 0, 0, 0, NULL, 'Scarlet Halls - Entrance'),
(1001, 1, 90, 0, 0, 0, 0, 0, 0, NULL, 'Scarlet Halls (H) - Entrance'),
(1004, 0, 23, 0, 0, 0, 0, 0, 0, NULL, 'Scarlet Monastery - Entrance'),
(1004, 1, 90, 0, 0, 0, 0, 0, 0, NULL, 'Scarlet Monastery (H) - Entrance'),
(1007, 0, 33, 0, 0, 0, 0, 0, 0, NULL, 'Scholomance - Entrance'),
(1007, 1, 90, 0, 0, 0, 0, 0, 0, NULL, 'Scholomance (H) - Entrance');DELETE FROM `playercreateinfo_spell` WHERE Spell IN (119467, 125439);
DELETE FROM creature WHERE map=962;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES 
(1524, 56484, 962, 3, 1, 816.573, 2373.01, 390.863, 1.41372, 7200, 0, 0), -- Serpent's Spine Marksman
(1525, 56484, 962, 3, 1, 819.823, 2373.44, 390.863, 1.41372, 7200, 0, 0), -- Serpent's Spine Marksman
(1526, 56484, 962, 3, 1, 823.097, 2373.34, 390.863, 1.41372, 7200, 0, 0), -- Serpent's Spine Marksman
(1527, 56484, 962, 3, 1, 825.554, 2373.45, 390.539, 1.41372, 7200, 0, 0), -- Serpent's Spine Marksman
(1528, 56484, 962, 3, 1, 828.427, 2373.43, 390.539, 1.41372, 7200, 0, 0), -- Serpent's Spine Marksman
(1529, 56484, 962, 3, 1, 830.695, 2373.11, 390.192, 1.41372, 7200, 0, 0), -- Serpent's Spine Marksman
(1530, 56484, 962, 3, 1, 832.896, 2373.7, 390.539, 1.41372, 7200, 0, 0), -- Serpent's Spine Marksman
(1531, 56484, 962, 3, 1, 835.84, 2373, 390.539, 1.34275, 7200, 0, 0), -- Serpent's Spine Marksman
(1532, 56484, 962, 3, 1, 838.977, 2372.64, 390.863, 1.41372, 7200, 0, 0), -- Serpent's Spine Marksman
(1533, 56484, 962, 3, 1, 841.816, 2373.12, 390.863, 1.41372, 7200, 0, 0), -- Serpent's Spine Marksman
(1534, 56484, 962, 3, 1, 846.372, 2373.47, 390.863, 1.48353, 7200, 0, 0), -- Serpent's Spine Marksman
(1535, 56484, 962, 3, 1, 850.611, 2373.54, 390.863, 1.48353, 7200, 0, 0), -- Serpent's Spine Marksman
(1536, 56484, 962, 3, 1, 855.154, 2373.41, 390.863, 1.48353, 7200, 0, 0), -- Serpent's Spine Marksman
(1537, 56484, 962, 3, 1, 897.003, 2397.55, 390.865, 6.17846, 7200, 0, 0), -- Serpent's Spine Marksman
(1538, 56484, 962, 3, 1, 902.195, 2397.68, 390.865, 6.17846, 7200, 0, 0), -- Serpent's Spine Marksman
(1539, 56484, 962, 3, 1, 902.528, 2411.92, 390.865, 6.17846, 7200, 0, 0), -- Serpent's Spine Marksman
(1540, 56484, 962, 3, 1, 896.979, 2410.4, 390.863, 6.17846, 7200, 0, 0), -- Serpent's Spine Marksman
(1541, 56484, 962, 3, 1, 902.165, 2403.39, 390.863, 6.17846, 7200, 0, 0), -- Serpent's Spine Marksman
(1542, 56484, 962, 3, 1, 1058.4, 2376.16, 390.863, 1.41372, 7200, 0, 0), -- Serpent's Spine Marksman
(1543, 56484, 962, 3, 1, 1062.84, 2376.19, 390.863, 1.41372, 7200, 0, 0), -- Serpent's Spine Marksman
(1544, 56484, 962, 3, 1, 1075.22, 2377.05, 390.863, 1.41372, 7200, 0, 0), -- Serpent's Spine Marksman
(1545, 56484, 962, 3, 1, 1071.45, 2377.41, 390.841, 1.41372, 7200, 0, 0), -- Serpent's Spine Marksman
(1546, 56484, 962, 3, 1, 1067.58, 2376.27, 390.863, 1.41372, 7200, 0, 0), -- Serpent's Spine Marksman
(1547, 56484, 962, 3, 1, 1086.67, 2381.93, 390.863, 1.41372, 7200, 0, 0), -- Serpent's Spine Marksman
(1548, 56484, 962, 3, 1, 1082.32, 2385.14, 390.863, 1.48353, 7200, 0, 0), -- Serpent's Spine Marksman
(1549, 56484, 962, 3, 1, 1091.38, 2379.88, 390.863, 1.41372, 7200, 0, 0), -- Serpent's Spine Marksman
(1553, 56484, 962, 3, 1, 1090.47, 2384.82, 390.863, 1.48353, 7200, 0, 0), -- Serpent's Spine Marksman
(1556, 56484, 962, 3, 1, 1080.15, 2379.8, 390.863, 1.41372, 7200, 0, 0), -- Serpent's Spine Marksman
(1558, 56484, 962, 3, 1, 1088.77, 2377.49, 390.539, 1.41372, 7200, 0, 0), -- Serpent's Spine Marksman
(1562, 56484, 962, 3, 1, 1095.07, 2376.69, 390.865, 1.48353, 7200, 0, 0), -- Serpent's Spine Marksman
(1563, 56484, 962, 3, 1, 1083.54, 2377.16, 390.863, 1.41372, 7200, 0, 0), -- Serpent's Spine Marksman

(1564, 56501, 962, 3, 1, 806.207, 2454.77, 325.583, 3.08923, 7200, 0, 0), -- Panda Archer Target Stalker
(1565, 56501, 962, 3, 1, 815.398, 2457.46, 328.086, 3.08923, 7200, 0, 0), -- Panda Archer Target Stalker
(1566, 56501, 962, 3, 1, 823.123, 2459.17, 324.393, 3.08923, 7200, 0, 0), -- Panda Archer Target Stalker
(1567, 56501, 962, 3, 1, 830.875, 2459.58, 327.067, 3.08923, 7200, 0, 0), -- Panda Archer Target Stalker
(1568, 56501, 962, 3, 1, 841.899, 2460.6, 325.806, 3.08923, 7200, 0, 0), -- Panda Archer Target Stalker
(1569, 56501, 962, 3, 1, 847.403, 2461.05, 327.066, 3.08923, 7200, 0, 0), -- Panda Archer Target Stalker
(1570, 56501, 962, 3, 1, 858.611, 2461.33, 320.914, 3.08923, 7200, 0, 0), -- Panda Archer Target Stalker
(1571, 56501, 962, 3, 1, 866.7, 2458.77, 320.797, 3.08923, 7200, 0, 0), -- Panda Archer Target Stalker
(1572, 56501, 962, 3, 1, 1047.01, 2438.68, 346.342, 3.08923, 7200, 0, 0), -- Panda Archer Target Stalker
(1573, 56501, 962, 3, 1, 1056.95, 2432.9, 351.001, 3.08923, 7200, 0, 0), -- Panda Archer Target Stalker
(1574, 56501, 962, 3, 1, 1063.61, 2437.31, 350.444, 3.08923, 7200, 0, 0), -- Panda Archer Target Stalker
(1575, 56501, 962, 3, 1, 1071.77, 2437.81, 353.422, 3.08923, 7200, 0, 0), -- Panda Archer Target Stalker
(1576, 56501, 962, 3, 1, 1083.42, 2436.91, 362.596, 3.08923, 7200, 0, 0), -- Panda Archer Target Stalker
(1577, 56501, 962, 3, 1, 1093.36, 2431.13, 363.045, 3.08923, 7200, 0, 0), -- Panda Archer Target Stalker
(1578, 56501, 962, 3, 1, 1100.02, 2435.54, 366.47, 3.08923, 7200, 0, 0), -- Panda Archer Target Stalker
(1579, 56501, 962, 3, 1, 1108.18, 2436.04, 366.454, 3.08923, 7200, 0, 0), -- Panda Archer Target Stalker

(1580, 56502, 962, 3, 1, 953.674, 2425, 323.069, 4.72409, 7200, 0, 0), -- Panda Archer Target Gate Stalker
(1581, 56502, 962, 3, 1, 952.176, 2429.7, 334.453, 4.72409, 7200, 0, 0), -- Panda Archer Target Gate Stalker
(1582, 56502, 962, 3, 1, 955.958, 2423.43, 318.587, 4.72409, 7200, 0, 0), -- Panda Archer Target Gate Stalker
(1583, 56502, 962, 3, 1, 962.712, 2423.31, 318.5, 4.72409, 7200, 0, 0), -- Panda Archer Target Gate Stalker
(1584, 56502, 962, 3, 1, 965.665, 2425.78, 323.089, 4.72409, 7200, 0, 0), -- Panda Archer Target Gate Stalker

(1585, 56636, 962, 3, 1, 1264.23, 2304.49, 381.428, 0, 7200, 0, 0), -- Commander Ri'mok

(1586, 56706, 962, 3, 1, 1270.47, 2241.04, 441.438, 4.97022, 7200, 0, 0), -- Krik'thik Bombardier
(1587, 56706, 962, 3, 1, 1275.09, 2258.93, 447.501, 3.88834, 7200, 0, 0), -- Krik'thik Bombardier
(1588, 56706, 962, 3, 1, 1267.11, 2331.62, 411.022, 1.54921, 7200, 0, 0), -- Krik'thik Bombardier
(1589, 56706, 962, 3, 1, 1281.02, 2262.2, 417.085, 3.32515, 7200, 0, 0), -- Krik'thik Bombardier
(1590, 56706, 962, 3, 1, 1303.23, 2251.35, 442.911, 0.239436, 7200, 0, 0), -- Krik'thik Bombardier
(1591, 56706, 962, 3, 1, 1277.01, 2351.99, 408.726, 0.752908, 7200, 0, 0), -- Krik'thik Bombardier
(1592, 56706, 962, 3, 1, 1305.11, 2241.47, 419.18, 4.68102, 7200, 0, 0), -- Krik'thik Bombardier
(1593, 56706, 962, 3, 1, 1306.68, 2235.97, 429.533, 4.61602, 7200, 0, 0), -- Krik'thik Bombardier
(1594, 56706, 962, 3, 1, 1304.3, 2248.35, 416.64, 4.67113, 7200, 0, 0), -- Krik'thik Bombardier
(1595, 56706, 962, 3, 1, 1286.04, 2285.42, 409.066, 2.40876, 7200, 0, 0), -- Krik'thik Bombardier
(1596, 56706, 962, 3, 1, 1279.99, 2290.9, 422.056, 5.49978, 7200, 0, 0), -- Krik'thik Bombardier
(1597, 56706, 962, 3, 1, 1287.17, 2311.3, 427.976, 4.74121, 7200, 0, 0), -- Krik'thik Bombardier
(1598, 56706, 962, 3, 1, 1265.02, 2291.33, 411.847, 1.79741, 7200, 0, 0), -- Krik'thik Bombardier
(1599, 56706, 962, 3, 1, 1292.24, 2282.22, 418.732, 1.40926, 7200, 0, 0), -- Krik'thik Bombardier
(1600, 56706, 962, 3, 1, 1285.79, 2303.4, 407.609, 4.6891, 7200, 0, 0), -- Krik'thik Bombardier
(1601, 56706, 962, 3, 1, 1307.15, 2247.11, 424.889, 4.02988, 7200, 0, 0), -- Krik'thik Bombardier
(1602, 56706, 962, 3, 1, 1279.42, 2247.3, 456.654, 2.429, 7200, 0, 0), -- Krik'thik Bombardier
(1603, 56706, 962, 3, 1, 1343.85, 2294.86, 422.466, 0, 7200, 0, 0), -- Krik'thik Bombardier

(1604, 56709, 962, 3, 1, 1284.21, 2302.29, 383.368, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1605, 56709, 962, 3, 1, 1287.85, 2298.67, 384.455, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1606, 56709, 962, 3, 1, 1291.01, 2308.77, 385.568, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1607, 56709, 962, 3, 1, 1297.49, 2300.09, 387.11, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1608, 56709, 962, 3, 1, 1287.56, 2305.78, 384.141, 6.23082, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1609, 56709, 962, 3, 1, 1276.55, 2311.73, 382.097, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1610, 56709, 962, 3, 1, 1274.98, 2296.37, 381.889, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1611, 56709, 962, 3, 1, 1273.32, 2304.07, 381.345, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1612, 56709, 962, 3, 1, 1279.13, 2304.03, 382.114, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1613, 56709, 962, 3, 1, 1294.11, 2303.74, 386.043, 4.53786, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1614, 56709, 962, 3, 1, 1283.33, 2311.8, 383.125, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1615, 56709, 962, 3, 1, 1280.91, 2297.69, 382.674, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1616, 56709, 962, 3, 1, 1297.24, 2310.01, 387.245, 4.53786, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1617, 56709, 962, 3, 1, 1269.98, 2309.2, 381.653, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1618, 56709, 962, 3, 1, 1267.95, 2300.16, 381.653, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1619, 56709, 962, 3, 1, 1293.19, 2295.76, 385.935, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1620, 56709, 962, 3, 1, 1299.62, 2304.86, 387.401, 4.53786, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1621, 56709, 962, 3, 1, 1303.7, 2308.85, 388.81, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1622, 56709, 962, 3, 1, 1310.52, 2305.35, 390.304, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1623, 56709, 962, 3, 1, 1308.42, 2298.58, 389.906, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1624, 56709, 962, 3, 1, 1313.03, 2308.51, 391.388, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1625, 56709, 962, 3, 1, 1329.93, 2298.3, 394.654, 0.314159, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1626, 56709, 962, 3, 1, 1325.16, 2306.67, 393.986, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1627, 56709, 962, 3, 1, 1312.93, 2295.23, 392.469, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1628, 56709, 962, 3, 1, 1325.21, 2294.34, 394.221, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1629, 56709, 962, 3, 1, 1304.79, 2302.48, 388.669, 0, 7200, 5, 1), -- Flying Mantid Bombing Stalker
(1630, 56709, 962, 3, 1, 1325.87, 2301.78, 393.851, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1631, 56709, 962, 3, 1, 1308.6, 2311.64, 390.122, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1632, 56709, 962, 3, 1, 1301.53, 2297.18, 388.006, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1633, 56709, 962, 3, 1, 1319.42, 2296.04, 392.985, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1634, 56709, 962, 3, 1, 1333.22, 2309.17, 395.298, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1635, 56709, 962, 3, 1, 1319.99, 2309.69, 392.922, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker
(1718, 56709, 962, 3, 1, 1317.69, 2300.85, 392.129, 0, 7200, 0, 0), -- Flying Mantid Bombing Stalker

(1719, 56742, 962, 3, 1, 1212.68, 2308.27, 388.04, 4.59022, 7200, 0, 0), -- Serpent's Spine Defender

(1720, 56743, 962, 3, 1, 1212.49, 2300.65, 388.037, 1.64061, 7200, 0, 0), -- Serpent's Spine Defender

(1721, 56745, 962, 3, 1, 1209.17, 2301.86, 388.036, 0.645772, 7200, 0, 0), -- Serpent's Spine Trainee

(1722, 56799, 962, 3, 1, 1078.64, 2335.16, 450.867, 4.13453, 7200, 0, 0), -- Krik'thik Bombadier
(1724, 56799, 962, 3, 1, 970.028, 2464.49, 431.254, 3.07057, 7200, 0, 0), -- Krik'thik Bombadier
(1725, 56799, 962, 3, 1, 1151.62, 2411.25, 462.444, 3.72346, 7200, 0, 0), -- Krik'thik Bombadier
(1726, 56799, 962, 3, 1, 940.662, 2431.32, 442.264, 5.75113, 7200, 0, 0), -- Krik'thik Bombadier

(1727, 56822, 962, 3, 1, 1238.01, 2304.64, 430.908, 0, 7200, 0, 0), -- Boss 2 North Stalker
(1728, 56823, 962, 3, 1, 1195.3, 2348.94, 431.004, 0, 7200, 0, 0), -- Boss 2 West Stalker
(1729, 56824, 962, 3, 1, 1153.4, 2304.58, 430.908, 0, 7200, 0, 0), -- Boss 2 South Stalker
(1730, 56825, 962, 3, 1, 1195.39, 2263.44, 430.908, 0, 7200, 0, 0), -- Boss 2 East Stalker

(1731, 56875, 962, 3, 1, 694.762, 2214, 335.953, 5.38806, 7200, 0, 0), -- Krik'thik Demolisher
(1732, 56875, 962, 3, 1, 683.388, 2214.27, 336.545, 5.35761, 7200, 0, 0), -- Krik'thik Demolisher
(1733, 56875, 962, 3, 1, 643.521, 2233.37, 385.189, 0.168366, 7200, 0, 0), -- Krik'thik Demolisher

(1734, 56876, 962, 3, 1, 690.162, 2209.4, 338.299, 5.3338, 7200, 0, 0), -- Krik'thik Sapper
(1735, 56876, 962, 3, 1, 648.658, 2245.12, 393.004, 0.0185368, 7200, 0, 0), -- Krik'thik Sapper
(1736, 56876, 962, 3, 1, 638.32, 2236.86, 394.267, 0.137414, 7200, 0, 0), -- Krik'thik Sapper

(1737, 56889, 962, 3, 1, 716.668, 2195.68, 386.449, 4.41561, 7200, 0, 0), -- Serpent's Spine Defender
(1738, 56889, 962, 3, 1, 727.45, 2196.79, 386.154, 4.84008, 7200, 0, 0), -- Serpent's Spine Defender

(1739, 56890, 962, 3, 1, 718.26, 2140.09, 401.531, 6.22961, 7200, 0, 0), -- Krik'thik Infiltrator
(1740, 56890, 962, 3, 1, 727.753, 2191.33, 387.598, 1.62634, 7200, 0, 0), -- Krik'thik Infiltrator
(1743, 56890, 962, 3, 1, 715.45, 2191.69, 387.502, 1.27395, 7200, 0, 0), -- Krik'thik Infiltrator

(1744, 56906, 962, 3, 1, 721.816, 2322.56, 388.027, 1.46518, 7200, 5, 1), -- Saboteur Kip'tilak

(1745, 56913, 962, 3, 1, 1179.24, 2300.14, 430.9, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1746, 56913, 962, 3, 1, 1181.36, 2329.39, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1747, 56913, 962, 3, 1, 1178.93, 2307.53, 430.899, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1748, 56913, 962, 3, 1, 1185.92, 2345.01, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1749, 56913, 962, 3, 1, 1178.7, 2316.77, 430.906, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1750, 56913, 962, 3, 1, 1186.31, 2337.17, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1751, 56913, 962, 3, 1, 1184.65, 2324.6, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1752, 56913, 962, 3, 1, 1185.65, 2318.5, 430.901, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1753, 56913, 962, 3, 1, 1184.77, 2309.17, 430.894, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1754, 56913, 962, 3, 1, 1189.13, 2331.75, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1755, 56913, 962, 3, 1, 1183.65, 2299.71, 430.895, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1756, 56913, 962, 3, 1, 1181.72, 2291.49, 430.902, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1757, 56913, 962, 3, 1, 1191.83, 2341.68, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1758, 56913, 962, 3, 1, 1192.63, 2346.93, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1759, 56913, 962, 3, 1, 1180.6, 2280.15, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1760, 56913, 962, 3, 1, 1192.67, 2325.61, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1761, 56913, 962, 3, 1, 1184.68, 2286.16, 430.907, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1762, 56913, 962, 3, 1, 1192.63, 2312.08, 430.887, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1763, 56913, 962, 3, 1, 1191.21, 2304.45, 431.261, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1764, 56913, 962, 3, 1, 1188.62, 2292.36, 430.897, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1765, 56913, 962, 3, 1, 1197.71, 2342.06, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1766, 56913, 962, 3, 1, 1197.13, 2333.91, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1767, 56913, 962, 3, 1, 1195.09, 2319.56, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1768, 56913, 962, 3, 1, 1185.15, 2272.61, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1769, 56913, 962, 3, 1, 1187.06, 2277.89, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1770, 56913, 962, 3, 1, 1201.41, 2346.72, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1771, 56913, 962, 3, 1, 1191.31, 2283.92, 430.905, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1772, 56913, 962, 3, 1, 1195.31, 2298.11, 431.229, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1773, 56913, 962, 3, 1, 1201.8, 2327.67, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1774, 56913, 962, 3, 1, 1199.26, 2310.5, 431.173, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1775, 56913, 962, 3, 1, 1185.51, 2262.1, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1776, 56913, 962, 3, 1, 1201.89, 2319.73, 430.903, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1777, 56913, 962, 3, 1, 1204.85, 2342.96, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1778, 56913, 962, 3, 1, 1204.4, 2334.65, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1779, 56913, 962, 3, 1, 1200.15, 2303.6, 431.169, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1780, 56913, 962, 3, 1, 1196.09, 2287.6, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1781, 56913, 962, 3, 1, 1190.93, 2268.9, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1782, 56913, 962, 3, 1, 1194.26, 2275.46, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1783, 56913, 962, 3, 1, 1200.99, 2293.04, 430.895, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1784, 56913, 962, 3, 1, 1209.79, 2328.4, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1785, 56913, 962, 3, 1, 1208.8, 2321.47, 430.907, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1786, 56913, 962, 3, 1, 1208.71, 2312.11, 430.899, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1787, 56913, 962, 3, 1, 1200.28, 2278.51, 430.908, 1.5708, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1788, 56913, 962, 3, 1, 1202.68, 2286.4, 430.904, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1791, 56913, 962, 3, 1, 1205.59, 2296.97, 430.894, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1792, 56913, 962, 3, 1, 1195.41, 2260.94, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1793, 56913, 962, 3, 1, 1209.26, 2302.91, 430.898, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1794, 56913, 962, 3, 1, 1200.18, 2268.23, 430.908, 1.5708, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1795, 56913, 962, 3, 1, 1209, 2289.48, 430.905, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1796, 56913, 962, 3, 1, 1210.3, 2292.93, 430.904, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1797, 56913, 962, 3, 1, 1205.96, 2271.91, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1798, 56913, 962, 3, 1, 1209.49, 2279.71, 430.908, 1.53589, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers
(1799, 56913, 962, 3, 1, 1205.22, 2262.39, 430.908, 1.5708, 7200, 0, 0), -- Boss 2 West/East Strafe Stalkers

(1800, 56932, 962, 3, 1, 1155.3, 2292.52, 418.644, 0.0174533, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1801, 56932, 962, 3, 1, 1153.21, 2309.71, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1802, 56932, 962, 3, 1, 1159.31, 2306.86, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1803, 56932, 962, 3, 1, 1164.83, 2292.04, 430.932, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1804, 56932, 962, 3, 1, 1154.47, 2312.21, 374.48, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1805, 56932, 962, 3, 1, 1178.49, 2313.71, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1806, 56932, 962, 3, 1, 1176.44, 2307.02, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1807, 56932, 962, 3, 1, 1160.99, 2297.71, 431.004, 0.0174533, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1809, 56932, 962, 3, 1, 1172.37, 2317.63, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1810, 56932, 962, 3, 1, 1153.58, 2302.29, 372.551, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1811, 56932, 962, 3, 1, 1171.28, 2297.9, 430.908, 0.0174533, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1812, 56932, 962, 3, 1, 1164.96, 2312.87, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1813, 56932, 962, 3, 1, 1170.29, 2311.1, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1814, 56932, 962, 3, 1, 1155.34, 2294.66, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1815, 56932, 962, 3, 1, 1155.47, 2315.49, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1816, 56932, 962, 3, 1, 1172.72, 2288.72, 431.004, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1817, 56932, 962, 3, 1, 1151.5, 2300.33, 431.004, 0.0174533, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1818, 56932, 962, 3, 1, 1168.06, 2303.83, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1819, 56932, 962, 3, 1, 1180.25, 2302.34, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1821, 56932, 962, 3, 1, 1183.74, 2316.8, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1824, 56932, 962, 3, 1, 1179.23, 2295.72, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1825, 56932, 962, 3, 1, 1184.81, 2309.94, 430.906, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1826, 56932, 962, 3, 1, 1182.47, 2289.48, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1827, 56932, 962, 3, 1, 1187.34, 2307.64, 430.888, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1828, 56932, 962, 3, 1, 1185.81, 2297.59, 430.905, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1829, 56932, 962, 3, 1, 1192.31, 2319.52, 430.905, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1830, 56932, 962, 3, 1, 1192.01, 2315.11, 430.9, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1831, 56932, 962, 3, 1, 1185.96, 2288.27, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1832, 56932, 962, 3, 1, 1190.73, 2303.4, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1833, 56932, 962, 3, 1, 1189.87, 2293.1, 430.904, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1834, 56932, 962, 3, 1, 1196.95, 2307.67, 430.888, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1835, 56932, 962, 3, 1, 1199.69, 2320.04, 430.9, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1836, 56932, 962, 3, 1, 1196.35, 2298.72, 430.89, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1837, 56932, 962, 3, 1, 1201.5, 2314.25, 430.892, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1838, 56932, 962, 3, 1, 1195.91, 2289.59, 430.902, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1839, 56932, 962, 3, 1, 1205.64, 2306.02, 430.891, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1840, 56932, 962, 3, 1, 1208.93, 2320.51, 430.9, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1841, 56932, 962, 3, 1, 1205.66, 2298.57, 430.894, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1842, 56932, 962, 3, 1, 1205.1, 2290.39, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1843, 56932, 962, 3, 1, 1210.85, 2313.62, 430.89, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1844, 56932, 962, 3, 1, 1212.17, 2304.21, 431.322, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1845, 56932, 962, 3, 1, 1212.52, 2297.42, 431.336, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1846, 56932, 962, 3, 1, 1216.92, 2314.78, 430.899, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1847, 56932, 962, 3, 1, 1218.14, 2306.8, 430.895, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1848, 56932, 962, 3, 1, 1214.45, 2290.56, 430.901, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1849, 56932, 962, 3, 1, 1234.19, 2308.07, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1850, 56932, 962, 3, 1, 1226.56, 2302.57, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1851, 56932, 962, 3, 1, 1235.82, 2295.09, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1852, 56932, 962, 3, 1, 1224.18, 2310.5, 430.905, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1853, 56932, 962, 3, 1, 1234.73, 2302.2, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1854, 56932, 962, 3, 1, 1221.4, 2289.76, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1855, 56932, 962, 3, 1, 1227.51, 2295.32, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1856, 56932, 962, 3, 1, 1220.45, 2297.72, 430.901, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1857, 56932, 962, 3, 1, 1237.36, 2314.08, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1858, 56932, 962, 3, 1, 1221.62, 2318.21, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1859, 56932, 962, 3, 1, 1239.46, 2307.42, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1860, 56932, 962, 3, 1, 1239.48, 2298.63, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers
(1861, 56932, 962, 3, 1, 1229.53, 2313.46, 430.908, 6.26573, 7200, 0, 0), -- Boss 2 North/South Strafe Stalkers

(1862, 58108, 962, 3, 1, 1028.28, 2362.36, 392.002, 0.568619, 7200, 0, 0), -- Krik'thik Infiltrator
(1863, 58108, 962, 3, 1, 855.168, 2291.76, 366.547, 2.73499, 7200, 0, 0), -- Krik'thik Infiltrator
(1864, 58108, 962, 3, 1, 855.089, 2311.14, 366.547, 3.27507, 7200, 0, 0), -- Krik'thik Infiltrator
(1865, 58108, 962, 3, 1, 855.585, 2301.48, 366.547, 3.03068, 7200, 0, 0), -- Krik'thik Infiltrator
(1866, 58108, 962, 3, 1, 830.555, 2344.51, 390.337, 1.99199, 7200, 0, 0), -- Krik'thik Infiltrator

(1867, 58109, 962, 3, 1, 846.844, 2297.38, 379.906, 0, 7200, 0, 0), -- Mantid Infiltrator Rope Stalker
(1869, 58109, 962, 3, 1, 846.938, 2306.96, 379.211, 0, 7200, 0, 0), -- Mantid Infiltrator Rope Stalker
(1870, 58109, 962, 3, 1, 846.941, 2316.69, 379.211, 0, 7200, 0, 0), -- Mantid Infiltrator Rope Stalker

(1871, 58146, 962, 3, 1, 1019.69, 2364.38, 392.002, 1.44954, 7200, 0, 0), -- Serpent's Spine Defender
(1872, 58146, 962, 3, 1, 1024.07, 2361.88, 392.153, 1.63519, 7200, 0, 0), -- Serpent's Spine Defender
(1873, 58146, 962, 3, 1, 1029.98, 2365.96, 392.222, 3.15829, 7200, 0, 0), -- Serpent's Spine Defender
(1874, 58146, 962, 3, 1, 1193.08, 2304.77, 432.068, 3.0639, 7200, 0, 0), -- Serpent's Spine Defender
(1875, 58146, 962, 3, 1, 1195.42, 2301.68, 432.871, 3.0639, 7200, 0, 0), -- Serpent's Spine Defender
(1876, 58146, 962, 3, 1, 1197.49, 2306.59, 433.536, 3.0639, 7200, 0, 0), -- Serpent's Spine Defender
(1877, 58146, 962, 3, 1, 876.585, 2369.5, 392.004, 3.11861, 7200, 0, 0), -- Serpent's Spine Defender
(1878, 58146, 962, 3, 1, 876.759, 2363.81, 392.004, 3.14082, 7200, 0, 0), -- Serpent's Spine Defender
(1879, 58146, 962, 3, 1, 875.93, 2366.61, 392.007, 3.13943, 7200, 0, 0), -- Serpent's Spine Defender

(1880, 58202, 962, 3, 1, 840.469, 2297.56, 381.642, 3.12414, 7200, 0, 0), -- Serpent's Spine Defender
(1882, 58202, 962, 3, 1, 840.036, 2306.99, 381.642, 3.12414, 7200, 0, 0), -- Serpent's Spine Defender
(1883, 58202, 962, 3, 1, 840.115, 2316.74, 381.642, 3.07178, 7200, 0, 0), -- Serpent's Spine Defender

(1884, 58824, 962, 3, 1, 851.08, 2204.27, 354.595, 0.907571, 7200, 0, 0), -- Courtyard Defender
(1969, 58824, 962, 3, 1, 856.083, 2204.26, 354.568, 0.942478, 7200, 0, 0), -- Courtyard Defender
(1970, 58824, 962, 3, 1, 865.328, 2204.17, 354.648, 0.994838, 7200, 0, 0), -- Courtyard Defender
(1971, 58824, 962, 3, 1, 838.359, 2245.38, 354.65, 0.680678, 7200, 0, 0), -- Courtyard Defender
(1972, 58824, 962, 3, 1, 874.76, 2203.93, 354.631, 1.02974, 7200, 0, 0), -- Courtyard Defender
(1973, 58824, 962, 3, 1, 838.196, 2254.89, 354.66, 0.628318, 7200, 0, 0), -- Courtyard Defender
(1974, 58824, 962, 3, 1, 884.936, 2204.43, 354.662, 1.09956, 7200, 0, 0), -- Courtyard Defender
(1975, 58824, 962, 3, 1, 837.741, 2264.08, 354.649, 0.558505, 7200, 0, 0), -- Courtyard Defender
(1976, 58824, 962, 3, 1, 894.368, 2204.19, 354.671, 1.15192, 7200, 0, 0), -- Courtyard Defender
(1977, 58824, 962, 3, 1, 838.378, 2273.39, 354.619, 0.506145, 7200, 0, 0), -- Courtyard Defender
(1978, 58824, 962, 3, 1, 1076.36, 2204.16, 354.659, 2.30383, 7200, 0, 0), -- Courtyard Defender
(1979, 58824, 962, 3, 1, 1078.95, 2206.42, 354.644, 2.32129, 7200, 0, 0), -- Courtyard Defender
(1980, 58824, 962, 3, 1, 1078.46, 2236.11, 354.617, 2.44346, 7200, 0, 0), -- Courtyard Defender
(1981, 58824, 962, 3, 1, 1079.1, 2245.42, 354.647, 2.49582, 7200, 0, 0), -- Courtyard Defender
(1982, 58824, 962, 3, 1, 1078.32, 2255.04, 354.644, 2.54818, 7200, 0, 0), -- Courtyard Defender
(1983, 58824, 962, 3, 1, 1078.95, 2264.35, 354.677, 2.60054, 7200, 0, 0), -- Courtyard Defender
(1984, 58824, 962, 3, 1, 1078.7, 2274.23, 354.646, 2.67035, 7200, 0, 0), -- Courtyard Defender
(1985, 58824, 962, 3, 1, 1078.67, 2283.49, 354.669, 2.72271, 7200, 0, 0), -- Courtyard Defender
(1986, 58824, 962, 3, 1, 1078.91, 2225.75, 354.657, 2.40855, 7200, 0, 0), -- Courtyard Defender
(1987, 58824, 962, 3, 1, 1078.27, 2216.44, 354.664, 2.35619, 7200, 0, 0), -- Courtyard Defender
(1988, 58824, 962, 3, 1, 1066.97, 2203.94, 354.558, 2.26893, 7200, 0, 0), -- Courtyard Defender
(1989, 58824, 962, 3, 1, 1061.1, 2203.84, 354.573, 2.23402, 7200, 0, 0), -- Courtyard Defender
(1990, 58824, 962, 3, 1, 1022.81, 2204.05, 354.659, 2.04204, 7200, 0, 0), -- Courtyard Defender
(1991, 58824, 962, 3, 1, 997.174, 2228.62, 363.851, 1.95477, 7200, 0, 0), -- Courtyard Defender
(1992, 58824, 962, 3, 1, 1051.22, 2204.24, 354.651, 2.19912, 7200, 0, 0), -- Courtyard Defender

(1993, 58844, 962, 3, 1, 967.993, 2228.91, 363.891, 1.65806, 7200, 0, 0), -- Panda Tar Tosser
(1994, 58844, 962, 3, 1, 958.549, 2229.37, 363.92, 1.5708, 7200, 0, 0), -- Panda Tar Tosser

(1995, 59721, 962, 3, 1, 1078.41, 2379.06, 390.626, 1.5857, 7200, 0, 0), -- Great Wall Explosion Target Stalker 2

(1996, 59723, 962, 3, 1, 772.889, 2481.76, 448.383, 0, 7200, 0, 0), -- Great Wall Explosion Caster Stalker
(1997, 59723, 962, 3, 1, 1132.48, 2429.31, 445.856, 0, 7200, 0, 0), -- Great Wall Explosion Caster Stalker

(1998, 59750, 962, 3, 1, 1192.17, 2324.91, 388.04, 4.9513, 7200, 0, 0), -- Serpent's Spine Cannoneer
(1999, 59750, 962, 3, 1, 1198.26, 2324.01, 388.038, 4.61387, 7200, 0, 0), -- Serpent's Spine Cannoneer
(2000, 59750, 962, 3, 1, 1193.99, 2285.37, 388.037, 1.40392, 7200, 0, 0), -- Serpent's Spine Cannoneer
(2001, 59750, 962, 3, 1, 1199.02, 2284.88, 388.04, 1.67828, 7200, 0, 0), -- Serpent's Spine Cannoneer

(2002, 59778, 962, 3, 1, 1193.92, 2374.73, 442.499, 5.43655, 7200, 0, 0), -- Krik'thik Striker
(2003, 59778, 962, 3, 1, 1198.65, 2380.93, 446.218, 0, 7200, 0, 0), -- Krik'thik Striker
(2004, 59778, 962, 3, 1, 1192.31, 2376.16, 442.818, 4.86921, 7200, 0, 0), -- Krik'thik Striker

(2005, 59792, 962, 3, 1, 1376.67, 2280.48, 401.585, 0, 7200, 0, 0), -- Great Wall Bonfire Caster Stalker

(2006, 59793, 962, 3, 1, 1377.26, 2280.45, 426.956, 0, 7200, 0, 0), -- Great Wall Bonfire Target Stalker 1

(2007, 59794, 962, 3, 1, 1236.32, 2279.63, 436.736, 1.42327, 7200, 0, 0), -- Krik'thik Disruptor
(2008, 59794, 962, 3, 1, 1237.39, 2333.17, 437.577, 1.4169, 7200, 0, 0), -- Krik'thik Disruptor

(2009, 59800, 962, 3, 1, 1019.81, 2368.54, 392.222, 4.31151, 7200, 0, 0), -- Krik'thik Rager
(2010, 59800, 962, 3, 1, 1142.59, 2392.37, 401.218, 2.67338, 7200, 5, 1), -- Krik'thik Rager
(2011, 59800, 962, 3, 1, 1137.38, 2397.01, 401.067, 2.99242, 7200, 5, 1), -- Krik'thik Rager
(2012, 59800, 962, 3, 1, 1278.63, 2326.33, 378.444, 4.71117, 7200, 0, 0), -- Krik'thik Rager
(2013, 59800, 962, 3, 1, 1283.13, 2326.2, 378.578, 4.68335, 7200, 0, 0), -- Krik'thik Rager
(2014, 59800, 962, 3, 1, 1300.05, 2325.22, 382.683, 4.66647, 7200, 0, 0), -- Krik'thik Rager
(2015, 59800, 962, 3, 1, 1304.55, 2325.08, 382.838, 4.59558, 7200, 0, 0), -- Krik'thik Rager

(2016, 59801, 962, 3, 1, 1025.73, 2366.9, 392.222, 3.83199, 7200, 0, 0), -- Krik'thik Wind Shaper
(2018, 59801, 962, 3, 1, 855.474, 2320.97, 366.547, 3.39458, 7200, 0, 0), -- Krik'thik Wind Shaper
(2019, 59801, 962, 3, 1, 1129.93, 2396.34, 401.437, 3.09771, 7200, 5, 1), -- Krik'thik Wind Shaper
(2020, 59801, 962, 3, 1, 1274.71, 2326.51, 378.081, 4.72244, 7200, 0, 0), -- Krik'thik Wind Shaper
(2021, 59801, 962, 3, 1, 1296.13, 2325.4, 382.34, 4.54564, 7200, 0, 0), -- Krik'thik Wind Shaper

(2022, 59820, 962, 3, 1, 1047.15, 2245.24, 296.189, 2.19641, 7200, 0, 0), -- Pandaren Cannoneer
(2023, 59820, 962, 3, 1, 862.033, 2247.05, 296.189, 0.56662, 7200, 0, 0), -- Pandaren Cannoneer
(2024, 59820, 962, 3, 1, 924.019, 2238.89, 296.189, 1.63317, 7200, 0, 0), -- Pandaren Cannoneer

(2025, 59834, 962, 3, 1, 1295.67, 2324.82, 382.075, 0, 7200, 0, 0), -- Boss 3 Encounter Add Generator
(2026, 59834, 962, 3, 1, 1289.14, 2325.06, 383.151, 0, 7200, 0, 0), -- Boss 3 Encounter Add Generator
(2027, 59834, 962, 3, 1, 1275.06, 2325.04, 378.163, 0, 7200, 0, 0), -- Boss 3 Encounter Add Generator
(2028, 59834, 962, 3, 1, 1300.45, 2324.82, 383.688, 0, 7200, 0, 0), -- Boss 3 Encounter Add Generator
(2029, 59834, 962, 3, 1, 1282.91, 2324.46, 378.25, 0, 7200, 0, 0), -- Boss 3 Encounter Add Generator
(2030, 59834, 962, 3, 1, 1278.64, 2324.42, 376.938, 0, 7200, 0, 0), -- Boss 3 Encounter Add Generator
(2031, 59834, 962, 3, 1, 1304.64, 2324.52, 384.497, 0, 7200, 0, 0), -- Boss 3 Encounter Add Generator

(2032, 59886, 962, 3, 1, 751.165, 2515.04, 407.191, 0, 7200, 0, 0), -- Great Wall Panda Archer Target Spawner
(2033, 59886, 962, 3, 1, 831.082, 2526.3, 407.719, 0, 7200, 0, 0), -- Great Wall Panda Archer Target Spawner
(2034, 59886, 962, 3, 1, 868.352, 2516.59, 402.279, 0, 7200, 0, 0), -- Great Wall Panda Archer Target Spawner
(2035, 59886, 962, 3, 1, 809.818, 2537.77, 414.671, 0, 7200, 0, 0), -- Great Wall Panda Archer Target Spawner
(2036, 59886, 962, 3, 1, 853.08, 2533.71, 418.179, 0, 7200, 0, 0), -- Great Wall Panda Archer Target Spawner
(2037, 59886, 962, 3, 1, 846.108, 2512.9, 387.43, 0, 7200, 0, 0), -- Great Wall Panda Archer Target Spawner
(2038, 59886, 962, 3, 1, 774.132, 2528.43, 387.347, 0, 7200, 0, 0), -- Great Wall Panda Archer Target Spawner
(2039, 59886, 962, 3, 1, 763.062, 2527.11, 415.098, 0, 7200, 0, 0), -- Great Wall Panda Archer Target Spawner
(2040, 59886, 962, 3, 1, 797.543, 2530.68, 394.69, 0, 7200, 0, 0), -- Great Wall Panda Archer Target Spawner
(2041, 59886, 962, 3, 1, 797.604, 2512.93, 379.045, 0, 7200, 0, 0), -- Great Wall Panda Archer Target Spawner

(2042, 59892, 962, 3, 1, 765.685, 2521.56, 416.174, 5.15397, 7200, 0, 0), -- Krik'thik Invader
(2043, 59892, 962, 3, 1, 754.285, 2509.75, 408.44, 5.24551, 7200, 0, 0), -- Krik'thik Invader
(2044, 59892, 962, 3, 1, 831.407, 2520.17, 408.937, 4.76548, 7200, 0, 0), -- Krik'thik Invader
(2045, 59892, 962, 3, 1, 867.21, 2510.56, 403.63, 4.52513, 7200, 0, 0), -- Krik'thik Invader
(2046, 59892, 962, 3, 1, 799.049, 2524.76, 396.235, 4.96139, 7200, 0, 0), -- Krik'thik Invader
(2047, 59892, 962, 3, 1, 776.401, 2522.77, 389.058, 5.09388, 7200, 0, 0), -- Krik'thik Invader
(2048, 59892, 962, 3, 1, 850.311, 2498.96, 415.626, 4.6329, 7200, 0, 0), -- Krik'thik Invader
(2049, 59892, 962, 3, 1, 845.839, 2506.76, 389.185, 4.66871, 7200, 0, 0), -- Krik'thik Invader
(2050, 59892, 962, 3, 1, 799.275, 2507.05, 381.01, 4.98948, 7200, 0, 0), -- Krik'thik Invader
(2051, 59892, 962, 3, 1, 818.244, 2489, 411.471, 4.88345, 7200, 0, 0), -- Krik'thik Invader
(2052, 59892, 962, 3, 1, 765.903, 2521.1, 416.124, 5.15397, 7200, 0, 0), -- Krik'thik Invader
(2053, 59892, 962, 3, 1, 867.114, 2510.05, 403.613, 4.52513, 7200, 0, 0), -- Krik'thik Invader
(2054, 59892, 962, 3, 1, 754.537, 2509.33, 408.413, 5.24551, 7200, 0, 0), -- Krik'thik Invader
(2055, 59892, 962, 3, 1, 831.433, 2519.69, 408.906, 4.76548, 7200, 0, 0), -- Krik'thik Invader
(2056, 59892, 962, 3, 1, 776.609, 2522.25, 389.09, 5.09388, 7200, 0, 0), -- Krik'thik Invader
(2057, 59892, 962, 3, 1, 799.182, 2524.24, 396.243, 4.96139, 7200, 0, 0), -- Krik'thik Invader
(2058, 59892, 962, 3, 1, 849.985, 2494.87, 415.122, 4.6329, 7200, 0, 0), -- Krik'thik Invader
(2059, 59892, 962, 3, 1, 852.518, 2526.66, 419.042, 4.6329, 7200, 0, 0), -- Krik'thik Invader
(2060, 59892, 962, 3, 1, 867.01, 2509.5, 403.596, 4.52513, 7200, 0, 0), -- Krik'thik Invader
(2061, 59892, 962, 3, 1, 831.463, 2519.12, 408.87, 4.76548, 7200, 0, 0), -- Krik'thik Invader
(2062, 59892, 962, 3, 1, 754.817, 2508.85, 408.382, 5.24551, 7200, 0, 0), -- Krik'thik Invader
(2063, 59892, 962, 3, 1, 766.122, 2520.64, 416.073, 5.15397, 7200, 0, 0), -- Krik'thik Invader
(2064, 59892, 962, 3, 1, 799.307, 2523.75, 396.251, 4.96139, 7200, 0, 0), -- Krik'thik Invader
(2065, 59892, 962, 3, 1, 776.79, 2521.8, 389.118, 5.09388, 7200, 0, 0), -- Krik'thik Invader
(2066, 59892, 962, 3, 1, 811.036, 2530.72, 415.647, 4.88345, 7200, 0, 0), -- Krik'thik Invader
(2067, 59892, 962, 3, 1, 845.794, 2505.74, 389.255, 4.66871, 7200, 0, 0), -- Krik'thik Invader
(2068, 59892, 962, 3, 1, 799.555, 2506.07, 381.132, 4.98948, 7200, 0, 0), -- Krik'thik Invader
(2069, 59892, 962, 3, 1, 852.471, 2526.08, 418.97, 4.6329, 7200, 0, 0), -- Krik'thik Invader
(2071, 59892, 962, 3, 1, 755.101, 2508.37, 408.35, 5.24551, 7200, 0, 0), -- Krik'thik Invader
(2073, 59892, 962, 3, 1, 766.36, 2520.14, 416.018, 5.15397, 7200, 0, 0), -- Krik'thik Invader
(2074, 59892, 962, 3, 1, 831.491, 2518.59, 408.836, 4.76548, 7200, 0, 0), -- Krik'thik Invader
(2075, 59892, 962, 3, 1, 866.914, 2509, 403.579, 4.52513, 7200, 0, 0), -- Krik'thik Invader
(2076, 59892, 962, 3, 1, 777.022, 2521.22, 389.154, 5.09388, 7200, 0, 0), -- Krik'thik Invader
(2077, 59892, 962, 3, 1, 799.456, 2523.16, 396.261, 4.96139, 7200, 0, 0), -- Krik'thik Invader
(2078, 59892, 962, 3, 1, 811.127, 2530.2, 415.595, 4.88345, 7200, 0, 0), -- Krik'thik Invader
(2079, 59892, 962, 3, 1, 799.702, 2505.55, 381.196, 4.98948, 7200, 0, 0), -- Krik'thik Invader
(2081, 59892, 962, 3, 1, 845.772, 2505.22, 389.29, 4.66871, 7200, 0, 0), -- Krik'thik Invader
(2082, 59892, 962, 3, 1, 852.434, 2525.61, 418.913, 4.6329, 7200, 0, 0), -- Krik'thik Invader
(2083, 59892, 962, 3, 1, 831.519, 2518.07, 408.802, 4.76548, 7200, 0, 0), -- Krik'thik Invader
(2084, 59892, 962, 3, 1, 766.575, 2519.68, 415.968, 5.15397, 7200, 0, 0), -- Krik'thik Invader
(2085, 59892, 962, 3, 1, 755.344, 2507.96, 408.323, 5.24551, 7200, 0, 0), -- Krik'thik Invader
(2086, 59892, 962, 3, 1, 866.822, 2508.51, 403.564, 4.52513, 7200, 0, 0), -- Krik'thik Invader
(2087, 59892, 962, 3, 1, 799.58, 2522.67, 396.269, 4.96139, 7200, 0, 0), -- Krik'thik Invader
(2088, 59892, 962, 3, 1, 777.206, 2520.76, 389.182, 5.09388, 7200, 0, 0), -- Krik'thik Invader
(2089, 59892, 962, 3, 1, 811.217, 2529.67, 415.543, 4.88345, 7200, 0, 0), -- Krik'thik Invader
(2091, 59892, 962, 3, 1, 845.748, 2504.68, 389.326, 4.66871, 7200, 0, 0), -- Krik'thik Invader
(2092, 59892, 962, 3, 1, 799.844, 2505.05, 381.258, 4.98948, 7200, 0, 0), -- Krik'thik Invader
(2094, 59892, 962, 3, 1, 852.392, 2525.08, 418.847, 4.6329, 7200, 0, 0), -- Krik'thik Invader
(2095, 59892, 962, 3, 1, 831.546, 2517.55, 408.769, 4.76548, 7200, 0, 0), -- Krik'thik Invader
(2096, 59892, 962, 3, 1, 766.794, 2519.22, 415.918, 5.15397, 7200, 0, 0), -- Krik'thik Invader
(2097, 59892, 962, 3, 1, 866.722, 2507.99, 403.547, 4.52513, 7200, 0, 0), -- Krik'thik Invader
(2098, 59892, 962, 3, 1, 755.605, 2507.52, 408.294, 5.24551, 7200, 0, 0), -- Krik'thik Invader
(2099, 59892, 962, 3, 1, 799.707, 2522.17, 396.277, 4.96139, 7200, 0, 0), -- Krik'thik Invader
(2100, 59892, 962, 3, 1, 777.391, 2520.3, 389.21, 5.09388, 7200, 0, 0), -- Krik'thik Invader
(2101, 59892, 962, 3, 1, 811.302, 2529.18, 415.493, 4.88345, 7200, 0, 0), -- Krik'thik Invader
(2102, 59892, 962, 3, 1, 845.726, 2504.17, 389.36, 4.66871, 7200, 0, 0), -- Krik'thik Invader
(2103, 59892, 962, 3, 1, 799.981, 2504.57, 381.318, 4.98948, 7200, 0, 0), -- Krik'thik Invader
(2104, 59892, 962, 3, 1, 852.35, 2524.55, 418.782, 4.6329, 7200, 0, 0), -- Krik'thik Invader
(2105, 59892, 962, 3, 1, 754.075, 2510.11, 408.464, 5.24551, 7200, 0, 0), -- Krik'thik Invader
(2106, 59892, 962, 3, 1, 765.5, 2521.96, 416.217, 5.15397, 7200, 0, 0), -- Krik'thik Invader
(2107, 59892, 962, 3, 1, 831.385, 2520.59, 408.964, 4.76548, 7200, 0, 0), -- Krik'thik Invader
(2108, 59892, 962, 3, 1, 867.289, 2510.98, 403.643, 4.52513, 7200, 0, 0), -- Krik'thik Invader
(2109, 59892, 962, 3, 1, 798.953, 2525.14, 396.228, 4.96139, 7200, 0, 0), -- Krik'thik Invader
(2110, 59892, 962, 3, 1, 776.256, 2523.13, 389.036, 5.09388, 7200, 0, 0), -- Krik'thik Invader
(2111, 59892, 962, 3, 1, 810.79, 2532.14, 415.79, 4.88345, 7200, 0, 0), -- Krik'thik Invader
(2112, 59892, 962, 3, 1, 799.162, 2507.45, 380.96, 4.98948, 7200, 0, 0), -- Krik'thik Invader
(2113, 59892, 962, 3, 1, 845.858, 2507.2, 389.156, 4.66871, 7200, 0, 0), -- Krik'thik Invader
(2114, 59892, 962, 3, 1, 852.593, 2527.61, 419.159, 4.6329, 7200, 0, 0), -- Krik'thik Invader
(2116, 59892, 962, 3, 1, 799.08, 2524.64, 396.237, 4.96139, 7200, 0, 0), -- Krik'thik Invader
(2117, 59892, 962, 3, 1, 776.448, 2522.65, 389.066, 5.09388, 7200, 0, 0), -- Krik'thik Invader
(2118, 59892, 962, 3, 1, 810.863, 2531.72, 415.747, 4.88345, 7200, 0, 0), -- Krik'thik Invader
(2119, 59892, 962, 3, 1, 845.839, 2506.75, 389.186, 4.66871, 7200, 0, 0), -- Krik'thik Invader
(2120, 59892, 962, 3, 1, 799.28, 2507.04, 381.012, 4.98948, 7200, 0, 0), -- Krik'thik Invader
(2121, 59892, 962, 3, 1, 852.561, 2527.21, 419.109, 4.6329, 7200, 0, 0), -- Krik'thik Invader
(2122, 59892, 962, 3, 1, 765.877, 2521.16, 416.13, 5.15397, 7200, 0, 0), -- Krik'thik Invader
(2123, 59892, 962, 3, 1, 831.431, 2519.72, 408.908, 4.76548, 7200, 0, 0), -- Krik'thik Invader
(2124, 59892, 962, 3, 1, 754.514, 2509.36, 408.415, 5.24551, 7200, 0, 0), -- Krik'thik Invader
(2125, 59892, 962, 3, 1, 867.126, 2510.11, 403.615, 4.52513, 7200, 0, 0), -- Krik'thik Invader
(2127, 59892, 962, 3, 1, 776.589, 2522.3, 389.087, 5.09388, 7200, 0, 0), -- Krik'thik Invader
(2128, 59892, 962, 3, 1, 799.169, 2524.29, 396.242, 4.96139, 7200, 0, 0), -- Krik'thik Invader
(2129, 59892, 962, 3, 1, 810.932, 2531.32, 415.707, 4.88345, 7200, 0, 0), -- Krik'thik Invader
(2130, 59892, 962, 3, 1, 845.821, 2506.35, 389.213, 4.66871, 7200, 0, 0), -- Krik'thik Invader
(2131, 59892, 962, 3, 1, 799.39, 2506.65, 381.06, 4.98948, 7200, 0, 0), -- Krik'thik Invader
(2132, 59892, 962, 3, 1, 852.518, 2526.67, 419.043, 4.6329, 7200, 0, 0), -- Krik'thik Invader
(2133, 59892, 962, 3, 1, 766.111, 2520.66, 416.076, 5.15397, 7200, 0, 0), -- Krik'thik Invader
(2134, 59892, 962, 3, 1, 831.459, 2519.19, 408.874, 4.76548, 7200, 0, 0), -- Krik'thik Invader
(2135, 59892, 962, 3, 1, 867.027, 2509.59, 403.598, 4.52513, 7200, 0, 0), -- Krik'thik Invader
(2136, 59892, 962, 3, 1, 754.771, 2508.93, 408.387, 5.24551, 7200, 0, 0), -- Krik'thik Invader
(2137, 59892, 962, 3, 1, 776.78, 2521.82, 389.116, 5.09388, 7200, 0, 0), -- Krik'thik Invader
(2138, 59892, 962, 3, 1, 799.296, 2523.79, 396.25, 4.96139, 7200, 0, 0), -- Krik'thik Invader
(2139, 59892, 962, 3, 1, 811.033, 2530.74, 415.649, 4.88345, 7200, 0, 0), -- Krik'thik Invader
(2140, 59892, 962, 3, 1, 799.55, 2506.09, 381.129, 4.98948, 7200, 0, 0), -- Krik'thik Invader
(2141, 59892, 962, 3, 1, 845.796, 2505.77, 389.252, 4.66871, 7200, 0, 0), -- Krik'thik Invader
(2142, 59892, 962, 3, 1, 852.766, 2529.78, 419.426, 4.6329, 7200, 0, 0), -- Krik'thik Invader
(2143, 59892, 962, 3, 1, 755.018, 2508.51, 408.359, 5.24551, 7200, 0, 0), -- Krik'thik Invader
(2144, 59892, 962, 3, 1, 766.291, 2520.28, 416.034, 5.15397, 7200, 0, 0), -- Krik'thik Invader
(2145, 59892, 962, 3, 1, 831.483, 2518.74, 408.845, 4.76548, 7200, 0, 0), -- Krik'thik Invader
(2146, 59892, 962, 3, 1, 866.943, 2509.15, 403.584, 4.52513, 7200, 0, 0), -- Krik'thik Invader
(2147, 59892, 962, 3, 1, 811.111, 2530.28, 415.604, 4.88345, 7200, 0, 0), -- Krik'thik Invader
(2148, 59892, 962, 3, 1, 799.677, 2505.64, 381.185, 4.98948, 7200, 0, 0), -- Krik'thik Invader
(2149, 59892, 962, 3, 1, 845.776, 2505.31, 389.284, 4.66871, 7200, 0, 0), -- Krik'thik Invader
(2150, 59892, 962, 3, 1, 852.446, 2525.77, 418.932, 4.6329, 7200, 0, 0), -- Krik'thik Invader
(2151, 59892, 962, 3, 1, 866.847, 2508.64, 403.568, 4.52513, 7200, 0, 0), -- Krik'thik Invader
(2152, 59892, 962, 3, 1, 766.502, 2519.83, 415.985, 5.15397, 7200, 0, 0), -- Krik'thik Invader
(2153, 59892, 962, 3, 1, 831.509, 2518.26, 408.814, 4.76548, 7200, 0, 0), -- Krik'thik Invader
(2154, 59892, 962, 3, 1, 755.261, 2508.1, 408.332, 5.24551, 7200, 0, 0), -- Krik'thik Invader
(2155, 59892, 962, 3, 1, 811.186, 2529.85, 415.56, 4.88345, 7200, 0, 0), -- Krik'thik Invader
(2156, 59892, 962, 3, 1, 845.756, 2504.86, 389.314, 4.66871, 7200, 0, 0), -- Krik'thik Invader
(2157, 59892, 962, 3, 1, 799.795, 2505.23, 381.237, 4.98948, 7200, 0, 0), -- Krik'thik Invader
(2158, 59892, 962, 3, 1, 852.407, 2525.27, 418.87, 4.6329, 7200, 0, 0), -- Krik'thik Invader
(2159, 59892, 962, 3, 1, 831.535, 2517.77, 408.783, 4.76548, 7200, 0, 0), -- Krik'thik Invader
(2160, 59892, 962, 3, 1, 766.702, 2519.41, 415.939, 5.15397, 7200, 0, 0), -- Krik'thik Invader
(2161, 59892, 962, 3, 1, 755.499, 2507.7, 408.306, 5.24551, 7200, 0, 0), -- Krik'thik Invader
(2162, 59892, 962, 3, 1, 866.765, 2508.21, 403.554, 4.52513, 7200, 0, 0), -- Krik'thik Invader
(2163, 59892, 962, 3, 1, 777.297, 2520.53, 389.196, 5.09388, 7200, 0, 0), -- Krik'thik Invader
(2164, 59892, 962, 3, 1, 799.638, 2522.44, 396.272, 4.96139, 7200, 0, 0), -- Krik'thik Invader
(2165, 59892, 962, 3, 1, 811.266, 2529.39, 415.514, 4.88345, 7200, 0, 0), -- Krik'thik Invader
(2166, 59892, 962, 3, 1, 799.925, 2504.77, 381.293, 4.98948, 7200, 0, 0), -- Krik'thik Invader
(2167, 59892, 962, 3, 1, 845.736, 2504.4, 389.345, 4.66871, 7200, 0, 0), -- Krik'thik Invader
(2168, 59892, 962, 3, 1, 852.37, 2524.81, 418.814, 4.6329, 7200, 0, 0), -- Krik'thik Invader
(2169, 59892, 962, 3, 1, 765.377, 2522.22, 416.245, 5.15397, 7200, 0, 0), -- Krik'thik Invader
(2170, 59892, 962, 3, 1, 867.342, 2511.26, 403.652, 4.52513, 7200, 0, 0), -- Krik'thik Invader
(2171, 59892, 962, 3, 1, 753.914, 2510.38, 408.482, 5.24551, 7200, 0, 0), -- Krik'thik Invader
(2172, 59892, 962, 3, 1, 831.368, 2520.91, 408.984, 4.76548, 7200, 0, 0), -- Krik'thik Invader
(2173, 59892, 962, 3, 1, 799.765, 2521.94, 396.28, 4.96139, 7200, 0, 0), -- Krik'thik Invader
(2174, 59892, 962, 3, 1, 777.481, 2520.07, 389.224, 5.09388, 7200, 0, 0), -- Krik'thik Invader
(2175, 59892, 962, 3, 1, 800.052, 2504.32, 381.349, 4.98948, 7200, 0, 0), -- Krik'thik Invader
(2176, 59892, 962, 3, 1, 845.715, 2503.92, 389.377, 4.66871, 7200, 0, 0), -- Krik'thik Invader
(2177, 59892, 962, 3, 1, 811.345, 2528.93, 415.468, 4.88345, 7200, 0, 0), -- Krik'thik Invader
(2178, 59892, 962, 3, 1, 852.612, 2527.84, 419.187, 4.6329, 7200, 0, 0), -- Krik'thik Invader
(2179, 59892, 962, 3, 1, 867.243, 2510.74, 403.635, 4.52513, 7200, 0, 0), -- Krik'thik Invader
(2180, 59892, 962, 3, 1, 831.396, 2520.37, 408.95, 4.76548, 7200, 0, 0), -- Krik'thik Invader
(2181, 59892, 962, 3, 1, 765.589, 2521.77, 416.196, 5.15397, 7200, 0, 0), -- Krik'thik Invader
(2182, 59892, 962, 3, 1, 754.171, 2509.95, 408.453, 5.24551, 7200, 0, 0), -- Krik'thik Invader
(2183, 59892, 962, 3, 1, 776.354, 2522.89, 389.051, 5.09388, 7200, 0, 0), -- Krik'thik Invader
(2184, 59892, 962, 3, 1, 799.013, 2524.9, 396.232, 4.96139, 7200, 0, 0), -- Krik'thik Invader
(2185, 59892, 962, 3, 1, 810.213, 2535.48, 416.124, 4.88345, 7200, 0, 0), -- Krik'thik Invader
(2186, 59892, 962, 3, 1, 846.006, 2510.58, 388.928, 4.66871, 7200, 0, 0), -- Krik'thik Invader
(2187, 59892, 962, 3, 1, 798.235, 2510.71, 380.556, 4.98948, 7200, 0, 0), -- Krik'thik Invader
(2188, 59892, 962, 3, 1, 852.58, 2527.45, 419.139, 4.6329, 7200, 0, 0), -- Krik'thik Invader
(2189, 59892, 962, 3, 1, 765.784, 2521.35, 416.151, 5.15397, 7200, 0, 0), -- Krik'thik Invader
(2190, 59892, 962, 3, 1, 867.164, 2510.32, 403.622, 4.52513, 7200, 0, 0), -- Krik'thik Invader
(2191, 59892, 962, 3, 1, 754.4, 2509.56, 408.428, 5.24551, 7200, 0, 0), -- Krik'thik Invader
(2192, 59892, 962, 3, 1, 831.418, 2519.96, 408.923, 4.76548, 7200, 0, 0), -- Krik'thik Invader
(2193, 59892, 962, 3, 1, 776.502, 2522.52, 389.074, 5.09388, 7200, 0, 0), -- Krik'thik Invader
(2194, 59892, 962, 3, 1, 799.111, 2524.52, 396.239, 4.96139, 7200, 0, 0), -- Krik'thik Invader
(2195, 59892, 962, 3, 1, 810.899, 2531.51, 415.727, 4.88345, 7200, 0, 0), -- Krik'thik Invader
(2197, 59892, 962, 3, 1, 845.829, 2506.54, 389.201, 4.66871, 7200, 0, 0), -- Krik'thik Invader
(2198, 59892, 962, 3, 1, 799.336, 2506.84, 381.036, 4.98948, 7200, 0, 0), -- Krik'thik Invader
(2199, 59892, 962, 3, 1, 852.54, 2526.95, 419.077, 4.6329, 7200, 0, 0), -- Krik'thik Invader
(2200, 59892, 962, 3, 1, 765.984, 2520.93, 416.105, 5.15397, 7200, 0, 0), -- Krik'thik Invader
(2201, 59892, 962, 3, 1, 754.643, 2509.15, 408.401, 5.24551, 7200, 0, 0), -- Krik'thik Invader
(2202, 59892, 962, 3, 1, 831.444, 2519.48, 408.893, 4.76548, 7200, 0, 0), -- Krik'thik Invader
(2203, 59892, 962, 3, 1, 867.08, 2509.88, 403.608, 4.52513, 7200, 0, 0), -- Krik'thik Invader
(2204, 59892, 962, 3, 1, 775.307, 2525.5, 388.891, 5.09388, 7200, 0, 0), -- Krik'thik Invader
(2205, 59892, 962, 3, 1, 798.319, 2527.63, 396.188, 4.96139, 7200, 0, 0), -- Krik'thik Invader
(2206, 59892, 962, 3, 1, 845.81, 2506.1, 389.23, 4.66871, 7200, 0, 0), -- Krik'thik Invader
(2207, 59892, 962, 3, 1, 799.456, 2506.42, 381.089, 4.98948, 7200, 0, 0), -- Krik'thik Invader
(2208, 59892, 962, 3, 1, 810.974, 2531.08, 415.684, 4.88345, 7200, 0, 0), -- Krik'thik Invader
(2209, 59892, 962, 3, 1, 852.503, 2526.48, 419.02, 4.6329, 7200, 0, 0), -- Krik'thik Invader
(2210, 59892, 962, 3, 1, 831.468, 2519.02, 408.863, 4.76548, 7200, 0, 0), -- Krik'thik Invader
(2211, 59892, 962, 3, 1, 766.168, 2520.54, 416.062, 5.15397, 7200, 0, 0), -- Krik'thik Invader
(2288, 59892, 962, 3, 1, 754.858, 2508.78, 408.377, 5.24551, 7200, 0, 0), -- Krik'thik Invader
(2289, 59892, 962, 3, 1, 867, 2509.45, 403.594, 4.52513, 7200, 0, 0), -- Krik'thik Invader
(2290, 59892, 962, 3, 1, 799.336, 2523.63, 396.253, 4.96139, 7200, 0, 0), -- Krik'thik Invader
(2291, 59892, 962, 3, 1, 776.83, 2521.7, 389.124, 5.09388, 7200, 0, 0), -- Krik'thik Invader
(2292, 59892, 962, 3, 1, 811.059, 2530.59, 415.634, 4.88345, 7200, 0, 0), -- Krik'thik Invader
(2293, 59892, 962, 3, 1, 799.594, 2505.93, 381.149, 4.98948, 7200, 0, 0), -- Krik'thik Invader
(2294, 59892, 962, 3, 1, 845.789, 2505.62, 389.263, 4.66871, 7200, 0, 0), -- Krik'thik Invader
(2295, 59892, 962, 3, 1, 852.751, 2529.59, 419.403, 4.6329, 7200, 0, 0), -- Krik'thik Invader
(2296, 59892, 962, 3, 1, 867.566, 2512.44, 403.69, 4.52513, 7200, 0, 0), -- Krik'thik Invader
(2297, 59892, 962, 3, 1, 764.863, 2523.3, 416.364, 5.15397, 7200, 0, 0), -- Krik'thik Invader
(2298, 59892, 962, 3, 1, 831.305, 2522.1, 409.06, 4.76548, 7200, 0, 0), -- Krik'thik Invader
(2299, 59892, 962, 3, 1, 753.3, 2511.42, 408.549, 5.24551, 7200, 0, 0), -- Krik'thik Invader
(2300, 59892, 962, 3, 1, 798.577, 2526.62, 396.204, 4.96139, 7200, 0, 0), -- Krik'thik Invader
(2301, 59892, 962, 3, 1, 775.69, 2524.54, 388.949, 5.09388, 7200, 0, 0), -- Krik'thik Invader
(2302, 59892, 962, 3, 1, 810.532, 2533.64, 415.94, 4.88345, 7200, 0, 0), -- Krik'thik Invader
(2303, 59892, 962, 3, 1, 845.924, 2508.71, 389.055, 4.66871, 7200, 0, 0), -- Krik'thik Invader
(2304, 59892, 962, 3, 1, 798.745, 2508.92, 380.779, 4.98948, 7200, 0, 0), -- Krik'thik Invader

(2305, 60415, 962, 3, 1, 1235.8, 2295.31, 430.908, 0, 7200, 0, 0), -- Flak Cannon
(2306, 60415, 962, 3, 1, 1235.78, 2313.64, 430.908, 0, 7200, 0, 0), -- Flak Cannon

(2307, 60421, 962, 3, 1, 1278.59, 2298.27, 422.715, 0, 7200, 0, 0), -- Flak Cannon Target Stalker
(2308, 60421, 962, 3, 1, 1299.24, 2311.64, 431.15, 0, 7200, 0, 0), -- Flak Cannon Target Stalker
(2309, 60421, 962, 3, 1, 1308.31, 2306.64, 427.273, 0, 7200, 0, 0), -- Flak Cannon Target Stalker
(2310, 60421, 962, 3, 1, 1317.19, 2297.08, 442.499, 0, 7200, 0, 0), -- Flak Cannon Target Stalker
(2311, 60421, 962, 3, 1, 1327.01, 2301.77, 429.042, 0, 7200, 0, 0), -- Flak Cannon Target Stalker
(2312, 60421, 962, 3, 1, 1328.23, 2289.27, 428.285, 0, 7200, 0, 0), -- Flak Cannon Target Stalker

(2313, 60502, 962, 3, 1, 1029.15, 2326.96, 390.78, 2.61171, 7200, 0, 0), -- Pandaren Disciple
(2314, 60502, 962, 3, 1, 1019.84, 2326.66, 390.78, 0.593728, 7200, 0, 0), -- Pandaren Disciple
(2315, 60502, 962, 3, 1, 1025.94, 2324.46, 390.78, 1.85385, 7200, 0, 0), -- Pandaren Disciple
(2316, 60502, 962, 3, 1, 1029.9, 2329.92, 390.783, 3.17529, 7200, 0, 0), -- Pandaren Disciple
(2317, 60502, 962, 3, 1, 1030.39, 2364.89, 392.002, 2.6176, 7200, 0, 0), -- Pandaren Disciple
(2318, 60502, 962, 3, 1, 1017.55, 2365.31, 392.002, 0.383918, 7200, 0, 0), -- Pandaren Disciple
(2319, 60502, 962, 3, 1, 1024.94, 2363.08, 392.222, 1.6211, 7200, 0, 0), -- Pandaren Disciple
(2320, 60502, 962, 3, 1, 1030.92, 2369.37, 392.002, 3.3291, 7200, 0, 0), -- Pandaren Disciple
(2321, 60502, 962, 3, 1, 1072.88, 2364.41, 390.871, 2.45066, 7200, 0, 0), -- Pandaren Disciple
(2322, 60502, 962, 3, 1, 1063.76, 2364.73, 390.755, 0.954205, 7200, 0, 0), -- Pandaren Disciple
(2323, 60502, 962, 3, 1, 1068.85, 2363.57, 390.843, 1.88669, 7200, 0, 0), -- Pandaren Disciple
(2324, 60502, 962, 3, 1, 1072.8, 2367.72, 390.172, 2.87892, 7200, 0, 0), -- Pandaren Disciple

(2325, 61213, 962, 3, 1, 1024.4, 2329.74, 390.863, 4.60756, 7200, 0, 0), -- Krik'thik Conscript
(2326, 61213, 962, 3, 1, 1066.98, 2369.28, 390.863, 4.60756, 7200, 0, 0), -- Krik'thik Conscript
(2327, 61213, 962, 3, 1, 1024.58, 2368.1, 392.222, 4.60756, 7200, 0, 0), -- Krik'thik Conscript

(2328, 63192, 962, 3, 1, 958.805, 2366.01, 401.785, 4.68497, 7200, 0, 0), -- Cosmetic Client Stalker

(2329, 63848, 962, 3, 1, 1377.24, 2280.54, 403.302, 0, 7200, 5, 1), -- Great Wall Bonfire Target Stalker 2

(2330, 63991, 962, 3, 1, 1087.04, 2309.7, 381.319, 0, 7200, 0, 0), -- Great Wall Explosion Target Stalker 3

(2331, 64467, 962, 3, 1, 715.872, 2113.33, 403.355, 1.3094, 7200, 0, 0), -- Bowmistress Li <Guard Captain>
(2332, 64467, 962, 3, 1, 958.271, 2171.18, 296.192, 1.56877, 7200, 0, 0), -- Bowmistress Li <Guard Captain>

(2333, 64553, 962, 3, 1, 1371.44, 2282.97, 401.408, 2.71809, 7200, 0, 0), -- "Lighting the Way" Kill Credit

(2334, 64710, 962, 3, 1, 845.174, 2299.46, 383.764, 6.0888, 7200, 0, 0), -- Rope

(2335, 65111, 962, 3, 1, 725.826, 2139.69, 401.589, 3.19946, 7200, 0, 0), -- Serpent's Spine Defender

(2336, 65337, 962, 3, 1, 721.424, 2159, 396.785, 5.21535, 7200, 0, 0), -- Serpent's Spine Defender
(2337, 65337, 962, 3, 1, 727.872, 2165.83, 395.13, 0.319897, 7200, 0, 0), -- Serpent's Spine Defender
(2338, 65337, 962, 3, 1, 715.045, 2167.72, 394.575, 1.59296, 7200, 0, 0), -- Serpent's Spine Defender
(2339, 65337, 962, 3, 1, 722.74, 2262.42, 388.123, 4.66873, 7200, 0, 0), -- Serpent's Spine Defender
(2340, 65337, 962, 3, 1, 718.734, 2263.41, 388.132, 4.66873, 7200, 0, 0), -- Serpent's Spine Defender
(2341, 65337, 962, 3, 1, 720.675, 2263.83, 388.12, 4.66873, 7200, 0, 0), -- Serpent's Spine Defender
(2342, 65337, 962, 3, 1, 724.307, 2263.87, 388.129, 4.66873, 7200, 0, 0), -- Serpent's Spine Defender
(2343, 65337, 962, 3, 1, 723.007, 2265.69, 388.07, 4.66873, 7200, 0, 0), -- Serpent's Spine Defender
(2344, 65337, 962, 3, 1, 719.665, 2265.72, 388.054, 4.66873, 7200, 0, 0), -- Serpent's Spine Defender

(2345, 65537, 962, 3, 1, 1236.45, 2303.82, 432.396, 3.01942, 7200, 0, 0), -- Serpent's Spine Cannoneer

(2346, 65555, 962, 3, 1, 1370, 2283.6, 401.328, 2.70164, 7200, 0, 0), -- Bonfire Teleport Stalker

(2347, 66419, 962, 3, 1, 1022.17, 2399.77, 390.856, 5.93393, 7200, 0, 0), -- Serpent's Spine Marksman
(2348, 66419, 962, 3, 1, 1025.6, 2386.09, 392.233, 1.57271, 7200, 0, 0), -- Serpent's Spine Marksman
(2388, 66419, 962, 3, 1, 1029.74, 2399.65, 390.832, 1.21151, 7200, 0, 0), -- Serpent's Spine Marksman
(2389, 66419, 962, 3, 1, 1023.83, 2406.97, 390.863, 5.23829, 7200, 0, 0), -- Serpent's Spine Marksman

(2390, 66905, 962, 3, 1, 1200.34, 2272.93, 388.246, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2391, 66905, 962, 3, 1, 1199.78, 2336.76, 388.247, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2499, 66905, 962, 3, 1, 1190.16, 2272.2, 388.244, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2503, 66905, 962, 3, 1, 1098.7, 2305.02, 381.319, 3.17729, 7200, 0, 0), -- Pandaren Cannoneer
(2504, 66905, 962, 3, 1, 1190.23, 2336.8, 388.251, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2505, 66905, 962, 3, 1, 1200.34, 2272.93, 388.246, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2506, 66905, 962, 3, 1, 1190.16, 2272.2, 388.244, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2507, 66905, 962, 3, 1, 1199.78, 2336.76, 388.247, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2508, 66905, 962, 3, 1, 1190.23, 2336.8, 388.251, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2509, 66905, 962, 3, 1, 1200.34, 2272.93, 388.246, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2510, 66905, 962, 3, 1, 1190.16, 2272.2, 388.244, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2511, 66905, 962, 3, 1, 1199.78, 2336.76, 388.247, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2512, 66905, 962, 3, 1, 1190.23, 2336.8, 388.251, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2513, 66905, 962, 3, 1, 1200.34, 2272.93, 388.246, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2514, 66905, 962, 3, 1, 1199.78, 2336.76, 388.247, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2515, 66905, 962, 3, 1, 1190.23, 2336.8, 388.251, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2516, 66905, 962, 3, 1, 1190.16, 2272.2, 388.244, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2517, 66905, 962, 3, 1, 1190.16, 2272.2, 388.244, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2518, 66905, 962, 3, 1, 1200.34, 2272.93, 388.246, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2519, 66905, 962, 3, 1, 1190.23, 2336.8, 388.251, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2520, 66905, 962, 3, 1, 1199.78, 2336.76, 388.247, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2521, 66905, 962, 3, 1, 1199.78, 2336.76, 388.247, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2522, 66905, 962, 3, 1, 1200.34, 2272.93, 388.246, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2523, 66905, 962, 3, 1, 1190.23, 2336.8, 388.251, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2524, 66905, 962, 3, 1, 1190.16, 2272.2, 388.244, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2525, 66905, 962, 3, 1, 1190.23, 2336.8, 388.251, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2526, 66905, 962, 3, 1, 1200.34, 2272.93, 388.246, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2527, 66905, 962, 3, 1, 1190.16, 2272.2, 388.244, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2528, 66905, 962, 3, 1, 1199.78, 2336.76, 388.247, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2529, 66905, 962, 3, 1, 1190.23, 2336.8, 388.251, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2530, 66905, 962, 3, 1, 1190.16, 2272.2, 388.244, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2531, 66905, 962, 3, 1, 1199.78, 2336.76, 388.247, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2532, 66905, 962, 3, 1, 1200.34, 2272.93, 388.246, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2533, 66905, 962, 3, 1, 1199.78, 2336.76, 388.247, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2534, 66905, 962, 3, 1, 1200.34, 2272.93, 388.246, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2535, 66905, 962, 3, 1, 1190.23, 2336.8, 388.251, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2536, 66905, 962, 3, 1, 1190.16, 2272.2, 388.244, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2537, 66905, 962, 3, 1, 1190.16, 2272.2, 388.244, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2538, 66905, 962, 3, 1, 1190.23, 2336.8, 388.251, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2539, 66905, 962, 3, 1, 1199.78, 2336.76, 388.247, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2540, 66905, 962, 3, 1, 1200.34, 2272.93, 388.246, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2541, 66905, 962, 3, 1, 1199.78, 2336.76, 388.247, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2542, 66905, 962, 3, 1, 1190.23, 2336.8, 388.251, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2543, 66905, 962, 3, 1, 1190.16, 2272.2, 388.244, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2544, 66905, 962, 3, 1, 1200.34, 2272.93, 388.246, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2545, 66905, 962, 3, 1, 1200.34, 2272.93, 388.246, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2546, 66905, 962, 3, 1, 1190.16, 2272.2, 388.244, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2547, 66905, 962, 3, 1, 1190.23, 2336.8, 388.251, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2548, 66905, 962, 3, 1, 1199.78, 2336.76, 388.247, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2549, 66905, 962, 3, 1, 1200.34, 2272.93, 388.246, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2550, 66905, 962, 3, 1, 1199.78, 2336.76, 388.247, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2551, 66905, 962, 3, 1, 1190.16, 2272.2, 388.244, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2552, 66905, 962, 3, 1, 1190.23, 2336.8, 388.251, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2553, 66905, 962, 3, 1, 1199.78, 2336.76, 388.247, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2555, 66905, 962, 3, 1, 1190.23, 2336.8, 388.251, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2556, 66905, 962, 3, 1, 1200.34, 2272.93, 388.246, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2557, 66905, 962, 3, 1, 1190.16, 2272.2, 388.244, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2558, 66905, 962, 3, 1, 1190.16, 2272.2, 388.244, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2559, 66905, 962, 3, 1, 1190.23, 2336.8, 388.251, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2560, 66905, 962, 3, 1, 1200.34, 2272.93, 388.246, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2561, 66905, 962, 3, 1, 1199.78, 2336.76, 388.247, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2562, 66905, 962, 3, 1, 1190.23, 2336.8, 388.251, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2563, 66905, 962, 3, 1, 1199.78, 2336.76, 388.247, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2564, 66905, 962, 3, 1, 1200.34, 2272.93, 388.246, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2565, 66905, 962, 3, 1, 1190.16, 2272.2, 388.244, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2566, 66905, 962, 3, 1, 1200.34, 2272.93, 388.246, 1.66522, 7200, 0, 0), -- Pandaren Cannoneer
(2567, 66905, 962, 3, 1, 1190.23, 2336.8, 388.251, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2568, 66905, 962, 3, 1, 1199.78, 2336.76, 388.247, 4.71409, 7200, 0, 0), -- Pandaren Cannoneer
(2569, 66905, 962, 3, 1, 1190.16, 2272.2, 388.244, 1.66522, 7200, 0, 0); -- Pandaren Cannoneer

CREATE TABLE IF NOT EXISTS `battle_pet_breed` (
    `species` smallint(5) unsigned NOT NULL DEFAULT '0',
    `breed` tinyint(3) unsigned NOT NULL DEFAULT '0',
PRIMARY KEY (`species`,`breed`));

INSERT INTO `battle_pet_breed` (`species`, `breed`) VALUES
    (39, 11),
    (40, 8),
    (41, 4),
    (42, 7),
    (43, 3),
    (44, 5),
    (45, 3),
    (46, 12),
    (47, 3),
    (47, 5),
    (49, 3),
    (50, 11),
    (51, 3),
    (51, 5),
    (52, 3),
    (52, 10),
    (52, 11),
    (52, 12),
    (55, 3),
    (55, 5),
    (55, 6),
    (55, 9),
    (56, 8),
    (57, 4),
    (58, 8),
    (59, 9),
    (64, 9),
    (65, 12),
    (67, 3),
    (67, 8),
    (67, 10),
    (68, 3),
    (68, 6),
    (68, 7),
    (69, 3),
    (69, 10),
    (69, 11),
    (70, 3),
    (70, 5),
    (70, 12),
    (72, 3),
    (72, 5),
    (72, 11),
    (74, 11),
    (75, 5),
    (77, 3),
    (78, 12),
    (83, 8),
    (84, 3),
    (84, 10),
    (84, 11),
    (84, 12),
    (85, 9),
    (86, 7),
    (87, 5),
    (89, 6),
    (90, 7),
    (92, 3),
    (93, 4),
    (94, 5),
    (95, 3),
    (106, 10),
    (107, 3),
    (111, 12),
    (114, 12),
    (116, 12),
    (117, 3),
    (118, 3),
    (119, 3),
    (120, 3),
    (121, 3),
    (122, 5),
    (124, 3),
    (125, 5),
    (126, 10),
    (127, 7),
    (128, 3),
    (130, 3),
    (131, 8),
    (132, 6),
    (136, 3),
    (136, 7),
    (136, 10),
    (136, 12),
    (137, 3),
    (137, 5),
    (137, 11),
    (138, 3),
    (138, 12),
    (139, 3),
    (139, 8),
    (139, 12),
    (140, 3),
    (140, 4),
    (140, 12),
    (141, 3),
    (141, 6),
    (141, 12),
    (142, 3),
    (142, 5),
    (143, 3),
    (143, 5),
    (143, 8),
    (144, 3),
    (144, 5),
    (144, 11),
    (145, 3),
    (145, 5),
    (145, 6),
    (145, 8),
    (146, 3),
    (146, 5),
    (146, 9),
    (146, 12),
    (149, 9),
    (153, 7),
    (155, 7),
    (156, 3),
    (157, 3),
    (158, 3),
    (159, 6),
    (160, 4),
    (162, 10),
    (163, 8),
    (164, 3),
    (165, 9),
    (166, 7),
    (167, 3),
    (167, 6),
    (167, 9),
    (168, 5),
    (169, 5),
    (170, 7),
    (171, 7),
    (172, 10),
    (173, 7),
    (174, 3),
    (175, 5),
    (179, 7),
    (180, 7),
    (183, 3),
    (186, 3),
    (186, 4),
    (186, 5),
    (186, 8),
    (187, 8),
    (188, 6),
    (189, 3),
    (190, 6),
    (190, 7),
    (190, 12),
    (191, 3),
    (192, 9),
    (193, 4),
    (194, 3),
    (194, 5),
    (194, 8),
    (194, 9),
    (194, 11),
    (195, 5),
    (195, 8),
    (195, 9),
    (195, 11),
    (196, 7),
    (197, 8),
    (198, 11),
    (199, 10),
    (200, 5),
    (201, 7),
    (202, 7),
    (203, 12),
    (204, 3),
    (205, 7),
    (206, 3),
    (206, 9),
    (206, 11),
    (207, 8),
    (209, 3),
    (210, 5),
    (211, 6),
    (212, 3),
    (213, 5),
    (214, 3),
    (215, 10),
    (216, 3),
    (217, 7),
    (218, 4),
    (220, 9),
    (224, 10),
    (225, 3),
    (226, 4),
    (227, 4),
    (228, 7),
    (229, 3),
    (231, 3),
    (232, 8),
    (233, 8),
    (234, 10),
    (235, 10),
    (236, 6),
    (237, 3),
    (238, 11),
    (239, 4),
    (240, 3),
    (241, 12),
    (242, 8),
    (243, 10),
    (244, 7),
    (245, 8),
    (246, 3),
    (247, 6),
    (248, 8),
    (249, 7),
    (250, 8),
    (251, 3),
    (253, 3),
    (254, 3),
    (254, 5),
    (254, 8),
    (255, 6),
    (256, 7),
    (258, 6),
    (259, 12),
    (260, 8),
    (261, 4),
    (262, 5),
    (264, 8),
    (265, 6),
    (266, 8),
    (267, 3),
    (268, 4),
    (270, 9),
    (271, 3),
    (271, 9),
    (271, 10),
    (272, 6),
    (272, 9),
    (277, 11),
    (278, 9),
    (279, 8),
    (280, 3),
    (281, 3),
    (282, 3),
    (283, 3),
    (285, 10),
    (286, 7),
    (287, 3),
    (289, 6),
    (291, 12),
    (292, 3),
    (293, 9),
    (294, 8),
    (296, 3),
    (297, 4),
    (298, 3),
    (301, 11),
    (302, 7),
    (303, 5),
    (306, 11),
    (307, 3),
    (308, 3),
    (309, 8),
    (310, 8),
    (311, 3),
    (316, 3),
    (317, 3),
    (318, 3),
    (319, 10),
    (320, 7),
    (321, 6),
    (323, 8),
    (325, 5),
    (325, 8),
    (325, 10),
    (328, 4),
    (329, 8),
    (330, 10),
    (331, 3),
    (332, 3),
    (333, 11),
    (335, 12),
    (336, 3),
    (337, 12),
    (338, 7),
    (339, 12),
    (340, 3),
    (341, 3),
    (342, 3),
    (343, 3),
    (343, 8),
    (346, 3),
    (347, 3),
    (348, 8),
    (374, 7),
    (374, 8),
    (374, 9),
    (374, 10),
    (374, 11),
    (374, 12),
    (378, 3),
    (378, 5),
    (378, 7),
    (378, 9),
    (378, 11),
    (378, 12),
    (379, 3),
    (379, 5),
    (379, 10),
    (379, 11),
    (379, 12),
    (380, 3),
    (380, 12),
    (381, 9),
    (383, 8),
    (383, 11),
    (385, 3),
    (385, 5),
    (385, 8),
    (385, 11),
    (385, 12),
    (386, 3),
    (386, 5),
    (386, 11),
    (387, 3),
    (387, 5),
    (388, 3),
    (388, 6),
    (388, 7),
    (389, 3),
    (389, 4),
    (389, 5),
    (389, 6),
    (389, 7),
    (389, 8),
    (389, 9),
    (389, 10),
    (389, 11),
    (389, 12),
    (391, 3),
    (391, 5),
    (391, 9),
    (392, 3),
    (392, 5),
    (392, 9),
    (392, 11),
    (393, 5),
    (393, 6),
    (393, 7),
    (393, 9),
    (393, 11),
    (393, 12),
    (395, 3),
    (395, 6),
    (395, 7),
    (396, 8),
    (396, 11),
    (397, 3),
    (397, 9),
    (397, 11),
    (397, 12),
    (398, 3),
    (398, 5),
    (398, 9),
    (398, 11),
    (399, 3),
    (399, 5),
    (399, 8),
    (400, 3),
    (400, 4),
    (400, 9),
    (400, 11),
    (401, 3),
    (401, 6),
    (401, 7),
    (402, 3),
    (402, 9),
    (402, 12),
    (403, 3),
    (403, 5),
    (403, 8),
    (403, 10),
    (404, 3),
    (404, 12),
    (405, 3),
    (405, 4),
    (405, 7),
    (406, 3),
    (406, 6),
    (406, 7),
    (406, 9),
    (407, 3),
    (407, 5),
    (407, 8),
    (407, 11),
    (408, 3),
    (408, 5),
    (408, 11),
    (409, 5),
    (409, 8),
    (409, 10),
    (410, 3),
    (410, 5),
    (410, 9),
    (410, 11),
    (411, 3),
    (411, 4),
    (412, 3),
    (412, 5),
    (412, 11),
    (414, 3),
    (414, 8),
    (415, 6),
    (415, 7),
    (415, 9),
    (416, 3),
    (416, 5),
    (416, 8),
    (417, 3),
    (417, 5),
    (417, 9),
    (417, 11),
    (418, 3),
    (418, 11),
    (419, 3),
    (419, 11),
    (420, 3),
    (420, 7),
    (420, 12),
    (421, 3),
    (421, 9),
    (421, 12),
    (422, 3),
    (422, 5),
    (422, 8),
    (423, 3),
    (423, 6),
    (423, 9),
    (424, 5),
    (424, 6),
    (424, 7),
    (424, 9),
    (424, 11),
    (424, 12),
    (425, 3),
    (425, 8),
    (425, 10),
    (427, 3),
    (427, 9),
    (428, 3),
    (428, 5),
    (428, 8),
    (428, 11),
    (429, 4),
    (429, 6),
    (429, 7),
    (429, 9),
    (430, 5),
    (431, 8),
    (431, 9),
    (432, 3),
    (432, 8),
    (433, 3),
    (433, 5),
    (433, 7),
    (437, 3),
    (437, 10),
    (438, 6),
    (438, 9),
    (439, 6),
    (439, 9),
    (440, 3),
    (440, 4),
    (440, 8),
    (441, 5),
    (441, 7),
    (441, 9),
    (442, 6),
    (442, 7),
    (442, 9),
    (442, 12),
    (443, 3),
    (443, 5),
    (443, 7),
    (443, 9),
    (443, 11),
    (443, 12),
    (445, 6),
    (445, 7),
    (445, 9),
    (445, 10),
    (446, 3),
    (446, 4),
    (446, 6),
    (446, 9),
    (447, 3),
    (447, 5),
    (447, 9),
    (447, 11),
    (447, 12),
    (448, 3),
    (448, 5),
    (448, 7),
    (448, 9),
    (448, 11),
    (448, 12),
    (449, 3),
    (449, 5),
    (449, 11),
    (450, 3),
    (450, 6),
    (452, 3),
    (452, 5),
    (452, 10),
    (452, 11),
    (452, 12),
    (453, 6),
    (453, 7),
    (454, 3),
    (454, 5),
    (454, 9),
    (454, 11),
    (455, 7),
    (455, 8),
    (455, 10),
    (456, 3),
    (456, 6),
    (456, 7),
    (456, 8),
    (457, 6),
    (457, 9),
    (458, 3),
    (458, 6),
    (459, 3),
    (459, 5),
    (459, 8),
    (459, 11),
    (460, 3),
    (460, 6),
    (460, 9),
    (461, 3),
    (463, 6),
    (463, 9),
    (464, 3),
    (464, 5),
    (464, 9),
    (465, 3),
    (465, 4),
    (465, 8),
    (466, 3),
    (466, 5),
    (466, 7),
    (467, 3),
    (467, 6),
    (467, 7),
    (467, 9),
    (468, 3),
    (468, 6),
    (468, 7),
    (468, 9),
    (469, 3),
    (469, 4),
    (469, 6),
    (469, 7),
    (469, 9),
    (470, 3),
    (470, 4),
    (470, 7),
    (470, 11),
    (471, 3),
    (471, 5),
    (471, 6),
    (471, 8),
    (471, 9),
    (471, 10),
    (472, 3),
    (472, 4),
    (472, 5),
    (472, 6),
    (472, 7),
    (472, 8),
    (472, 10),
    (472, 11),
    (473, 3),
    (473, 9),
    (474, 5),
    (475, 3),
    (475, 12),
    (477, 3),
    (477, 11),
    (478, 3),
    (478, 9),
    (478, 11),
    (478, 12),
    (479, 3),
    (479, 11),
    (480, 3),
    (480, 6),
    (480, 9),
    (482, 3),
    (482, 5),
    (483, 3),
    (483, 12),
    (484, 3),
    (484, 5),
    (484, 8),
    (484, 11),
    (485, 6),
    (485, 7),
    (485, 9),
    (487, 3),
    (487, 5),
    (487, 7),
    (487, 10),
    (487, 11),
    (487, 12),
    (488, 12),
    (489, 4),
    (489, 8),
    (489, 10),
    (491, 5),
    (491, 8),
    (492, 3),
    (492, 6),
    (492, 9),
    (493, 3),
    (493, 6),
    (493, 7),
    (494, 3),
    (494, 6),
    (494, 7),
    (495, 3),
    (495, 12),
    (496, 3),
    (496, 9),
    (496, 12),
    (497, 6),
    (497, 7),
    (497, 9),
    (497, 12),
    (498, 3),
    (498, 8),
    (498, 9),
    (499, 7),
    (500, 3),
    (500, 4),
    (500, 6),
    (500, 7),
    (500, 8),
    (500, 10),
    (502, 3),
    (502, 12),
    (503, 3),
    (503, 5),
    (503, 11),
    (504, 3),
    (504, 4),
    (504, 10),
    (505, 3),
    (505, 5),
    (505, 7),
    (505, 11),
    (506, 3),
    (506, 6),
    (506, 9),
    (506, 11),
    (507, 3),
    (507, 9),
    (507, 12),
    (508, 3),
    (508, 4),
    (508, 7),
    (509, 6),
    (509, 7),
    (509, 9),
    (511, 3),
    (511, 5),
    (511, 11),
    (512, 4),
    (512, 6),
    (512, 9),
    (512, 12),
    (513, 3),
    (513, 4),
    (513, 5),
    (513, 6),
    (513, 7),
    (513, 8),
    (513, 9),
    (513, 10),
    (513, 11),
    (513, 12),
    (514, 3),
    (514, 4),
    (514, 5),
    (514, 6),
    (514, 7),
    (514, 8),
    (514, 9),
    (514, 10),
    (514, 11),
    (514, 12),
    (515, 3),
    (515, 4),
    (515, 5),
    (515, 6),
    (515, 7),
    (515, 8),
    (515, 9),
    (515, 10),
    (515, 11),
    (515, 12),
    (517, 3),
    (517, 8),
    (518, 3),
    (518, 6),
    (519, 6),
    (519, 7),
    (519, 9),
    (521, 10),
    (523, 6),
    (523, 9),
    (525, 3),
    (525, 5),
    (525, 12),
    (528, 9),
    (528, 12),
    (529, 4),
    (529, 7),
    (530, 3),
    (530, 6),
    (530, 7),
    (530, 9),
    (532, 4),
    (532, 6),
    (534, 3),
    (534, 4),
    (534, 5),
    (534, 8),
    (534, 10),
    (535, 3),
    (535, 9),
    (536, 7),
    (536, 9),
    (537, 3),
    (537, 4),
    (537, 6),
    (537, 7),
    (538, 6),
    (538, 8),
    (539, 3),
    (539, 5),
    (539, 10),
    (539, 11),
    (539, 12),
    (540, 3),
    (540, 5),
    (540, 10),
    (540, 11),
    (540, 12),
    (541, 5),
    (541, 6),
    (541, 7),
    (541, 9),
    (542, 3),
    (542, 12),
    (543, 3),
    (543, 5),
    (543, 9),
    (544, 5),
    (544, 11),
    (544, 12),
    (545, 3),
    (545, 5),
    (545, 8),
    (546, 6),
    (546, 9),
    (546, 12),
    (547, 3),
    (547, 5),
    (547, 7),
    (547, 9),
    (547, 10),
    (547, 11),
    (548, 3),
    (548, 4),
    (548, 5),
    (548, 6),
    (548, 7),
    (548, 8),
    (548, 9),
    (548, 10),
    (548, 11),
    (548, 12),
    (549, 3),
    (549, 5),
    (549, 10),
    (549, 11),
    (549, 12),
    (550, 3),
    (550, 5),
    (550, 10),
    (550, 11),
    (550, 12),
    (552, 3),
    (552, 4),
    (552, 6),
    (552, 7),
    (552, 11),
    (553, 3),
    (553, 5),
    (553, 10),
    (553, 11),
    (553, 12),
    (554, 3),
    (554, 7),
    (554, 9),
    (555, 5),
    (555, 6),
    (555, 7),
    (555, 9),
    (556, 6),
    (556, 9),
    (556, 12),
    (557, 4),
    (557, 5),
    (557, 7),
    (557, 10),
    (557, 11),
    (558, 8),
    (559, 4),
    (559, 6),
    (559, 7),
    (559, 9),
    (560, 3),
    (560, 5),
    (560, 10),
    (562, 3),
    (562, 5),
    (562, 8),
    (564, 3),
    (564, 6),
    (564, 12),
    (565, 3),
    (565, 12),
    (566, 3),
    (566, 9),
    (566, 12),
    (567, 5),
    (567, 10),
    (568, 3),
    (568, 9),
    (569, 3),
    (569, 12),
    (570, 3),
    (570, 10),
    (571, 3),
    (571, 5),
    (572, 4),
    (572, 6),
    (573, 3),
    (573, 5),
    (573, 10),
    (573, 11),
    (626, 4),
    (626, 5),
    (626, 7),
    (626, 8),
    (626, 10),
    (626, 11),
    (627, 7),
    (627, 10),
    (628, 3),
    (628, 10),
    (629, 3),
    (629, 4),
    (629, 7),
    (630, 3),
    (630, 5),
    (630, 8),
    (630, 10),
    (631, 7),
    (631, 9),
    (632, 3),
    (632, 10),
    (633, 3),
    (633, 7),
    (633, 9),
    (633, 12),
    (634, 3),
    (634, 9),
    (635, 3),
    (635, 8),
    (635, 10),
    (637, 3),
    (637, 5),
    (637, 11),
    (638, 5),
    (638, 6),
    (638, 7),
    (638, 9),
    (639, 3),
    (639, 5),
    (639, 10),
    (639, 11),
    (639, 12),
    (640, 3),
    (640, 5),
    (640, 7),
    (640, 9),
    (640, 11),
    (640, 12),
    (641, 3),
    (641, 5),
    (641, 7),
    (641, 9),
    (641, 11),
    (641, 12),
    (644, 3),
    (644, 5),
    (644, 10),
    (644, 11),
    (644, 12),
    (645, 3),
    (645, 8),
    (645, 9),
    (646, 3),
    (646, 5),
    (646, 11),
    (646, 12),
    (647, 3),
    (647, 5),
    (647, 8),
    (647, 10),
    (647, 11),
    (648, 3),
    (648, 6),
    (648, 7),
    (648, 12),
    (649, 3),
    (649, 7),
    (649, 8),
    (650, 8),
    (652, 11),
    (665, 11),
    (671, 8),
    (675, 3),
    (675, 5),
    (675, 10),
    (675, 11),
    (675, 12),
    (677, 3),
    (677, 5),
    (677, 8),
    (678, 6),
    (678, 9),
    (679, 5),
    (679, 11),
    (680, 5),
    (680, 11),
    (699, 3),
    (699, 9),
    (699, 11),
    (699, 12),
    (702, 3),
    (702, 5),
    (702, 12),
    (703, 3),
    (703, 11),
    (706, 3),
    (706, 5),
    (706, 8),
    (707, 3),
    (707, 5),
    (708, 3),
    (708, 5),
    (708, 10),
    (708, 11),
    (708, 12),
    (709, 3),
    (709, 5),
    (709, 11),
    (709, 12),
    (710, 3),
    (710, 9),
    (710, 11),
    (711, 3),
    (711, 5),
    (711, 11),
    (712, 3),
    (712, 5),
    (712, 11),
    (713, 3),
    (713, 9),
    (714, 3),
    (714, 4),
    (714, 10),
    (714, 11),
    (716, 7),
    (716, 9),
    (717, 5),
    (717, 6),
    (717, 9),
    (717, 12),
    (718, 3),
    (718, 8),
    (718, 9),
    (722, 3),
    (722, 9),
    (722, 10),
    (722, 12),
    (723, 3),
    (723, 6),
    (723, 9),
    (724, 5),
    (724, 10),
    (725, 3),
    (725, 11),
    (726, 3),
    (726, 5),
    (726, 7),
    (727, 3),
    (727, 5),
    (727, 8),
    (727, 11),
    (727, 12),
    (728, 3),
    (728, 5),
    (728, 11),
    (728, 12),
    (729, 3),
    (729, 5),
    (729, 7),
    (729, 9),
    (729, 11),
    (729, 12),
    (730, 3),
    (730, 5),
    (730, 9),
    (730, 11),
    (730, 12),
    (731, 7),
    (731, 9),
    (732, 8),
    (732, 9),
    (733, 3),
    (733, 5),
    (733, 11),
    (737, 5),
    (737, 8),
    (739, 5),
    (739, 8),
    (740, 3),
    (740, 9),
    (740, 10),
    (740, 11),
    (740, 12),
    (741, 3),
    (741, 12),
    (742, 3),
    (742, 5),
    (743, 3),
    (743, 5),
    (743, 12),
    (744, 5),
    (744, 6),
    (744, 7),
    (744, 9),
    (744, 11),
    (744, 12),
    (745, 3),
    (745, 9),
    (746, 4),
    (746, 6),
    (747, 3),
    (747, 9),
    (747, 10),
    (747, 12),
    (748, 3),
    (748, 5),
    (748, 7),
    (748, 8),
    (748, 9),
    (748, 10),
    (748, 11),
    (748, 12),
    (749, 5),
    (749, 11),
    (750, 3),
    (750, 5),
    (750, 11),
    (751, 3),
    (751, 9),
    (751, 11),
    (751, 12),
    (752, 3),
    (752, 4),
    (752, 12),
    (753, 3),
    (753, 9),
    (754, 3),
    (754, 5),
    (754, 7),
    (754, 8),
    (754, 9),
    (754, 11),
    (754, 12),
    (755, 8),
    (755, 10),
    (756, 3),
    (756, 5),
    (756, 9),
    (757, 5),
    (758, 8),
    (792, 3),
    (792, 12),
    (802, 4),
    (817, 7),
    (818, 8),
    (819, 5),
    (820, 12),
    (821, 11),
    (823, 3),
    (823, 5),
    (823, 9),
    (823, 12),
    (834, 7),
    (835, 5),
    (836, 5),
    (837, 3),
    (837, 4),
    (837, 5),
    (837, 6),
    (837, 7),
    (837, 8),
    (837, 9),
    (837, 10),
    (837, 11),
    (837, 12),
    (838, 3),
    (838, 4),
    (838, 5),
    (838, 6),
    (838, 7),
    (838, 8),
    (838, 9),
    (838, 10),
    (838, 11),
    (838, 12),
    (844, 5),
    (845, 3),
    (845, 8),
    (845, 10),
    (846, 5),
    (847, 12),
    (848, 3),
    (848, 5),
    (848, 9),
    (849, 3),
    (850, 3),
    (851, 3),
    (851, 5),
    (851, 7),
    (855, 3),
    (856, 9),
    (856, 12),
    (868, 9),
    (872, 3),
    (873, 3),
    (874, 3),
    (875, 8),
    (876, 7),
    (877, 9),
    (878, 3),
    (879, 7),
    (880, 7),
    (881, 8),
    (882, 9),
    (883, 6),
    (884, 8),
    (885, 8),
    (886, 9),
    (887, 8),
    (888, 3),
    (889, 3),
    (890, 3),
    (891, 5),
    (892, 8),
    (893, 3),
    (894, 5),
    (895, 8),
    (896, 6),
    (897, 9),
    (898, 3),
    (899, 8),
    (900, 6),
    (901, 6),
    (902, 9),
    (903, 10),
    (904, 9),
    (905, 8),
    (906, 5),
    (907, 9),
    (908, 8),
    (909, 5),
    (911, 7),
    (912, 4),
    (913, 8),
    (915, 7),
    (916, 8),
    (917, 6),
    (921, 7),
    (922, 4),
    (923, 6),
    (924, 9),
    (925, 7),
    (926, 8),
    (927, 5),
    (928, 8),
    (929, 9),
    (931, 9),
    (932, 6),
    (933, 5),
    (934, 6),
    (935, 8),
    (936, 3),
    (937, 8),
    (938, 9),
    (939, 7),
    (941, 8),
    (942, 6),
    (943, 9),
    (944, 6),
    (945, 7),
    (946, 5),
    (947, 9),
    (948, 5),
    (949, 8),
    (950, 6),
    (951, 7),
    (952, 7),
    (953, 6),
    (954, 3),
    (955, 5),
    (956, 6),
    (957, 8),
    (958, 3),
    (959, 8),
    (960, 4),
    (961, 4),
    (962, 8),
    (963, 7),
    (964, 3),
    (965, 3),
    (966, 9),
    (967, 8),
    (968, 7),
    (969, 9),
    (970, 8),
    (971, 7),
    (972, 9),
    (973, 8),
    (974, 8),
    (975, 6),
    (976, 7),
    (977, 9),
    (978, 8),
    (979, 6),
    (980, 6),
    (981, 8),
    (982, 5),
    (983, 6),
    (984, 9),
    (985, 7),
    (986, 4),
    (987, 9),
    (988, 8),
    (989, 9),
    (990, 5),
    (991, 8),
    (992, 6),
    (993, 5),
    (994, 8),
    (995, 9),
    (996, 6),
    (997, 7),
    (998, 9),
    (999, 6),
    (1000, 8),
    (1001, 8),
    (1002, 9),
    (1003, 9),
    (1004, 9),
    (1005, 9),
    (1006, 6),
    (1007, 4),
    (1008, 6),
    (1009, 7),
    (1010, 8),
    (1011, 7),
    (1012, 5),
    (1013, 3),
    (1013, 6),
    (1013, 9),
    (1039, 6),
    (1040, 6),
    (1042, 8),
    (1061, 5),
    (1062, 3),
    (1062, 9),
    (1062, 10),
    (1062, 12),
    (1063, 8),
    (1065, 8),
    (1066, 10),
    (1067, 7),
    (1068, 3),
    (1068, 5),
    (1068, 8),
    (1068, 10),
    (1073, 12),
    (1117, 3),
    (1124, 8),
    (1125, 3),
    (1126, 7),
    (1127, 8),
    (1128, 3),
    (1128, 12),
    (1129, 7),
    (1130, 8),
    (1131, 11),
    (1132, 5),
    (1133, 10),
    (1134, 9),
    (1135, 12),
    (1136, 3),
    (1137, 6),
    (1138, 9),
    (1139, 8),
    (1140, 3),
    (1141, 7),
    (1142, 11),
    (1143, 8),
    (1144, 9),
    (1145, 5),
    (1146, 7),
    (1147, 4),
    (1149, 8),
    (1150, 9),
    (1151, 7),
    (1152, 6),
    (1153, 4),
    (1154, 9),
    (1155, 6),
    (1156, 8),
    (1157, 3),
    (1157, 12),
    (1158, 12),
    (1159, 12),
    (1160, 8),
    (1161, 4),
    (1161, 8),
    (1161, 10),
    (1162, 5),
    (1162, 8),
    (1163, 6),
    (1163, 7),
    (1164, 3),
    (1164, 5),
    (1164, 11),
    (1165, 4),
    (1165, 8),
    (1165, 10),
    (1166, 4),
    (1166, 8),
    (1166, 10),
    (1167, 4),
    (1167, 8),
    (1167, 10),
    (1168, 12),
    (1174, 12),
    (1175, 3),
    (1175, 12),
    (1176, 3),
    (1177, 6),
    (1177, 7),
    (1177, 9),
    (1177, 10),
    (1178, 4),
    (1179, 3),
    (1180, 4),
    (1180, 5),
    (1180, 8),
    (1180, 10),
    (1181, 6),
    (1181, 9),
    (1182, 3),
    (1182, 12),
    (1183, 4),
    (1184, 4),
    (1185, 9),
    (1187, 4),
    (1188, 3),
    (1189, 9),
    (1190, 11),
    (1191, 6),
    (1192, 10),
    (1193, 12),
    (1194, 8),
    (1195, 5),
    (1196, 9),
    (1197, 7),
    (1198, 8),
    (1200, 8),
    (1201, 10),
    (1202, 3),
    (1202, 10),
    (1204, 11),
    (1205, 3),
    (1205, 6),
    (1205, 7),
    (1205, 9),
    (1205, 10),
    (1206, 10),
    (1207, 12),
    (1208, 11),
    (1209, 3),
    (1211, 4),
    (1211, 5),
    (1211, 8),
    (1211, 10),
    (1212, 4),
    (1212, 5),
    (1212, 8),
    (1212, 10),
    (1213, 4),
    (1213, 5),
    (1213, 8),
    (1213, 10),
    (1226, 3),
    (1226, 4),
    (1226, 5),
    (1226, 8),
    (1226, 10),
    (1227, 6),
    (1227, 7),
    (1227, 9),
    (1228, 4),
    (1228, 6),
    (1228, 8),
    (1229, 5),
    (1229, 8),
    (1229, 10),
    (1230, 6),
    (1230, 7),
    (1230, 9),
    (1231, 3),
    (1231, 10),
    (1231, 11),
    (1231, 12),
    (1232, 3),
    (1232, 10),
    (1232, 12),
    (1233, 4),
    (1233, 6),
    (1233, 7),
    (1233, 8),
    (1234, 3),
    (1234, 6),
    (1234, 7),
    (1234, 9),
    (1235, 5),
    (1235, 11),
    (1235, 12),
    (1236, 8),
    (1237, 8),
    (1238, 3),
    (1238, 6),
    (1243, 8),
    (1244, 7),
    (1245, 3),
    (1245, 6),
    (1245, 9),
    (1247, 8),
    (1248, 3),
    (1255, 8),
    (1256, 10),
    (1256, 11),
    (1257, 3),
    (1258, 8),
    (1259, 6),
    (1266, 4),
    (1267, 8),
    (1268, 6),
    (1269, 4),
    (1271, 5),
    (1276, 4),
    (1276, 5),
    (1276, 8),
    (1277, 5),
    (1278, 4),
    (1279, 6),
    (1280, 6),
    (1281, 5),
    (1282, 8),
    (1283, 4),
    (1284, 6),
    (1285, 5),
    (1286, 5),
    (1287, 4),
    (1288, 6),
    (1289, 4),
    (1290, 3),
    (1291, 3),
    (1292, 7),
    (1293, 5),
    (1295, 8),
    (1296, 6),
    (1297, 5),
    (1298, 4),
    (1299, 5),
    (1300, 4),
    (1301, 6),
    (1303, 8),
    (1304, 8),
    (1305, 8),
    (1311, 8),
    (1317, 8),
    (1319, 6),
    (1320, 5),
    (1321, 4),
    (1321, 5),
    (1321, 7),
    (1321, 8),
    (1321, 10),
    (1322, 4),
    (1322, 6),
    (1322, 7),
    (1323, 3),
    (1323, 4),
    (1323, 5),
    (1323, 10),
    (1323, 11),
    (1324, 3),
    (1324, 8),
    (1324, 9),
    (1324, 11),
    (1325, 3),
    (1325, 8),
    (1325, 9),
    (1326, 3),
    (1326, 8),
    (1326, 9),
    (1328, 3),
    (1328, 6),
    (1328, 7),
    (1328, 9),
    (1328, 12),
    (1329, 5),
    (1329, 8),
    (1329, 9),
    (1329, 11),
    (1330, 3),
    (1330, 4),
    (1330, 5),
    (1330, 6),
    (1330, 8),
    (1331, 4),
    (1331, 8),
    (1331, 10),
    (1332, 4),
    (1332, 5),
    (1332, 6),
    (1333, 3),
    (1333, 5),
    (1333, 8),
    (1333, 9),
    (1334, 6),
    (1334, 7),
    (1334, 9),
    (1335, 8),
    (1336, 3),
    (1336, 6),
    (1336, 7),
    (1336, 9),
    (1336, 12),
    (1337, 4),
    (1337, 6),
    (1338, 3),
    (1338, 7),
    (1338, 12),
    (1339, 3),
    (1343, 4),
    (1343, 5),
    (1343, 7),
    (1343, 8),
    (1343, 9),
    (1344, 7),
    (1345, 6),
    (1345, 9),
    (1346, 9),
    (1348, 8),
    (1349, 3),
    (1350, 3),
    (1351, 7),
    (1352, 6),
    (1354, 3),
    (1363, 8),
    (1364, 4),
    (1365, 9),
    (1386, 7);
 CREATE TABLE IF NOT EXISTS `battle_pet_quality` (
    `species` smallint(5) unsigned NOT NULL DEFAULT '0',
    `quality` tinyint(3) unsigned NOT NULL DEFAULT '0',
PRIMARY KEY (`species`,`quality`));

INSERT INTO `battle_pet_quality` (`species`, `quality`) VALUES
    (39, 2),
    (39, 3),
    (40, 2),
    (40, 3),
    (41, 2),
    (41, 3),
    (42, 2),
    (42, 3),
    (43, 2),
    (43, 3),
    (44, 2),
    (44, 3),
    (45, 2),
    (45, 3),
    (46, 2),
    (46, 3),
    (47, 2),
    (47, 3),
    (49, 3),
    (50, 2),
    (50, 3),
    (51, 2),
    (51, 3),
    (52, 2),
    (52, 3),
    (55, 2),
    (55, 3),
    (56, 3),
    (57, 3),
    (58, 3),
    (59, 3),
    (64, 2),
    (64, 3),
    (65, 2),
    (65, 3),
    (67, 2),
    (67, 3),
    (68, 2),
    (68, 3),
    (69, 0),
    (69, 1),
    (69, 2),
    (69, 3),
    (70, 2),
    (70, 3),
    (72, 2),
    (72, 3),
    (74, 2),
    (74, 3),
    (75, 2),
    (75, 3),
    (77, 2),
    (77, 3),
    (78, 2),
    (78, 3),
    (83, 2),
    (83, 3),
    (84, 2),
    (84, 3),
    (85, 2),
    (85, 3),
    (86, 2),
    (86, 3),
    (87, 3),
    (89, 2),
    (89, 3),
    (90, 2),
    (90, 3),
    (92, 3),
    (93, 3),
    (94, 3),
    (95, 2),
    (95, 3),
    (106, 2),
    (106, 3),
    (107, 3),
    (111, 3),
    (114, 3),
    (116, 2),
    (116, 3),
    (117, 2),
    (117, 3),
    (118, 2),
    (118, 3),
    (119, 2),
    (119, 3),
    (120, 2),
    (120, 3),
    (121, 2),
    (121, 3),
    (122, 2),
    (122, 3),
    (124, 2),
    (125, 2),
    (125, 3),
    (126, 2),
    (126, 3),
    (127, 2),
    (127, 3),
    (128, 2),
    (128, 3),
    (130, 3),
    (131, 3),
    (132, 3),
    (136, 2),
    (136, 3),
    (137, 2),
    (137, 3),
    (138, 2),
    (138, 3),
    (139, 2),
    (139, 3),
    (140, 2),
    (140, 3),
    (141, 2),
    (141, 3),
    (142, 2),
    (142, 3),
    (143, 2),
    (143, 3),
    (144, 2),
    (144, 3),
    (145, 2),
    (145, 3),
    (146, 3),
    (149, 2),
    (149, 3),
    (153, 2),
    (153, 3),
    (155, 3),
    (156, 3),
    (157, 2),
    (157, 3),
    (158, 2),
    (158, 3),
    (159, 2),
    (159, 3),
    (160, 3),
    (162, 3),
    (163, 3),
    (164, 3),
    (165, 3),
    (166, 2),
    (166, 3),
    (167, 3),
    (168, 3),
    (169, 3),
    (170, 2),
    (170, 3),
    (171, 2),
    (171, 3),
    (172, 3),
    (173, 3),
    (174, 3),
    (175, 3),
    (179, 3),
    (180, 2),
    (180, 3),
    (183, 3),
    (186, 3),
    (187, 3),
    (188, 3),
    (189, 3),
    (190, 2),
    (190, 3),
    (191, 2),
    (191, 3),
    (192, 2),
    (192, 3),
    (193, 3),
    (194, 2),
    (194, 3),
    (195, 2),
    (195, 3),
    (196, 2),
    (196, 3),
    (197, 2),
    (197, 3),
    (198, 3),
    (199, 3),
    (200, 2),
    (200, 3),
    (201, 2),
    (201, 3),
    (202, 2),
    (202, 3),
    (203, 3),
    (204, 2),
    (204, 3),
    (205, 2),
    (205, 3),
    (206, 2),
    (206, 3),
    (207, 2),
    (207, 3),
    (209, 2),
    (209, 3),
    (210, 2),
    (210, 3),
    (211, 3),
    (212, 2),
    (212, 3),
    (213, 2),
    (213, 3),
    (215, 2),
    (215, 3),
    (217, 3),
    (218, 2),
    (218, 3),
    (220, 2),
    (220, 3),
    (224, 2),
    (224, 3),
    (225, 2),
    (225, 3),
    (226, 2),
    (226, 3),
    (227, 2),
    (227, 3),
    (228, 3),
    (229, 3),
    (231, 2),
    (231, 3),
    (232, 2),
    (232, 3),
    (233, 2),
    (233, 3),
    (234, 3),
    (235, 2),
    (235, 3),
    (236, 2),
    (236, 3),
    (237, 2),
    (237, 3),
    (238, 2),
    (238, 3),
    (239, 3),
    (240, 2),
    (241, 3),
    (242, 3),
    (243, 3),
    (244, 3),
    (245, 3),
    (246, 3),
    (247, 2),
    (247, 3),
    (248, 3),
    (249, 3),
    (250, 2),
    (250, 3),
    (251, 2),
    (251, 3),
    (253, 3),
    (254, 2),
    (254, 3),
    (255, 3),
    (256, 3),
    (258, 3),
    (259, 2),
    (259, 3),
    (260, 2),
    (260, 3),
    (261, 2),
    (261, 3),
    (262, 2),
    (262, 3),
    (264, 3),
    (265, 3),
    (266, 3),
    (267, 2),
    (267, 3),
    (268, 3),
    (270, 3),
    (271, 2),
    (271, 3),
    (272, 2),
    (272, 3),
    (277, 3),
    (278, 3),
    (279, 3),
    (285, 3),
    (286, 3),
    (287, 2),
    (287, 3),
    (289, 2),
    (289, 3),
    (291, 3),
    (292, 2),
    (292, 3),
    (293, 3),
    (294, 3),
    (296, 3),
    (297, 3),
    (298, 3),
    (301, 3),
    (302, 3),
    (303, 3),
    (306, 2),
    (306, 3),
    (307, 3),
    (308, 2),
    (308, 3),
    (309, 3),
    (310, 3),
    (311, 3),
    (316, 3),
    (317, 3),
    (318, 3),
    (319, 2),
    (319, 3),
    (320, 3),
    (321, 2),
    (321, 3),
    (323, 3),
    (325, 3),
    (328, 3),
    (329, 3),
    (330, 3),
    (333, 3),
    (335, 3),
    (337, 3),
    (338, 3),
    (339, 3),
    (340, 2),
    (340, 3),
    (341, 2),
    (341, 3),
    (342, 2),
    (342, 3),
    (343, 3),
    (346, 3),
    (347, 3),
    (348, 3),
    (374, 0),
    (374, 1),
    (374, 2),
    (374, 3),
    (378, 0),
    (378, 1),
    (378, 2),
    (378, 3),
    (379, 0),
    (379, 1),
    (379, 2),
    (379, 3),
    (380, 0),
    (380, 1),
    (380, 2),
    (380, 3),
    (381, 3),
    (383, 0),
    (383, 1),
    (383, 2),
    (383, 3),
    (385, 0),
    (385, 1),
    (385, 2),
    (385, 3),
    (386, 0),
    (386, 1),
    (386, 2),
    (386, 3),
    (387, 0),
    (387, 1),
    (387, 2),
    (387, 3),
    (388, 0),
    (388, 1),
    (388, 2),
    (388, 3),
    (389, 0),
    (389, 1),
    (389, 2),
    (389, 3),
    (391, 0),
    (391, 1),
    (391, 2),
    (391, 3),
    (392, 0),
    (392, 1),
    (392, 2),
    (392, 3),
    (393, 0),
    (393, 1),
    (393, 2),
    (393, 3),
    (395, 0),
    (395, 1),
    (395, 2),
    (395, 3),
    (396, 0),
    (396, 1),
    (396, 2),
    (396, 3),
    (397, 0),
    (397, 1),
    (397, 2),
    (397, 3),
    (398, 0),
    (398, 1),
    (398, 2),
    (398, 3),
    (399, 0),
    (399, 1),
    (399, 2),
    (399, 3),
    (400, 0),
    (400, 1),
    (400, 2),
    (400, 3),
    (401, 0),
    (401, 1),
    (401, 2),
    (401, 3),
    (402, 0),
    (402, 1),
    (402, 2),
    (402, 3),
    (403, 0),
    (403, 1),
    (403, 2),
    (403, 3),
    (404, 0),
    (404, 1),
    (404, 2),
    (404, 3),
    (405, 0),
    (405, 1),
    (405, 2),
    (405, 3),
    (406, 0),
    (406, 1),
    (406, 2),
    (406, 3),
    (407, 0),
    (407, 1),
    (407, 2),
    (407, 3),
    (408, 0),
    (408, 1),
    (408, 2),
    (408, 3),
    (409, 0),
    (409, 1),
    (409, 2),
    (409, 3),
    (410, 0),
    (410, 1),
    (410, 2),
    (410, 3),
    (411, 0),
    (411, 1),
    (411, 2),
    (411, 3),
    (412, 0),
    (412, 1),
    (412, 2),
    (412, 3),
    (414, 0),
    (414, 1),
    (414, 2),
    (414, 3),
    (415, 0),
    (415, 1),
    (415, 2),
    (415, 3),
    (416, 0),
    (416, 1),
    (416, 2),
    (416, 3),
    (417, 0),
    (417, 1),
    (417, 2),
    (417, 3),
    (418, 0),
    (418, 1),
    (418, 2),
    (418, 3),
    (419, 0),
    (419, 1),
    (419, 2),
    (419, 3),
    (420, 0),
    (420, 1),
    (420, 2),
    (420, 3),
    (421, 0),
    (421, 1),
    (421, 2),
    (421, 3),
    (422, 0),
    (422, 1),
    (422, 2),
    (422, 3),
    (423, 0),
    (423, 1),
    (423, 2),
    (423, 3),
    (424, 0),
    (424, 1),
    (424, 2),
    (424, 3),
    (425, 0),
    (425, 1),
    (425, 2),
    (425, 3),
    (427, 0),
    (427, 1),
    (427, 2),
    (427, 3),
    (428, 0),
    (428, 1),
    (428, 2),
    (428, 3),
    (429, 0),
    (429, 1),
    (429, 2),
    (429, 3),
    (430, 0),
    (430, 1),
    (430, 2),
    (430, 3),
    (431, 0),
    (431, 1),
    (431, 2),
    (431, 3),
    (432, 0),
    (432, 1),
    (432, 2),
    (432, 3),
    (433, 0),
    (433, 1),
    (433, 2),
    (433, 3),
    (437, 0),
    (437, 1),
    (437, 2),
    (437, 3),
    (438, 0),
    (438, 1),
    (438, 2),
    (438, 3),
    (439, 0),
    (439, 1),
    (439, 2),
    (439, 3),
    (440, 0),
    (440, 1),
    (440, 2),
    (440, 3),
    (441, 0),
    (441, 1),
    (441, 2),
    (441, 3),
    (442, 0),
    (442, 1),
    (442, 2),
    (442, 3),
    (443, 0),
    (443, 1),
    (443, 2),
    (443, 3),
    (445, 0),
    (445, 1),
    (445, 2),
    (445, 3),
    (446, 0),
    (446, 1),
    (446, 2),
    (446, 3),
    (447, 0),
    (447, 1),
    (447, 2),
    (447, 3),
    (448, 0),
    (448, 1),
    (448, 2),
    (448, 3),
    (449, 0),
    (449, 1),
    (449, 2),
    (449, 3),
    (450, 0),
    (450, 1),
    (450, 2),
    (450, 3),
    (452, 0),
    (452, 1),
    (452, 2),
    (452, 3),
    (453, 0),
    (453, 1),
    (453, 2),
    (453, 3),
    (454, 0),
    (454, 1),
    (454, 2),
    (454, 3),
    (455, 0),
    (455, 1),
    (455, 2),
    (455, 3),
    (456, 0),
    (456, 1),
    (456, 2),
    (456, 3),
    (457, 0),
    (457, 1),
    (457, 2),
    (457, 3),
    (458, 0),
    (458, 1),
    (458, 2),
    (458, 3),
    (459, 0),
    (459, 1),
    (459, 2),
    (459, 3),
    (460, 0),
    (460, 1),
    (460, 2),
    (460, 3),
    (461, 0),
    (461, 1),
    (461, 2),
    (461, 3),
    (463, 0),
    (463, 1),
    (463, 2),
    (463, 3),
    (464, 0),
    (464, 1),
    (464, 2),
    (464, 3),
    (465, 0),
    (465, 1),
    (465, 2),
    (465, 3),
    (466, 0),
    (466, 1),
    (466, 2),
    (466, 3),
    (467, 0),
    (467, 1),
    (467, 2),
    (467, 3),
    (468, 0),
    (468, 1),
    (468, 2),
    (468, 3),
    (469, 0),
    (469, 1),
    (469, 2),
    (469, 3),
    (470, 0),
    (470, 1),
    (470, 2),
    (470, 3),
    (471, 0),
    (471, 1),
    (471, 2),
    (471, 3),
    (472, 0),
    (472, 1),
    (472, 2),
    (472, 3),
    (473, 0),
    (473, 1),
    (473, 2),
    (473, 3),
    (474, 0),
    (474, 1),
    (474, 2),
    (474, 3),
    (475, 0),
    (475, 1),
    (475, 2),
    (475, 3),
    (477, 0),
    (477, 1),
    (477, 2),
    (477, 3),
    (478, 0),
    (478, 1),
    (478, 2),
    (478, 3),
    (479, 0),
    (479, 1),
    (479, 2),
    (479, 3),
    (480, 0),
    (480, 1),
    (480, 2),
    (480, 3),
    (482, 0),
    (482, 1),
    (482, 2),
    (482, 3),
    (483, 0),
    (483, 1),
    (483, 2),
    (483, 3),
    (484, 0),
    (484, 1),
    (484, 2),
    (484, 3),
    (485, 0),
    (485, 1),
    (485, 2),
    (485, 3),
    (487, 0),
    (487, 1),
    (487, 2),
    (487, 3),
    (488, 0),
    (488, 1),
    (488, 2),
    (488, 3),
    (489, 0),
    (489, 1),
    (489, 2),
    (489, 3),
    (491, 0),
    (491, 1),
    (491, 2),
    (491, 3),
    (492, 0),
    (492, 1),
    (492, 2),
    (492, 3),
    (493, 0),
    (493, 1),
    (493, 2),
    (493, 3),
    (494, 0),
    (494, 1),
    (494, 2),
    (494, 3),
    (495, 0),
    (495, 1),
    (495, 2),
    (495, 3),
    (496, 0),
    (496, 1),
    (496, 2),
    (496, 3),
    (497, 0),
    (497, 1),
    (497, 2),
    (497, 3),
    (498, 0),
    (498, 1),
    (498, 2),
    (498, 3),
    (499, 0),
    (499, 1),
    (499, 2),
    (499, 3),
    (500, 0),
    (500, 1),
    (500, 2),
    (500, 3),
    (502, 0),
    (502, 1),
    (502, 2),
    (502, 3),
    (503, 0),
    (503, 1),
    (503, 2),
    (503, 3),
    (504, 0),
    (504, 1),
    (504, 2),
    (504, 3),
    (505, 0),
    (505, 1),
    (505, 2),
    (505, 3),
    (506, 0),
    (506, 1),
    (506, 2),
    (506, 3),
    (507, 0),
    (507, 1),
    (507, 2),
    (507, 3),
    (508, 0),
    (508, 1),
    (508, 2),
    (508, 3),
    (509, 0),
    (509, 1),
    (509, 2),
    (509, 3),
    (511, 0),
    (511, 1),
    (511, 2),
    (511, 3),
    (512, 0),
    (512, 1),
    (512, 2),
    (512, 3),
    (513, 0),
    (513, 1),
    (513, 2),
    (513, 3),
    (514, 0),
    (514, 1),
    (514, 2),
    (514, 3),
    (515, 0),
    (515, 1),
    (515, 2),
    (515, 3),
    (517, 0),
    (517, 1),
    (517, 2),
    (517, 3),
    (518, 0),
    (518, 1),
    (518, 2),
    (518, 3),
    (519, 0),
    (519, 1),
    (519, 2),
    (519, 3),
    (521, 0),
    (521, 1),
    (521, 2),
    (521, 3),
    (523, 0),
    (523, 1),
    (523, 2),
    (523, 3),
    (525, 0),
    (525, 1),
    (525, 2),
    (525, 3),
    (528, 0),
    (528, 1),
    (528, 2),
    (528, 3),
    (529, 0),
    (529, 1),
    (529, 2),
    (529, 3),
    (530, 0),
    (530, 1),
    (530, 2),
    (530, 3),
    (532, 0),
    (532, 1),
    (532, 2),
    (532, 3),
    (534, 0),
    (534, 1),
    (534, 2),
    (534, 3),
    (535, 0),
    (535, 1),
    (535, 2),
    (535, 3),
    (536, 0),
    (536, 1),
    (536, 2),
    (536, 3),
    (537, 0),
    (537, 1),
    (537, 2),
    (537, 3),
    (538, 0),
    (538, 1),
    (538, 2),
    (538, 3),
    (539, 0),
    (539, 1),
    (539, 2),
    (539, 3),
    (540, 0),
    (540, 1),
    (540, 2),
    (540, 3),
    (541, 0),
    (541, 1),
    (541, 2),
    (541, 3),
    (542, 0),
    (542, 1),
    (542, 2),
    (542, 3),
    (543, 0),
    (543, 1),
    (543, 2),
    (543, 3),
    (544, 0),
    (544, 1),
    (544, 2),
    (544, 3),
    (545, 0),
    (545, 1),
    (545, 2),
    (545, 3),
    (546, 0),
    (546, 1),
    (546, 2),
    (546, 3),
    (547, 0),
    (547, 1),
    (547, 2),
    (547, 3),
    (548, 0),
    (548, 1),
    (548, 2),
    (548, 3),
    (549, 0),
    (549, 1),
    (549, 2),
    (549, 3),
    (550, 0),
    (550, 1),
    (550, 2),
    (550, 3),
    (552, 0),
    (552, 1),
    (552, 2),
    (552, 3),
    (553, 0),
    (553, 1),
    (553, 2),
    (553, 3),
    (554, 0),
    (554, 1),
    (554, 2),
    (554, 3),
    (555, 0),
    (555, 1),
    (555, 2),
    (555, 3),
    (556, 0),
    (556, 1),
    (556, 2),
    (556, 3),
    (557, 0),
    (557, 1),
    (557, 2),
    (557, 3),
    (558, 0),
    (558, 1),
    (558, 2),
    (558, 3),
    (559, 0),
    (559, 1),
    (559, 2),
    (559, 3),
    (560, 0),
    (560, 1),
    (560, 2),
    (560, 3),
    (562, 0),
    (562, 1),
    (562, 2),
    (562, 3),
    (564, 0),
    (564, 1),
    (564, 2),
    (564, 3),
    (565, 0),
    (565, 1),
    (565, 2),
    (565, 3),
    (566, 0),
    (566, 1),
    (566, 2),
    (566, 3),
    (567, 0),
    (567, 1),
    (567, 2),
    (567, 3),
    (568, 0),
    (568, 1),
    (568, 2),
    (568, 3),
    (569, 0),
    (569, 1),
    (569, 2),
    (569, 3),
    (570, 0),
    (570, 1),
    (570, 2),
    (570, 3),
    (571, 0),
    (571, 1),
    (571, 2),
    (571, 3),
    (572, 0),
    (572, 1),
    (572, 2),
    (572, 3),
    (573, 0),
    (573, 1),
    (573, 2),
    (573, 3),
    (626, 0),
    (626, 1),
    (626, 2),
    (626, 3),
    (627, 0),
    (627, 1),
    (627, 2),
    (627, 3),
    (628, 0),
    (628, 1),
    (628, 2),
    (628, 3),
    (629, 2),
    (629, 3),
    (630, 2),
    (630, 3),
    (631, 0),
    (631, 1),
    (631, 2),
    (631, 3),
    (632, 0),
    (632, 1),
    (632, 2),
    (632, 3),
    (633, 0),
    (633, 1),
    (633, 2),
    (633, 3),
    (634, 0),
    (634, 1),
    (634, 2),
    (634, 3),
    (635, 0),
    (635, 1),
    (635, 2),
    (635, 3),
    (637, 0),
    (637, 1),
    (637, 2),
    (637, 3),
    (638, 0),
    (638, 1),
    (638, 2),
    (638, 3),
    (639, 0),
    (639, 1),
    (639, 2),
    (639, 3),
    (640, 0),
    (640, 1),
    (640, 2),
    (640, 3),
    (641, 0),
    (641, 1),
    (641, 2),
    (641, 3),
    (644, 0),
    (644, 1),
    (644, 2),
    (644, 3),
    (645, 0),
    (645, 1),
    (645, 2),
    (645, 3),
    (646, 0),
    (646, 1),
    (646, 2),
    (646, 3),
    (647, 0),
    (647, 1),
    (647, 2),
    (647, 3),
    (648, 0),
    (648, 1),
    (648, 2),
    (648, 3),
    (649, 0),
    (649, 1),
    (649, 2),
    (649, 3),
    (650, 3),
    (652, 2),
    (652, 3),
    (665, 3),
    (671, 3),
    (675, 0),
    (675, 1),
    (675, 2),
    (675, 3),
    (677, 0),
    (677, 1),
    (677, 2),
    (677, 3),
    (678, 0),
    (678, 1),
    (678, 2),
    (678, 3),
    (679, 0),
    (679, 1),
    (679, 2),
    (679, 3),
    (680, 0),
    (680, 1),
    (680, 2),
    (680, 3),
    (699, 0),
    (699, 1),
    (699, 2),
    (699, 3),
    (702, 0),
    (702, 1),
    (702, 2),
    (702, 3),
    (703, 0),
    (703, 1),
    (703, 2),
    (703, 3),
    (706, 0),
    (706, 1),
    (706, 2),
    (706, 3),
    (707, 0),
    (707, 1),
    (707, 2),
    (707, 3),
    (708, 0),
    (708, 1),
    (708, 2),
    (708, 3),
    (709, 0),
    (709, 1),
    (709, 2),
    (709, 3),
    (710, 0),
    (710, 1),
    (710, 2),
    (710, 3),
    (711, 0),
    (711, 1),
    (711, 2),
    (711, 3),
    (712, 0),
    (712, 1),
    (712, 2),
    (712, 3),
    (713, 0),
    (713, 1),
    (713, 2),
    (713, 3),
    (714, 0),
    (714, 1),
    (714, 2),
    (714, 3),
    (716, 0),
    (716, 1),
    (716, 2),
    (716, 3),
    (717, 0),
    (717, 1),
    (717, 2),
    (717, 3),
    (718, 0),
    (718, 1),
    (718, 2),
    (718, 3),
    (722, 0),
    (722, 1),
    (722, 2),
    (722, 3),
    (723, 0),
    (723, 1),
    (723, 2),
    (723, 3),
    (724, 0),
    (724, 1),
    (724, 2),
    (724, 3),
    (725, 0),
    (725, 1),
    (725, 2),
    (725, 3),
    (726, 0),
    (726, 1),
    (726, 2),
    (726, 3),
    (727, 0),
    (727, 1),
    (727, 2),
    (727, 3),
    (728, 0),
    (728, 1),
    (728, 2),
    (728, 3),
    (729, 0),
    (729, 1),
    (729, 2),
    (729, 3),
    (730, 0),
    (730, 1),
    (730, 2),
    (730, 3),
    (731, 0),
    (731, 1),
    (731, 2),
    (731, 3),
    (732, 0),
    (732, 1),
    (732, 2),
    (732, 3),
    (733, 0),
    (733, 1),
    (733, 2),
    (733, 3),
    (737, 0),
    (737, 1),
    (737, 2),
    (737, 3),
    (739, 0),
    (739, 1),
    (739, 2),
    (739, 3),
    (740, 0),
    (740, 1),
    (740, 2),
    (740, 3),
    (741, 0),
    (741, 1),
    (741, 2),
    (741, 3),
    (742, 0),
    (742, 1),
    (742, 2),
    (742, 3),
    (743, 0),
    (743, 1),
    (743, 2),
    (743, 3),
    (744, 0),
    (744, 1),
    (744, 2),
    (744, 3),
    (745, 0),
    (745, 1),
    (745, 2),
    (745, 3),
    (746, 0),
    (746, 1),
    (746, 2),
    (746, 3),
    (747, 0),
    (747, 1),
    (747, 2),
    (747, 3),
    (748, 0),
    (748, 1),
    (748, 2),
    (748, 3),
    (749, 0),
    (749, 1),
    (749, 2),
    (749, 3),
    (750, 0),
    (750, 1),
    (750, 2),
    (750, 3),
    (751, 0),
    (751, 1),
    (751, 2),
    (751, 3),
    (752, 0),
    (752, 1),
    (752, 2),
    (752, 3),
    (753, 0),
    (753, 1),
    (753, 2),
    (753, 3),
    (754, 0),
    (754, 1),
    (754, 2),
    (754, 3),
    (755, 0),
    (755, 1),
    (755, 2),
    (755, 3),
    (756, 0),
    (756, 1),
    (756, 2),
    (756, 3),
    (757, 2),
    (758, 3),
    (792, 2),
    (792, 3),
    (802, 2),
    (802, 3),
    (817, 0),
    (817, 1),
    (817, 2),
    (817, 3),
    (818, 0),
    (818, 1),
    (818, 2),
    (818, 3),
    (819, 0),
    (819, 1),
    (819, 2),
    (819, 3),
    (820, 2),
    (820, 3),
    (821, 3),
    (823, 0),
    (823, 1),
    (823, 2),
    (823, 3),
    (834, 3),
    (835, 3),
    (836, 3),
    (837, 0),
    (837, 1),
    (837, 2),
    (837, 3),
    (838, 0),
    (838, 1),
    (838, 2),
    (838, 3),
    (844, 2),
    (844, 3),
    (845, 2),
    (845, 3),
    (846, 0),
    (846, 1),
    (846, 2),
    (846, 3),
    (847, 2),
    (847, 3),
    (848, 3),
    (851, 0),
    (851, 1),
    (851, 2),
    (851, 3),
    (855, 3),
    (856, 3),
    (868, 3),
    (903, 3),
    (1013, 0),
    (1013, 1),
    (1013, 2),
    (1013, 3),
    (1039, 2),
    (1039, 3),
    (1040, 2),
    (1040, 3),
    (1042, 0),
    (1042, 1),
    (1042, 2),
    (1042, 3),
    (1061, 2),
    (1061, 3),
    (1062, 0),
    (1062, 1),
    (1062, 2),
    (1062, 3),
    (1063, 3),
    (1068, 0),
    (1068, 1),
    (1068, 2),
    (1068, 3),
    (1073, 3),
    (1117, 3),
    (1124, 3),
    (1125, 3),
    (1126, 3),
    (1127, 3),
    (1128, 0),
    (1128, 1),
    (1128, 2),
    (1128, 3),
    (1142, 3),
    (1143, 3),
    (1144, 3),
    (1145, 3),
    (1146, 3),
    (1147, 3),
    (1149, 3),
    (1150, 3),
    (1151, 3),
    (1152, 3),
    (1153, 3),
    (1154, 3),
    (1155, 3),
    (1156, 3),
    (1157, 0),
    (1157, 1),
    (1157, 2),
    (1157, 3),
    (1158, 0),
    (1158, 1),
    (1158, 2),
    (1158, 3),
    (1159, 0),
    (1159, 1),
    (1159, 2),
    (1159, 3),
    (1160, 0),
    (1160, 1),
    (1160, 2),
    (1160, 3),
    (1161, 0),
    (1161, 1),
    (1161, 2),
    (1161, 3),
    (1162, 0),
    (1162, 1),
    (1162, 2),
    (1162, 3),
    (1163, 0),
    (1163, 1),
    (1163, 2),
    (1163, 3),
    (1164, 0),
    (1164, 1),
    (1164, 2),
    (1164, 3),
    (1165, 0),
    (1165, 1),
    (1165, 2),
    (1165, 3),
    (1166, 0),
    (1166, 1),
    (1166, 2),
    (1166, 3),
    (1167, 0),
    (1167, 1),
    (1167, 2),
    (1167, 3),
    (1168, 3),
    (1174, 2),
    (1174, 3),
    (1175, 0),
    (1175, 1),
    (1175, 2),
    (1175, 3),
    (1176, 3),
    (1177, 3),
    (1178, 3),
    (1179, 0),
    (1179, 1),
    (1179, 2),
    (1179, 3),
    (1180, 3),
    (1181, 0),
    (1181, 1),
    (1181, 2),
    (1181, 3),
    (1182, 0),
    (1182, 1),
    (1182, 2),
    (1182, 3),
    (1183, 3),
    (1184, 3),
    (1185, 3),
    (1196, 3),
    (1197, 3),
    (1198, 3),
    (1200, 3),
    (1201, 3),
    (1202, 3),
    (1204, 3),
    (1205, 3),
    (1206, 3),
    (1207, 3),
    (1208, 3),
    (1209, 3),
    (1211, 3),
    (1212, 3),
    (1213, 3),
    (1226, 3),
    (1227, 3),
    (1228, 3),
    (1229, 3),
    (1230, 3),
    (1231, 3),
    (1232, 3),
    (1233, 3),
    (1234, 3),
    (1235, 3),
    (1236, 3),
    (1237, 3),
    (1238, 0),
    (1238, 1),
    (1238, 2),
    (1238, 3),
    (1243, 3),
    (1244, 3),
    (1245, 3),
    (1248, 3),
    (1256, 3),
    (1266, 3),
    (1276, 3),
    (1303, 3),
    (1304, 3),
    (1305, 3),
    (1320, 2),
    (1320, 3),
    (1321, 2),
    (1321, 3),
    (1322, 3),
    (1323, 2),
    (1323, 3),
    (1324, 0),
    (1324, 1),
    (1324, 2),
    (1324, 3),
    (1325, 0),
    (1325, 1),
    (1325, 2),
    (1325, 3),
    (1326, 0),
    (1326, 1),
    (1326, 2),
    (1326, 3),
    (1328, 3),
    (1329, 3),
    (1330, 2),
    (1330, 3),
    (1331, 3),
    (1332, 3),
    (1333, 3),
    (1334, 3),
    (1335, 3),
    (1336, 3),
    (1337, 3),
    (1338, 3),
    (1343, 3),
    (1344, 3),
    (1345, 2),
    (1345, 3),
    (1346, 3),
    (1348, 3),
    (1349, 2),
    (1349, 3),
    (1351, 2),
    (1351, 3),
    (1363, 3),
    (1364, 3),
    (1365, 3),
    (1386, 3);
DELETE FROM `creature_template` WHERE `entry` IN (2671, 7385, 7384, 7383, 7382, 7380, 7381, 7386, 7390, 7391, 7387, 7389, 7394, 7395, 7547, 7543, 7544, 7545, 7550, 7549, 7555, 7553, 7554, 14421, 7560, 7561, 7565, 7562, 7567, 8376, 30379, 9656, 9657, 9662, 10259, 10598, 11325, 11326, 11327, 12419, 14878, 15186, 15358, 15429, 15699, 15710, 15706, 15698, 15705, 16069, 16085, 16456, 16547, 16548, 16549, 16701, 17255, 18381, 18839, 20408, 20472, 21010, 21009, 21008, 21018, 21055, 21064, 21063, 21056, 21076, 22445, 22943, 23198, 23234, 23231, 23258, 23266, 23274, 23909, 24388, 24389, 24480, 24753, 25062, 25109, 25110, 25146, 25147, 25706, 26050, 26056, 26119, 27217, 27346, 27914, 28470, 28513, 28883, 29089, 29147, 24968, 29726, 31575, 32589, 32590, 32592, 32591, 32595, 32643, 32791, 32818, 32841, 32939, 33188, 33194, 33197, 33198, 33200, 33219, 33226, 33205, 33227, 33238, 33274, 33239, 33578, 33810, 34278, 34364, 33530, 33529, 34587, 34694, 34724, 34930, 35396, 35395, 35400, 35387, 35399, 35397, 35398, 35394, 35468, 36482, 36511, 36607, 36871, 36908, 36909, 36910, 36911, 36979, 37865, 38374, 40198, 40295, 40624, 40703, 42078, 42177, 42183, 43800, 43916, 45128, 45247, 45340, 46898, 46896, 47944, 48107, 48242, 48609, 48641, 48982, 49586, 49588, 49587, 49590, 50468, 50586, 51635, 51090, 50545, 50722, 51122, 51601, 51600, 51632, 51649, 52226, 52343, 52344, 52831, 52894, 53048, 53225, 53232, 53283, 53623, 53658, 53661, 53884, 54027, 54128, 54227, 54374, 54383, 54438, 54491, 54539, 54541, 54730, 54487, 55187, 55215, 55356, 55367, 55386, 55571, 55574, 56031, 56266, 58163, 59020, 60649, 61080, 61081, 62992, 61086, 61088, 61143, 61141, 61142, 61158, 61160, 61167, 61168, 61384, 61171, 61253, 61255, 61257, 61258, 61259, 61312, 61370, 61313, 61317, 61318, 61319, 61320, 61321, 61322, 61323, 61324, 61327, 61326, 61328, 61329, 61366, 61367, 61071, 61369, 61314, 61372, 61383, 61169, 61385, 61420, 61425, 61386, 61438, 61439, 61440, 61441, 61459, 61443, 61375, 61689, 61690, 61691, 61704, 61703, 61718, 61165, 61751, 61752, 61753, 61757, 61758, 61889, 61890, 61826, 61830, 61905, 62019, 62020, 62022, 62034, 62050, 62051, 62114, 62115, 62116, 62118, 62117, 62119, 62120, 62121, 62129, 62130, 62176, 62177, 62178, 62181, 62184, 62185, 62186, 62187, 62189, 62190, 62201, 62257, 62256, 62246, 62258, 62312, 62313, 62314, 62315, 62316, 62317, 62370, 62373, 62375, 62255, 62191, 62242, 62250, 61686, 62523, 62524, 62526, 62555, 62564, 62583, 62620, 62621, 62627, 62640, 62648, 62628, 62669, 62697, 62816, 62819, 62820, 62835, 62852, 62854, 62884, 62885, 62886, 62892, 62893, 62895, 62896, 62899, 62888, 62900, 62904, 62905, 62914, 62921, 62922, 62924, 62925, 62395, 62864, 62927, 62953, 62991, 62994, 62997, 62998, 62999, 63001, 63002, 63003, 63004, 63005, 63006, 61829, 61828, 61827, 63097, 63098, 62127, 62364, 61677, 62435, 61325, 62638, 62625, 62695, 61755, 62693, 62641, 62906, 62664, 62818, 61368, 62815, 63365, 63559, 54745, 63832, 62954, 64246, 63304, 64248, 63953, 63715, 63919, 63716, 63062, 63064, 63094, 63095, 63096, 63057, 63358, 63060, 65054, 63288, 63291, 65124, 65185, 63293, 63550, 63551, 63547, 59702, 63585, 63557, 63558, 63555, 65187, 63549, 65190, 63954, 63957, 64804, 64242, 64352, 64238, 63548, 65203, 63850, 63838, 63841, 63842, 63847, 63849, 65215, 65216, 62887, 62916, 14755, 14756, 65314, 65313, 65323, 65324, 65321, 64232, 63621, 62907, 64634, 64632, 64633, 62915, 62182, 64899, 61877, 61883, 62829, 59358, 66104, 66105, 62894, 66491, 66450, 66950, 66984, 67022, 67230, 67233, 63370, 67319, 67329, 67332, 67443, 16445, 68267, 68466, 68467, 68468, 68502, 68506, 68601, 68656, 68657, 68655, 68654, 68665, 68664, 68666, 68661, 68662, 68663, 68660, 68659, 68658, 68804, 68805, 68806, 68819, 68820, 68838, 68839, 68841, 68845, 68846, 68850, 15361, 69208, 69648, 69649, 69748, 69778, 69794, 69796, 69818, 69819, 69820, 69849, 69848, 69891, 69893, 69892, 70083, 70098, 70144, 70082, 70154, 70257, 70258, 70259, 70260, 70451, 70452, 70453, 71014, 71015, 71016, 71033, 71017, 71018, 71019, 71020, 71021, 71022, 71023, 71159, 71163, 71199, 71200, 71201, 71488, 71693, 71942, 72160, 72462, 72463, 72464, 73011, 73534, 73352, 73533, 73542, 73543, 73368, 73356, 73532, 73364, 73350, 73351, 73355, 73354, 73357, 73367, 73366, 73359, 73668, 73688, 73730, 73732, 73738, 73741, 73809, 34770, 74402, 74405, 74413, 77137);
INSERT INTO `creature_template` (`entry`, `name`, `subname`, `IconName`, `type_flags`, `type_flags2`, `type`, `family`, `rank`, `KillCredit1`, `KillCredit2`, `PetSpellDataId`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `Health_mod`, `Mana_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `exp_unk`, `WDBVerified`) VALUES
(2671, 'Mechanical Squirrel', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 7937, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 2671
(7385, 'Bombay Cat', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 5556, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7385
(7384, 'Cornish Rex Cat', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 5586, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7384
(7383, 'Black Tabby Cat', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 5448, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7383
(7382, 'Orange Tabby Cat', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 5554, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7382
(7380, 'Siamese Cat', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 5585, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7380
(7381, 'Silver Tabby Cat', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 5555, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7381
(7386, 'White Kitten', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 9990, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7386
(7390, 'Cockatiel', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 6191, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7390
(7391, 'Hyacinth Macaw', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 6192, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7391
(7387, 'Green Wing Macaw', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 5207, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7387
(7389, 'Senegal', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 6190, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7389
(7394, 'Ancona Chicken', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 5369, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7394
(7395, 'Undercity Cockroach', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 2177, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7395
(7547, 'Azure Whelpling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 6293, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7547
(7543, 'Dark Whelpling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 6288, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7543
(7544, 'Crimson Whelpling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 6290, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7544
(7545, 'Emerald Whelpling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 6291, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7545
(7550, 'Wood Frog', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 901, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7550
(7549, 'Tree Frog', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 6295, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7549
(7555, 'Hawk Owl', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 6299, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7555
(7553, 'Great Horned Owl', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 4615, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7553
(7554, 'Snowy Owl', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 6298, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7554
(14421, 'Brown Prairie Dog', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 1072, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 14421
(7560, 'Snowshoe Rabbit', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 328, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7560
(7561, 'Albino Snake', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 2955, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7561
(7565, 'Black Kingsnake', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 1206, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7565
(7562, 'Brown Snake', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 2957, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7562
(7567, 'Crimson Snake', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 6303, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 7567
(8376, 'Mechanical Chicken', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 7920, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 8376
(30379, 'Westfall Chicken', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 304, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 30379
(9656, 'Pet Bombling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 8909, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 9656
(9657, 'Lil'' Smoky', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 8910, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 9657
(9662, 'Sprite Darter Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 6294, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 9662
(10259, 'Worg Pup', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 9563, 0, 9572, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 10259
(10598, 'Smolderweb Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 27718, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 10598
(11325, 'Panda Cub', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 10990, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 11325
(11326, 'Mini Diablo', 'Lord of Terror', '', 0, 0, 12, 0, 0, 0, 0, 0, 10992, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 11326
(11327, 'Zergling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 10993, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 11327
(12419, 'Lifelike Toad', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 901, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 12419
(14878, 'Jubling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 14938, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 14878
(15186, 'Murky', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 15369, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 15186
(15358, 'Lurky', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 15398, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 15358
(15429, 'Disgusting Oozeling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 15436, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 15429
(15699, 'Tranquil Mechanical Yeti', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 10269, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 15699
(15710, 'Tiny Snowman', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 13610, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 15710
(15706, 'Winter Reindeer', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 15904, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 15706
(15698, 'Father Winter''s Helper', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 15660, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 15698
(15705, 'Winter''s Little Helper', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 15663, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 15705
(16069, 'Gurky', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 15984, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 16069
(16085, 'Peddlefeet', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 15992, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 16085
(16456, 'Poley', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 16189, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 16456
(16547, 'Speedy', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 16259, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 16547
(16548, 'Mr. Wiggles', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 16257, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 16548
(16549, 'Whiskers the Rat', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 2176, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 16549
(16701, 'Spirit of Summer', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 16587, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 16701
(17255, 'Hippogryph Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 16943, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 17255
(18381, 'Netherwhelp', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 17723, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 18381
(18839, 'Magical Crawdad', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 18269, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 18839
(20408, 'Mana Wyrmling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 19600, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 20408
(20472, 'Brown Rabbit', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 4626, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 20472
(21010, 'Blue Moth', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 19987, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 21010
(21009, 'Red Moth', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 19986, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 21009
(21008, 'Yellow Moth', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 19985, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 21008
(21018, 'White Moth', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 19999, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 21018
(21055, 'Golden Dragonhawk Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 20026, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 21055
(21064, 'Red Dragonhawk Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 20027, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 21064
(21063, 'Silver Dragonhawk Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 20037, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 21063
(21056, 'Blue Dragonhawk Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 20029, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 21056
(21076, 'Firefly', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 20042, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 21076
(22445, 'Miniwing', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 20996, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 22445
(22943, 'Wolpertinger', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 22349, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 22943
(23198, 'Lucky', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 21328, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 23198
(23234, 'Bananas', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 21362, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 23234
(23231, 'Willy', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 21381, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 23231
(23258, 'Egbert', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 21382, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 23258
(23266, 'Peanut', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 21393, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 23266
(23274, 'Stinker', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 16633, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 23274
(23909, 'Sinister Squashling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 21900, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 23909
(24388, 'Toothy', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 22388, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 24388
(24389, 'Muckbreath', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 22389, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 24389
(24480, 'Mojo', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 22459, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 164, 0, 18414), -- 24480
(24753, 'Pint-Sized Pink Pachyderm', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 22629, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 24753
(25062, 'Tiny Sporebat', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 22855, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 25062
(25109, 'Rocket Chicken', '', '', 0, 1024, 12, 0, 0, 0, 0, 0, 22903, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 25109
(25110, 'Dragon Kite', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 22966, 22991, 22990, 22992, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 25110
(25146, 'Golden Pig', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 21304, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 25146
(25147, 'Silver Pig', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 22938, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 25147
(25706, 'Searing Scorchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 8409, 0, 0, 0, 0.75, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 25706
(26050, 'Snarly', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 23507, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 26050
(26056, 'Chuck', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 23506, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 26056
(26119, 'Phoenix Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 23574, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 26119
(27217, 'Spirit of Competition', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 24393, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 27217
(27346, 'Essence of Competition', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 24620, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 27346
(27914, 'Ethereal Soul-Trader', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 25002, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 27914
(28470, 'Nether Ray Fry', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 25457, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 28470
(28513, 'Vampiric Batling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 4185, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 28513
(28883, 'Frosty', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 28456, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 28883
(29089, 'Mini Tyrael', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 25900, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 29089
(29147, 'Ghostly Skull', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 28089, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 29147
(24968, 'Clockwork Rocket Bot', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 22776, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 24968
(29726, 'Mr. Chilly', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 26452, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 29726
(31575, 'Giant Sewer Rat', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 27627, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 31575
(32589, 'Tickbird Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 45919, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 32589
(32590, 'White Tickbird Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 28215, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 32590
(32592, 'Proto-Drake Whelp', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 28217, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 32592
(32591, 'Cobra Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 28084, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 32591
(32595, 'Pengu', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 28216, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 32595
(32643, 'Kirin Tor Familiar', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 45937, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 32643
(32791, 'Spring Rabbit', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 6302, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 32791
(32818, 'Plump Turkey', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 45968, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 32818
(32841, 'Baby Blizzard Bear', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 16189, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 32841
(32939, 'Little Fawn', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 28397, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 32939
(33188, 'Teldrassil Sproutling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 28482, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 33188
(33194, 'Dun Morogh Cub', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 28489, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 33194
(33197, 'Tirisfal Batling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 4732, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 33197
(33198, 'Durotar Scorpion', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 15470, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 33198
(33200, 'Elwynn Lamb', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 16205, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 33200
(33219, 'Mulgore Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 28502, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 33219
(33226, 'Strand Crawler', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 45880, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 33226
(33205, 'Ammen Vale Lashling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 28493, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 33205
(33227, 'Enchanted Broom', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 45960, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 33227
(33238, 'Argent Squire', '', '', 0, 8388608, 12, 0, 0, 0, 0, 0, 28946, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 33238
(33274, 'Mechanopeep', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 28539, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 33274
(33239, 'Argent Gruntling', '', '', 0, 8388608, 12, 0, 0, 0, 0, 0, 28948, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 33239
(33578, 'Murkimus the Gladiator', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 28734, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 33578
(33810, 'Sen''jin Fetish', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 46939, 46956, 46955, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 33810
(34278, 'Withers', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 45943, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 34278
(34364, 'Calico Cat', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 11709, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 34364
(33530, 'Curious Oracle Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 25173, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 33530
(33529, 'Curious Wolvar Pup', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 25384, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 33529
(34587, 'Warbot', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 29279, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 34587
(34694, 'Grunty', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 29348, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 34694
(34724, 'Shimmering Wyrmling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 29372, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 34724
(34930, 'Jade Tiger', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 29605, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 34930
(35396, 'Darting Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 29805, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 35396
(35395, 'Deviate Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 29807, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 35395
(35400, 'Gundrak Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 29803, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 35400
(35387, 'Leaping Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 29802, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 35387
(35399, 'Obsidian Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 29809, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 35399
(35397, 'Ravasaur Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 29810, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 35397
(35398, 'Razormaw Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 29808, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 35398
(35394, 'Razzashi Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 29806, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 35394
(35468, 'Onyx Panther', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 29819, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 35468
(36482, 'Tuskarr Kite', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 30157, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 36482
(36511, 'Spectral Tiger Cub', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 30409, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 36511
(36607, 'Onyxian Whelpling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 30356, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 36607
(36871, 'Core Hound Pup', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 30462, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 36871
(36908, 'Gryphon Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 30412, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 36908
(36909, 'Wind Rider Cub', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 30413, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 36909
(36910, 'Zipao Tiger', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 30402, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 36910
(36911, 'Pandaren Monk', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 30414, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 36911
(36979, 'Lil'' K.T.', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 30507, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 36979
(37865, 'Perky Pug', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 31174, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 37865
(38374, 'Toxic Wasteling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 31073, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 38374
(40198, 'Frigid Frostling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 31722, 0, 0, 0, 0.75, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 40198
(40295, 'Blue Clockwork Rocket Bot', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 22778, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 40295
(40624, 'Celestial Dragon', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 31956, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 40624
(40703, 'Lil'' XT', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 32031, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 40703
(42078, 'Mini Thor', 'The Destroyer', '', 0, 0, 12, 0, 0, 0, 0, 0, 32670, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 42078
(42177, 'Blue Mini Jouster', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 32699, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 42177
(42183, 'Gold Mini Jouster', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 32707, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 42183
(43800, 'Personal World Destroyer', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 33512, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 43800
(43916, 'De-Weaponized Mechanical Companion', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 33559, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 43916
(45128, 'Crawling Claw', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 34262, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 45128
(45247, 'Pebble', '', '', 0, 262144, 12, 0, 0, 0, 0, 0, 45940, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 45247
(45340, 'Fossilized Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 34413, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 45340
(46898, 'Enchanted Lantern', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 36902, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 46898
(46896, 'Lil'' Deathwing', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 35338, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 46896
(47944, 'Dark Phoenix Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 37136, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 47944
(48107, 'Rustberg Gull', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 36499, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 48107
(48242, 'Armadillo Pup', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 36220, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 48242
(48609, 'Clockwork Gnome', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 36211, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 48609
(48641, 'Fox Kit', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 33217, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 48641
(48982, 'Tiny Shale Spider', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 36637, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 48982
(49586, 'Guild Page', '', '', 0, 8388608, 12, 0, 0, 0, 0, 0, 37200, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 49586
(49588, 'Guild Page', '', '', 0, 8388608, 12, 0, 0, 0, 0, 0, 37199, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 49588
(49587, 'Guild Herald', '', '', 0, 8388608, 12, 0, 0, 0, 0, 0, 37198, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 49587
(49590, 'Guild Herald', '', '', 0, 8388608, 12, 0, 0, 0, 0, 0, 37196, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 49590
(50468, 'Landro''s Lil'' XT', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 32031, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 50468
(50586, 'Mr. Grubbs', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 9905, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 50586
(51635, 'Scooter the Snail', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 38135, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 51635
(51090, 'Singing Sunflower', '', '', 0, 1024, 12, 0, 0, 0, 0, 0, 37154, 0, 0, 0, 0.5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 51090
(50545, 'Magic Lamp', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 36901, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 50545
(50722, 'Elementium Geode', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 45878, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 50722
(51122, 'Deathy', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 36896, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 51122
(51601, 'Moonkin Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 37526, 0, 37527, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 51601
(51600, 'Lil'' Ragnaros', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 37541, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 51600
(51632, 'Tiny Flamefly', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 28435, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 51632
(51649, 'Moonkin Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 37527, 0, 37526, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 51649
(52226, 'Panther Cub', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 37814, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 52226
(52343, 'Landro''s Lichling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 30507, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 52343
(52344, 'Nightsaber Cub', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 37846, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 52344
(52831, 'Winterspring Cub', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 37712, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 52831
(52894, 'Lashtail Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 38065, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 52894
(53048, 'Legs', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 38134, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 53048
(53225, 'Pterrordax Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 38229, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 53225
(53232, 'Voodoo Figurine', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 38232, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 53232
(53283, 'Guardian Cub', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 38359, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 53283
(53623, 'Cenarion Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 16943, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 53623
(53658, 'Hyjal Bear Cub', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 38455, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 53658
(53661, 'Crimson Lasher', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 38429, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 53661
(53884, 'Feline Familiar', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 38539, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 53884
(54027, 'Lil'' Tarecgosa', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 38614, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 54027
(54128, 'Creepy Crate', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 38638, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 54128
(54227, 'Nuts', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 38693, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 54227
(54374, 'Brilliant Kaliri', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 38776, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 54374
(54383, 'Purple Puffer', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 38777, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 54383
(54438, 'Murkablo', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 38803, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 54438
(54491, 'Darkmoon Monkey', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 46001, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 54491
(54539, 'Alliance Balloon', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 38343, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 54539
(54541, 'Horde Balloon', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 38342, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 54541
(54730, 'Gregarious Grell', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 38919, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 54730
(54487, 'Darkmoon Turtle', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 38809, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 54487
(55187, 'Darkmoon Balloon', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 38344, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 55187
(55215, 'Lumpy', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 45939, 0, 0, 0, 0.75, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 55215
(55356, 'Darkmoon Tonk', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 15381, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 55356
(55367, 'Darkmoon Zeppelin', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 17192, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 55367
(55386, 'Sea Pony', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 39109, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 55386
(55571, 'Lunar Lantern', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 39163, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 55571
(55574, 'Festival Lantern', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 39333, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 55574
(56031, 'Darkmoon Cub', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 39137, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 56031
(56266, 'Fetish Shaman', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 39380, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 56266
(58163, 'Soul of the Aspects', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 40019, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 58163
(59020, 'Eye of the Legion', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 40538, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 0, 18414), -- 59020
(60649, 'Black Lamb', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42906, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 60649
(61080, 'Rabbit', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 328, 0, 4626, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61080
(61081, 'Squirrel', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 134, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61081
(62992, 'Bucktooth Flapper', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 40713, 0, 40716, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 62992
(61086, 'Porcupette', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 41833, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61086
(61088, 'Eternal Strider', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 43798, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61088
(61143, 'Mouse', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 4959, 0, 4960, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61143
(61141, 'Prairie Dog', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 1072, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61141
(61142, 'Snake', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 1206, 2957, 6303, 2958, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61142
(61158, 'Shore Crab', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45880, 32791, 32789, 32790, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61158
(61160, 'Tiny Harvester', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 41886, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61160
(61167, 'Mountain Cottontail', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 328, 25340, 4626, 6302, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61167
(61168, 'Redridge Rat', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 1141, 2176, 1418, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61168
(61384, 'Cockroach', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 2177, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61384
(61171, 'Fledgling Buzzard', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 41887, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61171
(61253, 'Dusk Spiderling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 2536, 1160, 9829, 14952, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61253
(61255, 'Skunk', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 16633, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61255
(61257, 'Black Rat', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 1141, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 88, 0, 18414), -- 61257
(61258, 'Rat Snake', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 3126, 36544, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61258
(61259, 'Widow Spiderling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45905, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61259
(61312, 'Strand Crab', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 32789, 32791, 32790, 45880, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61312
(61370, 'Swamp Moth', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36944, 36956, 36952, 36957, 0.190476, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 61370
(61313, 'Parrot', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 6189, 9554, 20995, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 61313
(61317, 'Long-tailed Mole', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 4959, 0, 36357, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61317
(61318, 'Tree Python', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36578, 4343, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61318
(61319, 'Beetle', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 7511, 8971, 15467, 15468, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61319
(61320, 'Forest Spiderling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45902, 0, 45903, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61320
(61321, 'Lizard Hatchling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36583, 36584, 36599, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 61321
(61322, 'Polly', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42509, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61322
(61323, 'Wharf Rat', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 1141, 2176, 1418, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61323
(61324, 'Baby Ape', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 21362, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 61324
(61327, 'Spider', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45902, 0, 45903, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61327
(61326, 'Scorpid', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 15469, 15470, 5905, 15471, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61326
(61328, 'Fire Beetle', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 8971, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 88, 0, 18414), -- 61328
(61329, 'Scorpling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 41960, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 61329
(61366, 'Rat', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 1141, 2176, 1418, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61366
(61367, 'Water Snake', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 1986, 2954, 1987, 2956, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61367
(61071, 'Small Frog', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 6297, 6295, 1924, 901, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 61071
(61369, 'Toad', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 901, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61369
(61314, 'Crimson Moth', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36671, 0, 0, 0, 0.190476, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 61314
(61372, 'Moccasin', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 1986, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61372
(61383, 'Lava Crab', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 28507, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61383
(61169, 'Roach', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 2177, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61169
(61385, 'Ash Viper', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 4268, 35802, 36544, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61385
(61420, 'Ash Spiderling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45904, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61420
(61425, 'Molten Hatchling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42051, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61425
(61386, 'Lava Beetle', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 41981, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 88, 0, 18414), -- 61386
(61438, 'Gold Beetle', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 15467, 0, 15468, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61438
(61439, 'Rattlesnake', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 35804, 1987, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61439
(61440, 'Stripe-Tailed Scorpid', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 15469, 36617, 5905, 36616, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61440
(61441, 'Spiky Lizard', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36585, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 61441
(61459, 'Little Black Ram', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42068, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61459
(61443, 'King Snake', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 2954, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61443
(61375, 'Restless Shadeling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45917, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61375
(61689, 'Snow Cub', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42203, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61689
(61690, 'Alpine Hare', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36342, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61690
(61691, 'Irradiated Roach', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 2177, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61691
(61704, 'Grasslands Cottontail', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 328, 6302, 4626, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61704
(61703, 'Tiny Twister', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45936, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61703
(61718, 'Jade Oozeling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42218, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61718
(61165, 'Fawn', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 654, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61165
(61751, 'Hare', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 1560, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61751
(61752, 'Brown Marmot', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45899, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61752
(61753, 'Maggot', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 9904, 0, 9906, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61753
(61757, 'Red-Tailed Chipmunk', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36620, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61757
(61758, 'Infested Bear Cub', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42229, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 61758
(61889, 'Undercity Rat', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 1141, 2176, 1418, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61889
(61890, 'Blighted Squirrel', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42334, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61890
(61826, 'Blighthawk', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42265, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61826
(61830, 'Festering Maggot', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 9904, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61830
(61905, 'Lost of Lordaeron', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45952, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61905
(62019, 'Cat', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 5585, 5556, 5555, 5586, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62019
(62020, 'Ruby Sapling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42335, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62020
(62022, 'Larva', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 9906, 9905, 9904, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62022
(62034, 'Spirit Crab', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42342, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62034
(62050, 'Grey Moth', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42343, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62050
(62051, 'Ravager Hatchling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42344, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62051
(62114, 'Spiny Lizard', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36583, 36585, 36584, 36598, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 62114
(62115, 'Dung Beetle', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 7511, 8971, 15467, 15468, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62115
(62116, 'Creepy Crawly', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 46940, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62116
(62118, 'Twilight Beetle', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 0, 0, 36644, 7511, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62118
(62117, 'Twilight Spider', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36236, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62117
(62119, 'Robo-Chick', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 7920, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62119
(62120, 'Rabid Nut Varmint 5000', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 26532, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62120
(62121, 'Turquoise Turtle', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 27881, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62121
(62129, 'Cheetah Cub', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42362, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62129
(62130, 'Giraffe Calf', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45896, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62130
(62176, 'Gazelle Fawn', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45958, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62176
(62177, 'Forest Moth', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36944, 36956, 36952, 36957, 0.190476, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 62177
(62178, 'Elfin Rabbit', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 328, 15918, 4626, 6302, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62178
(62181, 'Topaz Shale Hatchling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36648, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62181
(62184, 'Rock Viper', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 4268, 0, 36544, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62184
(62185, 'Horny Toad', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36583, 36599, 36598, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 62185
(62186, 'Desert Spider', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45906, 0, 45907, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62186
(62187, 'Stone Armadillo', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42381, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62187
(62189, 'Alpine Chipmunk', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36620, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62189
(62190, 'Coral Snake', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36580, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62190
(62201, 'Spawn of Onyxia', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42745, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62201
(62257, 'Sand Kitten', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 5586, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62257
(62256, 'Stinkbug', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 7511, 8971, 15467, 15468, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62256
(62246, 'Shimmershell Snail', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42407, 42409, 42408, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 873, 0, 18414), -- 62246
(62258, 'Silithid Hatchling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42416, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62258
(62312, 'Frog', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 6297, 6295, 1924, 901, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 62312
(62313, 'Rusty Snail', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42409, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62313
(62314, 'Tainted Cockroach', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 2177, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62314
(62315, 'Tainted Moth', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36944, 36956, 36952, 36957, 0.190476, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 62315
(62316, 'Tainted Rat', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 1141, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62316
(62317, 'Minfernal', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 46003, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62317
(62370, 'Spotted Bell Frog', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 6297, 6296, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62370
(62373, 'Silky Moth', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36944, 36953, 36952, 36955, 0.190476, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 62373
(62375, 'Diemetradon Hatchling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45913, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62375
(62255, 'Twilight Iguana', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42415, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62255
(62191, 'Venomspitter Hatchling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45908, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62191
(62242, 'Crested Owl', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 6300, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62242
(62250, 'Darkshore Cub', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42412, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62250
(61686, 'Tiny Bog Beast', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42202, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61686
(62523, 'Sidewinder', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 35804, 0, 35802, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62523
(62524, 'Scarab Hatchling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 35113, 35112, 36594, 36595, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62524
(62526, 'Qiraji Guardling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42523, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62526
(62555, 'Flayer Youngling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42553, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62555
(62564, 'Sporeling Sprout', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42554, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62564
(62583, 'Warpstalker Hatchling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45998, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62583
(62620, 'Clefthoof Runt', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42575, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62620
(62621, 'Fel Flame', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45923, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62621
(62627, 'Fledgling Nether Ray', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45988, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62627
(62640, 'Devouring Maggot', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45885, 45887, 45886, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62640
(62648, 'Turkey', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45968, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 62648
(62628, 'Scalded Basilisk Hatchling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45953, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62628
(62669, 'Fjord Worg Pup', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42617, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62669
(62697, 'Oily Slimeling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42781, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62697
(62816, 'Stunted Shardhorn', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42708, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62816
(62819, 'Imperial Eagle Chick', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42709, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62819
(62820, 'Water Waveling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 30159, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62820
(62835, 'Tundra Penguin', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 25390, 25390, 25391, 25391, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 62835
(62852, 'Dragonbone Hatchling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42735, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62852
(62854, 'Scourged Whelpling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42737, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62854
(62884, 'Grotto Vole', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 4959, 36357, 4960, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62884
(62885, 'Carrion Rat', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 1141, 0, 1418, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 88, 0, 18414), -- 62885
(62886, 'Fire-Proof Roach', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 2177, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62886
(62892, 'Mac Frog', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 6297, 6295, 1924, 901, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 62892
(62893, 'Locust', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 2177, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62893
(62895, 'Oasis Moth', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36944, 36956, 36952, 36957, 0.190476, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 62895
(62896, 'Leopard Scorpid', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 15469, 15470, 5905, 36616, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62896
(62899, 'Tol''vir Scarab', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42771, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 62899
(62888, 'Nordrassil Wisp', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45820, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 62888
(62900, 'Wildhammer Gryphon Hatchling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 30412, 36336, 36332, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 841, 0, 18414), -- 62900
(62904, 'Yellow-Bellied Marmot', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45900, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62904
(62905, 'Highlands Mouse', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 4959, 0, 4960, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62905
(62914, 'Twilight Fiendling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42783, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62914
(62921, 'Stowaway Rat', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 1141, 0, 1418, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 88, 0, 18414), -- 62921
(62922, 'Crimson Shale Hatchling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36605, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62922
(62924, 'Deepholm Cockroach', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45897, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62924
(62925, 'Crystal Beetle', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 15467, 36613, 36644, 7511, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62925
(62395, 'Nether Faerie Dragon', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42467, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62395
(62864, 'Arctic Fox Kit', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42757, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62864
(62927, 'Crimson Geode', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45879, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62927
(62953, 'Sea Gull', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45995, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62953
(62991, 'Coral Adder', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36544, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 62991
(62994, 'Emerald Turtle', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 27883, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 62994
(62997, 'Jungle Darter', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 6297, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 62997
(62998, 'Mirror Strider', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45852, 0, 45853, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 62998
(62999, 'Temple Snake', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 2955, 0, 43195, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 62999
(63001, 'Silkbead Snail', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 38380, 42409, 42407, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 873, 0, 18414), -- 63001
(63002, 'Garden Frog', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 38831, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 63002
(63003, 'Masked Tanuki', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 40093, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 63003
(63004, 'Grove Viper', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 35802, 0, 2957, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 63004
(63005, 'Spirebound Crab', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45880, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 63005
(63006, 'Sandy Petrel', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45994, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 63006
(61829, 'Bat', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 4732, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 220, 0, 18414), -- 61829
(61828, 'Infected Squirrel', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 10090, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61828
(61827, 'Infected Fawn', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 37686, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61827
(63097, 'Shore Crawler', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 32790, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 63097
(63098, 'Gilnean Raven', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 42872, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 63098
(62127, 'Emerald Boa', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36578, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62127
(62364, 'Ash Lizard', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36583, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 62364
(61677, 'Mountain Skunk', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 16633, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61677
(62435, 'Crystal Spider', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45909, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62435
(61325, 'Adder', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 1986, 1987, 1206, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61325
(62638, 'Skittering Cavern Crawler', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 20923, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 62638
(62625, 'Nether Roach', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 2177, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62625
(62695, 'Borean Marmot', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 1072, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62695
(61755, 'Snowshoe Hare', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 328, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 133, 0, 18414), -- 61755
(62693, 'Arctic Hare', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 328, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 133, 0, 18414), -- 62693
(62641, 'Fjord Rat', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 22175, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 88, 0, 18414), -- 62641
(62906, 'Highlands Turkey', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45970, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 62906
(62664, 'Chicken', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 304, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62664
(62818, 'Grizzly Squirrel', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 134, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62818
(61368, 'Huge Toad', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 5379, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 61368
(62815, 'Biletoad', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 1924, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62815
(63365, 'Terrible Turnip', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 44655, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 63365
(63559, 'Tiny Goldfish', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 47955, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 69, 0, 18414), -- 63559
(54745, 'Sand Scarab', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 39694, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 54745
(63832, 'Lucky Quilen Cub', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 43255, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 63832
(62954, 'Stormwind Rat', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 1141, 2176, 1418, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62954
(64246, 'Shy Bandicoon', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 40089, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 64246
(63304, 'Jungle Grub', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 40226, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 63304
(64248, 'Summit Kid', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 43485, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 64248
(63953, 'Kuitan Mongoose', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 43347, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 63953
(63715, 'Jumping Spider', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45911, 0, 45912, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 63715
(63919, 'Leopard Tree Frog', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 6296, 43311, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 63919
(63716, 'Masked Tanuki Pup', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 43194, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 63716
(63062, 'Bandicoon', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 40089, 40094, 40090, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 63062
(63064, 'Bandicoon Kit', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 41834, 42860, 42861, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 63064
(63094, 'Malayan Quillrat', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45991, 0, 45992, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 63094
(63095, 'Malayan Quillrat Pup', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45990, 0, 45989, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 63095
(63096, 'Marsh Fiddler', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 43360, 0, 43361, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 63096
(63057, 'Sifang Otter', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42856, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 63057
(63358, 'Sifang Otter Pup', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42983, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 63358
(63060, 'Softshell Snapling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 27679, 0, 43339, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 42, 0, 18414), -- 63060
(65054, 'Feverbite Hatchling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 44816, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 65054
(63288, 'Amethyst Spiderling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45910, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 63288
(63291, 'Savory Beetle', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45996, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 63291
(65124, 'Luyu Moth', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36955, 0, 0, 0, 0.190476, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 65124
(65185, 'Mei Li Sparkler', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 28434, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 65185
(63293, 'Spiny Terrapin', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42859, 0, 42874, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 42, 0, 18414), -- 63293
(63550, 'Alpine Foxling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36388, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 133, 0, 18414), -- 63550
(63551, 'Alpine Foxling Kit', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 42757, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 133, 0, 18414), -- 63551
(63547, 'Plains Monitor', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45999, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 63547
(59702, 'Prairie Mouse', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 4959, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 59702
(63585, 'Szechuan Chicken', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 304, 43152, 5369, 43153, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 63585
(63557, 'Tolai Hare', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 6302, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 133, 0, 18414), -- 63557
(63558, 'Tolai Hare Pup', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 28998, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 133, 0, 18414), -- 63558
(63555, 'Zooey Snake', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 4440, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 63555
(65187, 'Amber Moth', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36956, 0, 0, 0, 0.190476, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 65187
(63549, 'Grassland Hopper', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 43344, 0, 43352, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 63549
(65190, 'Mongoose', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 43347, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 65190
(63954, 'Mongoose Pup', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 43346, 0, 43350, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 63954
(63957, 'Yakrat', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 4959, 0, 36357, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 63957
(64804, 'Silent Hedgehog', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45984, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 64804
(64242, 'Clouded Hedgehog', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45985, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 64242
(64352, 'Rapana Whelk', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45882, 34590, 45883, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 873, 0, 18414), -- 64352
(64238, 'Resilient Roach', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45898, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 64238
(63548, 'Crunchy Scorpion', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 15469, 36617, 5905, 36616, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 63548
(65203, 'Emperor Crab', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45881, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 65203
(63850, 'Effervescent Glowfly', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 46000, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 63850
(63838, 'Gilded Moth', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36956, 0, 0, 0, 0.190476, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 63838
(63841, 'Golden Civet', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 43428, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 63841
(63842, 'Golden Civet Kitten', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 43259, 0, 43260, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 63842
(63847, 'Dancing Water Skimmer', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 40584, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 133, 0, 18414), -- 63847
(63849, 'Yellow-Bellied Bullfrog', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 6297, 22468, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 63849
(65215, 'Garden Moth', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36671, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 65215
(65216, 'Shrine Fly', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 20042, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 65216
(62887, 'Death''s Head Cockroach', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 2177, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62887
(62916, 'Fungal Moth', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36944, 36953, 36952, 36955, 0.190476, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 62916
(14755, 'Tiny Green Dragon', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 14778, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 14755
(14756, 'Tiny Red Dragon', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 14779, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 14756
(65314, 'Jade Crane Chick', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 43868, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 65314
(65313, 'Thundering Serpent Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 43865, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 65313
(65323, 'Wild Jade Hatchling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 43875, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 65323
(65324, 'Wild Golden Hatchling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 45064, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 65324
(65321, 'Wild Crimson Hatchling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 43874, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 65321
(64232, 'Singing Cricket', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 43127, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 64232
(63621, 'Feral Vermling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 45894, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 63621
(62907, 'Highlands Skunk', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 22447, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62907
(64634, 'Grinder', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 45938, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 64634
(64632, 'Hopling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 43597, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 64632
(64633, 'Aqua Strider', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 45854, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 64633
(62915, 'Emerald Shale Hatchling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36603, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62915
(62182, 'Amethyst Shale Hatchling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36604, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 62182
(64899, 'Mechanical Pandaren Dragonling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 45386, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 64899
(61877, 'Jade Owl', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 15905, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 61877
(61883, 'Sapphire Cub', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 42297, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 61883
(62829, 'Fishy', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 42721, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 76, 0, 18414), -- 62829
(59358, 'Darkmoon Rabbit', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 45957, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 59358
(66104, 'Chi-Ji Kite', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 44791, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 66104
(66105, 'Yu''lon Kite', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 44792, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 66105
(62894, 'Horned Lizard', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 36583, 36585, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 62894
(66491, 'Venus', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 45195, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 66491
(66450, 'Jade Tentacle', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 45987, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 66450
(66950, 'Pandaren Water Spirit', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 45942, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 66950
(66984, 'Baneling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 45527, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 66984
(67022, 'Wanderer''s Festival Hatchling', '', 'wildpetcapturable', 0, 0, 14, 0, 6, 0, 0, 0, 44779, 0, 0, 0, 0.3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 67022
(67230, 'Imperial Moth', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 44551, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 67230
(67233, 'Imperial Silkworm', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 40521, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 67233
(63370, 'Red Cricket', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 46385, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 63370
(67319, 'Darkmoon Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 46163, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 67319
(67329, 'Darkmoon Glowfly', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 46171, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 67329
(67332, 'Darkmoon Eye', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 46174, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 0, 18414), -- 67332
(67443, 'Crow', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 36743, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 67443
(16445, 'Terky', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 15398, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 16445
(68267, 'Cinder Kitten', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 46720, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 68267
(68466, 'Pandaren Fire Spirit', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 46809, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 68466
(68467, 'Pandaren Air Spirit', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 46810, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 68467
(68468, 'Pandaren Earth Spirit', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 46811, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 68468
(68502, 'Spectral Cub', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 30409, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 68502
(68506, 'Sumprush Rodent', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 40714, 0, 40717, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 68506
(68601, 'Clock''em', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 46882, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 68601
(68656, 'Giant Bone Spider', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 46898, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 68656
(68657, 'Fungal Abomination', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 46896, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 68657
(68655, 'Mr. Bigglesworth', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 46897, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 68655
(68654, 'Stitched Pup', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 46921, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 68654
(68665, 'Harbinger of Flame', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 46900, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 68665
(68664, 'Corefire Imp', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 46923, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 68664
(68666, 'Ashstone Core', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 46902, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 68666
(68661, 'Untamed Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 46903, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 68661
(68662, 'Chrominius', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 46925, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 68662
(68663, 'Death Talon Whelpguard', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 46905, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 68663
(68660, 'Viscidus Globule', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 46924, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 68660
(68659, 'Anubisath Idol', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 46922, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 68659
(68658, 'Mini Mindslayer', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 46909, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 68658
(68804, 'Harpy Youngling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 46936, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 0, 18414), -- 68804
(68805, 'Stunted Yeti', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 46937, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 0, 18414), -- 68805
(68806, 'Lofty Libram', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 46938, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 0, 18414), -- 68806
(68819, 'Arcane Eye', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 46941, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 0, 18414), -- 68819
(68820, 'Infinite Whelpling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 47636, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 68820
(68838, 'Fluxfire Feline', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 46947, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 68838
(68839, 'Anodized Robo Cub', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 46948, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 68839
(68841, 'Cogblade Raptor', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 47021, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 68841
(68845, 'Nexus Whelpling', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 47635, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 68845
(68846, 'Kun-Lai Runt', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 46953, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 68846
(68850, 'Emerald Proto-Whelp', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 46954, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 68850
(15361, 'Murki', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 15395, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 15361
(69208, 'Gusting Grimoire', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 47348, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 0, 18414), -- 69208
(69648, 'Thundertail Flapper', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 47633, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 69648
(69649, 'Red Panda', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 47634, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 69649
(69748, 'Living Sandling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 47252, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 69748
(69778, 'Sunreaver Micro-Sentry', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 47848, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 69778
(69794, 'Electrified Razortooth', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 47690, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 69794
(69796, 'Zandalari Kneebiter', '', '', 0, 0, 14, 0, 0, 0, 0, 0, 47731, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 69796
(69818, 'Elder Python', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 47887, 0, 47994, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 69818
(69819, 'Swamp Croaker', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 47989, 0, 47990, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 69819
(69820, 'Son of Animus', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 47708, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 69820
(69849, 'Stunted Direhorn', '', '', 0, 0, 14, 0, 0, 0, 0, 0, 48211, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 69849
(69848, 'Spectral Porcupette', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 47732, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 69848
(69891, 'Sunfur Panda', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 47747, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 69891
(69893, 'Snowy Panda', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 47749, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 69893
(69892, 'Mountain Panda', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 47748, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 69892
(70083, 'Pygmy Direhorn', '', '', 0, 0, 14, 0, 0, 0, 0, 0, 48212, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 70083
(70098, 'Spawn of G''nathus', '', '', 0, 0, 14, 0, 0, 0, 0, 0, 48091, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 70098
(70144, 'Ji-Kun Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 48001, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 70144
(70082, 'Pierre', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 47711, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 70082
(70154, 'Direhorn Runt', '', '', 0, 0, 14, 0, 0, 0, 0, 0, 48213, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 70154
(70257, 'Tiny Red Carp', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 47959, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 76, 0, 18414), -- 70257
(70258, 'Tiny Blue Carp', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 47957, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 76, 0, 18414), -- 70258
(70259, 'Tiny Green Carp', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 47958, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 76, 0, 18414), -- 70259
(70260, 'Tiny White Carp', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 47960, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 76, 0, 18414), -- 70260
(70451, 'Zandalari Anklerender', '', '', 0, 0, 14, 0, 0, 0, 0, 0, 48055, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 70451
(70452, 'Zandalari Footslasher', '', '', 0, 0, 14, 0, 0, 0, 0, 0, 48056, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 70452
(70453, 'Zandalari Toenibbler', '', '', 0, 0, 14, 0, 0, 0, 0, 0, 48057, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 70453
(71014, 'Lil'' Bad Wolf', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 48878, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 71014
(71015, 'Menagerie Custodian', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 48857, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 71015
(71016, 'Netherspace Abyssal', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 48856, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 71016
(71033, 'Fiendish Imp', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 48662, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 71033
(71017, 'Tideskipper', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 48855, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 71017
(71018, 'Tainted Waveling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 48666, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 71018
(71019, 'Coilfang Stalker', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 48661, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 71019
(71020, 'Pocket Reaver', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 48664, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 71020
(71021, 'Lesser Voidcaller', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 48668, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 71021
(71022, 'Phoenix Hawk Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 48663, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 71022
(71023, 'Tito', '', 'openhand', 0, 0, 12, 0, 0, 0, 0, 0, 48667, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 71023
(71159, 'Gahz''rooki', '', '', 0, 0, 14, 0, 0, 0, 0, 0, 48651, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 71159
(71163, 'Unborn Val''kyr', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 48650, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 71163
(71199, 'Living Fluid', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 48704, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 71199
(71200, 'Viscous Horror', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 48705, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 71200
(71201, 'Filthling', '', '', 0, 0, 14, 0, 0, 0, 0, 0, 48708, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 71201
(71488, 'Blossoming Ancient', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 48934, 48932, 48935, 48933, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 71488
(71693, 'Rascal-Bot', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 49084, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 71693
(71942, 'Xu-Fu, Cub of Xuen', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 49846, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 71942
(72160, 'Moon Moon', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 855, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 72160
(72462, 'Chi-Chi, Hatchling of Chi-Ji', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 49835, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 72462
(72463, 'Yu''la, Broodling of Yu''lon', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 49836, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 72463
(72464, 'Zao, Calfling of Niuzao', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 49845, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 72464
(73011, 'Lil'' Bling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 49289, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 73011
(73534, 'Azure Crane Chick', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 51413, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 73534
(73352, 'Blackfuse Bombling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 51268, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 73352
(73533, 'Ashleaf Spriteling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 40908, 0, 0, 0, 0.190476, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 73533
(73542, 'Ashwing Moth', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 51742, 0, 0, 0, 0.190476, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 73542
(73543, 'Flamering Moth', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 51301, 0, 0, 0, 0.190476, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 73543
(73368, 'Skywisp Moth', '', 'wildpetcapturable', 0, 0, 14, 0, 0, 0, 0, 0, 51740, 0, 51741, 0, 0.190476, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 73368
(73356, 'Ruby Droplet', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 51271, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 73356
(73532, 'Dandelion Frolicker', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 51408, 0, 0, 0, 0.190476, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 73532
(73364, 'Death Adder Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 51277, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 73364
(73350, 'Droplet of Y''Shaarj', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 51417, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 73350
(73351, 'Gooey Sha-ling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 51267, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 73351
(73355, 'Jademist Dancer', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 51270, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 73355
(73354, 'Kovok', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 51269, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 73354
(73357, 'Ominous Flame', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 51272, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 73357
(73367, 'Skunky Alemental', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 51279, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 73367
(73366, 'Spineclaw Crab', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 51278, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 73366
(73359, 'Gulp Froglet', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 47991, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 73359
(73668, 'Bonkers', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 51475, 51477, 51476, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 73668
(73688, 'Vengeful Porcupette', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 47858, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 73688
(73730, 'Gu''chi Swarmling', '', '', 0, 0, 14, 0, 0, 0, 0, 0, 51502, 0, 0, 0, 0.2, 1, 0, 0, 0, 0, 0, 0, 0, 42, 0, 18414), -- 73730
(73732, 'Harmonious Porcupette', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 47856, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 73732
(73738, 'Jadefire Spirit', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 51504, 0, 0, 0, 0.01, 1, 0, 0, 0, 0, 0, 0, 0, 100, 0, 18414), -- 73738
(73741, 'Rotten Little Helper', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 51505, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 73741
(73809, 'Sky Lantern', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 51530, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 73809
(34770, 'Macabre Marionette', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 29404, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18414), -- 34770
(74402, 'Alterac Brew-Pup', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 51988, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 74402
(74405, 'Murkalot', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 51990, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 74405
(74413, 'Treasure Goblin', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 51994, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414), -- 74413
(77137, 'Dread Hatchling', '', '', 0, 0, 12, 0, 0, 0, 0, 0, 53719, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 121, 0, 18414); -- 77137
DELETE FROM `creature_model_info` WHERE `modelid` IN (34, 46, 63, 116, 156, 199, 219, 259, 349, 357, 419, 444, 461, 463, 469, 595, 619, 629, 849, 891, 899, 1179, 2356, 2923, 2944, 3157, 3159, 3173, 4796, 4799, 4984, 5148, 5152, 5153, 5154, 5155, 5174, 5199, 46171, 46385, 46882, 46896, 46897, 46922, 46936, 46938, 46939, 46948, 46954, 46955, 46956, 47348, 47634, 47635, 47636, 47731, 47732, 47747, 47749, 47848, 47858, 47958, 47959, 47989, 47991, 47994, 48212, 48651, 48661, 48667, 48668, 48855, 48857, 48932, 48933, 48934, 48935, 49835, 49845, 49846, 51267, 51269, 51271, 51278, 51475, 51476, 51477, 51741, 51742, 53719);
INSERT INTO `creature_model_info` (`modelid`, `bounding_radius`, `combat_reach`, `gender`, `modelid_other_gender`) VALUES
(34, 0, 0, 2, 0),
(46, 0, 0, 2, 0),
(63, 0, 0, 2, 0),
(116, 0, 0, 2, 0),
(156, 0, 0, 2, 0),
(199, 0, 0, 2, 0),
(219, 0, 0, 2, 0),
(259, 0, 0, 2, 0),
(349, 0, 0, 2, 0),
(357, 0, 0, 2, 0),
(419, 0, 0, 2, 0),
(444, 0, 0, 2, 0),
(461, 0, 0, 2, 0),
(463, 0, 0, 2, 0),
(469, 0, 0, 2, 0),
(595, 0, 0, 2, 0),
(619, 0, 0, 2, 0),
(629, 0, 0, 2, 0),
(849, 0, 0, 2, 0),
(891, 0, 0, 2, 0),
(899, 0, 0, 2, 0),
(1179, 0, 0, 2, 0),
(2356, 0, 0, 2, 0),
(2923, 0, 0, 2, 0),
(2944, 0, 0, 2, 0),
(3157, 0, 0, 2, 0),
(3159, 0, 0, 2, 0),
(3173, 0, 0, 2, 0),
(4796, 0, 0, 2, 0),
(4799, 0, 0, 2, 0),
(4984, 0, 0, 2, 0),
(5148, 0, 0, 2, 0),
(5152, 0, 0, 2, 0),
(5153, 0, 0, 2, 0),
(5154, 0, 0, 2, 0),
(5155, 0, 0, 2, 0),
(5174, 0, 0, 2, 0),
(5199, 0, 0, 2, 0),
(46171, 0, 0, 2, 0),
(46385, 0, 0, 2, 0),
(46882, 0, 0, 2, 0),
(46896, 0, 0, 2, 0),
(46897, 0, 0, 2, 0),
(46922, 0, 0, 2, 0),
(46936, 0, 0, 2, 0),
(46938, 0, 0, 2, 0),
(46939, 0, 0, 2, 0),
(46948, 0, 0, 2, 0),
(46954, 0, 0, 2, 0),
(46955, 0, 0, 2, 0),
(46956, 0, 0, 2, 0),
(47348, 0, 0, 2, 0),
(47634, 0, 0, 2, 0),
(47635, 0, 0, 2, 0),
(47636, 0, 0, 2, 0),
(47731, 0, 0, 2, 0),
(47732, 0, 0, 2, 0),
(47747, 0, 0, 2, 0),
(47749, 0, 0, 2, 0),
(47848, 0, 0, 2, 0),
(47858, 0, 0, 2, 0),
(47958, 0, 0, 2, 0),
(47959, 0, 0, 2, 0),
(47989, 0, 0, 2, 0),
(47991, 0, 0, 2, 0),
(47994, 0, 0, 2, 0),
(48212, 0, 0, 2, 0),
(48651, 0, 0, 2, 0),
(48661, 0, 0, 2, 0),
(48667, 0, 0, 2, 0),
(48668, 0, 0, 2, 0),
(48855, 0, 0, 2, 0),
(48857, 0, 0, 2, 0),
(48932, 0, 0, 2, 0),
(48933, 0, 0, 2, 0),
(48934, 0, 0, 2, 0),
(48935, 0, 0, 2, 0),
(49835, 0, 0, 2, 0),
(49845, 0, 0, 2, 0),
(49846, 0, 0, 2, 0),
(51267, 0, 0, 2, 0),
(51269, 0, 0, 2, 0),
(51271, 0, 0, 2, 0),
(51278, 0, 0, 2, 0),
(51475, 0, 0, 2, 0),
(51476, 0, 0, 2, 0),
(51477, 0, 0, 2, 0),
(51741, 0, 0, 2, 0),
(51742, 0, 0, 2, 0),
(53719, 0, 0, 2, 0);
-- A: Baradin's Wardens - H: Hellscream's Reach
DELETE FROM player_factionchange_reputations WHERE alliance_id=1177 OR horde_id=1178;
INSERT INTO player_factionchange_reputations (alliance_id, horde_id) VALUES
(1177, 1178);

-- A: Wildhammer Clan - H: Dragonmaw Clan
DELETE FROM player_factionchange_reputations WHERE alliance_id=1174 OR horde_id=1172;
INSERT INTO player_factionchange_reputations (alliance_id, horde_id) VALUES
(1174, 1172);

-- A: Kirin Tor Offensive - H: Sunreaver Onslaught
DELETE FROM player_factionchange_reputations WHERE alliance_id=1387 OR horde_id=1388;
INSERT INTO player_factionchange_reputations (alliance_id, horde_id) VALUES
(1387, 1388);

-- A: Operation: Shieldwall - H: Dominance Offensive
DELETE FROM player_factionchange_reputations WHERE alliance_id=1376 OR horde_id=1375;
INSERT INTO player_factionchange_reputations (alliance_id, horde_id) VALUES
(1376, 1375);
