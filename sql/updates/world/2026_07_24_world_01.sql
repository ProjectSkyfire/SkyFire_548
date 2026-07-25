-- Zanzil Mindslave
UPDATE `creature_template` SET `lootid`=43223 WHERE `entry`=43223;
DELETE FROM `creature_loot_template` WHERE `entry`=43223;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(43223, 4016, -80, 1, 0, 1, 1), -- Zanzil's Mixture
(43223, 4306, 40, 1, 0, 1, 2), -- Silk Cloth
(43223, 2592, 10, 1, 0, 1, 1), -- Wool Cloth
(43223, 4607, 3, 1, 0, 1, 1), -- Delicious Cave Mold
(43223, 929, 3, 1, 0, 1, 1), -- Healing Potion
(43223, 4606, 2, 1, 0, 1, 1), -- Spongy Morel
(43223, 1708, 2, 1, 0, 1, 1), -- Sweet Nectar
(43223, 1205, 1, 1, 0, 1, 1), -- Melon Juice
(43223, 1710, 1, 1, 0, 1, 1), -- Greater Healing Potion
(43223, 3827, 1, 1, 0, 1, 1), -- Mana Potion
(43223, 24036, 20, 1, 1, -24036, 1), -- Trash References
(43223, 24116, 5, 1, 1, -24116, 1), -- Uncommon References
(43223, 24161, 1, 1, 1, -24161, 1), -- Rare/Epic References
(43223, 24736, 1, 1, 1, -24736, 1), -- Patterns
(43223, 44011, 10, 1, 0, -44011, 1); -- Stranglethorn Pages


-- Maywiki
UPDATE `quest_template` SET `OfferRewardText`='<The troll looks into the package.>\r\n\r\nYou have my thanks. You may tell your young dwarven friend that I am not sad. I am waiting. For you.\r\n\r\nI have identified you as the most promising dwarf here in this strange town. I have a task for you, if you would be willing to take it.' WHERE `Id`=26808;
