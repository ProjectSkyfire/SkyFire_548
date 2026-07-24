-- Thrashtail Basilisk
DELETE FROM `creature_loot_template` WHERE `entry`=1550 AND `item`=58880;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(1550, 58880, -80); -- Sassy's Secret Ingredient

-- Ironjaw Basilisk
DELETE FROM `creature_loot_template` WHERE `entry`=1551 AND `item`=58880;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(1551, 58880, -80); -- Sassy's Secret Ingredient

-- There's Somebody Out There Who Wants It
UPDATE `quest_template` SET `RequestItemsText`='I hope you used the extraction procedure we discussed.' WHERE `Id`=26493;
UPDATE `quest_template` SET `OfferRewardText`='Great! These look... disgusting. But probably pretty valuable. Let\'s get to splitting these up. Some for me, some for Jasper, some for Chabal...' WHERE `Id`=26493;
