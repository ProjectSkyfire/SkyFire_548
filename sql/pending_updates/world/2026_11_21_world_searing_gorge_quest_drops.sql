-- Searing Gorge: three quest items had no drop row, so "Lunk's Task" (27956, Tempered Flywheel), "A Proper Antivenom"
-- (27958, Glassweb Venom) and "The Mysteries of the Fire-Gizzard" (28032, Fire-Gizzard) could not be finished.
-- Based on Legends of Azeroth 5.4.8.
DELETE FROM `creature_loot_template` WHERE (`entry`=5853 AND `item`=62805) OR (`entry`=5856 AND `item`=62809) OR (`entry`=9318 AND `item`=62914);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(5853, 62805, -83.3873, 1, 0, 1, 1),
(5856, 62809, -73.1818, 1, 0, 1, 1),
(9318, 62914, -75.7082, 1, 0, 1, 1);
