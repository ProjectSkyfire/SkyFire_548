-- Pandaren Start Buttons
DELETE FROM `playercreateinfo_action` WHERE `race`=24;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(24,  1,  0,  88163,   0), -- Pandaren - Warrior - Button 0: Auto Attack
(24,  1,  1,     78,   0), -- Pandaren - Warrior - Button 1: Heroic Strike
(24,  1,  8,  77273, 128), -- Pandaren - Warrior - Button 8: Small Rice Cake(Item)
(24,  1,  9, 107079,   0), -- Pandaren - Warrior - Button 9: Quaking Palm
(24,  3,  0,   3044,   0), -- Pandaren - Hunter - Button 0: Arcane Shot
(24,  3,  8,  77272, 128), -- Pandaren - Hunter - Button 8: Small Sugarcane Stalk(Item)
(24,  3,  9, 107079,   0), -- Pandaren - Hunter - Button 9: Quaking Palm
(24,  3,  10,     9,  48), -- Pandaren - Hunter - Button 10: Call Pet
(24,  3,  11,   982,   0), -- Pandaren - Hunter - Button 11: Revive Pet
(24,  4,  0,   1752,   0), -- Pandaren - Rogue - Button 0: Sinister Strike
(24,  4,  8,  77272, 128), -- Pandaren - Rogue - Button 8: Small Sugarcane Stalk(Item)
(24,  4,  9, 107079,   0), -- Pandaren - Rogue - Button 9: Quaking Palm
(24,  5,  0,    585,   0), -- Pandaren - Priest - Button 0: Smite
(24,  5,  8,  77264, 128), -- Pandaren - Priest - Button 8: Small Bamboo Shoot(Item)
(24,  5,  9, 107079,   0), -- Pandaren - Priest - Button 9: Quaking Palm
(24,  7,  0,   6603,   0), -- Pandaren - Shaman - Button 0: Auto Attack
(24,  7,  1,    403,   0), -- Pandaren - Shaman - Button 1: Lightning Bolt
(24,  7,  8,  77272, 128), -- Pandaren - Shaman - Button 8: Small Sugarcane Stalk(Item)
(24,  7,  9, 107079,   0), -- Pandaren - Shaman - Button 9: Quaking Palm
(24,  8,  0,  44614,   0), -- Pandaren - Mage - Button 0: Frostfire Bolt
(24,  8,  8,  77264, 128), -- Pandaren - Mage - Button 8: Small Bamboo Shoot(Item)
(24,  8,  9, 107079,   0); -- Pandaren - Mage - Button 9: Quaking Palm
-- Monk Start Buttons
DELETE FROM `playercreateinfo_action` WHERE `class`=10;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(24, 10, 72, 100780,   0), -- Pandaren - Monk - Button 72: Jab
(24, 10, 80,  77272, 128), -- Pandaren - Monk - Button 80: Small Sugarcane Stalk(Item)
(24, 10, 81, 107079,   0), -- Pandaren - Monk - Button 81: Quaking Palm
( 1, 10, 72, 100780,   0), -- Human - Monk - Button 72: Jab
( 1, 10, 81,  59752,   0), -- Human - Monk - Button 81: Every Man for Himself
( 3, 10, 72, 100780,   0), -- Dwarf - Monk - Button 72: Jab
( 3, 10, 81,  20594,   0), -- Dwarf - Monk - Button 81: Stoneform
( 4, 10, 72, 100780,   0), -- Night Elf - Monk - Button 72: Jab
( 4, 10, 81,  58984,   0), -- Night Elf - Monk - Button 81: Shadowmeld
( 7, 10, 72, 100780,   0), -- Gnome - Monk - Button 72: Jab
( 7, 10, 81,  20589,   0), -- Gnome - Monk - Button 81: Escape Artist
(11, 10, 72, 100780,   0), -- Draenei - Monk - Button 72: Jab
(11, 10, 81,  28880,   0), -- Draenei - Monk - Button 81: Gift of the Naaru
( 2, 10, 72, 100780,   0), -- Orc - Monk - Button 72: Jab
( 2, 10, 81,  33697,   0), -- Orc - Monk - Button 81: Blood Fury
( 5, 10, 72, 100780,   0), -- Undead - Monk - Button 72: Jab
( 5, 10, 81,   7744,   0), -- Undead - Monk - Button 81: Will of the Forsaken
( 5, 10, 82,  20577,   0), -- Undead - Monk - Button 82: Cannibalize
( 6, 10, 72, 100780,   0), -- Tauren - Monk - Button 72: Jab
( 6, 10, 81,  20549,   0), -- Tauren - Monk - Button 81: War Stomp
( 8, 10, 72, 100780,   0), -- Troll - Monk - Button 72: Jab
( 8, 10, 81,  26297,   0), -- Troll - Monk - Button 81: Berserking
(10, 10, 72, 100780,   0), -- Blood Elf - Monk - Button 72: Jab
(10, 10, 81, 129597,   0); -- Blood Elf - Monk - Button 81: Arcane Torrent
