-- Seed battle pet wild replacement pool data.
-- Source entries remain ambient critters; battle_pet_wild_pool performs runtime replacement.

DELETE FROM `battle_pet_wild_pool`;

INSERT INTO `battle_pet_wild_pool`
    (`zone`, `species`, `entry`, `max`, `minLevel`, `maxLevel`)
VALUES
    (1, 378, 721, 4, 1, 2), -- Rabbit - Dun Morogh
    (1, 441, 48935, 112, 1, 2), -- Alpine Hare - Dun Morogh
    (1, 442, 48956, 16, 1, 2), -- Irradiated Roach - Dun Morogh
    (3, 398, 2110, 23, 13, 14), -- Black Rat - Badlands
    (3, 406, 15475, 55, 13, 14), -- Beetle - Badlands
    (3, 430, 32258, 54, 13, 14), -- Gold Beetle - Badlands
    (3, 431, 48686, 14, 13, 14), -- Rattlesnake - Badlands
    (3, 432, 49833, 24, 13, 14), -- Stripe-Tailed Scorpid - Badlands
    (3, 433, 49840, 22, 13, 14), -- Spiky Lizard - Badlands
    (4, 412, 14881, 6, 16, 17), -- Spider - Blasted Lands
    (4, 414, 15476, 12, 16, 17), -- Scorpid - Blasted Lands
    (4, 415, 9699, 12, 16, 17), -- Fire Beetle - Blasted Lands
    (4, 635, 3300, 6, 16, 17), -- Adder - Blasted Lands
    (8, 402, 50000, 6, 14, 15), -- Swamp Moth - Swamp of Sorrows
    (8, 403, 9600, 10, 14, 15), -- Parrot - Swamp of Sorrows
    (8, 412, 14881, 6, 14, 15), -- Spider - Swamp of Sorrows
    (8, 417, 4075, 3, 14, 15), -- Rat - Swamp of Sorrows
    (8, 418, 4953, 29, 14, 15), -- Water Snake - Swamp of Sorrows
    (8, 420, 1420, 14, 14, 15), -- Toad - Swamp of Sorrows
    (8, 648, 6653, 13, 14, 15), -- Huge Toad - Swamp of Sorrows
    (10, 378, 721, 11, 5, 7), -- Rabbit - Duskwood
    (10, 379, 1412, 11, 5, 7), -- Squirrel - Duskwood
    (10, 385, 6271, 21, 5, 7), -- Mouse - Duskwood
    (10, 396, 49690, 49, 5, 7), -- Dusk Spiderling - Duskwood
    (10, 397, 17467, 38, 5, 7), -- Skunk - Duskwood
    (10, 398, 2110, 43, 5, 7), -- Black Rat - Duskwood
    (10, 399, 49673, 28, 5, 7), -- Rat Snake - Duskwood
    (10, 419, 13321, 31, 5, 7), -- Small Frog - Duskwood
    (10, 424, 4076, 37, 5, 7), -- Roach - Duskwood
    (10, 646, 620, 5, 5, 7), -- Chicken - Duskwood
    (11, 378, 721, 2, 6, 7), -- Rabbit - Wetlands
    (11, 379, 1412, 7, 6, 7), -- Squirrel - Wetlands
    (11, 385, 6271, 7, 6, 7), -- Mouse - Wetlands
    (11, 398, 2110, 9, 6, 7), -- Black Rat - Wetlands
    (11, 418, 4953, 29, 6, 7), -- Water Snake - Wetlands
    (11, 420, 1420, 21, 6, 7), -- Toad - Wetlands
    (11, 633, 31890, 9, 6, 7), -- Mountain Skunk - Wetlands
    (12, 378, 721, 44, 1, 2), -- Rabbit - Elwynn Forest
    (12, 379, 1412, 23, 1, 2), -- Squirrel - Elwynn Forest
    (12, 412, 14881, 6, 1, 2), -- Spider - Elwynn Forest
    (12, 419, 13321, 112, 1, 2), -- Small Frog - Elwynn Forest
    (12, 447, 890, 2, 1, 2), -- Fawn - Elwynn Forest
    (12, 459, 6368, 6, 1, 2), -- Cat - Elwynn Forest
    (12, 646, 620, 12, 1, 2), -- Chicken - Elwynn Forest
    (12, 675, 49540, 22, 1, 2), -- Stormwind Rat - Elwynn Forest
    (14, 418, 4953, 27, 1, 2), -- Water Snake - Durotar
    (14, 420, 1420, 24, 1, 2), -- Toad - Durotar
    (14, 448, 5951, 38, 1, 2), -- Hare - Durotar
    (14, 466, 49837, 16, 1, 2), -- Spiny Lizard - Durotar
    (14, 467, 49743, 14, 1, 2), -- Dung Beetle - Durotar
    (14, 635, 3300, 33, 1, 2), -- Adder - Durotar
    (15, 385, 6271, 16, 12, 13), -- Mouse - Dustwallow Marsh
    (15, 387, 2914, 39, 12, 13), -- Snake - Dustwallow Marsh
    (15, 398, 2110, 7, 12, 13), -- Black Rat - Dustwallow Marsh
    (15, 412, 14881, 15, 12, 13), -- Spider - Dustwallow Marsh
    (15, 420, 1420, 37, 12, 13), -- Toad - Dustwallow Marsh
    (16, 378, 721, 50, 3, 6), -- Rabbit - Azshara
    (16, 379, 1412, 39, 3, 6), -- Squirrel - Azshara
    (16, 388, 6827, 139, 3, 6), -- Shore Crab - Azshara
    (16, 397, 17467, 15, 3, 6), -- Skunk - Azshara
    (16, 412, 14881, 5, 3, 6), -- Spider - Azshara
    (16, 417, 4075, 7, 3, 6), -- Rat - Azshara
    (16, 424, 4076, 6, 3, 6), -- Roach - Azshara
    (16, 469, 49861, 2, 3, 6), -- Twilight Beetle - Azshara
    (16, 470, 48692, 2, 3, 6), -- Twilight Spider - Azshara
    (16, 471, 49773, 5, 3, 6), -- Robo-Chick - Azshara
    (16, 472, 49774, 5, 3, 6), -- Rabid Nut Varmint 5000 - Azshara
    (17, 386, 2620, 35, 3, 4), -- Prairie Dog - Northern Barrens
    (17, 419, 13321, 13, 3, 4), -- Small Frog - Northern Barrens
    (17, 631, 49725, 6, 3, 4), -- Emerald Boa - Northern Barrens
    (17, 635, 3300, 30, 3, 4), -- Adder - Northern Barrens
    (28, 378, 721, 5, 10, 11), -- Rabbit - Western Plaguelands
    (28, 379, 1412, 4, 10, 11), -- Squirrel - Western Plaguelands
    (28, 398, 2110, 28, 10, 11), -- Black Rat - Western Plaguelands
    (38, 379, 1412, 61, 3, 6), -- Squirrel - Loch Modan
    (38, 387, 2914, 9, 3, 6), -- Snake - Loch Modan
    (38, 417, 4075, 47, 3, 6), -- Rat - Loch Modan
    (38, 419, 13321, 27, 3, 6), -- Small Frog - Loch Modan
    (40, 378, 721, 2, 3, 4), -- Rabbit - Westfall
    (40, 379, 1412, 1, 3, 4), -- Squirrel - Westfall
    (40, 385, 6271, 16, 3, 4), -- Mouse - Westfall
    (40, 386, 2620, 14, 3, 4), -- Prairie Dog - Westfall
    (40, 387, 2914, 23, 3, 4), -- Snake - Westfall
    (40, 419, 13321, 8, 3, 4), -- Small Frog - Westfall
    (40, 646, 620, 15, 3, 4), -- Chicken - Westfall
    (44, 378, 721, 4, 4, 6), -- Rabbit - Redridge Mountains
    (44, 391, 49996, 50, 4, 6), -- Mountain Cottontail - Redridge Mountains
    (44, 392, 49995, 8, 4, 6), -- Redridge Rat - Redridge Mountains
    (44, 424, 4076, 15, 4, 6), -- Roach - Redridge Mountains
    (44, 646, 620, 2, 4, 6), -- Chicken - Redridge Mountains
    (46, 414, 15476, 25, 15, 16), -- Scorpid - Burning Steppes
    (46, 415, 9699, 12, 15, 16), -- Fire Beetle - Burning Steppes
    (46, 423, 9700, 15, 15, 16), -- Lava Crab - Burning Steppes
    (46, 425, 49568, 21, 15, 16), -- Ash Viper - Burning Steppes
    (47, 412, 14881, 5, 11, 12), -- Spider - The Hinterlands
    (47, 417, 4075, 11, 11, 12), -- Rat - The Hinterlands
    (47, 448, 5951, 26, 11, 12), -- Hare - The Hinterlands
    (47, 449, 22480, 29, 11, 12), -- Brown Marmot - The Hinterlands
    (47, 450, 16030, 18, 11, 12), -- Maggot - The Hinterlands
    (51, 415, 9699, 13, 13, 14), -- Fire Beetle - Searing Gorge
    (65, 536, 28440, 1, 22, 23), -- Tundra Penguin - Dragonblight
    (65, 641, 29328, 49, 22, 23), -- Arctic Hare - Dragonblight
    (66, 641, 29328, 16, 22, 23), -- Arctic Hare - Zul'Drak
    (66, 648, 6653, 4, 22, 23), -- Huge Toad - Zul'Drak
    (85, 378, 721, 6, 1, 2), -- Rabbit - Tirisfal Glades
    (85, 412, 14881, 12, 1, 2), -- Spider - Tirisfal Glades
    (85, 417, 4075, 94, 1, 2), -- Rat - Tirisfal Glades
    (85, 626, 20725, 23, 1, 2), -- Bat - Tirisfal Glades
    (85, 646, 620, 3, 1, 2), -- Chicken - Tirisfal Glades
    (130, 378, 721, 9, 3, 6), -- Rabbit - Silverpine Forest
    (130, 379, 1412, 5, 3, 6), -- Squirrel - Silverpine Forest
    (130, 387, 2914, 15, 3, 6), -- Snake - Silverpine Forest
    (130, 417, 4075, 6, 3, 6), -- Rat - Silverpine Forest
    (130, 420, 1420, 39, 3, 6), -- Toad - Silverpine Forest
    (130, 627, 10779, 3, 3, 6), -- Infected Squirrel - Silverpine Forest
    (139, 398, 2110, 44, 12, 13), -- Black Rat - Eastern Plaguelands
    (139, 412, 14881, 43, 12, 13), -- Spider - Eastern Plaguelands
    (139, 414, 15476, 35, 12, 13), -- Scorpid - Eastern Plaguelands
    (139, 626, 20725, 52, 12, 13), -- Bat - Eastern Plaguelands
    (139, 627, 10779, 19, 12, 13), -- Infected Squirrel - Eastern Plaguelands
    (141, 412, 14881, 17, 1, 2), -- Spider - Teldrassil
    (141, 419, 13321, 15, 1, 2), -- Small Frog - Teldrassil
    (141, 447, 890, 28, 1, 2), -- Fawn - Teldrassil
    (141, 452, 49778, 35, 1, 2), -- Red-Tailed Chipmunk - Teldrassil
    (141, 478, 49842, 4, 1, 2), -- Forest Moth - Teldrassil
    (141, 479, 49728, 27, 1, 2), -- Elfin Rabbit - Teldrassil
    (141, 507, 62242, 6, 1, 2), -- Crested Owl - Teldrassil
    (148, 378, 721, 35, 3, 6), -- Rabbit - Darkshore
    (148, 379, 1412, 40, 3, 6), -- Squirrel - Darkshore
    (148, 417, 4075, 46, 3, 6), -- Rat - Darkshore
    (148, 493, 48182, 50, 3, 6), -- Shimmershell Snail - Darkshore
    (210, 393, 26525, 59, 22, 23), -- Cockroach - Icecrown
    (215, 378, 721, 23, 1, 2), -- Rabbit - Mulgore
    (215, 385, 6271, 20, 1, 2), -- Mouse - Mulgore
    (215, 386, 2620, 33, 1, 2), -- Prairie Dog - Mulgore
    (267, 378, 721, 26, 6, 7), -- Rabbit - Hillsbrad Foothills
    (267, 379, 1412, 1, 6, 7), -- Squirrel - Hillsbrad Foothills
    (267, 412, 14881, 33, 6, 7), -- Spider - Hillsbrad Foothills
    (267, 417, 4075, 42, 6, 7), -- Rat - Hillsbrad Foothills
    (267, 420, 1420, 9, 6, 7), -- Toad - Hillsbrad Foothills
    (267, 450, 16030, 26, 6, 7), -- Maggot - Hillsbrad Foothills
    (267, 452, 49778, 30, 6, 7), -- Red-Tailed Chipmunk - Hillsbrad Foothills
    (267, 640, 26614, 6, 6, 7), -- Snowshoe Hare - Hillsbrad Foothills
    (267, 646, 620, 4, 6, 7), -- Chicken - Hillsbrad Foothills
    (267, 648, 6653, 12, 6, 7), -- Huge Toad - Hillsbrad Foothills
    (331, 379, 1412, 70, 4, 6), -- Squirrel - Ashenvale
    (331, 417, 4075, 45, 4, 6), -- Rat - Ashenvale
    (331, 420, 1420, 27, 4, 6), -- Toad - Ashenvale
    (331, 424, 4076, 51, 4, 6), -- Roach - Ashenvale
    (331, 450, 16030, 46, 4, 6), -- Maggot - Ashenvale
    (331, 478, 49842, 79, 4, 6), -- Forest Moth - Ashenvale
    (331, 495, 36591, 19, 4, 6), -- Frog - Ashenvale
    (361, 420, 1420, 31, 14, 15), -- Toad - Felwood
    (361, 497, 10017, 83, 14, 15), -- Tainted Cockroach - Felwood
    (361, 498, 50003, 26, 14, 15), -- Tainted Moth - Felwood
    (361, 499, 10016, 210, 14, 15), -- Tainted Rat - Felwood
    (394, 447, 890, 1, 21, 22), -- Fawn - Grizzly Hills
    (394, 633, 31890, 23, 21, 22), -- Mountain Skunk - Grizzly Hills
    (394, 647, 31889, 32, 21, 22), -- Grizzly Squirrel - Grizzly Hills
    (400, 414, 15476, 46, 13, 14), -- Scorpid - Thousand Needles
    (400, 424, 4076, 46, 13, 14), -- Roach - Thousand Needles
    (405, 417, 4075, 21, 7, 9), -- Rat - Desolace
    (405, 419, 13321, 23, 7, 9), -- Small Frog - Desolace
    (405, 424, 4076, 32, 7, 9), -- Roach - Desolace
    (405, 452, 49778, 9, 7, 9), -- Red-Tailed Chipmunk - Desolace
    (405, 478, 49842, 53, 7, 9), -- Forest Moth - Desolace
    (405, 479, 49728, 9, 7, 9), -- Elfin Rabbit - Desolace
    (405, 480, 49859, 23, 7, 9), -- Topaz Shale Hatchling - Desolace
    (405, 482, 50481, 34, 7, 9), -- Rock Viper - Desolace
    (405, 483, 49839, 34, 7, 9), -- Horny Toad - Desolace
    (405, 484, 49835, 32, 7, 9), -- Desert Spider - Desolace
    (405, 838, 49858, 25, 7, 9), -- Amethyst Shale Hatchling - Desolace
    (406, 378, 721, 25, 5, 7), -- Rabbit - Stonetalon Mountains
    (406, 412, 14881, 31, 5, 7), -- Spider - Stonetalon Mountains
    (406, 417, 4075, 6, 5, 7), -- Rat - Stonetalon Mountains
    (406, 424, 4076, 5, 5, 7), -- Roach - Stonetalon Mountains
    (406, 472, 49774, 5, 5, 7), -- Rabid Nut Varmint 5000 - Stonetalon Mountains
    (406, 487, 49779, 39, 5, 7), -- Alpine Chipmunk - Stonetalon Mountains
    (406, 488, 49724, 19, 5, 7), -- Coral Snake - Stonetalon Mountains
    (406, 633, 31890, 16, 5, 7), -- Mountain Skunk - Stonetalon Mountains
    (440, 430, 32258, 9, 13, 14), -- Gold Beetle - Tanaris
    (440, 431, 48686, 28, 13, 14), -- Rattlesnake - Tanaris
    (440, 432, 49833, 29, 13, 14), -- Stripe-Tailed Scorpid - Tanaris
    (440, 484, 49835, 17, 13, 14), -- Desert Spider - Tanaris
    (440, 491, 44710, 7, 13, 14), -- Sand Kitten - Tanaris
    (440, 492, 49836, 38, 13, 14), -- Stinkbug - Tanaris
    (440, 560, 44880, 21, 13, 14), -- Sea Gull - Tanaris
    (490, 403, 9600, 55, 15, 16), -- Parrot - Un'Goro Crater
    (490, 502, 49734, 30, 15, 16), -- Spotted Bell Frog - Un'Goro Crater
    (493, 378, 721, 18, 15, 16), -- Rabbit - Moonglade
    (493, 379, 1412, 49, 15, 16), -- Squirrel - Moonglade
    (493, 478, 49842, 16, 15, 16), -- Forest Moth - Moonglade
    (493, 503, 49844, 14, 15, 16), -- Silky Moth - Moonglade
    (495, 378, 721, 21, 20, 22), -- Rabbit - Howling Fjord
    (495, 388, 6827, 13, 20, 22), -- Shore Crab - Howling Fjord
    (495, 397, 17467, 22, 20, 22), -- Skunk - Howling Fjord
    (495, 412, 14881, 7, 20, 22), -- Spider - Howling Fjord
    (495, 417, 4075, 38, 20, 22), -- Rat - Howling Fjord
    (495, 420, 1420, 3, 20, 22), -- Toad - Howling Fjord
    (495, 424, 4076, 4, 20, 22), -- Roach - Howling Fjord
    (495, 450, 16030, 6, 20, 22), -- Maggot - Howling Fjord
    (495, 523, 24270, 8, 20, 22), -- Devouring Maggot - Howling Fjord
    (495, 525, 23801, 8, 20, 22), -- Turkey - Howling Fjord
    (495, 644, 24174, 26, 20, 22), -- Fjord Rat - Howling Fjord
    (495, 646, 620, 5, 20, 22), -- Chicken - Howling Fjord
    (616, 378, 721, 10, 22, 24), -- Rabbit - Mount Hyjal
    (616, 415, 9699, 79, 22, 23), -- Fire Beetle - Mount Hyjal
    (616, 469, 49861, 21, 22, 23), -- Twilight Beetle - Mount Hyjal
    (616, 479, 49728, 29, 22, 23), -- Elfin Rabbit - Mount Hyjal
    (616, 482, 50481, 16, 22, 23), -- Rock Viper - Mount Hyjal
    (616, 487, 49779, 39, 22, 23), -- Alpine Chipmunk - Mount Hyjal
    (616, 503, 49844, 54, 22, 23), -- Silky Moth - Mount Hyjal
    (616, 539, 50419, 9, 22, 23), -- Grotto Vole - Mount Hyjal
    (616, 540, 50485, 11, 22, 23), -- Carrion Rat - Mount Hyjal
    (616, 541, 49780, 68, 22, 24), -- Fire-Proof Roach - Mount Hyjal
    (616, 626, 20725, 15, 22, 23), -- Bat - Mount Hyjal
    (616, 632, 50478, 14, 22, 23), -- Ash Lizard - Mount Hyjal
    (616, 755, 49759, 17, 22, 23), -- Death's Head Cockroach - Mount Hyjal
    (618, 412, 14881, 11, 17, 18), -- Spider - Winterspring
    (618, 441, 48935, 113, 17, 18), -- Alpine Hare - Winterspring
    (618, 471, 49773, 3, 17, 18), -- Robo-Chick - Winterspring
    (618, 472, 49774, 5, 17, 18), -- Rabid Nut Varmint 5000 - Winterspring
    (618, 487, 49779, 117, 17, 18), -- Alpine Chipmunk - Winterspring
    (618, 634, 32261, 34, 17, 18), -- Crystal Spider - Winterspring
    (1377, 406, 15475, 128, 16, 17), -- Beetle - Silithus
    (1377, 414, 15476, 45, 16, 17), -- Scorpid - Silithus
    (1377, 433, 49840, 31, 16, 17), -- Spiky Lizard - Silithus
    (1377, 482, 50481, 8, 16, 17), -- Rock Viper - Silithus
    (1377, 484, 49835, 16, 16, 17), -- Desert Spider - Silithus
    (1377, 511, 49727, 21, 16, 17), -- Sidewinder - Silithus
    (1377, 512, 49746, 2, 16, 17), -- Scarab Hatchling - Silithus
    (1497, 424, 4076, 17, 1, 2), -- Roach - Undercity
    (1497, 454, 49965, 14, 1, 2), -- Undercity Rat - Undercity
    (1497, 626, 20725, 2, 1, 2), -- Bat - Undercity
    (1519, 378, 721, 20, 1, 1), -- Rabbit - Stormwind City
    (1519, 379, 1412, 17, 1, 1), -- Squirrel - Stormwind City
    (1519, 417, 4075, 13, 1, 1), -- Rat - Stormwind City
    (1519, 675, 49540, 36, 1, 1), -- Stormwind Rat - Stormwind City
    (1537, 404, 48972, 11, 1, 3), -- Long-tailed Mole - Ironforge
    (1637, 418, 4953, 7, 1, 1), -- Water Snake - Orgrimmar
    (1637, 420, 1420, 24, 1, 1), -- Toad - Orgrimmar
    (1637, 466, 49837, 38, 1, 1), -- Spiny Lizard - Orgrimmar
    (1637, 467, 49743, 41, 1, 1), -- Dung Beetle - Orgrimmar
    (1637, 471, 49773, 2, 1, 1), -- Robo-Chick - Orgrimmar
    (1657, 419, 13321, 30, 1, 3), -- Small Frog - Darnassus
    (1657, 452, 49778, 10, 1, 3), -- Red-Tailed Chipmunk - Darnassus
    (1657, 478, 49842, 17, 1, 3), -- Forest Moth - Darnassus
    (1657, 479, 49728, 10, 1, 3), -- Elfin Rabbit - Darnassus
    (3430, 378, 721, 6, 1, 2), -- Rabbit - Eversong Woods
    (3430, 387, 2914, 7, 1, 2), -- Snake - Eversong Woods
    (3430, 419, 13321, 5, 1, 2), -- Small Frog - Eversong Woods
    (3430, 420, 1420, 4, 1, 2), -- Toad - Eversong Woods
    (3433, 387, 2914, 2, 3, 6), -- Snake - Ghostlands
    (3433, 412, 14881, 6, 3, 6), -- Spider - Ghostlands
    (3433, 417, 4075, 11, 3, 6), -- Rat - Ghostlands
    (3433, 419, 13321, 4, 3, 6), -- Small Frog - Ghostlands
    (3433, 420, 1420, 4, 3, 6), -- Toad - Ghostlands
    (3433, 450, 16030, 12, 3, 6), -- Maggot - Ghostlands
    (3433, 461, 16068, 2, 3, 6), -- Larva - Ghostlands
    (3483, 414, 15476, 11, 17, 18), -- Scorpid - Hellfire Peninsula
    (3483, 635, 3300, 13, 17, 18), -- Adder - Hellfire Peninsula
    (3487, 378, 721, 1, 1, 3), -- Rabbit - Silvermoon City
    (3487, 459, 6368, 3, 1, 3), -- Cat - Silvermoon City
    (3519, 397, 17467, 4, 18, 19), -- Skunk - Terokkar Forest
    (3519, 417, 4075, 23, 18, 19), -- Rat - Terokkar Forest
    (3521, 387, 2914, 108, 18, 19), -- Snake - Zangarmarsh
    (3521, 419, 13321, 137, 18, 19), -- Small Frog - Zangarmarsh
    (3522, 378, 721, 4, 18, 20), -- Rabbit - Blade's Edge Mountains
    (3522, 449, 22480, 1, 18, 20), -- Brown Marmot - Blade's Edge Mountains
    (3522, 637, 22306, 15, 18, 20), -- Skittering Cavern Crawler - Blade's Edge Mountains
    (3523, 385, 6271, 4, 20, 21), -- Mouse - Netherstorm
    (3523, 459, 6368, 3, 20, 21), -- Cat - Netherstorm
    (3524, 378, 721, 8, 1, 2), -- Rabbit - Azuremyst Isle
    (3524, 379, 1412, 8, 1, 2), -- Squirrel - Azuremyst Isle
    (3524, 397, 17467, 6, 1, 2), -- Skunk - Azuremyst Isle
    (3525, 397, 17467, 5, 3, 6), -- Skunk - Bloodmyst Isle
    (3525, 417, 4075, 9, 3, 6), -- Rat - Bloodmyst Isle
    (3525, 627, 10779, 6, 3, 6), -- Infected Squirrel - Bloodmyst Isle
    (3537, 388, 6827, 25, 20, 22), -- Shore Crab - Borean Tundra
    (3537, 536, 28440, 15, 20, 22), -- Tundra Penguin - Borean Tundra
    (3537, 639, 31685, 46, 20, 22), -- Borean Marmot - Borean Tundra
    (3537, 641, 29328, 19, 20, 22), -- Arctic Hare - Borean Tundra
    (3703, 417, 4075, 4, 18, 19), -- Rat - Shattrath City
    (3711, 379, 1412, 30, 21, 22), -- Squirrel - Sholazar Basin
    (3711, 649, 3835, 17, 21, 22), -- Biletoad - Sholazar Basin
    (4709, 386, 2620, 21, 9, 10), -- Prairie Dog - Southern Barrens
    (4709, 419, 13321, 21, 9, 10), -- Small Frog - Southern Barrens
    (4709, 631, 49725, 6, 9, 10), -- Emerald Boa - Southern Barrens
    (4709, 635, 3300, 25, 9, 10), -- Adder - Southern Barrens
    (4922, 398, 2110, 3, 23, 24), -- Black Rat - Twilight Highlands
    (4922, 414, 15476, 3, 23, 24), -- Scorpid - Twilight Highlands
    (4922, 418, 4953, 1, 23, 24), -- Water Snake - Twilight Highlands
    (4922, 431, 48686, 3, 23, 24), -- Rattlesnake - Twilight Highlands
    (4922, 470, 48692, 3, 23, 24), -- Twilight Spider - Twilight Highlands
    (4922, 548, 48915, 9, 23, 24), -- Wildhammer Gryphon Hatchling - Twilight Highlands
    (4922, 549, 48683, 4, 23, 24), -- Yellow-Bellied Marmot - Twilight Highlands
    (4922, 550, 48689, 3, 23, 24), -- Highlands Mouse - Twilight Highlands
    (4922, 645, 48706, 10, 23, 24), -- Highlands Turkey - Twilight Highlands
    (4922, 647, 31889, 1, 23, 24), -- Grizzly Squirrel - Twilight Highlands
    (4922, 648, 6653, 2, 23, 24), -- Huge Toad - Twilight Highlands
    (4922, 823, 48688, 2, 23, 24), -- Highlands Skunk - Twilight Highlands
    (5034, 467, 49743, 162, 23, 24), -- Dung Beetle - Uldum
    (5034, 484, 49835, 152, 23, 24), -- Desert Spider - Uldum
    (5034, 511, 49727, 120, 23, 24), -- Sidewinder - Uldum
    (5034, 542, 50491, 197, 23, 24), -- Mac Frog - Uldum
    (5034, 543, 50490, 131, 23, 24), -- Locust - Uldum
    (5034, 544, 50496, 186, 23, 24), -- Oasis Moth - Uldum
    (5034, 545, 49832, 119, 23, 24), -- Leopard Scorpid - Uldum
    (5034, 631, 49725, 101, 23, 24), -- Emerald Boa - Uldum
    (5034, 851, 49732, 112, 23, 24), -- Horned Lizard - Uldum
    (5785, 380, 59357, 9, 23, 25), -- Bucktooth Flapper - The Jade Forest
    (5785, 564, 59666, 45, 23, 25), -- Emerald Turtle - The Jade Forest
    (5785, 565, 59668, 10, 23, 25), -- Jungle Darter - The Jade Forest
    (5785, 566, 59085, 5, 23, 25), -- Mirror Strider - The Jade Forest
    (5785, 567, 58698, 17, 23, 25), -- Temple Snake - The Jade Forest
    (5785, 569, 58696, 1, 23, 25), -- Garden Frog - The Jade Forest
    (5785, 570, 63003, 18, 23, 25), -- Masked Tanuki - The Jade Forest
    (5785, 571, 59312, 70, 23, 25), -- Grove Viper - The Jade Forest
    (5785, 572, 58675, 24, 23, 25), -- Spirebound Crab - The Jade Forest
    (5785, 573, 58230, 8, 23, 25), -- Sandy Petrel - The Jade Forest
    (5785, 699, 63715, 15, 23, 25), -- Jumping Spider - The Jade Forest
    (5785, 702, 63919, 12, 23, 25), -- Leopard Tree Frog - The Jade Forest
    (5785, 703, 63716, 2, 23, 25), -- Masked Tanuki Pup - The Jade Forest
    (5785, 753, 56829, 10, 23, 25), -- Garden Moth - The Jade Forest
    (5785, 754, 59770, 10, 23, 25), -- Shrine Fly - The Jade Forest
    (5805, 677, 64246, 4, 23, 25), -- Shy Bandicoon - Valley of the Four Winds
    (5805, 706, 63062, 36, 23, 25), -- Bandicoon - Valley of the Four Winds
    (5805, 707, 63064, 4, 23, 25), -- Bandicoon Kit - Valley of the Four Winds
    (5805, 708, 63094, 27, 23, 25), -- Malayan Quillrat - Valley of the Four Winds
    (5805, 709, 63095, 9, 23, 25), -- Malayan Quillrat Pup - Valley of the Four Winds
    (5805, 710, 63096, 25, 23, 25), -- Marsh Fiddler - Valley of the Four Winds
    (5805, 711, 63057, 9, 23, 25), -- Sifang Otter - Valley of the Four Winds
    (5805, 713, 63060, 23, 23, 25), -- Softshell Snapling - Valley of the Four Winds
    (5840, 749, 63841, 1, 24, 25), -- Golden Civet - Vale of Eternal Blossoms
    (5840, 750, 63842, 1, 24, 25), -- Golden Civet Kitten - Vale of Eternal Blossoms
    (5840, 752, 63849, 1, 24, 25), -- Yellow-Bellied Bullfrog - Vale of Eternal Blossoms
    (5841, 679, 64248, 11, 23, 25), -- Summit Kid - Kun-Lai Summit
    (5841, 724, 63550, 3, 23, 25), -- Alpine Foxling - Kun-Lai Summit
    (5841, 725, 63551, 3, 23, 25), -- Alpine Foxling Kit - Kun-Lai Summit
    (5841, 726, 63547, 22, 23, 25), -- Plains Monitor - Kun-Lai Summit
    (5841, 727, 59702, 15, 23, 25), -- Prairie Mouse - Kun-Lai Summit
    (5841, 728, 63585, 1, 23, 25), -- Szechuan Chicken - Kun-Lai Summit
    (5841, 729, 63557, 9, 23, 25), -- Tolai Hare - Kun-Lai Summit
    (5841, 730, 63558, 7, 23, 25), -- Tolai Hare Pup - Kun-Lai Summit
    (5841, 731, 63555, 9, 23, 25), -- Zooey Snake - Kun-Lai Summit
    (5841, 747, 63850, 1, 23, 25), -- Effervescent Glowfly - Kun-Lai Summit
    (5842, 680, 63953, 17, 24, 25), -- Kuitan Mongoose - Townlong Steppes
    (5842, 732, 63980, 20, 24, 25), -- Amber Moth - Townlong Steppes
    (5842, 733, 63549, 27, 24, 25), -- Grassland Hopper - Townlong Steppes
    (5842, 737, 65190, 8, 24, 25), -- Mongoose - Townlong Steppes
    (5842, 739, 63954, 12, 24, 25), -- Mongoose Pup - Townlong Steppes
    (5842, 740, 63957, 4, 24, 25), -- Yakrat - Townlong Steppes
    (5842, 741, 64804, 1, 24, 25), -- Silent Hedgehog - Townlong Steppes
    (5842, 742, 64242, 1, 24, 25), -- Clouded Hedgehog - Townlong Steppes
    (6006, 706, 63062, 1, 23, 24), -- Bandicoon - The Veiled Stair
    (6006, 708, 63094, 1, 23, 24), -- Malayan Quillrat - The Veiled Stair
    (6134, 560, 44880, 5, 23, 25), -- Sea Gull - Krasarang Wilds
    (6134, 678, 63304, 42, 23, 25), -- Jungle Grub - Krasarang Wilds
    (6134, 708, 63094, 2, 23, 25), -- Malayan Quillrat - Krasarang Wilds
    (6134, 716, 63288, 8, 23, 25), -- Amethyst Spiderling - Krasarang Wilds
    (6134, 717, 63291, 14, 23, 25), -- Savory Beetle - Krasarang Wilds
    (6134, 718, 63289, 45, 23, 25), -- Luyu Moth - Krasarang Wilds
    (6134, 722, 63361, 12, 23, 25), -- Mei Li Sparkler - Krasarang Wilds
    (6134, 723, 63293, 21, 23, 25), -- Spiny Terrapin - Krasarang Wilds
    (6138, 732, 63980, 28, 24, 25), -- Amber Moth - Dread Wastes
    (6138, 741, 64804, 10, 24, 25), -- Silent Hedgehog - Dread Wastes
    (6138, 742, 64242, 15, 24, 25), -- Clouded Hedgehog - Dread Wastes
    (6138, 743, 64352, 5, 24, 25), -- Rapana Whelk - Dread Wastes
    (6138, 744, 64238, 14, 24, 25), -- Resilient Roach - Dread Wastes
    (6138, 745, 63548, 6, 24, 25), -- Crunchy Scorpion - Dread Wastes
    (6138, 746, 63329, 5, 24, 25), -- Emperor Crab - Dread Wastes
    (6507, 1175, 69648, 10, 25, 25), -- Thundertail Flapper - Isle of Thunder
    (6507, 1179, 69794, 4, 25, 25), -- Electrified Razortooth - Isle of Thunder
    (6507, 1181, 69818, 10, 25, 25), -- Elder Python - Isle of Thunder
    (6507, 1182, 69819, 6, 25, 25), -- Swamp Croaker - Isle of Thunder
    (6757, 417, 61366, 2, 25, 25), -- Rat - Timeless Isle
    (6757, 1324, 73542, 10, 25, 25), -- Ashwing Moth - Timeless Isle
    (6757, 1325, 73543, 11, 25, 25); -- Flamering Moth - Timeless Isle
