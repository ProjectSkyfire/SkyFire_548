-- Ulduar: Cache of Storms loot. 194312 / 194313 are the 10 player normal / hard mode caches, 194315 / 194314 the 25 player ones.
-- The 25 player tables were swapped (hard mode loot sat on the normal cache), the normal one had Enchanted Bridle
-- instead of the Fragment of Val'anyr, and the hard mode only items used loot mode 2, which a chest never rolls.
DELETE FROM `gameobject_loot_template` WHERE `entry` IN (26955, 26956, 27073, 27074);
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
-- 10 player, normal (194312)
(27073, 1, 100, 1, 0, -34371, 1),
(27073, 2, 100, 1, 0, -12032, 1),
(27073, 47241, 100, 1, 0, 1, 1),
-- 10 player, hard mode (194313)
(27074, 1, 100, 1, 0, -34371, 1),
(27074, 2, 100, 1, 0, -12032, 1),
(27074, 45784, -100, 1, 0, 1, 1),
(27074, 45928, 0, 1, 1, 1, 1),
(27074, 45929, 0, 1, 1, 1, 1),
(27074, 45930, 0, 1, 1, 1, 1),
(27074, 45931, 0, 1, 1, 1, 1),
(27074, 45933, 0, 1, 1, 1, 1),
(27074, 47241, 100, 1, 0, 1, 1),
-- 25 player, hard mode (194314)
(26955, 1, 100, 1, 0, -34372, 1),
(26955, 2, 100, 1, 0, -12033, 2),
(26955, 3, 10, 1, 0, -34154, 1),
(26955, 45038, 18, 1, 0, 1, 1),
(26955, 45087, 10, 1, 0, 1, 1),
(26955, 45470, 0, 1, 1, 1, 1),
(26955, 45471, 0, 1, 1, 1, 1),
(26955, 45472, 0, 1, 1, 1, 1),
(26955, 45473, 0, 1, 1, 1, 1),
(26955, 45474, 0, 1, 1, 1, 1),
(26955, 45570, 0, 1, 1, 1, 1),
(26955, 45817, -100, 1, 0, 1, 1),
(26955, 47241, 100, 1, 0, 1, 1),
-- 25 player, normal (194315)
(26956, 1, 100, 1, 0, -34372, 1),
(26956, 2, 100, 1, 0, -12033, 2),
(26956, 3, 10, 1, 0, -34154, 1),
(26956, 45038, 8, 1, 0, 1, 1),
(26956, 45087, 10, 1, 0, 1, 1),
(26956, 47241, 100, 1, 0, 1, 1);
