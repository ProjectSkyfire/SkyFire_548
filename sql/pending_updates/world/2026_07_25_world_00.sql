-- Warrior Training
DELETE FROM `creature_queststarter` WHERE `id`=15278 AND `quest`=8329;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(15278, 8329);

UPDATE `quest_template` SET `NextQuestIdChain`=27091 WHERE `Id`=8329;

-- Charge!
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Charge. Locate a Training Dummy outside the Sunspire and practice using Charge.' WHERE `Id`=27091;
UPDATE `quest_template` SET `Details`='Good. You are off to a bright start. This road will be long, and as you become more accomplished, you will begin to discover that you can do things you could never have imagined before.$B$BGo, get more experience, and learn a new technique. Once you have, demonstrate it on one of the training dummies.' WHERE `Id`=27091;

-- Delios <Warrior Trainer>
SET @CGUID := 100064;
DELETE FROM `creature` WHERE `guid`=@CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 43010, 530, 10381.3, -6422.5, 38.6156, 2.68781);

-- Paladin Training
DELETE FROM `creature_queststarter` WHERE `id`=15278 AND `quest`=9676;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(15278, 9676);

UPDATE `quest_template` SET `NextQuestIdChain`=10069 WHERE `Id`=9676;

-- Ways of the Light
DELETE FROM `creature_questender` WHERE `quest`=10069;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(15280, 10069);

UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Seal of Command. Cast Seal of Command on yourself, then locate a Training Dummy outside the Sunspire and hit it.' WHERE  `Id`=10069;
UPDATE `quest_template` SET `Details`='It is time for your first lesson in the ways of the Light, $n. As you fight more battles, it will bring you closer to the light, and you\'ll start to uncover new abilities. Go, get more experience, and learn a new ability. Return and demonstrate it for me, and we\'ll continue your training.' WHERE `Id`=10069;
UPDATE `quest_template` SET `OfferRewardText`='' WHERE `Id`=10069;

-- Hunter Training
DELETE FROM `creature_queststarter` WHERE `id`=15278 AND `quest`=9393;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(15278, 9393);

UPDATE `quest_template` SET `NextQuestIdChain`=10070 WHERE `Id`=9393;

-- Steady Shot
DELETE FROM `creature_questender` WHERE `quest`=10070;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(15513, 10070);

UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Steady Shot. Locate a Training Dummy outside the Sunspire and practice using Steady Shot 5 times.' WHERE `Id`=10070;
UPDATE `quest_template` SET `Details`='And now, it is time for your first lesson.$B$BAs you become more skilled, you will begin to discover new techniques.$B$BGo out, get some more experience, and learn your first new ability. Once you do, go find a training dummy and practice your new skill.' WHERE `Id`=10070;
UPDATE `quest_template` SET `OfferRewardText`='You have done well, $n. You should be proud of your progress.' WHERE `Id`=10070;

-- Rogue Training
DELETE FROM `creature_queststarter` WHERE `id`=15278 AND `quest`=9392;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(15278, 9392);

UPDATE `quest_template` SET `NextQuestIdChain`=10071 WHERE `Id`=9392;

-- Evisceration
DELETE FROM `creature_questender` WHERE `quest`=10071;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(15285, 10071);

UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Eviscerate, then locate a Training Dummy outside the Sunspire and practice using Eviscerate 3 times.' WHERE `Id`=10071;
UPDATE `quest_template` SET `Details`='As you grow in power, you\'ll start to figure out the tricks of the trade.$B$BOne of the first things you\'ll probably figure out is how to Eviscerate your enemy after landing some initial attacks. Get some experience, learn how to do it, and then find one of the training dummies outside and give it a try.' WHERE `Id`=10071;
UPDATE `quest_template` SET `OfferRewardText`='' WHERE `Id`=10071;

-- Priest Training
DELETE FROM `creature_queststarter` WHERE `id`=15278 AND `quest`=8564;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(15278, 8564);

UPDATE `quest_template` SET `NextQuestIdChain`=10072 WHERE `Id`=8564;

-- Learning the Word
DELETE FROM `creature_questender` WHERE `quest`=10072;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(15284, 10072);

UPDATE `quest_template` SET `Title`='Learning the Word' WHERE `Id`=10072;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Shadow Word: Pain. Practice casting it 5 times on nearby training dummies.' WHERE  `Id`=10072;
UPDATE `quest_template` SET `Details`='As you grow in skill, so will the array of powers that you can call upon.$B$BI\'d like you to go, continue to become more experienced, and discover a new spell. Once you do, come back and demonstrate it for me.' WHERE `Id`=10072;
UPDATE `quest_template` SET `OfferRewardText`='' WHERE  `Id`=10072;

-- Mage Training
DELETE FROM `creature_queststarter` WHERE `id`=15278 AND `quest`=8328;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(15278, 8328);

UPDATE `quest_template` SET `NextQuestIdChain`=10068 WHERE `Id`=8328;

-- Frost Nova
DELETE FROM `creature_questender` WHERE `quest`=10068;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(15279, 10068);

UPDATE `quest_template` SET `Details`='As you begin to grow in power, your connection to the arcane will develop, granting you access to new spells.$B$BGo, develop your power, and then demonstrate what you learn for me.' WHERE `Id`=10068;
UPDATE `quest_template` SET `OfferRewardText`='' WHERE  `Id`=10068;

-- Warlock Training
DELETE FROM `creature_queststarter` WHERE `id`=15278 AND `quest`=8563;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(15278, 8563);

UPDATE `quest_template` SET `NextQuestIdChain`=10073 WHERE `Id`=8563;

-- Corruption
DELETE FROM `creature_questender` WHERE `quest`=10073;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(15283, 10073);

UPDATE `quest_template` SET `Title`='Corruption' WHERE `Id`=10073;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Corruption. Locate a Training Dummy outside the Sunspire and practice casting Corruption 5 times.' WHERE `Id`=10073;
UPDATE `quest_template` SET `Details`='By now, you\'ve heard the rumors about our kind, the warlocks. Pay them no mind; they are uttered by those jealous of our fearsome powers.$B$BAs you become more experienced, you will uncover mysteries of controlling fel energies. Go, continue your journey until you uncover a new technique. I want you to practice using this new spell on the training dummies outside the Sunspire.' WHERE `Id`=10073;
UPDATE `quest_template` SET `OfferRewardText`='' WHERE  `Id`=10073;

-- Monk Training
DELETE FROM `creature_queststarter` WHERE `id`=15278 AND `quest`=31170;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(15278, 31170);

UPDATE `quest_template` SET `RequiredClasses`=512 WHERE `Id`=31170;
UPDATE `quest_template` SET `RequiredRaces`=512 WHERE `Id`=31170;
UPDATE `quest_template` SET `NextQuestIdChain`=31171 WHERE `Id`=31170;
UPDATE `quest_template` SET `OfferRewardText`='Ah, welcome. I have been teaching your people the ways of the $c, and they seem to be well suited for it. Perhaps you too will take to it as quickly as they.' WHERE `Id`=31170;

-- Tiger Palm
UPDATE `quest_template` SET `RequiredRaces`=512 WHERE `Id`=31171;
UPDATE `quest_template` SET `OfferRewardText`='I think, you\'ve mastered the basics of the Tiger Palm technique, $n. Perhaps you will impress me, after all.' WHERE `Id`=31171;

-- Pao <Monk Trainer>
SET @CGUID := 100055;
DELETE FROM `creature` WHERE `guid`=@CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 63332, 530, 10378.9, -6398.02, 38.6156, 3.87931);

-- The Sunspire Training Dummy
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=44937;

DELETE FROM `smart_scripts` WHERE `entryorguid`=44937 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(44937, 0, 0, 0, 8, 0, 100, 0, 100, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Charge - Give Quest Credit'),
(44937, 0, 1, 0, 8, 0, 100, 0, 35395, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Crusader Strike - Give Quest Credit'),
(44937, 0, 2, 0, 8, 0, 100, 0, 56641, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Steady Shot - Give Quest Credit'),
(44937, 0, 3, 0, 8, 0, 100, 0, 2098, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Eviscerate - Give Quest Credit'),
(44937, 0, 4, 0, 8, 0, 100, 0, 589, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Shadow Word: Pain - Give Quest Credit'),
(44937, 0, 5, 0, 8, 0, 100, 0, 122, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Frost Nova - Give Quest Credit'),
(44937, 0, 6, 0, 8, 0, 100, 0, 172, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Corruption - Give Quest Credit'),
(44937, 0, 7, 0, 8, 0, 100, 0, 100787, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Tiger Palm - Give Quest Credit');
