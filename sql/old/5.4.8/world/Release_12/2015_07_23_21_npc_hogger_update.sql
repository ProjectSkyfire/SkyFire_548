-- Hogger npc id 448
UPDATE `creature_template` SET spell1='87337', spell2='87351', spell3='87352', spell4='87366', lootid='448', pickpocketloot='448' WHERE entry='448';
DELETE FROM `creature_loot_template` WHERE entry='448';
INSERT INTO `creature_loot_template` VALUES 
('448', '117', '6.72', '1', '0', '1', '1'),
('448', '159', '2.52', '1', '0', '1', '1'),
('448', '1495', '2.1', '1', '0', '1', '1'),
('448', '1506', '2.1', '1', '0', '1', '1'),
('448', '1512', '2.1', '1', '0', '1', '1'),
('448', '1730', '2.1', '1', '0', '1', '1'),
('448', '1734', '3.36', '1', '0', '1', '1'),
('448', '2214', '2.94', '1', '0', '1', '1'),
('448', '2589', '34.87', '1', '0', '1', '4');

DELETE FROM `pickpocketing_loot_template` WHERE entry='448';
REPLACE INTO `pickpocketing_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`) VALUES
('448', '858', '0.02', '1', '0'),
('448', '4541', '0.02', '1', '0');
