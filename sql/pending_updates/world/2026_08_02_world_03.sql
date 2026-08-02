-- Blackrock Overseer
UPDATE `creature_template` SET `lootid`=43185 WHERE `entry`=43185;

DELETE FROM `creature_loot_template` WHERE `entry`=43185;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(43185, 2589, 30, 1, 0, 1, 3), -- Linen cloth
(43185, 2592, 30, 1, 0, 1, 2), -- Wool cloth
(43185, 2287, 5, 1, 0, 1, 1), -- Haunch of Meat
(43185, 1179, 5, 1, 0, 1, 1), -- Ice Cold Milk
(43185, 858, 3, 1, 0, 1, 1), -- Lesser Healing Potion
(43185, 2455, 3, 1, 0, 1, 1), -- Minor Mana Potion
(43185, 1210, 1, 1, 0, 1, 1), -- Shadowgem
(43185, 1206, 1, 1, 0, 1, 1), -- Moss Agate
(43185, 4294, 0.3, 1, 0, 1, 1), -- Pattern: Hillman's Belt
(43185, 5574, 0.2, 1, 0, 1, 1), -- White Leather Bag
(43185, 24076, 1, 1, 1, -24076, 1), -- ilvl 14-15 uncommon
(43185, 24077, 1, 1, 1, -24077, 1), -- ilvl 16-17 uncommon
(43185, 24078, 1, 1, 1, -24078, 1), -- ilvl 18-19 uncommon
(43185, 24070, 5, 1, 1, -24070, 1), -- Trash items
(43185, 58936, -80, 1, 0, 1, 1); -- Blackrock Orc Missive

-- They've Wised Up...
UPDATE `quest_template` SET `RequestItemsText`='What have you found out?' WHERE  `Id`=26544;
UPDATE `quest_template` SET `OfferRewardText`='<The gnomecorder buzzes and whirs.>$B$BJust put the missive in the little compartment and I\'ll read it. I am fluent in orcish.$B$B<You hear a distraught yell in the background.>$B$BIt looks like these orcs are here under the direct command of Yowler. That means Yowler must have the orc invasion plan.' WHERE `Id`=26544;

-- Yowler
DELETE FROM `creature_loot_template` WHERE `entry`=518 AND `item`=58937;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(518, 58937, -100, 1, 0, 1, 1); -- Blackrock Invasion Plans

-- Yowler Must Die!
UPDATE `quest_template` SET `RequestItemsText`='Do you have the Blackrock invasion plans?' WHERE `Id`=26545;
UPDATE `quest_template` SET `OfferRewardText`='<Magistrate Solomon takes the plans from you and begins reading.>$B$BShadowhide army!? Damn those orcs to hell! These invasion plans speak of a massive build up of orcish and gnoll forces in the east. Looks like Gath\'Ilzogg, the Blackrock general, is preparing to march his armies across Lakeshire and attack Stormwind directly!' WHERE `Id`=26545;
