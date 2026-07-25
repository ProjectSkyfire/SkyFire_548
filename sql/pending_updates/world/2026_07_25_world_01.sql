-- Your First Lesson
UPDATE `quest_template` SET `PrevQuestId`=0 WHERE `Id`=26958;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Charge. Find a Training Dummy on the northwest side of the Crash Site and practice using Charge.' WHERE `Id`=26958;
UPDATE `quest_template` SET `Details`='Let\'s start your training right away. As you get more experience in combat, you\'ll start to develop new techniques.$B$BGo, get some more experience, learn something new, and demonstrate it on the dummies to the northwest.' WHERE `Id`=26958;

-- The Light's Power
UPDATE `quest_template` SET `PrevQuestId`=0 WHERE `Id`=26966;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Seal of Command. Apply Seal of Command, find a Training Dummy on the northwest side of the Crash Site and hit it with an attack.' WHERE `Id`=26966;
UPDATE `quest_template` SET `Details`='As a paladin, you\'re no stranger to the Light that touches all of us. But you also have the unique power to call upon the Light to help you in battle.$B$BAs you gain more experience, you\'ll grow even closer to the Light and be able to call upon new powers in battle. Go, grow in experience until you have learned a new power, and then demonstrate it for me.' WHERE `Id`=26966;

-- Steadying Your Shot
UPDATE `quest_template` SET `PrevQuestId`=0 WHERE `Id`=26963;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Steady Shot. Find a Training Dummy on the northwest side of the Crash Site and practice using Steady Shot 5 times.' WHERE `Id`=26963;
UPDATE `quest_template` SET `Details`='Let\'s start your training right away. It\'s going to be a struggle to survive here, and you\'ll play an important role in making sure our people are safe and well-supplied.$B$BAs you become more experiened, you\'ll begin to discover new abilities. Go ahead and go out, learn something new, and try it out. Return to me when you think you\'ve mastered it.' WHERE `Id`=26963;

-- Learning the Word
DELETE FROM `creature_queststarter` WHERE `id`=16502 AND `quest`=9291;
UPDATE `quest_template` SET `PrevQuestId`=0 WHERE `Id`=26970;
UPDATE `quest_template` SET `Title`='Learning the Word' WHERE `Id`=26970;
UPDATE `quest_template` SET `Objectives`='Reach level 3 to learn Shadow Word: Pain, then use it 5 times on target dummies near the crash site.' WHERE `Id`=26970;
UPDATE `quest_template` SET `Details`='As a priest, one of your most important duties is to use the Light\'s power to aid the wounded. Another is to use your power offensively, to protect them. As you grow more experienced, you will begin to uncover new abilities that you can use in your charge. Go, gain more experience, and then demonstrate the abilities you have learned to me.' WHERE `Id`=26970;

-- Primal Strike
DELETE FROM `creature_queststarter` WHERE `id`=17089 AND `quest`=9421;
UPDATE `quest_template` SET `PrevQuestId`=0 WHERE `Id`=26969;
UPDATE `quest_template` SET `Objectives`='Learn Primal Strike by reaching Level 3, and then find a Training Dummy on the northwest side of the Crash Site and practice using Primal Strike 3 times.' WHERE `Id`=26969;
UPDATE `quest_template` SET `Details`='Your walk with the elemental powers will take you to many places. As you grow in power, you will begin to learn new abilities. Let us start your journey with a simple, but useful, lesson. Learn the secret of Primal Strike, and then practice your new skill using the training dummies to the northwest of the Crash Site. Return to me when you are finished.' WHERE `Id`=26969;
UPDATE `quest_template` SET `OfferRewardText`='It is a pleasure to teach such a capable student. As long as you continue the traditions faithfully, the shamanic arts will never be lost to our people again.' WHERE `Id`=26969;

-- Frost Nova
DELETE FROM `creature_queststarter` WHERE `id`=16500 AND `quest`=9290;
UPDATE `quest_template` SET `PrevQuestId`=0 WHERE `Id`=26968;
UPDATE `quest_template` SET `Details`='In your future as a mage, you\'ll want to master a wide variety of spells. To do so, you need to deepen your connection by the arcane by growing in power. Go, get more experience as a mage, and demonstrate what you learn for me.' WHERE `Id`=26968;

-- Mojo Stormstout <Monk Trainer>
SET @CGUID := 100073;
DELETE FROM `creature` WHERE `guid`=@CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 63335, 530, -4194.7603, -13732.569, 72.96363, 4.66535);

UPDATE `creature_template` SET `gossip_menu_id`=14287 WHERE `entry`=63335;

-- Crash Site Training Dummy
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=44703;

DELETE FROM `smart_scripts` WHERE `entryorguid`=44703 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(44703, 0, 0, 0, 8, 0, 100, 0, 100, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Charge - Give Quest Credit'),
(44703, 0, 1, 0, 8, 0, 100, 0, 35395, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Crusader Strike - Give Quest Credit'),
(44703, 0, 2, 0, 8, 0, 100, 0, 56641, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Steady Shot - Give Quest Credit'),
(44703, 0, 3, 0, 8, 0, 100, 0, 589, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Shadow Word: Pain - Give Quest Credit'),
(44703, 0, 4, 0, 8, 0, 100, 0, 73899, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Primal Strike - Give Quest Credit'),
(44703, 0, 5, 0, 8, 0, 100, 0, 122, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Frost Nova - Give Quest Credit'),
(44703, 0, 6, 0, 8, 0, 100, 0, 100787, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Tiger Palm - Give Quest Credit');
