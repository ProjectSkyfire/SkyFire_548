-- The Lost Pilot
UPDATE `quest_template` SET `Details`='My friend and fellow Siege Engine pilot, Mori Hildelve, is lost in the hills. We were looking for a rare ore that\'s needed for a high-grade blastpowder, and during our search he drove his engine up a steep hill and busted it!$B$BStill convinced that ore was in these mountains, Hildelve charged me to guard our engines while he continued on foot.$B$BIt\'s been days, and I\'ve heard some beastly growls north of Helm\'s Bed Lake, west of the tunnel. Mori\'s tough as nails, but I\'m worried.$B$BPlease, $n. Find him.' WHERE `Id`=26854;
UPDATE `quest_template` SET `OfferRewardText`='Here is the corpse of a dwarf -- mauled, frozen, and picked clean by mountain scavengers. Clutched in its hand is a book containing the roughly scribbled notes of Mori Hildelve. And around its frame, still in superior condition, is a Brigadier\'s Vest.$B$BThis grisly scene must have been where the Siege Engine pilot met his end.' WHERE `Id`=26854;

-- A Pilot's Revenge
UPDATE `quest_template` SET `RequestItemsText`='Hello, $n. Do you have word of my friend Hildelve?' WHERE `Id`=26855;
UPDATE `quest_template` SET `OfferRewardText`='This is grim news, for Hildelve was a good friend. And I wish I could have been there with you when you found the beast, Mangeclaw!$B$BThank you, $n. Your deed will be remembered by the pilots of the Ironforge Siege Brigade.' WHERE `Id`=26855;

-- South Gate Status Report
UPDATE `quest_template` SET `OfferRewardText`='The South Gate status report? Excellent. I\'ve been awaiting news from them for a while.$B$BWith the increasing number of troggs in the hills on all sides, I fear the outposts will be the first to feel any aggressive action.' WHERE `Id`=13635;
