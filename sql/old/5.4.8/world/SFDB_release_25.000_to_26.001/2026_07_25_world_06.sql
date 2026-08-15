-- SkyFire npc_trainer SQL for Lien Farner (entry 47384)
-- Source: Wowhead Mists Classic
-- URL: https://www.wowhead.com/mop-classic/npc=47384/lien-farner
-- Generated: 2026-07-25T17:46:55.2349984+00:00
-- Mapping: trainingcost => spellcost; skill[0] + learnedat => reqskill/reqskillvalue; level => reqlevel.
-- Wowhead learnedat=9999 is treated as no required profession skill.

DELETE FROM `npc_trainer` WHERE `entry` = 47384;
INSERT INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`) VALUES
(47384, 2018, 10, 0, 0, 0), -- Blacksmithing
(47384, 2108, 10, 0, 0, 0), -- Leatherworking
(47384, 2153, 50, 165, 15, 0), -- Handstitched Leather Pants
(47384, 2160, 100, 165, 40, 0), -- Embossed Leather Vest
(47384, 2161, 100, 165, 55, 0), -- Embossed Leather Boots
(47384, 2162, 100, 165, 60, 0), -- Embossed Leather Cloak
(47384, 2259, 10, 0, 0, 0), -- Alchemy
(47384, 2331, 100, 171, 25, 0), -- Minor Mana Potion
(47384, 2332, 150, 171, 40, 0), -- Minor Rejuvenation Potion
(47384, 2334, 250, 171, 50, 0), -- Elixir of Minor Fortitude
(47384, 2337, 1000, 171, 55, 0), -- Lesser Healing Potion
(47384, 2366, 10, 0, 0, 0), -- Herb Gathering
(47384, 2385, 50, 197, 10, 0), -- Brown Linen Vest
(47384, 2386, 200, 197, 65, 0), -- Linen Boots
(47384, 2392, 50, 197, 40, 0), -- Red Linen Shirt
(47384, 2393, 22, 197, 1, 0), -- White Linen Shirt
(47384, 2394, 50, 197, 40, 0), -- Blue Linen Shirt
(47384, 2395, 300, 197, 70, 0), -- Barbaric Linen Vest
(47384, 2396, 200, 197, 70, 0), -- Green Linen Shirt
(47384, 2397, 200, 197, 60, 0), -- Reinforced Linen Cape
(47384, 2402, 250, 197, 75, 0), -- Woolen Cape
(47384, 2575, 10, 0, 0, 0), -- Mining
(47384, 2658, 200, 186, 65, 0), -- Smelt Silver
(47384, 2659, 200, 186, 50, 0), -- Smelt Bronze
(47384, 2661, 100, 164, 35, 0), -- Copper Chain Belt
(47384, 2662, 50, 164, 1, 0), -- Copper Chain Pants
(47384, 2665, 100, 164, 65, 0), -- Coarse Sharpening Stone
(47384, 2666, 200, 164, 70, 0), -- Runed Copper Belt
(47384, 2737, 50, 164, 15, 0), -- Copper Mace
(47384, 2738, 50, 164, 20, 0), -- Copper Axe
(47384, 2739, 50, 164, 25, 0), -- Copper Shortsword
(47384, 2964, 100, 197, 75, 0), -- Bolt of Woolen Cloth
(47384, 3116, 100, 164, 65, 0), -- Coarse Weightstone
(47384, 3170, 50, 171, 15, 0), -- Weak Troll's Blood Elixir
(47384, 3293, 250, 164, 35, 0), -- Copper Battle Axe
(47384, 3294, 500, 164, 70, 0), -- Thick War Axe
(47384, 3304, 50, 186, 50, 0), -- Smelt Tin
(47384, 3319, 50, 164, 20, 0), -- Copper Chain Boots
(47384, 3320, 100, 164, 25, 0), -- Rough Grinding Stone
(47384, 3323, 100, 164, 40, 0), -- Runed Copper Gauntlets
(47384, 3324, 200, 164, 45, 0), -- Runed Copper Pants
(47384, 3326, 250, 164, 75, 0), -- Coarse Grinding Stone
(47384, 3753, 75, 165, 25, 0), -- Handstitched Leather Belt
(47384, 3755, 100, 197, 45, 0), -- Linen Bag
(47384, 3756, 150, 165, 55, 0), -- Embossed Leather Gloves
(47384, 3759, 200, 165, 75, 0), -- Embossed Leather Pants
(47384, 3816, 50, 165, 35, 0), -- Cured Light Hide
(47384, 3840, 100, 197, 35, 0), -- Heavy Linen Gloves
(47384, 3841, 200, 197, 60, 0), -- Green Linen Bracers
(47384, 3842, 300, 197, 70, 0), -- Handstitched Linen Britches
(47384, 3908, 10, 0, 0, 0), -- Tailoring
(47384, 3914, 50, 197, 30, 0), -- Brown Linen Pants
(47384, 3922, 115, 202, 30, 0), -- Handful of Copper Bolts
(47384, 3923, 130, 202, 30, 0), -- Rough Copper Bomb
(47384, 3925, 150, 202, 50, 0), -- Rough Boomstick
(47384, 3929, 250, 202, 65, 0), -- Coarse Blasting Powder
(47384, 3931, 250, 202, 65, 0), -- Coarse Dynamite
(47384, 3977, 200, 202, 60, 0), -- Crude Scope
(47384, 4036, 10, 0, 0, 0), -- Engineering
(47384, 7408, 300, 164, 65, 0), -- Heavy Copper Maul
(47384, 7411, 10, 0, 0, 0), -- Enchanting
(47384, 7420, 50, 333, 15, 0), -- Enchant Chest - Minor Health
(47384, 7426, 100, 333, 40, 0), -- Enchant Chest - Minor Absorption
(47384, 7430, 150, 202, 50, 0), -- Arclight Spanner
(47384, 7457, 250, 333, 50, 0), -- Enchant Bracer - Minor Stamina
(47384, 7623, 50, 197, 30, 0), -- Brown Linen Robe
(47384, 7624, 50, 197, 30, 0), -- White Linen Robe
(47384, 7748, 250, 333, 60, 0), -- Enchant Chest - Lesser Health
(47384, 7771, 200, 333, 70, 0), -- Enchant Cloak - Minor Protection
(47384, 8465, 50, 197, 40, 0), -- Simple Dress
(47384, 8613, 10, 0, 0, 0), -- Skinning
(47384, 8776, 50, 197, 15, 0), -- Linen Belt
(47384, 8880, 100, 164, 30, 0), -- Copper Dagger
(47384, 9065, 150, 165, 70, 0), -- Light Leather Bracers
(47384, 9983, 100, 164, 30, 0), -- Copper Claymore
(47384, 12045, 50, 197, 20, 0), -- Simple Linen Boots
(47384, 12046, 300, 197, 75, 0), -- Simple Kilt
(47384, 14293, 50, 333, 10, 0), -- Lesser Magic Wand
(47384, 14807, 200, 333, 70, 0), -- Greater Magic Wand
(47384, 25229, 10, 0, 0, 0), -- Jewelcrafting
(47384, 25278, 200, 755, 50, 0), -- Bronze Setting
(47384, 25280, 200, 755, 50, 0), -- Elegant Silver Ring
(47384, 25283, 100, 755, 30, 0), -- Inlaid Malachite Ring
(47384, 25284, 400, 755, 60, 0), -- Simple Pearl Ring
(47384, 25287, 400, 755, 70, 0), -- Gloom Band
(47384, 25490, 300, 755, 50, 0), -- Solid Bronze Ring
(47384, 26926, 50, 755, 5, 0), -- Heavy Copper Ring
(47384, 26927, 300, 755, 50, 0), -- Thick Bronze Necklace
(47384, 26928, 100, 755, 30, 0), -- Ornate Tigerseye Necklace
(47384, 31252, 100, 755, 20, 0), -- Prospecting
(47384, 32178, 100, 755, 20, 0), -- Malachite Pendant
(47384, 32179, 100, 755, 20, 0), -- Tigerseye Band
(47384, 32801, 200, 755, 50, 0), -- Coarse Stone Statue
(47384, 36523, 600, 755, 75, 0), -- Brilliant Necklace
(47384, 37818, 500, 755, 65, 0), -- Bronze Band of Force
(47384, 45357, 10, 0, 0, 0), -- Inscription
(47384, 48248, 50, 773, 35, 0), -- Scroll of Recall
(47384, 50598, 400, 773, 75, 0), -- Scroll of Intellect II
(47384, 50605, 400, 773, 75, 0), -- Scroll of Spirit II
(47384, 50612, 400, 773, 75, 0), -- Scroll of Stamina II
(47384, 52739, 150, 773, 35, 0), -- Enchanting Vellum
(47384, 52843, 50, 773, 35, 0), -- Moonglow Ink
(47384, 53462, 300, 773, 75, 0), -- Midnight Ink
(47384, 58472, 50, 773, 15, 0), -- Scroll of Agility
(47384, 58484, 50, 773, 15, 0), -- Scroll of Strength
(47384, 61288, 1000, 773, 75, 0); -- Minor Inscription Research
