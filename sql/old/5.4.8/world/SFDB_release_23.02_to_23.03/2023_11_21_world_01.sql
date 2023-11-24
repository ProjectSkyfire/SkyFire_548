UPDATE `quest_template` SET `OfferRewardText`='It\'s good to see you aggain, $n.$B$BThis is the pool of the skunk, as I\'m sure you noticed.$B$BOver the many ages of Shen-zin Su, animals have died in some of these magical pools. Through their deaths, their spirits were infused into the waters, and anyone touching those waters will take their form.$B$BThere are several cursed pools here, some more dangerous than others.' WHERE `Id`=29521;
UPDATE `quest_template` SET `RequestItemsText`='Telling you of my strength means nothing. I must show you.' WHERE `Id`=29662;
UPDATE `quest_template` SET `RequestItemsText`='Have you done as I asked of you?' WHERE `Id`=29661;
UPDATE `quest_template` SET `RequestItemsText`='Only through discipline can we find harmony. Only through harmony can we reach our true potential as warriors.' WHERE `Id`=29663;
UPDATE `quest_template` SET `OfferRewardText`='Even now, your skills increase. Your footing becomes more sure. Your blows strike with more force.$B$BDiscipline and practice are the keys to reaching our full potential.' WHERE `Id`=29663;
UPDATE `quest_template` SET `OfferRewardText`='Good, You\'re a quick study.' WHERE `Id`=29661;
UPDATE `quest_template` SET `OfferRewardText`='Good.$B$BNo number of reeds have ever withstood my might.$B$BStep back, and let me dispel your doubts.' WHERE `Id`=29662;

UPDATE `quest_template` SET `PrevQuestId`=29662 WHERE `Id`=29676;

UPDATE `quest_template` SET `OfferRewardText`='Visitors are rare up here, but you are welcome. It is nice to meet you, young $c.' WHERE `Id`=29676;
UPDATE `quest_template` SET `RequestItemsText`='Balance is a delicate thing. It leans strongly on the wisdom of the elders. The lessons they preach are not always apparent to younger mind, so you must trust that their knowledge of the past will help us preserve the present.' WHERE  `Id`=29666;
UPDATE `quest_template` SET `RequestItemsText`='The decision to not return to the pools was a hard one. I don\'t think Shu ever truly understood. He does not know what it is to get older and weaker.' WHERE `Id`=29677;
UPDATE `quest_template` SET `OfferRewardText`='Shu\'s desires are not so diffrent from our own. He does not want to be alone. Unwanted, unloved. He longs to be needed, and to share his life with others.$B$BKnowing that, earning his trust is not terribly difficuult. He just wants company.' WHERE `Id`=29677;
UPDATE `quest_template` SET `OfferRewardText`='The water pincers are strange creatures. They seem to be drawn to still water.' WHERE `Id`=29666;
UPDATE `quest_template` SET `OfferRewardText`='It looks like you have his attention, $n. I think he likes you.' WHERE `Id`=29678;
UPDATE `quest_template` SET `OfferRewardText`='I have to admit, that looked pretty fun from my angle.$B$BAnd you seem to have made a new friend.' WHERE `Id`=29679;
UPDATE `quest_template` SET `OfferRewardText`='$n! I wondered when I would see you again!$B$BWell, the plus side is that finding Wugou, the spirit of earth, was easy enough. Waking him... not so much. I think he\'s going to wake quite sore from all the pushing and hitting I\'ve done trying to rouse him.' WHERE `Id`=29680;UPDATE `sf_world`.`quest_template` SET `RequestItemsText`='We need to kill them down to the very last one! We can leave that one alive I suppose, but if there\'s more than one, our troubles are going to start all over again!' WHERE  `Id`=29769;
UPDATE `quest_template` SET `RequestItemsText`='We need to kill them down to the very last one! We can leave that one alive I suppose, but if there\'s more than one, our troubles are going to start all over again!' WHERE `Id`=29769;
UPDATE `quest_template` SET `RequestItemsText`='Ever since Wugou, the spirit of earth, fell asleep, the dirt is drying out and it\'s getting harder and harder to get new seeds to sprout. We need to hold on to any food we can, or the island will have hard times ahead.' WHERE `Id`=29770;

UPDATE `quest_template` SET `PrevQuestId`=29521 WHERE `Id`=29663;
UPDATE `quest_template` SET `PrevQuestId`=29679, `NextQuestIdChain`=29769 WHERE `Id`=29680;
UPDATE `quest_template` SET `PrevQuestId`=29676, `NextQuestIdChain`=29678 WHERE `Id`=29666;
UPDATE `quest_template` SET `PrevQuestId`=29676, `NextQuestIdChain`=0 WHERE `Id`=29677;

