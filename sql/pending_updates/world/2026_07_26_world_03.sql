-- Simple Note
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=3091;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=3091;
UPDATE `quest_template` SET `RequiredClasses`=1 WHERE `Id`=3091;
UPDATE `quest_template` SET `RequiredRaces`=32 WHERE `Id`=3091;
-- The First Lesson
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=27020;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=27020;
UPDATE `quest_template` SET `RequiredClasses`=1 WHERE `Id`=27020;
UPDATE `quest_template` SET `RequiredRaces`=32 WHERE `Id`=27020;
UPDATE `quest_template` SET `PrevQuestId`=3091 WHERE `Id`=27020;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Charge. Locate a Training Dummy in Camp Narache and practice using Charge.' WHERE `Id`=27020;
UPDATE `quest_template` SET `Details`='You have done well. As you continue along your path, you will grow, learning new techniques that you couldn\'t have imagined before. Your first lesson is the power of experience. Go, learn a new ability, and demonstrate it for me.' WHERE  `Id`=27020;
UPDATE `quest_template` SET `OfferRewardText`='You have done well, $n. Return to me when you are in need of further training.' WHERE `Id`=27020;

-- Consecrated Note
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=27015;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=27015;
UPDATE `quest_template` SET `RequiredClasses`=2 WHERE `Id`=27015;
UPDATE `quest_template` SET `RequiredRaces`=32 WHERE `Id`=27015;
-- The Way of the Sunwalkers
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=27023;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=27023;
UPDATE `quest_template` SET `RequiredClasses`=2 WHERE `Id`=27023;
UPDATE `quest_template` SET `RequiredRaces`=32 WHERE `Id`=27023;
UPDATE `quest_template` SET `PrevQuestId`=27015 WHERE `Id`=27023;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Seal of Command. Locate a Training Dummy in Camp Narache, cast Seal of Command on yourself and hit the dummy.' WHERE `Id`=27023;
UPDATE `quest_template` SET `Details`='It is always a pleasure to meet a fellow Sunwalker. We are not as numerous as some of the more ancient orders, but our ways are no less powerful and our members no less respected.$B$BAs you continue on your journey, you will grow closer to the light and gain abilities you couldn\'t have imagined. Go, get more experience, and return when you\'ve learned a new ability. Demonstrate it to me.' WHERE  `Id`=27023;

-- Etched Note
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=3092;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=3092;
UPDATE `quest_template` SET `RequiredClasses`=4 WHERE `Id`=3092;
UPDATE `quest_template` SET `RequiredRaces`=32 WHERE `Id`=3092;
-- The Hunter's Path
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=27021;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=27021;
UPDATE `quest_template` SET `RequiredClasses`=4 WHERE `Id`=27021;
UPDATE `quest_template` SET `RequiredRaces`=32 WHERE `Id`=27021;
UPDATE `quest_template` SET `PrevQuestId`=3092 WHERE `Id`=27021;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Steady Shot. Locate a Training Dummy in Camp Narache and practice using Steady Shot 5 times.' WHERE `Id`=27021;
UPDATE `quest_template` SET `Details`='It is time for your first lesson, $n. You have a long way to go to master the ways of the hunter, but as you complete more of your journey, you begin to take new steps on that path.$B$BGo, get more experience, and figure out a new ability. Then, use it on the training dummies here.' WHERE `Id`=27021;
UPDATE `quest_template` SET `OfferRewardText`='You have done well, $n.' WHERE `Id`=27021;

-- Hallowed Note
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=27014;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=27014;
UPDATE `quest_template` SET `RequiredClasses`=16 WHERE `Id`=27014;
UPDATE `quest_template` SET `RequiredRaces`=32 WHERE `Id`=27014;
UPDATE `quest_template` SET `OfferRewardText`='You have arrived at last. During your training in Camp Narache, I will guide you in the ways of the $c. You have much to learn and I look forward to counting you among my students.' WHERE `Id`=27014;
-- Learning the Word
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=27066;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=27066;
UPDATE `quest_template` SET `RequiredClasses`=16 WHERE `Id`=27066;
UPDATE `quest_template` SET `RequiredRaces`=32 WHERE `Id`=27066;
UPDATE `quest_template` SET `PrevQuestId`=27014 WHERE `Id`=27066;
UPDATE `quest_template` SET `Title`='Learning the Word' WHERE `Id`=27066;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Shadow Word: Pain. Practice using it 5 times on Training Dummies in Camp Narache.' WHERE `Id`=27066;
UPDATE `quest_template` SET `Details`='As you become more skilled, your skills will expand, and you will gain access to many new powers. Go - get some experience out there, and come back to me when you\'ve learned something new. You can demonstrate on the training dummies nearby.' WHERE `Id`=27066;

-- Rune-Inscribed Note
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=3093;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=3093;
UPDATE `quest_template` SET `RequiredClasses`=64 WHERE `Id`=3093;
UPDATE `quest_template` SET `RequiredRaces`=32 WHERE `Id`=3093;
-- Primal Strike
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=27027;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=27027;
UPDATE `quest_template` SET `RequiredClasses`=64 WHERE `Id`=27027;
UPDATE `quest_template` SET `RequiredRaces`=32 WHERE `Id`=27027;
UPDATE `quest_template` SET `PrevQuestId`=3093 WHERE `Id`=27027;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Primal Strike, and then locate a Training Dummy in Camp Narache and practice using Primal Strike 3 times.' WHERE `Id`=27027;
UPDATE `quest_template` SET `Details`='I know you\'ve been busy helping the braves protect Camp Narache from the bristlebacks. We\'re going to need all the hammers and all the luck we can get. As your skills increase, so does your connection to the elements. This gives you access to new abilities. The first of these that we learn is Primal Strike. It should help you fell your enemies faster. Learn it, and then try it out.' WHERE `Id`=27027;
UPDATE `quest_template` SET `OfferRewardText`='Well done, $n. I see you will have no trouble mastering our more advanced arts as you gain experience. Return to me when you need further training.' WHERE `Id`=27027;

-- Calligraphed Note
DELETE FROM `creature_queststarter` WHERE `id`=44927 AND `quest`=31165;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(44927, 31165);

UPDATE `quest_template` SET `Level`=2 WHERE `Id`=31165;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=31165;
UPDATE `quest_template` SET `RequiredClasses`=512 WHERE `Id`=31165;
UPDATE `quest_template` SET `RequiredRaces`=32 WHERE `Id`=31165;
UPDATE `quest_template` SET `OfferRewardText`='I am honored that Chief Hawkwind would allow me to train you and grant you your rite of passage. While I am a master in the ways of the $n, I am but a child in the ways of your people.' WHERE `Id`=31165;
UPDATE `quest_template` SET `RequestItemsText`='Ah, I am pleased to meet you.' WHERE `Id`=31165;
-- Tiger Palm
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=31166;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=31166;
UPDATE `quest_template` SET `RequiredClasses`=512 WHERE `Id`=31166;
UPDATE `quest_template` SET `RequiredRaces`=32 WHERE `Id`=31166;
UPDATE `quest_template` SET `PrevQuestId`=31165 WHERE `Id`=31166;
UPDATE `quest_template` SET `OfferRewardText`='You have done well, $n.' WHERE `Id`=31166;

-- Shoyu <Monk Trainer>
SET @CGUID := 150047;
DELETE FROM `creature` WHERE `guid`=@CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 63327, 1, -2910.4878, -260.8889, 59.61716, 0.74814);

UPDATE `creature_template` SET `gossip_menu_id`=13895 WHERE `entry`=63296;

-- Verdant Note
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=3094;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=3094;
UPDATE `quest_template` SET `RequiredClasses`=1024 WHERE `Id`=3094;
UPDATE `quest_template` SET `RequiredRaces`=32 WHERE `Id`=3094;
-- Moonfire
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=27067;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=27067;
UPDATE `quest_template` SET `RequiredClasses`=1024 WHERE `Id`=27067;
UPDATE `quest_template` SET `RequiredRaces`=32 WHERE `Id`=27067;
UPDATE `quest_template` SET `PrevQuestId`=3094 WHERE `Id`=27067;
UPDATE `quest_template` SET `Title`='Moonfire' WHERE `Id`=27067;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Moonfire. Practice using it on a Training Dummy in Camp Narache.' WHERE `Id`=27067;
UPDATE `quest_template` SET `Details`='As you develop your skills, druid, you will begin to feel your connection to the Earthmother deepening. You will begin to understand new secrets of nature, and learn to cast new spells. Go now, get more experience, and return to me when you\'ve learned a new spell. Demonstrate what you\'ve learned.' WHERE `Id`=27067;
UPDATE `quest_template` SET `OfferRewardText`='You have a knack for the druidic arts, $n. Wherever you go, your skills will be in demand. Return to me when you are ready for more training.' WHERE `Id`=27067;
