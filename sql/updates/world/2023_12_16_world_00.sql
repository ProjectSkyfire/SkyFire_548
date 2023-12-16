UPDATE `quest_template` SET `RequestItemsText`='Elder Shaopai has long been responsible for documenting the great wisdoms of our people and hanging them for all to see. What amazes me is his fortitude in the light of the great sacrilege these hozen are commiting. Many years of work destroyed, and yet he remains calm. $B$BI must admit, that is a quality that I do not possess.' WHERE `Id`=29778;
UPDATE `quest_template` SET `RequestItemsText`='I do not wish to disparage the hozen for their aspirations - each race must walk its own path. In this moment, however, they are bringing harm to our bodies and our culture. That is a line that cannot be crossed.' WHERE `Id`=29777;
UPDATE `quest_template` SET `RequestItemsText`='The strength of the body cannot exist without the strength of the mind.' WHERE `Id`=29783;
UPDATE `quest_template` SET `OfferRewardText`='I am very thankful, young one. I will soon begin the slow task of documenting all of our wisdoms again, so that the scrolls can be hung for the benefit of all to come.' WHERE `Id`=29777;
UPDATE `quest_template` SET `OfferRewardText`='Good.$B$BMy head, it is harder than the strongest stone.$B$BStep back, and let me dispel your doubts.' WHERE `Id`=29783;
UPDATE `quest_template` SET `OfferRewardText`='You\'re a good $Gman:woman;, $n. You have proven that again and again, and I am glad to stand beside you.' WHERE `Id`=29778;
UPDATE `quest_template` SET `RequestItemsText`='The time for kindness is done. We have monkeys to kill.' WHERE `Id`=29779;
UPDATE `quest_template` SET `RequestItemsText`='Following the Huojin discipline often means making the hard decisions. This is one of those moments.' WHERE `Id`=29780;
UPDATE `quest_template` SET `RequestItemsText`='I\'m somewhat impressed they hadn\'t set their village on fire with these things.' WHERE `Id`=29781;
UPDATE `quest_template` SET `OfferRewardText`='$n, I\'m thinking we should celebrate with these later. What do you think?' WHERE `Id`=29781;
UPDATE `quest_template` SET `OfferRewardText`='$n, I think you\'ve got a bit of Huojin in you. Stick with me and I\'ll bring you around, I think.' WHERE `Id`=29780;
UPDATE `quest_template` SET `OfferRewardText`='Invigorating! A good fight always makes me feel better!' WHERE `Id`=29779;
UPDATE `quest_template` SET `OfferRewardText`='GOOD! A true challenge. Let us put this pillar to the test!' WHERE `Id`=29782;
UPDATE `quest_template` SET `OfferRewardText`='I\'m impressed. Most students that are even capable of reaching this spot do so soaking wet.' WHERE `Id`=29784;
UPDATE `quest_template` SET `OfferRewardText`='Dafeng looks frightened as you approach, but settles a bit at the sound of your voice.' WHERE `Id`=29785;
UPDATE `quest_template` SET `OfferRewardText`='You have helped Dafeng, the spirit of air, find his courage! The final spirit has been restored.' WHERE `Id`=29786;
UPDATE `quest_template` SET `OfferRewardText`='I do not have the strength of body or will that I once did. I\'m glad there are noble $r like you to fight on for our people. You\'ve come far, $n.' WHERE `Id`=29787;
UPDATE `quest_template` SET `RequestItemsText`='It seems that everywhere I turn, the land is troubled. Your efforts have already helped to ease it\'s pain, but something greater awaits you before all of this suffering truly behind to subside.' WHERE `Id`=29788;
UPDATE `quest_template` SET `RequestItemsText`='Rest feels good. The burdens of life have become a great weight after so many years.' WHERE `Id`=29789;
UPDATE `quest_template` SET `OfferRewardText`='The spirits thank you, $c.' WHERE `Id`=29788;
UPDATE `quest_template` SET `OfferRewardText`='You come through, as you always have. You are truly one of my greatest students. I think you will be something the likes of which the world has never seen.' WHERE `Id`=29789;
UPDATE `quest_template` SET `OfferRewardText`='I know, $n... I know. Such is the way of things.' WHERE `Id`=29790;
UPDATE `quest_template` SET `OfferRewardText`='You have undergone a great honor, $c. Have you come back wiser for it?' WHERE `Id`=29791;

UPDATE `creature_template` SET `ScriptName`='boss_ook_ook' WHERE `entry`=56637;

DELETE FROM `creature_text` WHERE `entry`=56637;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(56637, 0, 0, 'Me gonna ook you in the dooker!', 14, 0, 100, 0, 0, 28796, 'Ook-Ook YELL_AGGRO'),
(56637, 1, 0, 'Ook! Oooook!!', 14, 0, 100, 0, 0, 28797, 'Ook-Ook YELL_DEATH'),
(56637, 2, 0, 'In the dooker!', 14, 0, 100, 0, 0, 28799, 'Ook-Ook YELL_KILL'),
(56637, 3, 0, 'Get Ooking party started!', 14, 0, 100, 0, 0, 28800, 'Ook-Ook YELL_SPELL1'),
(56637, 4, 0, 'Come on and get your Ook on!', 14, 0, 100, 0, 0, 28801, 'Ook-Ook YELL_SPELL2'),
(56637, 5, 0, 'We gonna Ook all night!', 14, 0, 100, 0, 0, 28802, 'Ook-Ook YELL_SPELL3');
