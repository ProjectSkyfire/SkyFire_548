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
UPDATE `creature_template` SET `ScriptName`='boss_hoptallus' WHERE `entry`=56717;
UPDATE `creature_template` SET `ScriptName`='boss_revelosh' WHERE `entry`=6910;
UPDATE `creature_template` SET `ScriptName`='boss_galgann_firehammer' WHERE `entry`=7291;

DELETE FROM `creature_text` WHERE `entry`=7291;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(7291, 0, 0, 'By Thaurissan\'s beard! Slay them!', 14, 0, 100, 0, 0, 5852, 'Galgann Firehammer SAY_AGGRO');
UPDATE `creature_template` SET `ScriptName`='boss_grimlok' WHERE `entry`=4854;

DELETE FROM `creature_text` WHERE `entry`=4854;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(4854, 0, 0, 'Me Grimlok, king!', 14, 0, 100, 0, 0, 5853, 'Grimlok SAY_AGGRO'),
(4854, 1, 0, 'Die, die!', 14, 0, 100, 0, 0, 5854, 'Grimlok SAY_KILL');
DELETE FROM `creature` WHERE `map`=389;
DELETE FROM `gameobject` WHERE `map`=389;
UPDATE `creature_template` SET `ScriptName`='' WHERE `entry`=646;
DELETE FROM `game_tele` WHERE `id`=1526;
INSERT INTO `game_tele` (`id`, `position_x`, `position_y`, `position_z`, `orientation`, `map`, `name`) VALUES
(1526, 1023.719, 644.747, 156.671, 4.9, 646, 'TheStoneCore');
UPDATE `instance_template` SET `script`='instance_maraudon' WHERE `map`=349;

UPDATE `creature_template` SET `ScriptName`='boss_noxxion' WHERE `entry`=13282;
UPDATE `creature_template` SET `ScriptName`='boss_landslide' WHERE `entry`=12203;
UPDATE `creature_template` SET `ScriptName`='boss_celebras_the_cursed' WHERE `entry`=12225;
UPDATE `creature_template` SET `ScriptName`='boss_princess_theradras' WHERE `entry`=12201;
UPDATE `creature_template` SET `ScriptName`='boss_tinkerer_gizlock' WHERE `entry`=13601;

UPDATE `creature_template` SET `AIName`='' WHERE `entry`=13601;
DELETE FROM `smart_scripts` WHERE `entryorguid`=13601;

DELETE FROM `creature_text` WHERE `entry`=13601;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(13601, 0, 0, 'Mine! Mine! Mine! Gizlock is the ruler of this domain! You shall never reveal my presence!', 12, 0, 100, 0, 0, 0, 'Tinkerer Gizlock SAY_AGGRO');
DELETE FROM `areatrigger_scripts` WHERE `entry`=8287;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(8287, 'at_dawning_span');

DELETE FROM `creature_text` WHERE `entry`=64885;
INSERT INTO `creature_text` (`entry`, `text`, `type`, `probability`) VALUES
(64885, 'Hello $n! The Lorewalker is beginning her lesson just down these stairs if you want to listen in.', 12, 100);
UPDATE `quest_template` SET `RequestItemsText`='Get in there, $n! We won\'t let any past. I want you in the midst of them, scattering them left and right!' WHERE `Id`=14452;
UPDATE `quest_template` SET `RequestItemsText`='Not enough have returned, $n. Return and free more of our peopple. Be brave!' WHERE `Id`=24852;
UPDATE `quest_template` SET `RequestItemsText`='The roots still thrive. You must sop as many of the thorncallers as you can.' WHERE `Id`=14455;
UPDATE `quest_template` SET `RequestItemsText`='I can still see many of them from here, $n. More should fall.' WHERE `Id`=14459;
UPDATE `quest_template` SET `RequestItemsText`='Have all the thoughs been burned?' WHERE `Id`=14461;
UPDATE `quest_template` SET `RequestItemsText`='Do not be shy, $n. I am honored to have you lay the final offering.' WHERE `Id`=24861;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=38345;
UPDATE `gameobject_template` SET `AIName`='SmartGameObjectAI' WHERE `entry`=202112;
DELETE FROM `smart_scripts` WHERE entryorguid IN (202112, 38345, 3834500);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(202112, 1, 0, 1, 70, 0, 100, 0, 2, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 11, 38345, 5, 0, 0, 0, 0, 0, 'Quilboar Cage - On activate - set data on Captured Brave'),
(202112, 1, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 38345, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Quilboar Cage - On activate - quest credit'),
(38345, 0, 0, 0, 11, 0, 100, 1, 0, 0, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 15, 202112, 5, 0, 0, 0, 0, 0, 'Captured Brave - On respawn - reset cage'),
(38345, 0, 1, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 80, 3834500, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Brave - On dataset - Run Script'),
(3834500, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Brave - Script - set data 0'),
(3834500, 9, 1, 0, 0, 0, 100, 0, 2000, 2000, 2000, 2000, 0, 46, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Brave - Script - move 5 yards'),
(3834500, 9, 2, 0, 0, 0, 100, 0, 1000, 1000, 1000, 1000, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Brave - Script - Say random text'),
(3834500, 9, 3, 0, 0, 0, 100, 0, 4000, 4000, 4000, 4000, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Brave - Script - despawn');

DELETE FROM `creature_text` WHERE `entry`=38345;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(38345, 0, 0, 'I can move again!', 12, 0, 50, 0, 0, 0, 'Captured Brave'),
(38345, 0, 1, 'The quilboar will pay!', 12, 0, 50, 0, 0, 0, 'Captured Brave');
UPDATE `quest_template` SET `PrevQuestId`=14458 WHERE `Id`=14455;
UPDATE `quest_template` SET `PrevQuestId`=14458 WHERE `Id`=14456;
UPDATE `quest_template` SET `PrevQuestId`=14456 WHERE `Id`=14459;
UPDATE `quest_template` SET `PrevQuestId`=14456 WHERE `Id`=14461;
DELETE FROM `spell_scripts` WHERE `id`=71898;
INSERT INTO `spell_scripts` (`id`, `effIndex`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES
(71898, 1, 0, 8, 38438, 0, 0, 0, 0, 0, 0);

UPDATE `gameobject_template` SET `type`='8', `data0`='1647', `data1`='15' WHERE `entry`=202138;

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2981;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (2981, 298100);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2981, 0, 9, 0, 19, 0, 100, 0, 24861, 0, 0, 0, 0, 80, 298100, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Chief Hawkwind - On Quest Accept - Run Script'),
(298100, 9, 0, 0, 0, 0, 100, 0, 2000, 3000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Chief Hawkwind - On Script - Say Line 0'),
(298100, 9, 1, 0, 0, 0, 100, 0, 7000, 8000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Chief Hawkwind - On Script - Say Line 1'),
(298100, 9, 2, 0, 0, 0, 100, 0, 7000, 8000, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Chief Hawkwind - On Script - Say Line 2'),
(298100, 9, 3, 0, 0, 0, 100, 0, 7000, 8000, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Chief Hawkwind - On Script - Say Line 3');
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=33266;

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (33266, 3326600);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(33266, 0, 1, 0, 11, 0, 100, 0, 1000, 1000, 1000, 1000, 0, 5, 16, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Mor\'shan Defender - On Respawn - Set Emote State (EMOTE_ONESHOT_KNEEL)'),
(33266, 0, 2, 3, 8, 0, 100, 1, 62644, 0, 0, 0, 0, 5, 26, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Mor\'shan Defender - On Spell Hit "Apply Salve" - Set Emote State (EMOTE_STATE_STAND)'),
(33266, 0, 3, 0, 61, 0, 100, 0, 0, 1, 0, 0, 0, 80, 3326600, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Mor\'shan Defender - On Linked - Run Script'),
(3326600, 9, 1, 0, 0, 0, 100, 0, 2000, 2000, 2000, 2000, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Mor\'shan Defender - Script - say random text'),
(3326600, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 33266, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Wounded Mor\'shan Defender - Script - Give Killcredit (33266)'),
(3326600, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Mor\'shan Defender - Script - set run'),
(3326600, 9, 4, 0, 0, 0, 100, 0, 2000, 2000, 2000, 2000, 0, 69, 1, 0, 0, 0, 0, 0, 8, 0, 0, 0, 1217.22, -2246.04, 92.172, 0, 'Wounded Mor\'shan Defender - Script - move position'),
(3326600, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Mor\'shan Defender - Script - Despawn after 3 seconds');

DELETE FROM `creature_text` WHERE `entry`=33266;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(33266, 0, 0, 'Death to the Alliance!', 12, 0, 100, 0, 0, 0, 'Wounded Mor\'shan Defender - RANDOM_SAY_0'),
(33266, 0, 2, 'For the Horde!', 12, 0, 100, 0, 0, 0, 'Wounded Mor\'shan Defender - RANDOM_SAY_2'),
(33266, 0, 1, 'Many thanks, $c!', 12, 0, 100, 0, 0, 0, 'Wounded Mor\'shan Defender - RANDOM_SAY_1');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=17 AND `SourceEntry`=69855;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 69855, 0, 0, 31, 1, 3, 37080, 0, 0, 0, 0, '', 'Spell only hits Wounded Coldridge Mountaineer');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=37080;

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (37080, 3708000);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(37080, 0, 1, 0, 11, 0, 100, 0, 1000, 1000, 1000, 1000, 0, 5, 16, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Coldridge Mountaineer - On Respawn - Set Emote State (EMOTE_ONESHOT_KNEEL)'),
(37080, 0, 2, 3, 8, 0, 100, 1, 69855, 0, 0, 0, 0, 5, 26, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Coldridge Mountaineer - On Spell Hit "Heal Wounded Mountaineer" - Set Emote State (EMOTE_STATE_STAND)'),
(37080, 0, 3, 0, 61, 0, 100, 0, 0, 1, 0, 0, 0, 80, 3708000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Coldridge Mountaineer - On Linked - Run Script'),
(3708000, 9, 1, 0, 0, 0, 100, 0, 2000, 2000, 2000, 2000, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Coldridge Mountaineer - Script - say random text'),
(3708000, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Coldridge Mountaineer - Script - set run'),
(3708000, 9, 3, 0, 0, 0, 100, 0, 2000, 2000, 2000, 2000, 0, 46, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Coldridge Mountaineer - Script - move 5 yards'),
(3708000, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wounded Coldridge Mountaineer - Script - Despawn after 2 seconds');

DELETE FROM `creature_text` WHERE `entry`=37080;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(37080, 0, 0, 'Thanks! I can make it from here.', 12, 0, 100, 0, 0, 0, 'Wounded Coldridge Mountaineer - RANDOM_SAY_0'),
(37080, 0, 2, 'Yer a lifesaver! for Ironforge!', 12, 0, 100, 0, 0, 0, 'Wounded Coldridge Mountaineer - RANDOM_SAY_2'),
(37080, 0, 1, 'Ahh, much better. I owe ya one.', 12, 0, 100, 0, 0, 0, 'Wounded Coldridge Mountaineer - RANDOM_SAY_1');UPDATE `creature_template` SET `AIName`='' WHERE `entry`=448;
DELETE FROM `smart_scripts` WHERE `entryorguid`=448;
DELETE FROM `creature` WHERE `guid` IN (167936, 167937, 167938, 167939, 167940);

UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_hogger_elwynn' WHERE `entry`=448;
-- Krystel <Spirit of Harmony Vendor>
DELETE FROM `npc_vendor` WHERE `entry`=66678;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES 
(66678, 0, 402, 0, 0, 3901, 2),
(66678, 1, 79255, 0, 0, 3901, 1),
(66678, 2, 72988, 0, 0, 3901, 1),
(66678, 3, 89610, 0, 0, 3901, 1),
(66678, 4, 72238, 0, 0, 3901, 1),
(66678, 5, 74249, 0, 0, 3901, 1),
(66678, 6, 74250, 0, 0, 3901, 1),
(66678, 7, 74247, 0, 0, 3901, 1),
(66678, 8, 72092, 0, 0, 3901, 1),
(66678, 9, 72093, 0, 0, 3901, 1),
(66678, 10, 72094, 0, 0, 3901, 1),
(66678, 11, 72103, 0, 0, 3901, 1),
(66678, 12, 72120, 0, 0, 3901, 1),
(66678, 13, 79101, 0, 0, 3901, 1),
(66678, 14, 76734, 0, 0, 3901, 1);

-- Danky <Spirit of Harmony Vendor>
DELETE FROM `npc_vendor` WHERE `entry`=66685;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES 
(66685, 0, 402, 0, 0, 3902, 2),
(66685, 1, 79255, 0, 0, 3902, 1),
(66685, 2, 72988, 0, 0, 3902, 1),
(66685, 3, 89610, 0, 0, 3902, 1),
(66685, 4, 72238, 0, 0, 3902, 1),
(66685, 5, 74249, 0, 0, 3902, 1),
(66685, 6, 74250, 0, 0, 3902, 1),
(66685, 7, 74247, 0, 0, 3902, 1),
(66685, 8, 72092, 0, 0, 3902, 1),
(66685, 9, 72093, 0, 0, 3902, 1),
(66685, 10, 72094, 0, 0, 3902, 1),
(66685, 11, 72103, 0, 0, 3902, 1),
(66685, 12, 72120, 0, 0, 3902, 1),
(66685, 13, 79101, 0, 0, 3902, 1),
(66685, 14, 76734, 0, 0, 3902, 1);
-- Brewmaster Skye <Innkeeper>
DELETE FROM `npc_vendor` WHERE `entry`=63008;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
(63008, 0, 81407, 0, 0, 0, 1),
(63008, 1, 81415, 0, 0, 0, 1);
-- Brewmaster Skye <Innkeeper>
UPDATE `creature_template` SET `npcflag`=66177, `ScriptName`='' WHERE `entry`=63008;
SET @CGUID := 200258;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID-40 AND @CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`) VALUES
(@CGUID-0, 2951, 1, -2383.239013671875, 498.14892578125, 49.99972915649414062, 1.885769486427307128, 120),
(@CGUID-1, 2951, 1, -2396.437255859375, 478.356292724609375, 54.95770263671875, 2.389832019805908203, 120), 
(@CGUID-2, 2950, 1, -2339.19921875, 525.02154541015625, 41.11994552612304687, 1.484963774681091308, 120),
(@CGUID-3, 2949, 1, -2466.88037109375, 216.81439208984375, 55.3312225341796875, 5.866746902465820312, 120), 
(@CGUID-4, 2951, 1, -2434.23388671875, 200.8055877685546875, 50.07751083374023437, 2.353483200073242187, 120),
(@CGUID-5, 2949, 1, -2410.29248046875, 221.9026947021484375, 48.6411590576171875, 0.768368363380432128, 120), 
(@CGUID-6, 2950, 1, -2438.495361328125, 229.587982177734375, 52.18490982055664062, 6.277326107025146484, 120),
(@CGUID-7, 2951, 1, -2392.645263671875, 185.970550537109375, 47.71702194213867187, 0.327170073986053466, 120),
(@CGUID-8, 2949, 1, -2398.11083984375, 227.1201934814453125, 48.60992431640625, 6.180898189544677734, 120), 
(@CGUID-9, 2949, 1, -2423.6015625, 264.2030029296875, 58.52765655517578125, 2.875609159469604492, 120),
(@CGUID-10, 2950, 1, -2391.52294921875, 250.6894683837890625, 52.0338134765625, 3.141579389572143554, 120),
(@CGUID-11, 2949, 1, -2360.960205078125, 207.4921417236328125, 50.63480377197265625, 3.027401208877563476, 120),
(@CGUID-12, 2951, 1, -2398.08984375, 298.196563720703125, 66.20787811279296875, 4.127864837646484375, 120), 
(@CGUID-13, 2949, 1, -2410.768798828125, 345.213623046875, 65.8819732666015625, 0.352340936660766601, 120),
(@CGUID-14, 2949, 1, -2364.240478515625, 324.6826171875, 64.9104156494140625, 0.691504061222076416, 120),
(@CGUID-15, 2950, 1, -2392.513671875, 343.793670654296875, 64.91129302978515625, 4.734151840209960937, 120), 
(@CGUID-16, 2949, 1, -2390.897705078125, 364.170135498046875, 62.16577911376953125, 4.799655437469482421, 120),
(@CGUID-17, 2949, 1, -2367.9697265625, 351.4969482421875, 64.87273406982421875, 2.350087642669677734, 120),
(@CGUID-18, 2949, 1, -2360.724609375, 366.88385009765625, 64.91060638427734375, 0.80033886432647705, 120),
(@CGUID-19, 2950, 1, -2370.178466796875, 383.460845947265625, 66.1135406494140625, 5.159834861755371093, 120),
(@CGUID-20, 2949, 1, -2336.457275390625, 406.575408935546875, 56.14568328857421875, 2.381210803985595703, 120),
(@CGUID-21, 2950, 1, -2355.87255859375, 421.80450439453125, 63.30765151977539062, 6.192182540893554687, 120), 
(@CGUID-22, 2950, 1, -2341.2822265625, 392.33673095703125, 66.491668701171875, 0.0146669652312994, 120), 
(@CGUID-23, 2949, 1, -2319.22998046875, 405.3193359375, 53.72214508056640625, 2.254524469375610351, 120),
(@CGUID-24, 2950, 1, -2314.04833984375, 412.0537109375, 49.29518890380859375, 2.543779373168945312, 120),
(@CGUID-25, 2951, 1, -2368.56005859375, 446.6820068359375, 62.43163299560546875, 1.867502331733703613, 120), 
(@CGUID-26, 2951, 1, -2402.570068359375, 442.3330078125, 60.86490249633789062, 0.138304769992828369, 120),
(@CGUID-27, 2950, 1, -2355.8076171875, 444.02276611328125, 69.8309478759765625, 0.441808611154556274, 120),
(@CGUID-28, 2950, 1, -2331.940673828125, 430.646820068359375, 49.73831558227539062, 2.134458065032958984, 120), 
(@CGUID-29, 2951, 1, -2404.067138671875, 441.511688232421875, 60.63844680786132812, 2.066359996795654296, 120), 
(@CGUID-30, 2951, 1, -2383.057861328125, 456.610626220703125, 76.792388916015625, 2.24678802490234375, 120), 
(@CGUID-31, 2951, 1, -2341.821044921875, 463.93878173828125, 75.8529510498046875, 0.159238979220390319, 120), 
(@CGUID-32, 2951, 1, -2345.773193359375, 467.329010009765625, 46.83425140380859375, 1.741240262985229492, 120), 
(@CGUID-33, 2950, 1, -2319.45849609375, 489.51220703125, 45.78248214721679687, 2.000107765197753906, 120),
(@CGUID-34, 2951, 1, -2365.8701171875, 490.3170166015625, 45.56985092163085937, 6.011109828948974609, 120),
(@CGUID-35, 2951, 1, -2346.66259765625, 496.92718505859375, 44.02925872802734375, 2.114919185638427734, 120), 
(@CGUID-36, 2950, 1, -2401.96142578125, 472.151275634765625, 58.41176223754882812, 4.005495071411132812, 120),
(@CGUID-37, 2951, 1, -2418.970458984375, 473.519561767578125, 64.56475830078125, 1.818383336067199707, 120), 
(@CGUID-38, 2951, 1, -2383.239013671875, 498.14892578125, 49.99972915649414062, 1.885769486427307128, 120),
(@CGUID-39, 2951, 1, -2396.437255859375, 478.356292724609375, 54.95770263671875, 2.389832019805908203, 120), 
(@CGUID-40, 2950, 1, -2339.19921875, 525.02154541015625, 41.11994552612304687, 1.484963774681091308, 120);
UPDATE `creature_text` SET `type`=41 WHERE `entry`=36845;
DELETE FROM `creature_text` WHERE `entry`=448;

INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(448, 0, 0, 'Hogger is eating! Stop him!', 41, 0, 100, 0, 0, 0, 'Hogger WARNING_EATING'),
(448, 1, 0, 'Yipe! Help $n!', 14, 0, 100, 0, 0, 0, 'Hogger YELL_YIPE'),
(448, 2, 0, 'Grrr...', 12, 0, 100, 0, 0, 0, 'Hogger SAY_GRR'),
(448, 3, 0, 'Nooooo...', 12, 0, 100, 0, 0, 0, 'Hogger SAY_NO'),
(448, 4, 0, 'No hurt $n!', 14, 0, 100, 0, 0, 0, 'Hogger YELL_NO_HURT');
-- Brewmaster Skye
UPDATE `creature_template` SET `gossip_menu_id`=14224 WHERE `entry`=63008;

DELETE FROM `gossip_menu` WHERE `entry`=14224;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(14224, 62445);

DELETE FROM `npc_text` WHERE `ID`=62445;
INSERT INTO `npc_text` (`ID`, `text0_1`) VALUES
(62445, 'Should try the special.');

DELETE FROM `gossip_menu_option` WHERE `menu_id`=14224;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES
(14224, 0, 5, 'Make this inn your home.', 8, 65536, 0, 0, 0, 0, ''),
(14224, 1, 1, 'Let me browse your goods.', 3, 128, 0, 0, 0, 0, '');
-- Tinza Silvermug
DELETE FROM `gossip_menu_option` WHERE `menu_id`=12040;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES
(12040, 0, 5, 'Make this inn your home.', 8, 65536, 0, 0, 0, 0, ''),
(12040, 1, 1, 'Let me browse your goods.', 3, 128, 0, 0, 0, 0, '');

-- Delaniel
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=15 AND `SourceGroup`=6620;
DELETE FROM `smart_scripts` WHERE `entryorguid`=15433;
UPDATE `creature_template` SET `npcflag`=65667 WHERE `entry`=15433;
DELETE FROM `gossip_menu_option` WHERE `menu_id`=6620;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES
(6620, 0, 5, 'Make this inn your home.', 8, 65536, 0, 0, 0, 0, ''),
(6620, 1, 1, 'Let me browse your goods.', 3, 128, 0, 0, 0, 0, '');
UPDATE `creature_template` SET `gossip_menu_id`=14580, `npcflag`=1 WHERE `entry`=64043;

DELETE FROM `gossip_menu` WHERE `entry`=14580;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(14580, 65082);

DELETE FROM `npc_text` WHERE `ID`=65082;
INSERT INTO `npc_text` (`ID`, `text0_0`) VALUES
(65082, 'Why was the tank nervous before the fight?$B$B...$B$BBecause he was a worrier!');
UPDATE `creature_template` SET `gossip_menu_id`=15719 WHERE `entry`=66998;

DELETE FROM `gossip_menu` WHERE `entry`=15719;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(15719, 74212);

DELETE FROM `npc_text` WHERE `ID`=74212;
INSERT INTO `npc_text` (`ID`, `text0_0`) VALUES
(74212, 'Hello friend.');
-- Kurong Caskhead
DELETE FROM `npc_vendor` WHERE `entry`=64047;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
(64047, 0, 81406, 0, 0, 0, 1),
(64047, 1, 81414, 0, 0, 0, 1),
(64047, 2, 74636, 0, 0, 0, 1),
(64047, 3, 81407, 0, 0, 0, 1),
(64047, 4, 81415, 0, 0, 0, 1);
SET @CGUID := 120771;
DELETE FROM `creature` WHERE `guid` IN (120662, @CGUID-0, @CGUID-1);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID-0, 53714, 860, 1, 594, 0, 0, 0, 1471.21, 3399.65, 171.064, 1.51844, 30, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-1, 53714, 860, 1, 594, 0, 0, 0, 1468.5, 3399.89, 171.056, 1.50098, 30, 0, 0, 0, 0, 0, 0, 0, 0);
UPDATE creature_addon SET bytes1=0 WHERE guid=120679;
UPDATE creature_addon SET bytes1=0 WHERE guid=120680;
DELETE FROM `creature` WHERE `guid` IN (80137, 80406, 80558, 120729);
DELETE FROM `creature` WHERE `guid`=150047;
DELETE FROM `creature` WHERE `guid` BETWEEN 200259 AND 200325;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(200325, 36737, 996, 8, 0, 0, 0, 0, -1012.14, -3137.47, 27.1847, 4.66795, 7200, 0, 0, 112919, 0, 0, 0, 0, 0),
(200324, 60583, 996, 8, 0, 0, 0, 0, -1017.87, -3058.73, 12.9069, 1.50103, 7200, 0, 0, 303455, 0, 0, 0, 0, 0),
(200323, 64368, 996, 8, 0, 0, 0, 0, -982.193, -3032.26, 12.6524, 3.54567, 7200, 0, 0, 46510, 0, 0, 0, 0, 0),
(200322, 60999, 996, 8, 0, 0, 0, 0, -1017.89, -2758.34, 38.6544, 4.71308, 7200, 0, 0, 602217, 100, 0, 0, 0, 0),
(200321, 60585, 996, 8, 0, 0, 0, 0, -1009.94, -3045.02, 12.9069, 3.82705, 7200, 0, 0, 197089, 0, 0, 0, 0, 0),
(200320, 63025, 996, 8, 0, 0, 0, 0, -1018.43, -2784.43, 49.4293, 1.54984, 7200, 0, 0, 758400, 100, 0, 0, 0, 0),
(200319, 66100, 996, 8, 0, 0, 0, 0, -982.42, -3066.15, 12.6525, 2.69789, 7200, 0, 0, 46510, 0, 0, 0, 0, 0),
(200318, 60586, 996, 8, 0, 0, 0, 0, -1025.78, -3044.84, 12.9069, 5.6988, 7200, 0, 0, 197089, 0, 0, 0, 0, 0),
(200317, 64846, 996, 8, 0, 0, 0, 0, -1036.11, -3136.93, 27.3358, 5.41548, 7200, 0, 0, 2854, 0, 0, 0, 0, 0),
(200316, 64368, 996, 8, 0, 0, 0, 0, -1052.7, -3068.12, 12.6525, 0.577366, 7200, 0, 0, 46510, 0, 0, 0, 0, 0),
(200315, 66100, 996, 8, 0, 0, 0, 0, -1053.42, -3031.82, 12.6524, 5.8744, 7200, 0, 0, 46510, 0, 0, 0, 0, 0),
(200313, 63420, 996, 8, 0, 0, 0, 0, -1017.8, -3049.11, 12.9069, 4.69506, 7200, 0, 0, 1284, 0, 0, 0, 0, 0),
(200312, 60957, 996, 8, 0, 0, 0, 0, -1017.97, -2938.68, 19.4397, 4.69066, 7200, 0, 0, 21330, 0, 0, 0, 0, 0),
(200310, 62962, 996, 8, 0, 0, 0, 0, -1074.8, -3053.67, 12.2095, 4.65514, 7200, 0, 0, 6108, 0, 0, 0, 0, 0),
(200309, 62962, 996, 8, 0, 0, 0, 0, -980.799, -3090, 12.2095, 4.65514, 7200, 0, 0, 6108, 0, 0, 0, 0, 0),
(200308, 62962, 996, 8, 0, 0, 0, 0, -1024.49, -3101.46, 12.2165, 4.65514, 7200, 0, 0, 6108, 0, 0, 0, 0, 0),
(200307, 62962, 996, 8, 0, 0, 0, 0, -967.108, -3021.3, 12.2095, 4.65514, 7200, 0, 0, 6108, 0, 0, 0, 0, 0),
(200306, 62962, 996, 8, 0, 0, 0, 0, -1070.94, -3030.72, 12.2095, 4.65514, 7200, 0, 0, 6108, 0, 0, 0, 0, 0),
(200305, 62962, 996, 8, 0, 0, 0, 0, -993.757, -2999.41, 12.2095, 4.65514, 7200, 0, 0, 6108, 0, 0, 0, 0, 0),
(200304, 62962, 996, 8, 0, 0, 0, 0, -969.516, -3075.16, 12.2095, 4.65514, 7200, 0, 0, 6108, 0, 0, 0, 0, 0),
(200303, 62962, 996, 8, 0, 0, 0, 0, -1060.61, -3012.75, 12.2095, 4.65514, 7200, 0, 0, 6108, 0, 0, 0, 0, 0),
(200302, 62962, 996, 8, 0, 0, 0, 0, -1064.38, -3077.08, 12.2095, 4.65514, 7200, 0, 0, 6108, 0, 0, 0, 0, 0),
(200301, 62962, 996, 8, 0, 0, 0, 0, -1047.64, -2999.57, 12.2095, 4.65514, 7200, 0, 0, 6108, 0, 0, 0, 0, 0),
(200300, 62962, 996, 8, 0, 0, 0, 0, -1047.98, -3096.87, 12.2095, 4.65514, 7200, 0, 0, 6108, 0, 0, 0, 0, 0),
(200299, 62962, 996, 8, 0, 0, 0, 0, -979.097, -3006.9, 12.2095, 4.65514, 7200, 0, 0, 6108, 0, 0, 0, 0, 0),
(200298, 62962, 996, 8, 0, 0, 0, 0, -1001.01, -3098.93, 12.2095, 4.65514, 7200, 0, 0, 6108, 0, 0, 0, 0, 0),
(200297, 62848, 996, 8, 0, 0, 0, 0, -1017.79, -3048.64, 12.9069, 4.67533, 7200, 0, 0, 6320, 0, 0, 0, 0, 0),
(200296, 62962, 996, 8, 0, 0, 0, 0, -1018.58, -2992.52, 12.3928, 4.65514, 7200, 0, 0, 6108, 0, 0, 0, 0, 0),
(200295, 62962, 996, 8, 0, 0, 0, 0, -961.795, -3041.03, 12.2095, 4.65514, 7200, 0, 0, 6108, 0, 0, 0, 0, 0),
(200294, 62962, 996, 8, 0, 0, 0, 0, -963.307, -3059.72, 12.2095, 4.65514, 7200, 0, 0, 6108, 0, 0, 0, 0, 0),
(200293, 62442, 996, 8, 0, 0, 0, 0, -1017.99, -3049.62, 12.9069, 4.5815, 7200, 0, 0, 568800, 100, 0, 0, 0, 0),
(200292, 64443, 996, 8, 0, 0, 0, 0, -963.267, -3072.51, 12.2095, 0, 7200, 0, 0, 6108, 0, 0, 0, 0, 0),
(200291, 62995, 996, 8, 0, 0, 0, 0, -1038.37, -2934.91, 19.1783, 2.3128, 7200, 0, 0, 19652, 0, 0, 0, 0, 0),
(200290, 63275, 996, 8, 0, 0, 0, 0, -1025.07, -2898.18, 19.7061, 4.67586, 7200, 0, 0, 13743, 0, 0, 0, 0, 0),
(200289, 62995, 996, 8, 0, 0, 0, 0, -1044.99, -2901.2, 19.1783, 3.01376, 7200, 0, 0, 19652, 0, 0, 0, 0, 0),
(200288, 63275, 996, 8, 0, 0, 0, 0, -1042.7, -2895.1, 19.1783, 4.68451, 7200, 0, 0, 13743, 0, 0, 0, 0, 0),
(200287, 62995, 996, 8, 0, 0, 0, 0, -992.842, -2888.92, 19.1783, 2.48994, 7200, 0, 0, 19652, 0, 0, 0, 0, 0),
(200286, 63275, 996, 8, 0, 0, 0, 0, -998.486, -2914.14, 19.7061, 4.71225, 7200, 0, 0, 13743, 0, 0, 0, 0, 0),
(200285, 62995, 996, 8, 0, 0, 0, 0, -1002.59, -2913.23, 19.7061, 4.01461, 7200, 0, 0, 19652, 0, 0, 0, 0, 0),
(200284, 63275, 996, 8, 0, 0, 0, 0, -1002.27, -2932.6, 19.1783, 4.75939, 7200, 0, 0, 13743, 0, 0, 0, 0, 0),
(200283, 63275, 996, 8, 0, 0, 0, 0, -1008.39, -2901.33, 19.7061, 4.67959, 7200, 0, 0, 13743, 0, 0, 0, 0, 0),
(200282, 62995, 996, 8, 0, 0, 0, 0, -1028.25, -2923.66, 19.7061, 0, 7200, 0, 0, 19652, 0, 0, 0, 0, 0),
(200281, 62995, 996, 8, 0, 0, 0, 0, -1011.26, -2888.35, 19.7061, 4.3029, 7200, 0, 0, 19652, 0, 0, 0, 0, 0),
(200280, 63275, 996, 8, 0, 0, 0, 0, -992.594, -2895.52, 19.1783, 4.68538, 7200, 0, 0, 13743, 0, 0, 0, 0, 0),
(200279, 62995, 996, 8, 0, 0, 0, 0, -1027.07, -2899.24, 19.7061, 1.26187, 7200, 0, 0, 19652, 0, 0, 0, 0, 0),
(200278, 63275, 996, 8, 0, 0, 0, 0, -1018.22, -2935.31, 19.4397, 4.70664, 7200, 0, 0, 13743, 0, 0, 0, 0, 0),
(200277, 62995, 996, 8, 0, 0, 0, 0, -1023.39, -2888.51, 19.7061, 0.862035, 7200, 0, 0, 19652, 0, 0, 0, 0, 0),
(200276, 62995, 996, 8, 0, 0, 0, 0, -994.75, -2925.18, 19.1783, 4.80107, 7200, 0, 0, 19652, 0, 0, 0, 0, 0),
(200275, 63275, 996, 8, 0, 0, 0, 0, -1017.52, -2923.79, 19.7061, 4.61708, 7200, 0, 0, 13743, 0, 0, 0, 0, 0),
(200274, 63275, 996, 8, 0, 0, 0, 0, -1038.61, -2916.74, 19.7061, 4.69412, 7200, 0, 0, 13743, 0, 0, 0, 0, 0),
(200273, 62995, 996, 8, 0, 0, 0, 0, -1013.61, -2925.48, 19.7061, 1.11224, 7200, 0, 0, 19652, 0, 0, 0, 0, 0),
(200272, 63275, 996, 8, 0, 0, 0, 0, -1034.58, -2932.84, 19.1783, 4.60711, 7200, 0, 0, 13743, 0, 0, 0, 0, 0),
(200271, 62995, 996, 8, 0, 0, 0, 0, -1045.23, -2925.26, 19.1783, 1.96081, 7200, 0, 0, 19652, 0, 0, 0, 0, 0),
(200270, 62995, 996, 8, 0, 0, 0, 0, -998.684, -2934.78, 19.1783, 5.88203, 7200, 0, 0, 19652, 0, 0, 0, 0, 0),
(200269, 62995, 996, 8, 0, 0, 0, 0, -1012.43, -2941.38, 19.4397, 3.00527, 7200, 0, 0, 19652, 0, 0, 0, 0, 0),
(200268, 62995, 996, 8, 0, 0, 0, 0, -1042.06, -2888.43, 19.1783, 1.97428, 7200, 0, 0, 19652, 0, 0, 0, 0, 0),
(200267, 62995, 996, 8, 0, 0, 0, 0, -989.363, -2902.62, 19.1783, 0.642311, 7200, 0, 0, 19652, 0, 0, 0, 0, 0),
(200266, 62995, 996, 8, 0, 0, 0, 0, -1008.4, -2900.63, 19.7061, 4.64341, 7200, 0, 0, 19652, 0, 0, 0, 0, 0),
(200265, 62995, 996, 8, 0, 0, 0, 0, -1023.32, -2936.85, 19.4397, 3.93667, 7200, 0, 0, 19652, 0, 0, 0, 0, 0),
(200264, 62983, 996, 8, 0, 0, 0, 0, -1017.93, -2911.3, 19.9015, 4.70965, 7200, 0, 0, 450490, 1189, 0, 0, 0, 0),
(200263, 62995, 996, 8, 0, 0, 0, 0, -1035.01, -2910.32, 19.7061, 3.57302, 7200, 0, 0, 19652, 0, 0, 0, 0, 0),
(200262, 65736, 996, 8, 0, 0, 0, 0, -1214.8, -2824.82, 41.243, 3.50734, 7200, 0, 0, 5708, 0, 0, 0, 0, 0),
(200261, 65736, 996, 8, 0, 0, 0, 0, -832.076, -2745.4, 31.6775, 0.153694, 7200, 0, 0, 5708, 0, 0, 0, 0, 0),
(200260, 60788, 996, 8, 0, 0, 0, 0, -1017.84, -2771.98, 38.6544, 1.57828, 7200, 0, 0, 3160, 0, 0, 0, 0, 0),
(200259, 71095, 996, 8, 0, 0, 0, 0, -1017.91, -2771.98, 38.6544, 4.70927, 7200, 0, 0, 3160, 1189, 0, 0, 0, 0);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 200006 AND 200014;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(200006, 214849, 996, 8, 0, 0, -1017.88, -2771.91, 39.1506, 4.71239, 0, 0, -0.707107, 0.707107, 7200, 255, 0),
(200007, 214850, 996, 8, 0, 0, -1017.93, -2772, 23.1714, 4.71239, 0, 0, -0.707107, 0.707107, 7200, 255, 0),
(200008, 213074, 996, 8, 0, 0, -1017.86, -2911.34, 19.8182, 4.70886, 0, 0, -0.708352, 0.705859, 7200, 255, 0),
(200009, 214851, 996, 8, 0, 0, -1017.84, -2911.58, 20.3569, 4.71239, 0, 0, -0.707107, 0.707107, 7200, 255, 0),
(200010, 214852, 996, 8, 0, 0, -1017.93, -2911.35, -0.457318, 4.71239, 0, 0, -0.707107, 0.707107, 7200, 255, 0),
(200011, 212922, 996, 8, 0, 0, -1018.15, -2986.26, 12.3096, 4.75136, 0, 0, -0.693194, 0.720751, 7200, 255, 0),
(200012, 214854, 996, 8, 0, 0, -1017.84, -3049.01, 13.8251, 4.71239, 0, 0, -0.707107, 0.707107, 7200, 255, 0),
(200013, 214853, 996, 8, 0, 0, -1017.93, -3049.1, -6.98916, 4.71239, 0, 0, -0.707107, 0.707107, 7200, 255, 0),
(200014, 214525, 996, 8, 0, 0, -1021.25, -3157.25, 30.7474, 1.5719, 0, 0, 0.707498, 0.706716, 7200, 255, 1);
DELETE FROM `creature_addon` WHERE `guid` IN (48648,48655,48669,48690,48694,48699,48704,48707,48711,48712,48713,48730,48731,79223,79260,79333,79344);
SET @CGUID := 200359;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID-33 AND @CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID-0, 54587, 860, 1, 594, 0, 0, 0, 1433.77, 3373.08, 173.654, 6.13702, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-1, 54587, 860, 1, 594, 0, 0, 0, 1447.37, 3368.46, 173.653, 2.5731, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-2, 65471, 860, 1, 594, 0, 0, 0, 1434.03, 3369.41, 173.654, 0.486768, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-3, 65471, 860, 1, 594, 0, 0, 0, 1442.38, 3364.71, 173.651, 1.67661, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-4, 65471, 860, 1, 594, 0, 0, 0, 1435.96, 3366.72, 173.653, 0.849348, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-5, 65471, 860, 1, 594, 0, 0, 0, 1445.32, 3365.97, 173.651, 2.04959, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-6, 65471, 860, 1, 594, 0, 0, 0, 1439.01, 3365.12, 173.652, 1.26783, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-7, 54586, 860, 1, 594, 0, 0, 0, 1425.86, 3369.1, 173.886, 4.88692, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-8, 54586, 860, 1, 594, 0, 0, 0, 1418.97, 3384.1, 173.005, 2.99682, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-9, 54586, 860, 1, 594, 0, 0, 0, 1415.69, 3384.54, 173.015, 6.12097, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-10, 54586, 860, 1, 594, 0, 0, 0, 1430.55, 3379.32, 173.886, 1.42473, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-11, 54586, 860, 1, 594, 0, 0, 0, 1415.48, 3375.62, 173.008, 2.04204, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-12, 54586, 860, 1, 594, 0, 0, 0, 1445.2, 3381.24, 192.799, 2.78863, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-13, 54586, 860, 1, 594, 0, 0, 0, 1442.17, 3382.51, 192.799, 5.94768, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-14, 54586, 860, 1, 594, 0, 0, 0, 1439.11, 3384.18, 173.886, 2.3911, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-15, 54586, 860, 1, 594, 0, 0, 0, 1450.87, 3363.6, 173.886, 1.03354, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-16, 54586, 860, 1, 594, 0, 0, 0, 1457.95, 3378.23, 184.523, 1.90241, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-17, 54586, 860, 1, 594, 0, 0, 0, 1456.93, 3381.36, 184.515, 5.06145, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-18, 65470, 860, 1, 594, 0, 0, 0, 1425.91, 3361.5, 184.521, 5.67232, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-19, 65470, 860, 1, 594, 0, 0, 0, 1431.72, 3373.32, 184.527, 1.23918, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-20, 65470, 860, 1, 594, 0, 0, 0, 1426.45, 3365.83, 173.886, 1.72788, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-21, 65470, 860, 1, 594, 0, 0, 0, 1413.88, 3378.53, 173.016, 5.20108, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-22, 65470, 860, 1, 594, 0, 0, 0, 1432.81, 3376.42, 184.527, 4.39823, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-23, 65470, 860, 1, 594, 0, 0, 0, 1431.02, 3382.6, 173.885, 4.54886, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-24, 65470, 860, 1, 594, 0, 0, 0, 1439.58, 3364.46, 184.527, 1.62316, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-25, 65470, 860, 1, 594, 0, 0, 0, 1443.8, 3355.36, 184.522, 5.93412, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-26, 65470, 860, 1, 594, 0, 0, 0, 1436.63, 3386.38, 173.886, 5.55015, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-27, 65470, 860, 1, 594, 0, 0, 0, 1436.3, 3389.04, 184.52, 5.96903, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-28, 65470, 860, 1, 594, 0, 0, 0, 1439.08, 3367.77, 184.527, 4.93928, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-29, 65470, 860, 1, 594, 0, 0, 0, 1439.4, 3387.95, 184.521, 2.80998, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-30, 65470, 860, 1, 594, 0, 0, 0, 1446.91, 3354.23, 184.52, 2.77507, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-31, 65470, 860, 1, 594, 0, 0, 0, 1452.49, 3366.49, 173.886, 4.19258, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-32, 65470, 860, 1, 594, 0, 0, 0, 1428.56, 3359.54, 184.522, 2.51327, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-33, 61411, 860, 1, 594, 0, 0, 0, 1441.26, 3372.11, 173.379, 4.36139, 120, 0, 0, 0, 0, 0, 0, 0, 0);
DELETE FROM `script_waypoint` WHERE `entry` IN (11856, 5644, 10638);
UPDATE `creature_template` SET `AIName`='' WHERE `entry`=11681;
DELETE FROM `smart_scripts` WHERE `entryorguid`=11681;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry`=80208;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 80208, 0, 0, 18, 0, 1, 42940, 0, 0, 0, 0, '', 'Spray Water can hit only Northshire Vineyards Fire Trigger');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=1 AND `SourceGroup`=448 AND `SourceEntry`=1307;
DELETE FROM `conditions` WHERE  `SourceTypeOrReferenceId`=1 AND `SourceGroup`=215 AND `SourceEntry`=2794;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=17 AND `SourceEntry`=60089;
DELETE FROM `achievement_reward` WHERE `entry`=1683;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`, `item`, `spell`, `sender`, `subject`, `text`) VALUES
(1683, 133, 133, 0, 0, 0, '', '');
DELETE FROM `gameobject` WHERE `guid`=88172;
DELETE FROM `spell_area` WHERE `spell`=114611;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(114611, 5834, 29408, 0, 0, 0, 2, 1, 10, 11);

DELETE FROM `smart_scripts` WHERE  `entryorguid`=53566 AND `id`=9;

UPDATE `creature_loot_template` SET `ChanceOrQuestChance`=-100 WHERE `entry`=54631 AND `item`=72112;

UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=209326;

UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=209973;

UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=210986;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=202648;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=39337;

DELETE FROM `smart_scripts` WHERE `entryorguid`=-143353;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-143353, 0, 1, 2, 61, 0, 100, 1, 0, 0, 0, 0, 0, 33, 39336, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward Plainstrider - killcredit'),
(-143353, 0, 2, 0, 61, 0, 100, 1, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward plainstrider despawn'),
(-143353, 0, 0, 1, 2, 0, 100, 1, 0, 30, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, -94.2799, -3748.97, 16.8539, 5.6, 'Wayward Plainstrider - Move to pos');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-143358;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-143358, 0, 1, 2, 61, 0, 100, 1, 0, 0, 0, 0, 0, 33, 39336, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward Plainstrider - killcredit'),
(-143358, 0, 2, 0, 61, 0, 100, 1, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward plainstrider despawn'),
(-143358, 0, 0, 1, 2, 0, 100, 1, 0, 30, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 38.46, -3770.507, 17.882, 5.68, 'Wayward Plainstrider - Move to pos');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-143365;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-143365, 0, 1, 2, 61, 0, 100, 1, 0, 0, 0, 0, 0, 33, 39336, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward Plainstrider - killcredit'),
(-143365, 0, 2, 0, 61, 0, 100, 1, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward plainstrider despawn'),
(-143365, 0, 0, 1, 2, 0, 100, 1, 0, 30, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 38.46, -3770.507, 17.882, 5.68, 'Wayward Plainstrider - Move to pos');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-143364;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-143364, 0, 1, 2, 61, 0, 100, 1, 0, 0, 0, 0, 0, 33, 39336, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward Plainstrider - killcredit'),
(-143364, 0, 2, 0, 61, 0, 100, 1, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward plainstrider despawn'),
(-143364, 0, 0, 1, 2, 0, 100, 1, 0, 30, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 38.46, -3770.507, 17.882, 5.68, 'Wayward Plainstrider - Move to pos');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-143385;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-143385, 0, 1, 2, 61, 0, 100, 1, 0, 0, 0, 0, 0, 33, 39336, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward Plainstrider - killcredit'),
(-143385, 0, 2, 0, 61, 0, 100, 1, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward plainstrider despawn'),
(-143385, 0, 0, 1, 2, 0, 100, 1, 0, 30, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 38.46, -3770.507, 17.882, 5.68, 'Wayward Plainstrider - Move to pos');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-141389;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-141389, 0, 1, 2, 61, 0, 100, 1, 0, 0, 0, 0, 0, 33, 39336, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward Plainstrider - killcredit'),
(-141389, 0, 2, 0, 61, 0, 100, 1, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward plainstrider despawn'),
(-141389, 0, 0, 1, 2, 0, 100, 1, 0, 30, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 220.85, -3758.12345, 16.33, 4.82, 'Wayward Plainstrider - Move to pos');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-143391;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-143391, 0, 1, 2, 61, 0, 100, 1, 0, 0, 0, 0, 0, 33, 39336, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward Plainstrider - killcredit'),
(-143391, 0, 2, 0, 61, 0, 100, 1, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward plainstrider despawn'),
(-143391, 0, 0, 1, 2, 0, 100, 1, 0, 30, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 278.54, -3760.343, 16.30, 4.61, 'Wayward Plainstrider - Move to pos');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-143389;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-143389, 0, 1, 2, 61, 0, 100, 1, 0, 0, 0, 0, 0, 33, 39336, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward Plainstrider - killcredit'),
(-143389, 0, 2, 0, 61, 0, 100, 1, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward plainstrider despawn'),
(-143389, 0, 0, 1, 2, 0, 100, 1, 0, 30, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 409.677, -3824.360, 21.06, 1.62, 'Wayward Plainstrider - Move to pos');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-143407;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-143407, 0, 1, 2, 61, 0, 100, 1, 0, 0, 0, 0, 0, 33, 39336, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward Plainstrider - killcredit'),
(-143407, 0, 2, 0, 61, 0, 100, 1, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward plainstrider despawn'),
(-143407, 0, 0, 1, 2, 0, 100, 1, 0, 30, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 574.526, -3774.149, 16.82, 4.68, 'Wayward Plainstrider - Move to pos');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-141362;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-141362, 0, 1, 2, 61, 0, 100, 1, 0, 0, 0, 0, 0, 33, 39336, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward Plainstrider - killcredit'),
(-141362, 0, 2, 0, 61, 0, 100, 1, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward plainstrider despawn'),
(-141362, 0, 0, 1, 2, 0, 100, 1, 0, 30, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 574.526, -3774.149, 16.82, 4.68, 'Wayward Plainstrider - Move to pos');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-141396;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-141396, 0, 1, 2, 61, 0, 100, 1, 0, 0, 0, 0, 0, 33, 39336, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward Plainstrider - killcredit'),
(-141396, 0, 2, 0, 61, 0, 100, 1, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward plainstrider despawn'),
(-141396, 0, 0, 1, 2, 0, 100, 1, 0, 30, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 663.7949, -3811.1398, 20.27, 1.62, 'Wayward Plainstrider - Move to pos');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-141408;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-141408, 0, 1, 2, 61, 0, 100, 1, 0, 0, 0, 0, 0, 33, 39336, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward Plainstrider - killcredit'),
(-141408, 0, 2, 0, 61, 0, 100, 1, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward plainstrider despawn'),
(-141408, 0, 0, 1, 2, 0, 100, 1, 0, 30, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 772.607, -3785.005, 16.62, 4.52, 'Wayward Plainstrider - Move to pos');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-141409;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-141409, 0, 1, 2, 61, 0, 100, 1, 0, 0, 0, 0, 0, 33, 39336, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward Plainstrider - killcredit'),
(-141409, 0, 2, 0, 61, 0, 100, 1, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward plainstrider despawn'),
(-141409, 0, 0, 1, 2, 0, 100, 1, 0, 30, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 772.607, -3785.005, 16.62, 4.52, 'Wayward Plainstrider - Move to pos');
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=39385;

DELETE FROM `smart_scripts` WHERE `entryorguid`=39385;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(39385, 0, 0, 0, 0, 0, 100, 0, 2500, 3000, 17500, 18000, 0, 11, 13443, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Screamslash - IC - Cast Rend');


UPDATE `creature_template` SET `InhabitType`= 4 WHERE `entry`= 54567;
UPDATE `creature_addon` SET `bytes2`= 1, `emote` = 512 WHERE `guid`= 224522;
UPDATE `creature` SET `spawndist`= 0, `MovementType` = 0 WHERE `guid`= 224426;
DELETE FROM `achievement_reward` WHERE `entry`=6116;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`) VALUES
(6116, 285, 285);
DELETE FROM `achievement_reward` WHERE `entry`=6169;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`, `item`, `spell`, `sender`, `subject`, `text`) VALUES
(6169, 0, 0, 77068, 0, 34337, 'Recovered item', 'We recovered a lost item in the twisting nether and noted that it was yours. Please find said object enclosed.');
DELETE FROM `achievement_reward` WHERE `entry`=6177;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`) VALUES
(6177, 287, 287);
DELETE FROM `achievement_reward` WHERE `entry`=6375;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`, `item`, `spell`, `sender`, `subject`, `text`) VALUES
(6375, 0, 0, 90045, 0, 66973, 'In Recognition of Your Valiant Deeds', 'Congratulations,  Conquering the mighty challenges put before you is a laudable achievement, and in recognition of this mighty deed we entrust to you the egg of an Ancestral Phoenix. Visit me in the Vale of Eternal Blossoms and choose your boon companion.');
DELETE FROM `achievement_reward` WHERE `entry`=7934;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`, `item`, `spell`, `sender`, `subject`, `text`) VALUES
(7934, 0, 0, 93031, 0, 15990, 'Mr. Bigglesworth', 'Congrats!');
DELETE FROM `achievement_reward` WHERE `entry`=8397;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`) VALUES
(8397, 377, 365);
DELETE FROM `achievement_reward` WHERE `entry`=8482;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`) VALUES
(8482, 384, 384);
DELETE FROM `gossip_menu` WHERE `entry` = 13046;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES (13046, 18321);

DELETE FROM `gossip_menu_option` WHERE `menu_id` = 13046;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES (13046, 0, 2, 'I\'d like to travel by air.', 4, 8192, 0, 0, 0, 0, NULL);DELETE FROM `gossip_menu` WHERE `entry` = 13046;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES (13046, 18321);

DELETE FROM `gossip_menu_option` WHERE `menu_id` = 13046;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES (13046, 0, 2, 'I\'d like to travel by air.', 4, 8192, 0, 0, 0, 0, NULL);DELETE FROM `gossip_menu` WHERE `entry` = 11777;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES (11777, 16509);

DELETE FROM `gossip_menu_option` WHERE `menu_id` = 11777;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES (11777, 0, 2, 'Show me where I can fly.', 4, 8192, 0, 0, 0, 0, '');
DELETE FROM `item_template` WHERE `entry`=122199;
DELETE FROM `item_template` WHERE `entry`=120954;
DELETE FROM `item_template` WHERE `entry`=120950;