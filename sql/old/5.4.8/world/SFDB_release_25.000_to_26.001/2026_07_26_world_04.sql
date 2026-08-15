-- The Future of Gnomeregan (Warrior)
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26203;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26203;
UPDATE `quest_template` SET `RequiredClasses`=1 WHERE `Id`=26203;
UPDATE `quest_template` SET `RequiredRaces`=64 WHERE `Id`=26203;
UPDATE `quest_template` SET `OfferRewardText`='For a $r who has spent so much time inside Gnomeregan, you look positively fit and healthy, $n. That\'s good. Before we can complete what Operation: Gnomeregan began, we\'re going to need more warriors like you.' WHERE `Id`=26203;
-- The Arts of a Warrior
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26204;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26204;
UPDATE `quest_template` SET `RequiredClasses`=1 WHERE `Id`=26204;
UPDATE `quest_template` SET `RequiredRaces`=64 WHERE `Id`=26204;
UPDATE `quest_template` SET `PrevQuestId`=26203 WHERE `Id`=26204;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Charge. Locate a Training Dummy in New Tinkertown and use Charge on it.' WHERE `Id`=26204;
UPDATE `quest_template` SET `Details`='Learning is the best way to improve. As you learn more, you\'ll start to discover you can do things you never realized!$B$BGo, get some more experience, and learn a new technique. Then, come back and demonstrate it on a training dummy.' WHERE `Id`=26204;
UPDATE `quest_template` SET `OfferRewardText`='Good work! Keep practicing your charge and you\'ll have an instant advantage at the start of battle.' WHERE `Id`=26204;

-- The Future of Gnomeregan (Rogue)
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26206;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26206;
UPDATE `quest_template` SET `RequiredClasses`=8 WHERE `Id`=26206;
UPDATE `quest_template` SET `RequiredRaces`=64 WHERE `Id`=26206;
UPDATE `quest_template` SET `OfferRewardText`='It\'s good to meet you, $n. Nevin tells me that you managed to survive the dangers in Gnomeregan by using your wits. That\'s exactly what we need up here. Clever minds and quick blades.' WHERE  `Id`=26206;
-- The Arts of a Rogue
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26207;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26207;
UPDATE `quest_template` SET `RequiredClasses`=8 WHERE `Id`=26207;
UPDATE `quest_template` SET `RequiredRaces`=64 WHERE `Id`=26207;
UPDATE `quest_template` SET `PrevQuestId`=26206 WHERE `Id`=26207;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Eviscerate, then find a training dummy in New Tinkertown and practice using Eviscerate 3 times.' WHERE `Id`=26207;
UPDATE `quest_template` SET `Details`='As you learn more and become a better rogue, you\'ll start to learn new abilities.$B$BOne of the most potent ones is to chain together moves in a combination, exploiting your enemy\'s weakness and opening up the opportunity to use stronger maneuvers. Go, get some more experience and figure out how to do it. Once you can, come back here and try it out.' WHERE `Id`=26207;
UPDATE `quest_template` SET `OfferRewardText`='I\'m not surprised to see you\'re such a quick study. I look forward to showing you more of our tricks of the trade.' WHERE `Id`=26207;

-- The Future of Gnomeregan (Priest)
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26199;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26199;
UPDATE `quest_template` SET `RequiredClasses`=16 WHERE `Id`=26199;
UPDATE `quest_template` SET `RequiredRaces`=64 WHERE `Id`=26199;
UPDATE `quest_template` SET `OfferRewardText`='It\'s good to meet you, $n. I\'m sure your time in Gnomeregan afforded you ample opportunity to practice your arts. Things are somewhat safer here, but your talents will never go to waste.' WHERE `Id`=26199;
-- The Arts of a Priest
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26200;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26200;
UPDATE `quest_template` SET `RequiredClasses`=16 WHERE `Id`=26200;
UPDATE `quest_template` SET `RequiredRaces`=64 WHERE `Id`=26200;
UPDATE `quest_template` SET `PrevQuestId`=26199 WHERE `Id`=26200;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Shadow Word: Pain. Use it 5 times on training dummies in New Tinkertown.' WHERE `Id`=26200;
UPDATE `quest_template` SET `Details`='You know, as you go out into this crazy world and get more powerful, you\'ll also start to discover new powers that you didn\'t know before.$B$BWhy don\'t you go out, get some more experience, and then come back and show me what you\'ve learned!' WHERE `Id`=26200;
UPDATE `quest_template` SET `OfferRewardText`='Good work, $n. You have a natural talent for our arts.' WHERE `Id`=26200;

-- The Future of Gnomeregan (Mage)
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26197;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26197;
UPDATE `quest_template` SET `RequiredClasses`=128 WHERE `Id`=26197;
UPDATE `quest_template` SET `RequiredRaces`=64 WHERE `Id`=26197;
UPDATE `quest_template` SET `OfferRewardText`='$n, isn\'t it? It\'s a pleasure to finally meet you. While you were getting cleaned up, Nevin\'s team told me about your escape from Gnomeregan.' WHERE `Id`=26197;
-- The Arts of a Mage
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26198;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26198;
UPDATE `quest_template` SET `RequiredClasses`=128 WHERE `Id`=26198;
UPDATE `quest_template` SET `RequiredRaces`=64 WHERE `Id`=26198;
UPDATE `quest_template` SET `PrevQuestId`=26197 WHERE `Id`=26198;
UPDATE `quest_template` SET `Title`='The Arts of a Mage' WHERE `Id`=26198;
UPDATE `quest_template` SET `OfferRewardText`='You picked that one up quickly. I can\'t wait to show you some of my other favorite tricks.' WHERE `Id`=26198;

-- The Future of Gnomeregan (Warlock)
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26202;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26202;
UPDATE `quest_template` SET `RequiredClasses`=256 WHERE `Id`=26202;
UPDATE `quest_template` SET `RequiredRaces`=64 WHERE `Id`=26202;
-- The Power of a Warlock
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=26201;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=26201;
UPDATE `quest_template` SET `RequiredClasses`=256 WHERE `Id`=26201;
UPDATE `quest_template` SET `RequiredRaces`=64 WHERE `Id`=26201;
UPDATE `quest_template` SET `PrevQuestId`=26202 WHERE `Id`=26201;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Corruption, then cast it on a Training Dummy in New Tinkertown 5 times.' WHERE `Id`=26201;
UPDATE `quest_template` SET `Details`='I\'m not at all surprised that a warlock like you fought your way out of Gnomeregan. Few people, even among the gnomes, truly understand the nature of the power we wield. As you become more powerful, you\'re going to start hearing the fel voices revealing their secrets to you, teaching you new spells. I want you to go get some more experience. Once you learn something new, show me.' WHERE `Id`=26201;
UPDATE `quest_template` SET `OfferRewardText`='As you continue to gain power, return to me and I will teach you new spells and techniques.' WHERE `Id`=26201;

-- The Future of Gnomeregan (Monk)
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=31135;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=31135;
UPDATE `quest_template` SET `RequiredClasses`=512 WHERE `Id`=31135;
UPDATE `quest_template` SET `RequiredRaces`=64 WHERE `Id`=31135;
UPDATE `quest_template` SET `OfferRewardText`='Why, you look great! I\'ve heard bad things about what happens down there.$B$BWell, are you ready to start your training in the ways of the $c?' WHERE `Id`=31135;
-- The Arts of a Monk
UPDATE `quest_template` SET `Level`=2 WHERE `Id`=31138;
UPDATE `quest_template` SET `MinLevel`=2 WHERE `Id`=31138;
UPDATE `quest_template` SET `RequiredClasses`=512 WHERE `Id`=31138;
UPDATE `quest_template` SET `RequiredRaces`=64 WHERE `Id`=31138;
UPDATE `quest_template` SET `PrevQuestId`=31135 WHERE `Id`=31138;
UPDATE `quest_template` SET `OfferRewardText`='You picked that one up quickly. I can\'t wait to show you some of my other favorite tricks.' WHERE `Id`=31138;

-- Xi, Friend to the Small <Monk Trainer>
SET @CGUID := 171223;
DELETE FROM `creature` WHERE `guid`=@CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 63238, 0, -5166.229, 464.4757, 390.5133, 5.21252);

-- Monk Trainee
SET @CGUID := 171224;
DELETE FROM `creature` WHERE `guid`=@CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 63242, 0, -5162.328, 462.93228, 391.11148, 2.71076);

-- Monk Trainee
SET @CGUID := 171225;
DELETE FROM `creature` WHERE `guid`=@CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 63241, 0, -5167.2676, 460.65625, 390.8431, 1.26266);

-- Monk Trainee
SET @CGUID := 171226;
DELETE FROM `creature` WHERE `guid`=@CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 63239, 0, -5164.262, 460.68924, 391.2339, 2.07568);

