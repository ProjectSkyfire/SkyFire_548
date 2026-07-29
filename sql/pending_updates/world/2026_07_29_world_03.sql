-- A Triumph of Gnomish Ingenuity
UPDATE `quest_template` SET `OfferRewardText`='So the high tinker himself has heard of my little project? Splendid! I\'ve almost finished my latest prototype, but maybe you can help me chase down those last few parts.' WHERE `Id`=26566;

-- Scrounging for Parts
DELETE FROM `gameobject_loot_template` WHERE `entry`=30020;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(30020, 57764, -100, 1, 0, 1, 1);

UPDATE `quest_template` SET `RequestItemsText`='Did you get those parts?' WHERE `Id`=26222;
UPDATE `quest_template` SET `OfferRewardText`='These are fantastic! Let\'s see what we can put together. Would you be interested in testing out the prototype once I\'m done?' WHERE `Id`=26222;
