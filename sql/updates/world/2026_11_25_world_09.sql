-- Stormwind vendor audit against Wowhead MoP Classic, verified 2026-11-25.
-- Add supported, unambiguous offers; retain existing stock, prices and conditions.
-- Currency/token offers use matching costs from the installed client data.
-- Oversized quartermaster catalogs, modern-currency variants, guild requirements,
-- conditional pet recovery, absent NPCs and unsupported items are deferred.

INSERT INTO npc_vendor (entry,slot,item,maxcount,incrtime,ExtendedCost,type)
SELECT a.entry,0,a.item,a.maxcount,a.incrtime,a.ExtendedCost,1
FROM (
    SELECT 1257 AS entry,3372 AS item,0 AS maxcount,0 AS incrtime,0 AS ExtendedCost -- Cracked Vial
    UNION ALL SELECT 1257,5565,0,0,0 -- Infernal Stone
    UNION ALL SELECT 1257,8925,0,0,0 -- Tainted Vial
    UNION ALL SELECT 1257,16583,0,0,0 -- Demonic Figurine
    UNION ALL SELECT 1257,17021,0,0,0 -- Wild Berries
    UNION ALL SELECT 1257,17026,0,0,0 -- Wild Thornroot
    UNION ALL SELECT 1257,17028,0,0,0 -- Holy Candle
    UNION ALL SELECT 1257,17029,0,0,0 -- Sacred Candle
    UNION ALL SELECT 1257,17033,0,0,0 -- Symbol of Divinity
    UNION ALL SELECT 1257,17035,0,0,0 -- Stranglethorn Seed
    UNION ALL SELECT 1257,17036,0,0,0 -- Ashwood Seed
    UNION ALL SELECT 1257,17037,0,0,0 -- Hornbeam Seed
    UNION ALL SELECT 1257,17038,0,0,0 -- Ironwood Seed
    UNION ALL SELECT 1257,18256,0,0,0 -- Melted Vial
    UNION ALL SELECT 1257,21177,0,0,0 -- Symbol of Kings
    UNION ALL SELECT 1257,22147,0,0,0 -- Flintweed Seed
    UNION ALL SELECT 1257,22148,0,0,0 -- Wild Quillvine
    UNION ALL SELECT 1257,31167,0,0,0 -- Gateway Control Stone
    UNION ALL SELECT 1257,37201,0,0,0 -- Corpse Dust
    UNION ALL SELECT 1257,40411,0,0,0 -- Shattered Vial
    UNION ALL SELECT 1257,44605,0,0,0 -- Wild Spineleaf
    UNION ALL SELECT 1257,44614,0,0,0 -- Starleaf Seed
    UNION ALL SELECT 1257,44615,0,0,0 -- Devout Candle
    UNION ALL SELECT 1257,63388,0,0,0 -- Dust of Disappearance
    UNION ALL SELECT 1257,64670,0,0,0 -- Vanishing Powder
    UNION ALL SELECT 1257,79249,0,0,0 -- Tome of the Clear Mind
    UNION ALL SELECT 1275,5565,0,0,0 -- Infernal Stone
    UNION ALL SELECT 1275,16583,0,0,0 -- Demonic Figurine
    UNION ALL SELECT 1275,17021,0,0,0 -- Wild Berries
    UNION ALL SELECT 1275,17026,0,0,0 -- Wild Thornroot
    UNION ALL SELECT 1275,17028,0,0,0 -- Holy Candle
    UNION ALL SELECT 1275,17029,0,0,0 -- Sacred Candle
    UNION ALL SELECT 1275,17033,0,0,0 -- Symbol of Divinity
    UNION ALL SELECT 1275,17035,0,0,0 -- Stranglethorn Seed
    UNION ALL SELECT 1275,17036,0,0,0 -- Ashwood Seed
    UNION ALL SELECT 1275,17037,0,0,0 -- Hornbeam Seed
    UNION ALL SELECT 1275,17038,0,0,0 -- Ironwood Seed
    UNION ALL SELECT 1275,21177,0,0,0 -- Symbol of Kings
    UNION ALL SELECT 1275,22147,0,0,0 -- Flintweed Seed
    UNION ALL SELECT 1275,22148,0,0,0 -- Wild Quillvine
    UNION ALL SELECT 1275,31167,0,0,0 -- Gateway Control Stone
    UNION ALL SELECT 1275,37201,0,0,0 -- Corpse Dust
    UNION ALL SELECT 1275,44605,0,0,0 -- Wild Spineleaf
    UNION ALL SELECT 1275,44614,0,0,0 -- Starleaf Seed
    UNION ALL SELECT 1275,44615,0,0,0 -- Devout Candle
    UNION ALL SELECT 1275,63388,0,0,0 -- Dust of Disappearance
    UNION ALL SELECT 1275,64670,0,0,0 -- Vanishing Powder
    UNION ALL SELECT 1275,79249,0,0,0 -- Tome of the Clear Mind
    UNION ALL SELECT 1285,1179,0,0,0 -- Ice Cold Milk
    UNION ALL SELECT 1285,2512,0,0,0 -- Rough Arrow
    UNION ALL SELECT 1285,2515,0,0,0 -- Sharp Arrow
    UNION ALL SELECT 1285,2516,0,0,0 -- Light Shot
    UNION ALL SELECT 1285,2519,0,0,0 -- Heavy Shot
    UNION ALL SELECT 1285,2946,0,0,0 -- Broken Balanced Throwing Dagger
    UNION ALL SELECT 1285,2947,0,0,0 -- Broken Small Throwing Knife
    UNION ALL SELECT 1285,3030,0,0,0 -- Razor Arrow
    UNION ALL SELECT 1285,3033,0,0,0 -- Solid Shot
    UNION ALL SELECT 1285,3107,0,0,0 -- Broken Keen Throwing Knife
    UNION ALL SELECT 1285,3108,0,0,0 -- Broken Heavy Throwing Dagger
    UNION ALL SELECT 1285,3111,0,0,0 -- Broken Crude Throwing Axe
    UNION ALL SELECT 1285,3131,0,0,0 -- Broken Weighted Throwing Axe
    UNION ALL SELECT 1285,3135,0,0,0 -- Broken Sharp Throwing Axe
    UNION ALL SELECT 1285,3137,0,0,0 -- Broken Deadly Throwing Axe
    UNION ALL SELECT 1285,4471,0,0,0 -- Flint and Tinder
    UNION ALL SELECT 1285,4496,0,0,0 -- Small Brown Pouch
    UNION ALL SELECT 1285,4498,0,0,0 -- Brown Leather Satchel
    UNION ALL SELECT 1285,4540,0,0,0 -- Tough Hunk of Bread
    UNION ALL SELECT 1285,5042,0,0,0 -- Red Ribboned Wrapping Paper
    UNION ALL SELECT 1285,5048,0,0,0 -- Blue Ribboned Wrapping Paper
    UNION ALL SELECT 1285,11284,0,0,0 -- Accurate Slugs
    UNION ALL SELECT 1285,11285,0,0,0 -- Jagged Arrow
    UNION ALL SELECT 1285,15326,0,0,0 -- Broken Gleaming Throwing Axe
    UNION ALL SELECT 1285,15327,0,0,0 -- Broken Wicked Throwing Dagger
    UNION ALL SELECT 1285,25861,0,0,0 -- Crude Throwing Axe
    UNION ALL SELECT 1285,25872,0,0,0 -- Balanced Throwing Dagger
    UNION ALL SELECT 1285,25873,0,0,0 -- Keen Throwing Knife
    UNION ALL SELECT 1285,25875,0,0,0 -- Deadly Throwing Axe
    UNION ALL SELECT 1285,25876,0,0,0 -- Gleaming Throwing Axe
    UNION ALL SELECT 1285,28053,0,0,0 -- Wicked Arrow
    UNION ALL SELECT 1285,28060,0,0,0 -- Impact Shot
    UNION ALL SELECT 1285,28979,0,0,0 -- Light Throwing Knife
    UNION ALL SELECT 1285,29007,0,0,0 -- Weighted Throwing Axe
    UNION ALL SELECT 1285,29008,0,0,0 -- Sharp Throwing Axe
    UNION ALL SELECT 1285,29009,0,0,0 -- Heavy Throwing Dagger
    UNION ALL SELECT 1285,29010,0,0,0 -- Wicked Throwing Dagger
    UNION ALL SELECT 1285,29013,0,0,0 -- Jagged Throwing Axe
    UNION ALL SELECT 1285,29014,0,0,0 -- Blacksteel Throwing Dagger
    UNION ALL SELECT 1286,2324,0,0,0 -- Bleach
    UNION ALL SELECT 1286,2605,0,0,0 -- Green Dye
    UNION ALL SELECT 1286,2692,0,0,0 -- Hot Spices
    UNION ALL SELECT 1286,2928,0,0,0 -- Dust of Decay
    UNION ALL SELECT 1286,3372,0,0,0 -- Cracked Vial
    UNION ALL SELECT 1286,3713,0,0,0 -- Soothing Spices
    UNION ALL SELECT 1286,6529,0,0,0 -- Shiny Bauble
    UNION ALL SELECT 1286,8925,0,0,0 -- Tainted Vial
    UNION ALL SELECT 1286,18256,0,0,0 -- Melted Vial
    UNION ALL SELECT 1286,20815,0,0,0 -- Jeweler's Kit
    UNION ALL SELECT 1286,39505,0,0,0 -- Virtuoso Inking Set
    UNION ALL SELECT 1286,85663,0,0,0 -- Herbalist's Spade
    UNION ALL SELECT 1287,2101,0,0,0 -- Light Quiver
    UNION ALL SELECT 1287,2512,0,0,0 -- Rough Arrow
    UNION ALL SELECT 1287,2515,0,0,0 -- Sharp Arrow
    UNION ALL SELECT 1287,3030,0,0,0 -- Razor Arrow
    UNION ALL SELECT 1287,7371,0,0,0 -- Heavy Quiver
    UNION ALL SELECT 1287,11362,0,0,0 -- Medium Quiver
    UNION ALL SELECT 1297,2512,0,0,0 -- Rough Arrow
    UNION ALL SELECT 1297,2515,0,0,0 -- Sharp Arrow
    UNION ALL SELECT 1297,2516,0,0,0 -- Light Shot
    UNION ALL SELECT 1297,2519,0,0,0 -- Heavy Shot
    UNION ALL SELECT 1297,3030,0,0,0 -- Razor Arrow
    UNION ALL SELECT 1297,3033,0,0,0 -- Solid Shot
    UNION ALL SELECT 1297,5439,0,0,0 -- Small Quiver
    UNION ALL SELECT 1297,5441,0,0,0 -- Small Shot Pouch
    UNION ALL SELECT 1297,11284,0,0,0 -- Accurate Slugs
    UNION ALL SELECT 1297,11362,0,0,0 -- Medium Quiver
    UNION ALL SELECT 1298,2512,0,0,0 -- Rough Arrow
    UNION ALL SELECT 1298,2515,0,0,0 -- Sharp Arrow
    UNION ALL SELECT 1298,3030,0,0,0 -- Razor Arrow
    UNION ALL SELECT 1298,5439,0,0,0 -- Small Quiver
    UNION ALL SELECT 1298,11285,0,0,0 -- Jagged Arrow
    UNION ALL SELECT 1298,11303,2,3600,0 -- Fine Shortbow
    UNION ALL SELECT 1298,11306,1,3600,0 -- Sturdy Recurve
    UNION ALL SELECT 1298,11307,1,3600,0 -- Massive Longbow
    UNION ALL SELECT 1298,11362,0,0,0 -- Medium Quiver
    UNION ALL SELECT 1298,28053,0,0,0 -- Wicked Arrow
    UNION ALL SELECT 1302,785,2,3600,0 -- Mageroyal
    UNION ALL SELECT 1302,2453,2,3600,0 -- Bruiseweed
    UNION ALL SELECT 1302,3356,1,3600,0 -- Kingsblood
    UNION ALL SELECT 1302,3357,1,3600,0 -- Liferoot
    UNION ALL SELECT 1304,18649,1,3600,0 -- Schematic: Blue Firework
    UNION ALL SELECT 1307,4824,1,3600,0 -- Blurred Axe
    UNION ALL SELECT 1307,4826,1,3600,0 -- Marauder Axe
    UNION ALL SELECT 1307,4827,1,3600,0 -- Wizard's Belt
    UNION ALL SELECT 1307,4829,1,3600,0 -- Dreamer's Belt
    UNION ALL SELECT 1307,5565,0,0,0 -- Infernal Stone
    UNION ALL SELECT 1307,16583,0,0,0 -- Demonic Figurine
    UNION ALL SELECT 1307,17021,0,0,0 -- Wild Berries
    UNION ALL SELECT 1307,17026,0,0,0 -- Wild Thornroot
    UNION ALL SELECT 1307,17028,0,0,0 -- Holy Candle
    UNION ALL SELECT 1307,17029,0,0,0 -- Sacred Candle
    UNION ALL SELECT 1307,17033,0,0,0 -- Symbol of Divinity
    UNION ALL SELECT 1307,17035,0,0,0 -- Stranglethorn Seed
    UNION ALL SELECT 1307,17036,0,0,0 -- Ashwood Seed
    UNION ALL SELECT 1307,17037,0,0,0 -- Hornbeam Seed
    UNION ALL SELECT 1307,17038,0,0,0 -- Ironwood Seed
    UNION ALL SELECT 1307,21177,0,0,0 -- Symbol of Kings
    UNION ALL SELECT 1307,22147,0,0,0 -- Flintweed Seed
    UNION ALL SELECT 1307,22148,0,0,0 -- Wild Quillvine
    UNION ALL SELECT 1307,37201,0,0,0 -- Corpse Dust
    UNION ALL SELECT 1307,63388,0,0,0 -- Dust of Disappearance
    UNION ALL SELECT 1307,64670,0,0,0 -- Vanishing Powder
    UNION ALL SELECT 1307,79249,0,0,0 -- Tome of the Clear Mind
    UNION ALL SELECT 1308,5565,0,0,0 -- Infernal Stone
    UNION ALL SELECT 1308,16583,0,0,0 -- Demonic Figurine
    UNION ALL SELECT 1308,17021,0,0,0 -- Wild Berries
    UNION ALL SELECT 1308,17026,0,0,0 -- Wild Thornroot
    UNION ALL SELECT 1308,17028,0,0,0 -- Holy Candle
    UNION ALL SELECT 1308,17029,0,0,0 -- Sacred Candle
    UNION ALL SELECT 1308,17033,0,0,0 -- Symbol of Divinity
    UNION ALL SELECT 1308,17035,0,0,0 -- Stranglethorn Seed
    UNION ALL SELECT 1308,17036,0,0,0 -- Ashwood Seed
    UNION ALL SELECT 1308,17037,0,0,0 -- Hornbeam Seed
    UNION ALL SELECT 1308,17038,0,0,0 -- Ironwood Seed
    UNION ALL SELECT 1308,21177,0,0,0 -- Symbol of Kings
    UNION ALL SELECT 1308,22147,0,0,0 -- Flintweed Seed
    UNION ALL SELECT 1308,22148,0,0,0 -- Wild Quillvine
    UNION ALL SELECT 1308,31167,0,0,0 -- Gateway Control Stone
    UNION ALL SELECT 1308,37201,0,0,0 -- Corpse Dust
    UNION ALL SELECT 1308,44605,0,0,0 -- Wild Spineleaf
    UNION ALL SELECT 1308,44614,0,0,0 -- Starleaf Seed
    UNION ALL SELECT 1308,44615,0,0,0 -- Devout Candle
    UNION ALL SELECT 1308,63388,0,0,0 -- Dust of Disappearance
    UNION ALL SELECT 1308,64670,0,0,0 -- Vanishing Powder
    UNION ALL SELECT 1308,79249,0,0,0 -- Tome of the Clear Mind
    UNION ALL SELECT 1313,3356,1,3600,0 -- Kingsblood
    UNION ALL SELECT 1313,3372,0,0,0 -- Cracked Vial
    UNION ALL SELECT 1313,8925,0,0,0 -- Tainted Vial
    UNION ALL SELECT 1313,9301,1,3600,0 -- Recipe: Elixir of Shadow Power
    UNION ALL SELECT 1313,18256,0,0,0 -- Melted Vial
    UNION ALL SELECT 1313,40411,0,0,0 -- Shattered Vial
    UNION ALL SELECT 1316,955,3,3600,0 -- Scroll of Intellect
    UNION ALL SELECT 1316,1181,2,43200,0 -- Scroll of Spirit
    UNION ALL SELECT 1316,1711,2,3600,0 -- Scroll of Stamina II
    UNION ALL SELECT 1316,1712,2,3600,0 -- Scroll of Spirit II
    UNION ALL SELECT 1316,2290,2,3600,0 -- Scroll of Intellect II
    UNION ALL SELECT 1316,39354,0,0,0 -- Light Parchment
    UNION ALL SELECT 1325,2928,0,0,0 -- Dust of Decay
    UNION ALL SELECT 1325,2930,0,0,0 -- Essence of Pain
    UNION ALL SELECT 1325,3371,0,0,0 -- Crystal Vial
    UNION ALL SELECT 1325,3372,0,0,0 -- Cracked Vial
    UNION ALL SELECT 1325,3777,0,0,0 -- Lethargy Root
    UNION ALL SELECT 1325,4565,0,0,0 -- Simple Dagger
    UNION ALL SELECT 1325,5060,0,0,0 -- Thieves' Tools
    UNION ALL SELECT 1325,5140,0,0,0 -- Flash Powder
    UNION ALL SELECT 1325,5173,0,0,0 -- Deathweed
    UNION ALL SELECT 1325,8923,0,0,0 -- Essence of Agony
    UNION ALL SELECT 1325,8924,0,0,0 -- Dust of Deterioration
    UNION ALL SELECT 1325,8925,0,0,0 -- Tainted Vial
    UNION ALL SELECT 1326,2928,0,0,0 -- Dust of Decay
    UNION ALL SELECT 1326,2930,0,0,0 -- Essence of Pain
    UNION ALL SELECT 1326,3371,0,0,0 -- Crystal Vial
    UNION ALL SELECT 1326,3372,0,0,0 -- Cracked Vial
    UNION ALL SELECT 1326,3777,0,0,0 -- Lethargy Root
    UNION ALL SELECT 1326,4565,0,0,0 -- Simple Dagger
    UNION ALL SELECT 1326,5060,0,0,0 -- Thieves' Tools
    UNION ALL SELECT 1326,5140,0,0,0 -- Flash Powder
    UNION ALL SELECT 1326,5173,0,0,0 -- Deathweed
    UNION ALL SELECT 1326,8923,0,0,0 -- Essence of Agony
    UNION ALL SELECT 1326,8924,0,0,0 -- Dust of Deterioration
    UNION ALL SELECT 1326,8925,0,0,0 -- Tainted Vial
    UNION ALL SELECT 1351,5565,0,0,0 -- Infernal Stone
    UNION ALL SELECT 1351,16583,0,0,0 -- Demonic Figurine
    UNION ALL SELECT 1351,17021,0,0,0 -- Wild Berries
    UNION ALL SELECT 1351,17026,0,0,0 -- Wild Thornroot
    UNION ALL SELECT 1351,17028,0,0,0 -- Holy Candle
    UNION ALL SELECT 1351,17029,0,0,0 -- Sacred Candle
    UNION ALL SELECT 1351,17033,0,0,0 -- Symbol of Divinity
    UNION ALL SELECT 1351,17035,0,0,0 -- Stranglethorn Seed
    UNION ALL SELECT 1351,17036,0,0,0 -- Ashwood Seed
    UNION ALL SELECT 1351,17037,0,0,0 -- Hornbeam Seed
    UNION ALL SELECT 1351,17038,0,0,0 -- Ironwood Seed
    UNION ALL SELECT 1351,21177,0,0,0 -- Symbol of Kings
    UNION ALL SELECT 1351,22147,0,0,0 -- Flintweed Seed
    UNION ALL SELECT 1351,22148,0,0,0 -- Wild Quillvine
    UNION ALL SELECT 1351,31167,0,0,0 -- Gateway Control Stone
    UNION ALL SELECT 1351,37201,0,0,0 -- Corpse Dust
    UNION ALL SELECT 1351,44605,0,0,0 -- Wild Spineleaf
    UNION ALL SELECT 1351,44614,0,0,0 -- Starleaf Seed
    UNION ALL SELECT 1351,44615,0,0,0 -- Devout Candle
    UNION ALL SELECT 1351,63388,0,0,0 -- Dust of Disappearance
    UNION ALL SELECT 1351,64670,0,0,0 -- Vanishing Powder
    UNION ALL SELECT 1351,79249,0,0,0 -- Tome of the Clear Mind
    UNION ALL SELECT 4981,35954,0,0,0 -- Sweetened Goat's Milk
    UNION ALL SELECT 5483,2692,0,0,0 -- Hot Spices
    UNION ALL SELECT 5483,3713,0,0,0 -- Soothing Spices
    UNION ALL SELECT 5503,3372,0,0,0 -- Cracked Vial
    UNION ALL SELECT 5503,8925,0,0,0 -- Tainted Vial
    UNION ALL SELECT 5503,18256,0,0,0 -- Melted Vial
    UNION ALL SELECT 5503,40411,0,0,0 -- Shattered Vial
    UNION ALL SELECT 5510,2516,0,0,0 -- Light Shot
    UNION ALL SELECT 5510,2519,0,0,0 -- Heavy Shot
    UNION ALL SELECT 5510,5441,0,0,0 -- Small Shot Pouch
    UNION ALL SELECT 5519,4361,2,3600,0 -- Bent Copper Tube
    UNION ALL SELECT 5519,4363,2,3600,0 -- Broken Modulator
    UNION ALL SELECT 5519,4382,1,3600,0 -- Bronze Framework
    UNION ALL SELECT 5519,4389,1,3600,0 -- Gyrochronatom
    UNION ALL SELECT 5519,4404,3,3600,0 -- Silver Contact
    UNION ALL SELECT 5519,39354,0,0,0 -- Light Parchment
    UNION ALL SELECT 5519,90146,0,0,0 -- Tinker's Kit
    UNION ALL SELECT 6740,21815,0,0,0 -- Love Token
    UNION ALL SELECT 6740,21829,0,0,0 -- Perfume Bottle
    UNION ALL SELECT 6740,21833,0,0,0 -- Cologne Bottle
    UNION ALL SELECT 6740,35954,0,0,0 -- Sweetened Goat's Milk
    UNION ALL SELECT 12805,18448,0,0,0 -- Sergeant Major's Chain Armguards
    UNION ALL SELECT 12805,18449,0,0,0 -- Sergeant Major's Chain Armguards
    UNION ALL SELECT 12805,18452,0,0,0 -- Sergeant Major's Leather Armsplints
    UNION ALL SELECT 12805,18453,0,0,0 -- Sergeant Major's Leather Armsplints
    UNION ALL SELECT 12805,18454,0,0,0 -- Sergeant Major's Dragonhide Armsplints
    UNION ALL SELECT 12805,18455,0,0,0 -- Sergeant Major's Dragonhide Armsplints
    UNION ALL SELECT 12805,18456,0,0,0 -- Sergeant Major's Silk Cuffs
    UNION ALL SELECT 12805,18457,0,0,0 -- Sergeant Major's Silk Cuffs
    UNION ALL SELECT 12805,18664,0,0,0 -- A Treatise on Military Ranks
    UNION ALL SELECT 12805,18856,0,0,0 -- Insignia of the Alliance
    UNION ALL SELECT 12805,18857,0,0,0 -- Insignia of the Alliance
    UNION ALL SELECT 12805,18858,0,0,0 -- Insignia of the Alliance
    UNION ALL SELECT 12805,18859,0,0,0 -- Insignia of the Alliance
    UNION ALL SELECT 12805,18862,0,0,0 -- Insignia of the Alliance
    UNION ALL SELECT 12805,18863,0,0,0 -- Insignia of the Alliance
    UNION ALL SELECT 12805,18864,0,0,0 -- Insignia of the Alliance
    UNION ALL SELECT 13435,34262,0,0,0 -- Pattern: Winter Boots
    UNION ALL SELECT 13435,34319,0,0,0 -- Pattern: Red Winter Clothes
    UNION ALL SELECT 13435,34413,0,0,0 -- Recipe: Hot Apple Cider
    UNION ALL SELECT 28347,2892,0,0,0 -- Empty Poison Vial
    UNION ALL SELECT 28347,2893,0,0,0 -- Empty Poison Vial
    UNION ALL SELECT 28347,3775,0,0,0 -- Empty Poison Vial
    UNION ALL SELECT 28347,4565,0,0,0 -- Simple Dagger
    UNION ALL SELECT 28347,5237,0,0,0 -- Empty Poison Vial
    UNION ALL SELECT 28347,6947,0,0,0 -- Empty Poison Vial
    UNION ALL SELECT 28347,6949,0,0,0 -- Empty Poison Vial
    UNION ALL SELECT 28347,6950,0,0,0 -- Empty Poison Vial
    UNION ALL SELECT 28347,8926,0,0,0 -- Empty Poison Vial
    UNION ALL SELECT 28347,8927,0,0,0 -- Empty Poison Vial
    UNION ALL SELECT 28347,8928,0,0,0 -- Empty Poison Vial
    UNION ALL SELECT 28347,8984,0,0,0 -- Empty Poison Vial
    UNION ALL SELECT 28347,8985,0,0,0 -- Empty Poison Vial
    UNION ALL SELECT 28347,10918,0,0,0 -- Empty Poison Vial
    UNION ALL SELECT 28347,10920,0,0,0 -- Empty Poison Vial
    UNION ALL SELECT 28347,10921,0,0,0 -- Empty Poison Vial
    UNION ALL SELECT 28347,10922,0,0,0 -- Empty Poison Vial
    UNION ALL SELECT 28347,20844,0,0,0 -- Empty Poison Vial
    UNION ALL SELECT 28347,21835,0,0,0 -- Anesthetic Poison
    UNION ALL SELECT 28347,21927,0,0,0 -- Empty Poison Vial
    UNION ALL SELECT 28347,22054,0,0,0 -- Empty Poison Vial
    UNION ALL SELECT 28347,22055,0,0,0 -- Empty Poison Vial
    UNION ALL SELECT 28347,43231,0,0,0 -- Empty Poison Vial
    UNION ALL SELECT 28347,43233,0,0,0 -- Empty Poison Vial
    UNION ALL SELECT 28347,43235,0,0,0 -- Empty Poison Vial
    UNION ALL SELECT 28347,43237,0,0,0 -- Anesthetic Poison
    UNION ALL SELECT 29288,20815,0,0,0 -- Jeweler's Kit
    UNION ALL SELECT 29288,39505,0,0,0 -- Virtuoso Inking Set
    UNION ALL SELECT 29288,63388,0,0,0 -- Dust of Disappearance
    UNION ALL SELECT 29288,64670,0,0,0 -- Vanishing Powder
    UNION ALL SELECT 29288,79249,0,0,0 -- Tome of the Clear Mind
    UNION ALL SELECT 29288,85663,0,0,0 -- Herbalist's Spade
    UNION ALL SELECT 30730,1515,0,0,0 -- Rough Wooden Staff
    UNION ALL SELECT 30730,61978,0,0,2582 -- Blackfallow Ink
    UNION ALL SELECT 30730,79249,0,0,0 -- Tome of the Clear Mind
    UNION ALL SELECT 30730,79255,0,0,2583 -- Starlight Ink
    UNION ALL SELECT 30730,79740,0,0,0 -- Plain Wooden Staff
    UNION ALL SELECT 52027,79131,0,0,2551 -- Burnished Warden Staff
    UNION ALL SELECT 52027,93843,0,0,4314 -- Hardened Arcanite Reaper
    UNION ALL SELECT 52027,93844,0,0,4316 -- Refinished Warden Staff
    UNION ALL SELECT 52027,93845,0,0,4318 -- Gore-Steeped Skullforge Reaver
    UNION ALL SELECT 52027,93846,0,0,4320 -- Re-Engineered Lava Dredger
    UNION ALL SELECT 52027,93847,0,0,4322 -- Crushing Mass of McGowan
    UNION ALL SELECT 52027,93853,0,0,4329 -- Pious Aurastone Hammer
    UNION ALL SELECT 52027,93854,0,0,4331 -- Scholarly Headmaster's Charge
    UNION ALL SELECT 52027,93855,0,0,4333 -- War-Torn Ancient Bone Bow
    UNION ALL SELECT 52027,93856,0,0,4335 -- Noble Dal'Rend's Sacred Charge
    UNION ALL SELECT 52027,93857,0,0,4337 -- Vengeful Heartseeker
    UNION ALL SELECT 52027,93859,0,0,4341 -- Bloodstained Dreadmist Mantle
    UNION ALL SELECT 52027,93860,0,0,4343 -- Bloodstained Dreadmist Robe
    UNION ALL SELECT 52027,93862,0,0,4346 -- Supple Shadowcraft Spaulders
    UNION ALL SELECT 52027,93863,0,0,4348 -- Supple Shadowcraft Tunic
    UNION ALL SELECT 52027,93864,0,0,4350 -- Majestic Ironfeather Shoulders
    UNION ALL SELECT 52027,93865,0,0,4352 -- Majestic Ironfeather Breastplate
    UNION ALL SELECT 52027,93876,0,0,4355 -- Awakened Pauldrons of Elements
    UNION ALL SELECT 52027,93885,0,0,4357 -- Awakened Vest of Elements
    UNION ALL SELECT 52027,93887,0,0,4360 -- Grand Champion Herod's Shoulder
    UNION ALL SELECT 52027,93888,0,0,4362 -- Furious Deathdealer Breastplate
    UNION ALL SELECT 52027,93890,0,0,4365 -- Gleaming Spaulders of Valor
    UNION ALL SELECT 52027,93891,0,0,4367 -- Gleaming Breastplate of Valor
    UNION ALL SELECT 52027,93892,0,0,4369 -- Brushed Breastplate of Might
    UNION ALL SELECT 52027,93893,0,0,4371 -- Brushed Pauldrons of Might
    UNION ALL SELECT 52027,93896,0,0,4375 -- Forceful Hand of Justice
    UNION ALL SELECT 52027,93897,0,0,4377 -- Piercing Eye of the Beast
    UNION ALL SELECT 52027,93902,0,0,2550 -- Flamescarred Draconian Deflector
    UNION ALL SELECT 52027,93903,0,0,2550 -- Weathered Observer's Shield
    UNION ALL SELECT 52027,93904,0,0,2550 -- Musty Tome of the Lost
    UNION ALL SELECT 52031,61978,0,0,2582 -- Blackfallow Ink
    UNION ALL SELECT 52031,79255,0,0,2583 -- Starlight Ink
) AS a
JOIN item_template i ON i.entry=a.item
WHERE NOT EXISTS (SELECT 1 FROM npc_vendor v
    WHERE v.entry=a.entry AND v.item=a.item AND v.type=1);

-- Correct published stock limits while keeping established restock intervals.
-- Charys Yserian: Lesser Mana Potion
UPDATE npc_vendor SET maxcount=3
WHERE entry=1307 AND item=3385 AND type=1
  AND maxcount=2 AND incrtime=3600;
-- Maria Lumere: Silverleaf
UPDATE npc_vendor SET maxcount=3
WHERE entry=1313 AND item=765 AND type=1
  AND maxcount=2 AND incrtime=3600;
-- Maria Lumere: Mageroyal
UPDATE npc_vendor SET maxcount=4
WHERE entry=1313 AND item=785 AND type=1
  AND maxcount=2 AND incrtime=3600;
-- Maria Lumere: Peacebloom
UPDATE npc_vendor SET maxcount=3
WHERE entry=1313 AND item=2447 AND type=1
  AND maxcount=1 AND incrtime=3600;
-- Maria Lumere: Earthroot
UPDATE npc_vendor SET maxcount=4
WHERE entry=1313 AND item=2449 AND type=1
  AND maxcount=2 AND incrtime=3600;
-- Adair Gilroy: Scroll of Strength
UPDATE npc_vendor SET maxcount=4
WHERE entry=1316 AND item=954 AND type=1
  AND maxcount=2 AND incrtime=3600;
-- Adair Gilroy: Scroll of Stamina
UPDATE npc_vendor SET maxcount=3
WHERE entry=1316 AND item=1180 AND type=1
  AND maxcount=1 AND incrtime=3600;
-- Adair Gilroy: Scroll of Protection II
UPDATE npc_vendor SET maxcount=2
WHERE entry=1316 AND item=1478 AND type=1
  AND maxcount=1 AND incrtime=3600;
-- Adair Gilroy: Scroll of Agility
UPDATE npc_vendor SET maxcount=5
WHERE entry=1316 AND item=3012 AND type=1
  AND maxcount=2 AND incrtime=3600;
-- Jessara Cordell: Lesser Magic Essence
UPDATE npc_vendor SET maxcount=2
WHERE entry=1318 AND item=10938 AND type=1
  AND maxcount=1 AND incrtime=3600;
-- Jessara Cordell: Strange Dust
UPDATE npc_vendor SET maxcount=4
WHERE entry=1318 AND item=10940 AND type=1
  AND maxcount=3 AND incrtime=3600;
-- Billibub Cogspinner: Rough Blasting Powder
UPDATE npc_vendor SET maxcount=4
WHERE entry=5519 AND item=4357 AND type=1
  AND maxcount=2 AND incrtime=3600;
-- Billibub Cogspinner: Coarse Blasting Powder
UPDATE npc_vendor SET maxcount=4
WHERE entry=5519 AND item=4364 AND type=1
  AND maxcount=2 AND incrtime=3600;
