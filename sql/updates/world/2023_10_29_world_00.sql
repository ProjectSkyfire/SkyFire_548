UPDATE `creature_template` SET `lootid`=46254 WHERE `entry`=46254;

DELETE FROM `creature_loot_template` WHERE `entry`=46254;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `groupid`) VALUES
(46254, 2168, 31, 1), -- Corpse Rompers
(46254, 1934, 27, 1), -- Hogger's Trousers
(46254, 1959, 25, 1), -- Cold Iron Pick
(46254, 132569, 12, 1), -- Stolen Jailer's Greaves
(46254, 151074, 6, 1); -- Turnkey's Pauldrons
