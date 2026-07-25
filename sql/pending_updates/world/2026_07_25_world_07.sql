-- Simple Letter
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=3100;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=3100;
-- Charging into Battle
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26913;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26913;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Charge, then practice using Charge on a Training Dummy south of the entrance to Northshire Abbey.' WHERE `Id`=26913;
UPDATE `quest_template` SET `Details`='Now that you\'ve had the chance to test your abilities in battle, it\'s time to explain one more reason why training is so important.$B$BAs you get more experience, you\'ll be able to do new techniques you never thought of. Go, get some experience, learn a new skill, and demonstrate it on one of the training dummies.' WHERE `Id`=26913;
UPDATE `quest_template` SET `OfferRewardText`='You\'re a quick learner, $n.' WHERE `Id`=26913;

-- Consecrated Letter
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=3101;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=3101;
UPDATE `quest_template` SET `NextQuestId`=26918 WHERE `Id`=3101;
-- The Power of the Light
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26918;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26918;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Seal of Command. Cast Seal of Command on yourself and then hit a Training Dummy south of the entrance to Northshire Abbey.' WHERE `Id`=26918;
UPDATE `quest_template` SET `Details`='As you continue in your journey as a $c, you\'ll become closer to the light, learning new abilities. Go, get more experience, and learn a new ability. Demonstrate it for me, and we will continue your training.' WHERE `Id`=26918;
UPDATE `quest_template` SET `OfferRewardText`='You\'re a quick learner, $n. Return to me as you grow stronger and I\'ll teach you more about using the Light to vanquish your enemies and strengthen your allies.' WHERE `Id`=26918;

-- Etched Letter
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26910;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26910;
UPDATE `quest_template` SET `NextQuestId`=26917 WHERE `Id`=26910;
-- The Hunter's Path
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26917;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26917;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Steady Shot, then find a Training Dummy south of the entrance to Northshire Abbey and practice using Steady Shot 5 times.' WHERE `Id`=26917;
UPDATE `quest_template` SET `Details`='I\'ve heard about your help in clearing out some of the orcs. You\'ve shown that you can handle yourself in combat.$B$BAs you get more experience in combat, you\'ll start learning how to do new things. I want you to go, get some more experience, then try the new abilities you learn on one of the training dummies south of the entrance to the abbey.' WHERE `Id`=26917;
UPDATE `quest_template` SET `OfferRewardText`='I knew you would master Steady Shot quickly. I\'m looking forward to showing you other techniques as you get more experienced in the ways of the Hunter.' WHERE `Id`=26917;

-- Encrypted Letter
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=3102;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=3102;
UPDATE `quest_template` SET `NextQuestId`=26915 WHERE `Id`=3102;
-- The Deepest Cut
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26915;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26915;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Eviscerate, then find a Training Dummy south of the entrance to Northshire Abbey and practice Eviscerate 3 times.' WHERE `Id`=26915;
UPDATE `quest_template` SET `Details`='By now, you know the basics of combat as $c, but I want to teach you a new skill. As rogues, we are renowned for our ability to execute chains of combat techniques, often saving the most powerful strikes for the end.$B$BAs you get more experience, you\'ll learn new things - and this is one of the ones you\'ll learn first. Once you have, I want you to go try it on one of the training dummies south of the entrance to the abbey.' WHERE `Id`=26915;
UPDATE `quest_template` SET `OfferRewardText`='You\'ve caught on quickly, $n, and a quick mind will serve you well.' WHERE `Id`=26915;

-- Hallowed Letter
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=3103;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=3103;
UPDATE `quest_template` SET `NextQuestId`=26919 WHERE `Id`=3103;
-- Learning the Word
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26919;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26919;
UPDATE `quest_template` SET `OfferRewardText`='You\'re a quick learner, $n.' WHERE `Id`=26919;

-- Glyphic Letter
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=3104;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=3104;
UPDATE `quest_template` SET `NextQuestId`=26916 WHERE `Id`=3104;
-- Mastering the Arcane
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26916;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26916;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Frost Nova, then practice casting it 2 times on a Training Dummy south of the entrance to Northshire Abbey.' WHERE `Id`=26916;
UPDATE `quest_template` SET `Details`='As a mage, you will learn a variety of spells as your understanding grows. All you need to do is get some more experience, and the secrets of the arcane will reveal themselves to you. You\'ll see. Go develop your powers, and show me what you learn.' WHERE `Id`=26916;

-- Tainted Letter
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=3105;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=3105;
UPDATE `quest_template` SET `NextQuestId`=26916 WHERE `Id`=3105;
-- Corruption
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26914;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26914;
UPDATE `quest_template` SET `Title`='Corruption' WHERE `Id`=26914;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn the Corruption ability, then practice casting Corruption 5 times on a Training Dummy south of the entrance to Northshire Abbey.' WHERE `Id`=26914;
UPDATE `quest_template` SET `Details`='We must take care in how we speak around the abbey and its environs, but don\'t let that deter you from the path of power.$B$BAs you become more powerful, you will begin to hear the fel voices, granting you their power in the dark arts. Go, get more experience and learn a new spell. Once you have, practice it on the training dummies.' WHERE `Id`=26914;

-- Calligraphed Letter
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=31141;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=31141;
UPDATE `quest_template` SET `NextQuestId`=31142 WHERE `Id`=31141;
-- Palm of the Tiger
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=31142;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=31142;
