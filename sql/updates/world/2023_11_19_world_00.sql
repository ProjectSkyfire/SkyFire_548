UPDATE `quest_template` SET `OfferRewardText`='Well done, $n. My men can take it from here. Let\'s focus on getting the other survivors out now.' WHERE `Id`=27670;

UPDATE `quest_template` SET `NextQuestIdChain`=28167 WHERE `Id`=27670;

UPDATE `quest_template` SET `OfferRewardText`='Did Nevin send you ahead? That means he\'s getting ready to end the mission, but there are still survivors to be rescued. We have to help them!' WHERE `Id`=28167;

UPDATE `quest_template` SET `OfferRewardText`='I know Nevin will be pleased with all the survivors you managed to help. I still can\'t believe you managed to stay so strong down here in the radiation. You\'ve seen the shape the other survivors are in.$B$BLet\'s get you to the loading room for decontamination.' WHERE `Id`=27671;

UPDATE `quest_template` SET `NextQuestIdChain`=28169 WHERE `Id`=27671;

UPDATE `quest_template` SET `OfferRewardText`='It\'s good to meet you, $c. We rarely come across survivors as strong and capable as you are. I can help get you decontaminated and on your way out of here.' WHERE `Id`=28169;

UPDATE `quest_template` SET `NextQuestIdChain`=27635 WHERE `Id`=28169;

UPDATE `quest_template` SET `OfferRewardText`='There, now you\'re fit to head off to the surface and start your new life. The High Tinker will be delighted to hear of your arrival.' WHERE  `Id`=27635;

UPDATE `quest_template` SET `OfferRewardText`='It\'s good to see you again, $n. Without your help, we wouldn\'t have been able to get so many survivors out of Gnomeregan this time. Everyone is going to be thrilled to meet you and hear your story.' WHERE `Id`=27674;
