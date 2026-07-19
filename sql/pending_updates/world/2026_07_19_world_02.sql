-- Darkhide
UPDATE `creature_template` SET `lootid`=58435 WHERE `entry`=58435;
-- Darkhides Head
DELETE FROM `creature_loot_template` WHERE `entry`=58435;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(58435, 79058, -100);

UPDATE `quest_template` SET `RequestItemsText`='Puttin\' the "big" into "Big Game Hunter."' WHERE `Id`=30184;
UPDATE `quest_template` SET `OfferRewardText`='Now that\'s a proper trophy right there.' WHERE `Id`=30184;
