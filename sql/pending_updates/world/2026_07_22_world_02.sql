-- Hero's Call: Westfall! -- Elwynn Forest version
UPDATE `sf_world`.`quest_template` SET `OfferRewardText`='Murder, rookie. That\'S what you\'re looking at on the ground in front of us.' WHERE  `Id`=26378;

-- Hero's Call: Westfall! -- Stormwind Hero board version
UPDATE `quest_template` SET `OfferRewardText`='Murder, rookie. That\'S what you\'re looking at on the ground in front of us.' WHERE `Id`=28562;

-- Wanted: Redridge Gnolls
UPDATE `quest_template` SET `OfferRewardText`='A job well done deserves a reward, wouldn\'t you say?\r\n\r\nDon\'t spend this all in one place, $c. Better to spread it around, if you catch my drift.' WHERE `Id`=26504;

-- Hero's Call: Southern Barrens! -- Stormwind Hero board version
UPDATE `quest_template` SET `OfferRewardText`='We\'re facing heavy resistance from the Horde, $c. Any reinforcements are welcome.' WHERE `Id`=28551;

-- Report to Aubrey
UPDATE `quest_template` SET `OfferRewardText`='Hm? What unit are you with, $c?\r\n\r\nNo matter, we need all hands. My fleet is in tatters and they\'ve broke into the hold.\r\nYou armed? That\'s good enough for me.' WHERE `Id`=24921;
