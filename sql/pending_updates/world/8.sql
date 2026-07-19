-- Violet Citron
DELETE FROM `gameobject_loot_template` WHERE `entry`=42068;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(42068, 82342, -100);

UPDATE `quest_template` SET `RequestItemsText`='Mixed properly, the pollen from these flowers can eat away at the poison without doing any lasting damage to the body otherwise.' WHERE `Id`=30891;

UPDATE `quest_template` SET `OfferRewardText`='Marvelous. And in good condition too!\r\n\r\nThank you, kind $c.' WHERE `Id`=30891;
