-- Freshly Fallen Petal
DELETE FROM `gameobject_loot_template` WHERE `entry`=40485;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(40485, 75219, -100);

UPDATE `quest_template` SET `RequestItemsText`='There is only a small window of time after each petal falls that it may be used to achieve the perfect hue.' WHERE `Id`=29881;
UPDATE `quest_template` SET `OfferRewardText`='Ah! You have a wonderful eye for color my friend. These will make a superb pigment.' WHERE `Id`=29881;
