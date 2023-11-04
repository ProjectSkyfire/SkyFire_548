-- Hogger
UPDATE `creature_template` SET `lootid`=46254 WHERE `entry`=46254;
DELETE FROM `creature_loot_template` WHERE `entry`=46254;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `groupid`) VALUES
(46254, 2168, 31, 1), -- Corpse Rompers
(46254, 1934, 27, 1), -- Hogger's Trousers
(46254, 1959, 25, 1), -- Cold Iron Pick
(46254, 132569, 12, 1), -- Stolen Jailer's Greaves
(46254, 151074, 6, 1); -- Turnkey's Pauldrons

-- Randolph Moloch
UPDATE `creature_template` SET `lootid`=46383 WHERE `entry`=46383;
DELETE FROM `creature_loot_template` WHERE `entry`=46383;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `groupid`) VALUES
(46383, 63344, 30, 1), -- Standard Issue Prisoner Shoes
(46383, 63345, 26, 1), -- Noble's Robe
(46383, 63346, 25, 1), -- Wicked Dagger
(46383, 132570, 14, 1), -- Stolen Guards Chain Boots
(46383, 151077, 5, 1); -- Cast Iron Waistplate

-- Lord Overheat
UPDATE `creature_template` SET `lootid`=46264 WHERE `entry`=46264;
DELETE FROM `creature_loot_template` WHERE `entry`=46264;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `groupid`) VALUES
(46264, 62305, -100, 0), -- Lord Overheat's Fiery Core
(46264, 4676, 32, 1), -- Skeletal Gauntlets
(46264, 1929, 25, 1), -- Silk-Threaded Trousers
(46264, 5967, 23, 1), -- Girdle of Nobility
(46264, 151076, 5, 1), -- Fire-Hardened Shackles
(46264, 151075, 4, 1); -- Cinderstitch Tunic
