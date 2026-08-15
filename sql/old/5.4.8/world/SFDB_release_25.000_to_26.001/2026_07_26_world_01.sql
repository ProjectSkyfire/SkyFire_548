-- Simple Rune
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=3106;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=3106;
-- Getting Battle-Ready
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=24531;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=24531;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Charge, then use Charge on one of the Training Dummies outside the entrance to Anvilmar.' WHERE `Id`=24531;
UPDATE `quest_template` SET `Details`='Lesson one. Aside from your sword and shield, what\'s the most important tool you have in your arsenal?$B$B<Thran awaits your response.>$B$BOh never mind, I\'ll answer it for you. It\'s experience. As you get more experience, you\'ll discover new techniques, which means you can be much more effective than you imagined. Go, get some experience, and learn something new. Then come back and demonstrate.' WHERE `Id`=24531;

-- Consecrated Rune
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=3107;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=3107;
-- The Power of the Light
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=24528;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=24528;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Seal of Command. Use Seal of Command on yourself, and then hit one of the Training Dummies at the entrance to Anvilmar.' WHERE `Id`=24528;
UPDATE `quest_template` SET `Details`='Are ye ready to start? Wonderful! We\'re glad to have ye.$B$BAs ye get stronger, it brings ye closer to the light, and ye learn new techniques. Go ahead and get some more experience, and show me what ye learn.' WHERE `Id`=24528;
UPDATE `quest_template` SET `OfferRewardText`='There, ya did it. The Light favors quick learners like you.' WHERE `Id`=24528;

-- Etched Rune
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=3108;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=3108;
UPDATE `quest_template` SET `PrevQuestId`=0 WHERE `Id`=3108;
-- Oh, A Hunter's Life For Me
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=24530;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=24530;
UPDATE `quest_template` SET `PrevQuestId`=3108 WHERE `Id`=24530;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Steady Shot, then use Steady Shot 5 times on the Training Dummies outside the entrance to Anvilmar.' WHERE `Id`=24530;
UPDATE `quest_template` SET `Details`='The life of a hunter is steeped in tradition, ye know. Trackin\' mighty beasts through rugged terrain. Drinkin\'. Travelin\' the land, yer gun at yer side. Drinkin\'. Survivin\' on wits and courage and cunning, followed by a good stiff drink.$B$BBut before we get to the good stuff, though, ye\'ll need to learn how to do the beginner stuff. Go and get some more experience, and ye\'ll figure out how to do more things. Then ye\' drink.' WHERE `Id`=24530;
UPDATE `quest_template` SET `OfferRewardText`='Now that ye\'ve got a new $c trick under yer belt, go out there an\' track some beasts!' WHERE `Id`=24530;

-- Encrypted Rune
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=3109;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=3109;
-- Evisceratin' the Enemy
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=24532;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=24532;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Eviscerate, then use Eviscerate 3 times on the Training Dummies near the entrance to Anvilmar.' WHERE `Id`=24532;
UPDATE `quest_template` SET `Details`='When you get to be more experienced, you\'ll start to find you can take advantage of new abilities. One of the things that you\'ll learn is the ability to use combat maneuvers to weaken an opponent, and then hit them with a devastating technique. Go and get some experience, figure out how to do it, and then try it out on some of the dummies here.' WHERE `Id`=24532;

-- Hallowed Rune
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=3110;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=3110;
-- Words of Power
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=24533;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=24533;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Shadow Word: Pain, then cast it 5 times on training dummies.' WHERE `Id`=24533;
UPDATE `quest_template` SET `Details`='Now, if ya DO want to become somethin\' special, ye\'ll need to talk to me. A priest is nothin\' without his spells. And how do you learn new spells? Ya go and get more experience. So go out, get stronger, and then come show me what ya learn.' WHERE `Id`=24533;

-- Empowered Rune
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=24494;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=24494;
-- Your Path Begins Here
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=24527;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=24527;
UPDATE `quest_template` SET `Objectives`='Reach Level 3 to learn Primal Strike, and then use Primal Strike 2 times on a Training Dummy near the entrance of Anvilmar.' WHERE `Id`=24527;
UPDATE `quest_template` SET `Details`='Excellent! Let us begin.$B$BAs you grow along your journey, you will unlocked the ancient secrets of shamans. Primal Strike is the first such skill. The primal spirits, when called upon, will strengthen your arm to deliver a mighty blow.$B$BHelpful, isn\'t it?' WHERE `Id`=24527;
UPDATE `quest_template` SET `OfferRewardText`='This has been but one of many footfalls on the long path of the $c. Come back to me after you\'ve had some experience in the field, and i will help guide you on your next step.' WHERE `Id`=24527;

-- Arcane Rune
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=24496;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=24496;
-- Filling Up the Spellbook
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=24526;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=24526;
UPDATE `quest_template` SET `Title`='Filling Up the Spellbook' WHERE `Id`=24526;
UPDATE `quest_template` SET `Details`='Excellent! Let\'s get started.$B$BAs you grow in power, your connection to the arcane arts will deepen, and you\'ll gain access to new spells and abilities. Go, get some more experience, and you\'ll see what I mean. Come back when you learn something new and demonstrate it on the training dummies here.' WHERE `Id`=24526;
UPDATE `quest_template` SET `OfferRewardText`='There we go! One spell down, many many more to go. Come back to me after you\'ve had some experience casting this spell, and I\'ll show you some more tricks.' WHERE `Id`=24526;

-- Tainted Rune
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=3115;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=3115;
UPDATE `quest_template` SET `PrevQuestId`=0 WHERE  `Id`=3115;
UPDATE `quest_template` SET `PrevQuestId`=3115 WHERE `Id`=26904;
-- Corruption
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26904;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26904;
UPDATE `quest_template` SET `Title`='Corruption' WHERE `Id`=26904;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Corruption, then cast it 5 times on a Training Dummy near the entrance to Anvilmar.' WHERE `Id`=26904;
UPDATE `quest_template` SET `Details`='Few people, especially among our allies, truly understand the nature of the power we wield. They shun us for pursuing knowledge that they are too weak to use properly.$B$BAs you become more powerful, you will begin to discover new spells, further magnifying your power. Go, get more experience, then return once you\'ve learned a new spell. We\'ll see if you have it in you to truly master the dark arts.' WHERE `Id`=26904;

-- Lo <Monk Trainer>
SET @CGUID := 100106;
DELETE FROM `creature` WHERE `guid`=@CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 63285, 0, -6105.8037, 369.40625, 395.62598, 1.09540);

UPDATE `creature_template` SET `gossip_menu_id`=13895 WHERE `entry`=63285;

-- Elegant Rune
DELETE FROM `creature_queststarter` WHERE `id`=37087 AND `quest`=31150;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(37087, 31150);

UPDATE `quest_template` SET `Level`=2 WHERE `Id`=31150;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=31150;
UPDATE `quest_template` SET `RequiredClasses`=512 WHERE `Id`=31150;
UPDATE `quest_template` SET `RequiredRaces`=4 WHERE `Id`=31150;
-- Kick, Punch, It's All in the Mind
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=31151;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=31151;
UPDATE `quest_template` SET `RequiredClasses`=512 WHERE `Id`=31151;
UPDATE `quest_template` SET `RequiredRaces`=4 WHERE `Id`=31151;
UPDATE `quest_template` SET `PrevQuestId`=31150 WHERE `Id`=31151;

