DELETE FROM `creature_queststarter` WHERE `quest` IN
(3106, 3117, 3118, 3119, 26841, 31168, 3120);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(2077, 3106),
(2077, 3117),
(2077, 3118),
(2077, 3119),
(2077, 26841),
(2077, 31168),
(2077, 3120);

-- Simple Sigil
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=3106;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=3106;
UPDATE `quest_template` SET `RequiredClasses`=1 WHERE `Id`=3106;
UPDATE `quest_template` SET `RequiredRaces`=8 WHERE `Id`=3106;
-- Learning New Techniques
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26945;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26945;
UPDATE `quest_template` SET `RequiredClasses`=1 WHERE `Id`=26945;
UPDATE `quest_template` SET `RequiredRaces`=8 WHERE `Id`=26945;
UPDATE `quest_template` SET `PrevQuestId`=3106 WHERE `Id`=26945;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Charge. Look for a Training Dummy and practice using Charge.' WHERE `Id`=26945;
UPDATE `quest_template` SET `Details`='As you overcome the challenges before you, you\'ll begin to learn new techniques.$B$BGo, get more experience, and learn a new skill. Then demonstrate it for me on one of the training dummies.' WHERE  `Id`=26945;
UPDATE `quest_template` SET `OfferRewardText`='You\'ve proven a capable student. When it is time to resume your training, return to me and I will show you other techniques.' WHERE `Id`=26945;

-- Etched Sigil
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=3117;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=3117;
UPDATE `quest_template` SET `RequiredClasses`=4 WHERE `Id`=3117;
UPDATE `quest_template` SET `RequiredRaces`=8 WHERE `Id`=3117;
-- A Woodsman's Training
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26947;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26947;
UPDATE `quest_template` SET `RequiredClasses`=4 WHERE `Id`=26947;
UPDATE `quest_template` SET `RequiredRaces`=8 WHERE `Id`=26947;
UPDATE `quest_template` SET `PrevQuestId`=3117 WHERE `Id`=26947;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Steady Shot. Look for a Training Dummy southeast of Aldrassil, and practice using Steady Shot 5 times.' WHERE `Id`=26947;
UPDATE `quest_template` SET `Details`='When most people think of the kaldorei, they conjure visions of druids and sentinels, but the way of the hunter is just as ancient and just as venerable.$B$BI\'ve been observing you, and you\'ve had quite a bit of experience handling that bow. As you gain more, you\'ll start to learn some new abilities.$B$BI want you to go out, get some more experience, and figure out how to do something new. Then go use it on a training dummy near the southeastern edge of Aldrassil.' WHERE `Id`=26947;
UPDATE `quest_template` SET `OfferRewardText`='You are a quick study in our ways.' WHERE `Id`=26947;

-- Encrypted Sigil
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=3118;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=3118;
UPDATE `quest_template` SET `RequiredClasses`=8 WHERE `Id`=3118;
UPDATE `quest_template` SET `RequiredRaces`=8 WHERE `Id`=3118;
-- A Rogue's Advantage
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26946;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26946;
UPDATE `quest_template` SET `RequiredClasses`=8 WHERE `Id`=26946;
UPDATE `quest_template` SET `RequiredRaces`=8 WHERE `Id`=26946;
UPDATE `quest_template` SET `PrevQuestId`=3118 WHERE `Id`=26946;
UPDATE `quest_template` SET `Objectives`='Reach Level 3 and Learn Eviscerate. Look for a Training Dummy southeast of Aldrassil and practice using Eviscerate 3 times.' WHERE `Id`=26946;
UPDATE `quest_template` SET `Details`='I\'m certain that by now, you\'ve mastered using Sinister Strike against your enemies. But, as you learn more and advance as a rogue, you\'ll start to figure out new techniques. One you should figure out soon is called Eviscerate. It can be a strong way to end a combination of attacks.$B$BI want you to get some more experience, figure it out, then try it on a training dummy near the southeastern edge of Aldrassil.' WHERE `Id`=26946;
UPDATE `quest_template` SET `OfferRewardText`='You\'ve learned quickly. If you are as sharp as you semm to be, you may prove to be one of my best students yet.' WHERE `Id`=26946;

-- Hallowed Sigil
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=3119;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=3119;
UPDATE `quest_template` SET `RequiredClasses`=16 WHERE `Id`=3119;
UPDATE `quest_template` SET `RequiredRaces`=8 WHERE `Id`=3119;
-- Learning the Word
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26949;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26949;
UPDATE `quest_template` SET `RequiredClasses`=16 WHERE `Id`=26949;
UPDATE `quest_template` SET `RequiredRaces`=8 WHERE `Id`=26949;
UPDATE `quest_template` SET `PrevQuestId`=3119 WHERE `Id`=26949;
UPDATE `quest_template` SET `Title`='Learning the Word' WHERE `Id`=26949;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Shadow Word: Pain, then use it 5 times on training dummies.' WHERE `Id`=26949;
UPDATE `quest_template` SET `Details`='As you venture out into the world and develop your skills, you will begin to learn new abilities that you never knew you had within you. I\'d like you to go out, get some more experience, and then show me the first new ability you learn.' WHERE `Id`=26949;

-- Forbidden Sigil
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26841;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26841;
UPDATE `quest_template` SET `RequiredClasses`=128 WHERE `Id`=26841;
UPDATE `quest_template` SET `RequiredRaces`=8 WHERE `Id`=26841;
-- Frost Nova
DELETE FROM `creature_queststarter` WHERE `quest`=26940;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(43006, 26940);

UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26940;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26940;
UPDATE `quest_template` SET `RequiredClasses`=128 WHERE  `Id`=26940;
UPDATE `quest_template` SET `RequiredRaces`=8 WHERE  `Id`=26940;
UPDATE `quest_template` SET `PrevQuestId`=26841 WHERE `Id`=26940;
UPDATE `quest_template` SET `Title`='Frost Nova' WHERE `Id`=26940;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Frost Nova. Look for a Training Dummy near, and practice using it twice.' WHERE  `Id`=26940;
UPDATE `quest_template` SET `Details`='We are the heirs to a legacy of arcane magic and your path will not be an easy one. Many still hold us responsible for the actions of our forebears, but our talents are needed.$B$BThough you must be cautious, it is important to develop your powers, as that will deepen your connection to the arcane and grant you access to new spells. Go - get more experience with your spells, and demonstrate what you learn for me.' WHERE  `Id`=26940;
UPDATE `quest_template` SET `OfferRewardText`='I knew you would master this lesson quickly. You are a credit to the highborn and to all our kin. Remember, as you grow in power, return to me and I will instruct you further.' WHERE `Id`=26940;

-- Calligraphed Sigil
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=31168;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=31168;
UPDATE `quest_template` SET `RequiredClasses`=512 WHERE `Id`=31168;
UPDATE `quest_template` SET `RequiredRaces`=8 WHERE `Id`=31168;
UPDATE `quest_template` SET `PrevQuestId`=0 WHERE `Id`=31168;
-- The Art of the Monk
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=31169;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=31169;
UPDATE `quest_template` SET `RequiredClasses`=512 WHERE `Id`=31169;
UPDATE `quest_template` SET `RequiredRaces`=8 WHERE `Id`=31169;
UPDATE `quest_template` SET `PrevQuestId`=31168 WHERE `Id`=31169;

-- Verdant Sigil
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=3120;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=3120;
UPDATE `quest_template` SET `RequiredClasses`=1024 WHERE `Id`=3120;
UPDATE `quest_template` SET `RequiredRaces`=8 WHERE `Id`=3120;
-- Moonfire
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26948;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26948;
UPDATE `quest_template` SET `RequiredClasses`=1024 WHERE `Id`=26948;
UPDATE `quest_template` SET `RequiredRaces`=8 WHERE `Id`=26948;
UPDATE `quest_template` SET `PrevQuestId`=3120 WHERE `Id`=26948;
UPDATE `quest_template` SET `Title`='Moonfire' WHERE `Id`=26948;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Moonfire. Use it on a training dummy in Aldrassil.' WHERE `Id`=26948;
UPDATE `quest_template` SET `Details`='Druids play an important role in our society, but all too often we are seen as passive dreamers in a world that is changing around us.$B$BThe truth is, though, we must actively practice and go out onto the world to develop our connection to nature. So go, get some more experience. You will begin to feel your connection deepen, learning new skills. Demonstrate the first you learn for me.' WHERE `Id`=26948;
UPDATE `quest_template` SET `OfferRewardText`='You have mastered the basics of the druidic ways quickly, $n. I have no doubt that your skills will often come in hand. Return to me when you need further training.' WHERE `Id`=26948;
