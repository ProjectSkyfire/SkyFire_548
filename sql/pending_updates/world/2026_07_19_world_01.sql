UPDATE `quest_template` SET `RequestItemsText`='Between birds and sprites we are never going to meet our quota to the Stormstout Brewery.' WHERE `Id`=30028;

UPDATE `quest_template` SET `OfferRewardText`='Why thank you very much stranger. This has not been a good day, but you certainly have helped us make it better.\r\n\r\nPlease, take this, you\'ve earned it.' WHERE `Id`=30028;

DELETE FROM `gameobject_loot_template` WHERE `entry`=41153;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `mincountOrRef`, `maxcount`) VALUES
(41153, 78918, -100, 2, 4);

-- Lotus Tea Alliance Quest
UPDATE `quest_template` SET `RequestItemsText`='I have never seen the mogu this numerous before. They have always been a nuisance but never this bad.' WHERE `Id`=30351;

UPDATE `quest_template` SET `OfferRewardText`='That\'s it! You pick, I punch!' WHERE `Id`=30351;

-- Herbal Remedies Horde Quest
UPDATE `quest_template` SET `RequestItemsText`='I\'ve been punched, stabbed, thrown off of a cliff, hit in the face by someone\'s mace, hit in the face by own mace, and fell off the elevator at Thunder Bluff on more than one occasion.\r\n\r\nNone of that pain sounds even half as bad as what Leza is going through. I hope this tea helps...' WHERE `Id`=30130;

UPDATE `quest_template` SET `OfferRewardText`='Thank you hero. Kor is a brave warrior but he lacks compassion at times. Life must come before death, or there will be none of us left when the fighting is done.' WHERE  `Id`=30130;
