-- Zanzil Witch Doctor
DELETE FROM `creature_loot_template` WHERE `entry`=1490 AND `item`=58901;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(1490, 58901, 80); -- Zanzil's Formulation

-- Recipe for Disaster (Alliance version)
UPDATE `quest_template` SET `OfferRewardText`='This is fascinating! Just think of all the applications!$B$BThanks again, $n. Remember, if you ever need anything mixed for you... and I mean ANYTHING... you can always ask The Flask.' WHERE `Id`=26816;

-- Recipe for Disaster (Horde version)
UPDATE `quest_template` SET `RequestItemsText`='Did he use shimmerweed? I\'m guessing he used shimmerweed$B$BNo wait... amberseeds!  It\'s gotta be amberseeds.' WHERE `Id`=26535;
UPDATE `quest_template` SET `OfferRewardText`='This is fascinating! Just think of all the applications!$B$BJust so we\'re clear, I\'m not thinking about using this to make an army of slaves or anything like that.  No, no... I\'m much more interested in the ogre explosions!' WHERE `Id`=26535;
