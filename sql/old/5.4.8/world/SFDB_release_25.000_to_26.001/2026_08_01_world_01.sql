-- Hero's Call: Redridge Mountains!
UPDATE `quest_template` SET `OfferRewardText`='Heard Westfall was in some deep doody.$B$B<Parker doesn\'t even turn to face you.>$B$BWell, we got our own set of problems here. See them gnolls out there? Looks like they\'re actin\' up again. I don\'t like it. Don\'t like it one bit.' WHERE `Id`=28563;

-- Still Assessing the Threat
UPDATE `quest_template` SET `RequestItemsText`='Did you find the gnoll plans?' WHERE `Id`=26503;
UPDATE `quest_template` SET `OfferRewardText`='I knew it! Looks like Yowler is behind this uprising - which is incredible, because we keep killing gnolls named Yowler. I don\'t know how many sons the original Yowler had, but it\'s got to be close to a hundred.$B$BWell, looks like we got ourselves another Yowler to kill.$B$BMagistrate Solomon must be notified.' WHERE `Id`=26503;

-- Parker's Report
UPDATE `quest_template` SET `RequestItemsText`='Where did I put my monocle?! Conacher is that you?' WHERE `Id`=26505;
UPDATE `quest_template` SET `OfferRewardText`='<Magistrate Solomon puts on his monocle.>$B$BOh, you\'re not Conacher. What have you got there?$B$B<Magistrate Solomon takes Parker\'s report from you.>$B$BOh for pity\'s sake. Yowler? AGAIN?' WHERE `Id`=26505;

-- We Must Prepare!
UPDATE `quest_template` SET `RequestItemsText`='Have you recovered the gnomecorder?' WHERE `Id`=26510;
UPDATE `quest_template` SET `OfferRewardText`='Excellent! Let me make a few adjustments here and we should be good to go.' WHERE `Id`=26510;

-- Nida's Necklace
UPDATE `quest_template` SET `RequestItemsText`='Hi. I miss my necklace. My daddy got it for me. Daddy says that there are monsters in the lake. Did you beat up any monsters?' WHERE `Id`=26508;
UPDATE `quest_template` SET `OfferRewardText`='Thank you for finding my necklace mister $c... you are very kind!  My kitty thanks you too - isn\'t that right Effsee?' WHERE `Id`=26508;
