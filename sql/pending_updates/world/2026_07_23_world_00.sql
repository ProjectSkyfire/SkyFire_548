-- Akiris by the Bundle (Alliance version)
UPDATE `quest_template` SET `OfferRewardText`='Hooray! It\'s been days and days since we last had Akiris reeds!' WHERE  `Id`=26819;

-- Akiris by the Bundle (Horde version)
UPDATE `quest_template` SET `RequestItemsText`='Let\'s see... we\'re going to need some shotguns for our nature hikes, rowboats for the oil righ day trip, and...\r\n\r\nOh, you\'re back already?' WHERE  `Id`=26487;
UPDATE `quest_template` SET `OfferRewardText`='Hey hey! Not bad. not bad at all! You may have a future in resource acquisition. Or hotel managment.' WHERE `Id`=26487;

-- If They're Just Going to Leave Them Lying Around... (Horde version)
UPDATE `quest_template` SET `RequestItemsText`='Let\'s see \'em!' WHERE `Id`=26450;
UPDATE `quest_template` SET `OfferRewardText`='Nice work, $n. Now I\'ve just got to decide what to do with all of these...' WHERE `Id`=26450;

-- If They're Just Going to Leave Them Lying Around... (Alliance version)
UPDATE `quest_template` SET `OfferRewardText`='These statues are beautiful...\r\n\r\nWow, I had no idea the naga were so talented. Thanks. $n!' WHERE `Id`=26820;

-- Mixmaster Jasper
UPDATE `quest_template` SET `RequestItemsText`='Hey, is that the stuff? From the magazine?' WHERE `Id`=26494;
UPDATE `quest_template` SET `OfferRewardText`='I\'ll file these away for now. Maybe we can experiment later.\r\n\r\n<Jasper looks over at you.>\r\n\r\nYeah, you\'ll do.' WHERE `Id`=26494;

-- Zanzil's Secret (Horde version)
UPDATE `quest_template` SET `RequestItemsText`='Excellent... you\'re not a zombie yet.\r\nThat\'s good.\r\n\r\nOh, umm... nothing. It\'s just that... that was a potential risk. Did i forget to warn you?' WHERE `Id`=26533;
UPDATE `quest_template` SET `OfferRewardText`='<Jasper carefully analyzes the strange compound.>\r\n\r\nThis mixture is... I don\'t understand it.\r\nPerhaps we need to run some clinical trials.' WHERE `Id`=26533;

-- Zanzil's Secret (Alliance version)
UPDATE `quest_template` SET `RequestItemsText`='Excellent... you\'re not a zombie yet.\r\nThat\'s good.\r\n\r\nOh, umm... nothing.  It\'s just that... that was a potential risk.  Did I forget to warn you?' WHERE `Id`=26815;
UPDATE `quest_template` SET `OfferRewardText`='<Dask carefully analyzes the strange compound.>\r\n\r\nThis mixture is... PERFECT! Let\'s see what it does to your potentially-titan artifact here!' WHERE `Id`=26815;

-- The Holy Water of Clarity
UPDATE `quest_template` SET `RequestItemsText`='<Flem\'s ears perk up as you approach.>\r\n\r\nThe holy Water of Clarity? Is it here!?' WHERE `Id`=26433;
UPDATE `quest_template` SET `OfferRewardText`='<The goblin downs the water in one gulp.>\r\n\r\nAhh... do you know how hard it is to get fresh water in Stranglethorn? It\'s no wonder that the nagas guard their shrine so ferociously!\r\n\r\nOh, right... your artifact, and clarity.\r\nClarity...\r\n\r\nI\'ve got it!' WHERE `Id`=26433;

-- Diffractory Chromascope
UPDATE `quest_template` SET `RequestItemsText`='<Flem\'s eyes sparkle.>\r\n\r\nAre those gems I see?' WHERE  `Id`=26592;
UPDATE `quest_template` SET `OfferRewardText`='<Flem weighs the gems in his hands and nods in approval.>\r\n\r\nYes, these will do. Now to perform the analysis...\r\n\r\n<The goblin clacks the gems together for a while, then looks up at you.>\r\n\r\nOh, you\'re still here? You don\'t think I\'m a fraud or something, do you?' WHERE `Id`=26592;

-- I'm A Huge Liar and a Fraud
UPDATE `quest_template` SET `RequestItemsText`='Yes, $r? I\'m quite busy' WHERE  `Id`=26435;
UPDATE `sf_world`.`quest_template` SET `OfferRewardText`='Fascinating!\r\n\r\nI\'ll need some time to study this. Why don\'t you have a look around Booty Bay and come back later?' WHERE `Id`=26435;
