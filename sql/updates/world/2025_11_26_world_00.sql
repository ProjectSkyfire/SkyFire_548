-- Fix Blackrock Battle Worg (Entry: 49871) loot chances
-- Trinity’s classic sniff showed far lower pouch/book drop rates than retail Mop.
-- Normalize the whole loot table to Wowhead Mop data so the wolf only drops a
-- single bag occasionally instead of showering every kill with vendor trash.

DELETE FROM `creature_loot_template` WHERE `Entry` = 49871;

INSERT INTO `creature_loot_template` (`Entry`, `Item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(49871, 117,    0.942412, 1, 0, 1, 1),  -- Tough Jerky
(49871, 159,    0.487455, 1, 0, 1, 1),  -- Refreshing Spring Water
(49871, 774,    0.162485, 1, 0, 1, 1),  -- Malachite
(49871, 805,    0.067269, 1, 0, 1, 1),  -- Small Red Pouch
(49871, 818,    0.077668, 1, 0, 1, 1),  -- Tigerseye
(49871, 828,    0.152411, 1, 0, 1, 1),  -- Small Blue Pouch
(49871, 1367,   0.779927, 1, 0, 1, 1),  -- Ragged Leather Boots
(49871, 1374,   0.259976, 1, 0, 1, 1),  -- Frayed Shoes
(49871, 1378,   0.357467, 1, 0, 1, 1),  -- Frayed Pants
(49871, 1468,   0.259976, 1, 0, 1, 1),  -- Murloc Fin
(49871, 2211,   0.227479, 1, 0, 1, 1),  -- Bent Buckler
(49871, 2589,   1.494861, 1, 0, 1, 2),  -- Linen Cloth
(49871, 2598,   0.030872, 1, 0, 1, 1),  -- Pattern: Red Linen Robe
(49871, 3300,  23.682817, 1, 0, 1, 1),  -- Rabbit's Foot
(49871, 4496,   0.087742, 1, 0, 1, 1),  -- Small Brown Pouch
(49871, 49751,  0.001950, 1, 0, 1, 1),  -- Priceless Rockjaw Artifact
(49871, 5571,   0.106915, 1, 0, 1, 1),  -- Small Black Pouch
(49871, 5572,   0.061744, 1, 0, 1, 1),  -- Small Green Pouch
(49871, 23384,  0.014624, 1, 0, 1, 1),  -- Tarnished Plate Bracers
(49871, 25467,  0.003250, 1, 0, 1, 1),  -- Broken Balanced Stone
(49871, 62328, 73.083735, 1, 0, 1, 1),  -- Shed Fur
(49871, 62512,  0.002600, 1, 0, 1, 1),  -- Depleted Totem
(49871, 67310,  0.062069, 1, 0, 1, 1);  -- Plans: Hardened Obsidium Belt

