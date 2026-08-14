-- The Trogg Threat
UPDATE `quest_template` SET `RequestItemsText`='Do you have eight trogg stone teeth to show me? If not, there is still work to be done, $n.' WHERE `Id`=26145;
UPDATE `quest_template` SET `OfferRewardText`='I am quite impressed, $n. You have done the Dwarven race a great favor by defending our lands. King Magni Bronzebeard himself would be proud!' WHERE `Id`=26145;

-- In Defense of the King's Lands
UPDATE `quest_template` SET `Details`='We need to protect the Loch, $n! With so many of the King\'s soldiers fighting valiantly on remote battlefields, we\'ve become overwhelmed on the home front. Troggs are tunneling up from every crevice! The trogg infestation poses the largest threat to Ironforge.$B$BWe need you, brave adventurer, to venture forth and lay waste to the trogg threat. Slay twelve Stonesplinter troggs and venture back.' WHERE `Id`=26146;
UPDATE `quest_template` SET `RequestItemsText`='Loch Modan is under siege, $n! We need every able-bodied member of the Alliance to aid our cause. Have you killed those troggs yet?' WHERE `Id`=26146;
UPDATE `quest_template` SET `OfferRewardText`='Well done, $n! You demonstrated uncanny courage on the battlefield. Our war against the Troggs will be won with efforts such as yours. You have served King Magni well.' WHERE `Id`=26146;

-- A Decisive Strike
UPDATE `quest_template` SET `RequestItemsText`='Your mission is not complete until Grawmug has been brought to death. Khaz Modan is counting on you, $n.' WHERE `Id`=26148;
UPDATE `quest_template` SET `OfferRewardText`='Most excellent, $n! You have brought hope to Loch Modan. With Grawmug dead, we stand a better chance of seeing the Troggs eliminated from the land.' WHERE `Id`=26148;

-- Bigger and Uglier
UPDATE `sf_world`.`quest_template` SET `OfferRewardText`='I knew the second I laid eyes on you, $n, that you would make Ironforge proud. You have served our people well. I salute you, $n.' WHERE `Id`=26147;

-- Onward to Thelsamar
UPDATE `quest_template` SET `RequestItemsText`='What can I do for you?' WHERE `Id`=26176;
UPDATE `quest_template` SET `OfferRewardText`='Glad to hear you\'ve been clearing out the troggs. Let\'s have a look at this report, and...$B$B...Is that a beer stain?' WHERE `Id`=26176;
