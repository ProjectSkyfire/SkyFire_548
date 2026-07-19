DELETE FROM `gameobject_questender` WHERE `id`=211316;
INSERT INTO `gameobject_questender` (`id`, `quest`) VALUES
(211316, 30628);

UPDATE `quest_template` SET `OfferRewardText`='Peering inside, you find...' WHERE `Id`=30628;
