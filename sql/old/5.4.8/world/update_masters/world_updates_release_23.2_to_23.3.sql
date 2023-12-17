UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=54135;
DELETE FROM `smart_scripts` WHERE `entryorguid`=54135;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(54135, 0, 0, 0, 19, 0, 100, 0, 29421, 0, 0, 0, 0, 44, 169, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Master Li Fei - On quest accept - unset phaseid 169'),
(54135, 0, 1, 0, 19, 0, 100, 0, 29421, 0, 0, 0, 0, 44, 631, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Master Li Fei - On quest accept - set phaseid 631'),
(54135, 0, 2, 0, 19, 0, 100, 0, 29422, 0, 0, 0, 0, 44, 631, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Master Li Fei - On quest accept - unset phaseid 631'),
(54135, 0, 3, 0, 19, 0, 100, 0, 29422, 0, 0, 0, 0, 44, 632, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Master Li Fei - On quest accept - set phaseid 632');

UPDATE `creature` SET `phaseId`=631 WHERE `guid`=224847;

UPDATE `gameobject` SET `phaseId`=631 WHERE `guid`=88310;
UPDATE `gameobject` SET `phaseId`=631 WHERE `guid`=88311;
UPDATE `gameobject` SET `phaseId`=631 WHERE `guid`=88306;
UPDATE `gameobject` SET `phaseId`=631 WHERE `guid`=88308;
UPDATE `gameobject` SET `phaseId`=631 WHERE `guid`=88314;
UPDATE `gameobject` SET `phaseId`=631 WHERE `guid`=88309;
UPDATE `gameobject` SET `phaseId`=631 WHERE `guid`=88312;

UPDATE `creature_template` SET `Health_mod`=36 WHERE `entry`=54734;

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=54734;
DELETE FROM `smart_scripts` WHERE `entryorguid`=54734;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(54734, 0, 0, 0, 0, 0, 0, 0, 4000, 4500, 10000, 10500, 0, 11, 108958, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Master Li Fei - IC - Feet of Fury'),
(54734, 0, 1, 0, 0, 0, 0, 0, 5000, 6000, 11000, 12000, 0, 11, 108944, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Master Li Fei - IC - Flying Shadow Kick'),
(54734, 0, 2, 0, 2, 0, 100, 0, 1, 20, 0, 0, 0, 33, 54734, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Master Li Fei - at 20% health - give credit'),
(54734, 0, 3, 0, 2, 0, 100, 0, 1, 20, 0, 0, 0, 44, 631, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Master Li Fei - at 20% health - unset phaseid 631'),
(54734, 0, 4, 0, 2, 0, 100, 0, 1, 20, 0, 0, 0, 44, 632, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Master Li Fei - at 20% health - set phaseid 632'),
(54734, 0, 5, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Master Li Fei - on link - despawn');

DELETE FROM `creature` WHERE `guid`=257979;

SET @CGUID := 257982;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID-2 AND @CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID-0, 54135, 860, 1, 632, 0, 0, 0, 1331.78, 3942.8, 110.606, 6.00393, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-1, 57779, 860, 1, 632, 0, 0, 0, 1257.89, 3925.59, 127.856, 0.506145, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-2, 54787, 860, 1, 632, 0, 0, 0, 1259.8, 3926.66, 128.28, 0.453786, 120, 0, 0, 0, 0, 0, 0, 0, 0);

SET @OGUID := 100016;
DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID-1 AND @OGUID-0;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(@OGUID-0, 209369, 860, 1, 631, 0, 1333.55, 3945.18, 110.544, 0.488691, 0, 0, 0.241921, 0.970296, 120, 255, 1),
(@OGUID-1, 209369, 860, 1, 632, 0, 1333.55, 3945.18, 110.544, 0.488691, 0, 0, 0.241921, 0.970296, 120, 255, 1);

UPDATE `gameobject` SET `phaseId`=632 WHERE `guid`=88305;
UPDATE `gameobject` SET `phaseId`=632 WHERE `guid`=88313;
UPDATE `gameobject` SET `phaseId`=632 WHERE `guid`=88307;
DELETE FROM `areatrigger_scripts` WHERE `entry`=7835;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(7835, 'at_temple_of_five_dawns');
UPDATE `quest_template` SET `OfferRewardText`='You\'ve got some fight in you! I think I like you!' WHERE `Id`=29417;
UPDATE `quest_template` SET `RequestItemsText`='When opportunity rises, it is our responsibility to reach out and grab it.Second guessing is as good as giving up.' WHERE `Id`=29523;
UPDATE `quest_template` SET `RequestItemsText`='I hope you\'re not shy about getting your hands dirty.' WHERE `Id`=29418;
UPDATE `quest_template` SET `OfferRewardText`='$n! You\'ve returned, roots in hand! You\'re a down-to-business kind of $G guy:gal;, aren\'t you? I like that!' WHERE `Id`=29418;
UPDATE `quest_template` SET `OfferRewardText`='You know what it is to seize opportunity. I think you and I are kindred spirits, $n.' WHERE `Id`=29523;
UPDATE `quest_template` SET `OfferRewardText`='Life isn\'t about finding yourself. Life is about creating yourself. There is a path before you, but you choose the trials you will face, and the trials you will overcome.' WHERE `Id`=29420;
UPDATE `quest_template` SET `RequestItemsText`='You are not yet ready to face me. First, you must light the braziers.' WHERE `Id`=29664;
UPDATE `quest_template` SET `OfferRewardText`='These fires will give you the strength that you\'ve not yet obtained. They will illuminate your potential.' WHERE `Id`=29664;
UPDATE `quest_template` SET `OfferRewardText`='Remember always, the superior warrior is modest in his speech, but exceeds in his actions.' WHERE `Id`=29421;
UPDATE `quest_template` SET `RequestItemsText`='<Huo rests quietly within the pool.>' WHERE `Id`=29422;
UPDATE `quest_template` SET `OfferRewardText`='<Through the glare, it looks like Huo is smiling. He seems to have warmed up to you.>' WHERE `Id`=29422;
UPDATE `quest_template` SET `OfferRewardText`='$n, you\'ve done very well. Huo, the spirit of fire, is here. The temple flames are reignited. You accomplished the challenge I put before you, and with seeming ease.$B$BThere is a greater purpose in bringing Huo to the temple. It is time that I tell you more of things to come.' WHERE `Id`=29423;
UPDATE `quest_template` SET `OfferRewardText`='It\'s good to see you aggain, $n.$B$BThis is the pool of the skunk, as I\'m sure you noticed.$B$BOver the many ages of Shen-zin Su, animals have died in some of these magical pools. Through their deaths, their spirits were infused into the waters, and anyone touching those waters will take their form.$B$BThere are several cursed pools here, some more dangerous than others.' WHERE `Id`=29521;
UPDATE `quest_template` SET `RequestItemsText`='Telling you of my strength means nothing. I must show you.' WHERE `Id`=29662;
UPDATE `quest_template` SET `RequestItemsText`='Have you done as I asked of you?' WHERE `Id`=29661;
UPDATE `quest_template` SET `RequestItemsText`='Only through discipline can we find harmony. Only through harmony can we reach our true potential as warriors.' WHERE `Id`=29663;
UPDATE `quest_template` SET `OfferRewardText`='Even now, your skills increase. Your footing becomes more sure. Your blows strike with more force.$B$BDiscipline and practice are the keys to reaching our full potential.' WHERE `Id`=29663;
UPDATE `quest_template` SET `OfferRewardText`='Good, You\'re a quick study.' WHERE `Id`=29661;
UPDATE `quest_template` SET `OfferRewardText`='Good.$B$BNo number of reeds have ever withstood my might.$B$BStep back, and let me dispel your doubts.' WHERE `Id`=29662;

UPDATE `quest_template` SET `PrevQuestId`=29662 WHERE `Id`=29676;

UPDATE `quest_template` SET `OfferRewardText`='Visitors are rare up here, but you are welcome. It is nice to meet you, young $c.' WHERE `Id`=29676;
UPDATE `quest_template` SET `RequestItemsText`='Balance is a delicate thing. It leans strongly on the wisdom of the elders. The lessons they preach are not always apparent to younger mind, so you must trust that their knowledge of the past will help us preserve the present.' WHERE  `Id`=29666;
UPDATE `quest_template` SET `RequestItemsText`='The decision to not return to the pools was a hard one. I don\'t think Shu ever truly understood. He does not know what it is to get older and weaker.' WHERE `Id`=29677;
UPDATE `quest_template` SET `OfferRewardText`='Shu\'s desires are not so diffrent from our own. He does not want to be alone. Unwanted, unloved. He longs to be needed, and to share his life with others.$B$BKnowing that, earning his trust is not terribly difficuult. He just wants company.' WHERE `Id`=29677;
UPDATE `quest_template` SET `OfferRewardText`='The water pincers are strange creatures. They seem to be drawn to still water.' WHERE `Id`=29666;
UPDATE `quest_template` SET `OfferRewardText`='It looks like you have his attention, $n. I think he likes you.' WHERE `Id`=29678;
UPDATE `quest_template` SET `OfferRewardText`='I have to admit, that looked pretty fun from my angle.$B$BAnd you seem to have made a new friend.' WHERE `Id`=29679;
UPDATE `quest_template` SET `OfferRewardText`='$n! I wondered when I would see you again!$B$BWell, the plus side is that finding Wugou, the spirit of earth, was easy enough. Waking him... not so much. I think he\'s going to wake quite sore from all the pushing and hitting I\'ve done trying to rouse him.' WHERE `Id`=29680;UPDATE `sf_world`.`quest_template` SET `RequestItemsText`='We need to kill them down to the very last one! We can leave that one alive I suppose, but if there\'s more than one, our troubles are going to start all over again!' WHERE  `Id`=29769;
UPDATE `quest_template` SET `RequestItemsText`='We need to kill them down to the very last one! We can leave that one alive I suppose, but if there\'s more than one, our troubles are going to start all over again!' WHERE `Id`=29769;
UPDATE `quest_template` SET `RequestItemsText`='Ever since Wugou, the spirit of earth, fell asleep, the dirt is drying out and it\'s getting harder and harder to get new seeds to sprout. We need to hold on to any food we can, or the island will have hard times ahead.' WHERE `Id`=29770;

UPDATE `quest_template` SET `PrevQuestId`=29521 WHERE `Id`=29663;
UPDATE `quest_template` SET `PrevQuestId`=29679, `NextQuestIdChain`=29769 WHERE `Id`=29680;
UPDATE `quest_template` SET `PrevQuestId`=29676, `NextQuestIdChain`=29678 WHERE `Id`=29666;
UPDATE `quest_template` SET `PrevQuestId`=29676, `NextQuestIdChain`=0 WHERE `Id`=29677;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (103069, 103070, 103077);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(103069, 'spell_gen_rock_jump_a'),
(103070, 'spell_gen_rock_jump_b'),
(103077, 'spell_gen_rock_jump_c');

UPDATE `gameobject_template` SET `data2`=5, `data3`=103071 WHERE `entry`=209575;
UPDATE `gameobject_template` SET `data2`=5, `data3`=103072 WHERE `entry`=209576;
UPDATE `gameobject_template` SET `data2`=5, `data3`=103078 WHERE `entry`=209577;

DELETE FROM `areatrigger_scripts` WHERE `entry`=7783;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(7783, 'at_pool_of_reflection');

DELETE FROM `spell_area` WHERE `spell`=116571;
INSERT INTO `spell_area` (`spell`, `area`, `autocast`) VALUES
(116571, 5862, 1);

SET @OGUID := 100017;
DELETE FROM `gameobject` WHERE `guid`=@OGUID-0;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@OGUID-0, 250001, 860, 1118.11, 2870.63, 92.1829, 0);
DELETE FROM `creature` WHERE `id` IN (225338, 225331, 225292);

DELETE FROM `creature_queststarter` WHERE `id`=54975 AND `quest`=29679;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(54975, 29679);

UPDATE `quest_template` SET `PrevQuestId`=29521 WHERE `Id`=29661;
UPDATE `quest_template` SET `PrevQuestId`=29678 WHERE `Id`=29679;
UPDATE `quest_template` SET `PrevQuestId`=29423 WHERE `Id`=29521;

UPDATE `quest_template` SET `OfferRewardText`='$n, that\'s the way to do it! You show up, and problems start getting solved. That\'s just one of many reasons why I like you!' WHERE `Id`=29769;
UPDATE `quest_template` SET `OfferRewardText`='Some of these have mouth-sized chunks bitten out of them! Ah well, we can still make use.$B$BThank you for your help, friend.' WHERE `Id`=29770;
UPDATE `quest_template` SET `RequestItemsText`='Have you done as I\'ve asked?' WHERE `Id`=29771;
UPDATE `quest_template` SET `OfferRewardText`='Good.$B$BMy head, it is harder than the densest wood.$B$BStep back, and let me dispel your doubts.' WHERE `Id`=29771;
UPDATE `quest_template` SET `RequestItemsText`='Well, they\'re as warmed as they\'re going to get. Do you have the mallet?' WHERE `Id`=29768;
UPDATE `quest_template` SET `OfferRewardText`='Hah, $n! Now that\'s a mallet if I\'ve ever seen one! Let\'s make some noise.' WHERE `Id`=29768;
UPDATE `quest_template` SET `OfferRewardText`='Huh... well... that\'s definitely the ringing I expected, but I was hoping for a bit more from Wugou. This guy\'s a deep sleeper, that\'s for sure.$B$BIt\'s alright, $n. I\'ve got another idea, and there\'s not much preparation needed at all - just how I like it.' WHERE `Id`=29772;
UPDATE `quest_template` SET `OfferRewardText`='YEAH!!! $n, we make quite a team. Persistence over planning, I always say!' WHERE `Id`=29774;
UPDATE `quest_template` SET `OfferRewardText`='You have done well, young student. Three of the four spirits have been returned to the temple. We are closer to the answers we seek.' WHERE `Id`=29775;

UPDATE `creature_template` SET `npcflag`=1, `gossip_menu_id`=13140 WHERE  `entry`=55556;
DELETE FROM `npc_text` WHERE `ID`=54024;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`) VALUES
(54024, '<Shu looks at you expectantly.>', '');
DELETE FROM `gossip_menu` WHERE `entry`=13140;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(13140, 54024);

UPDATE `creature_template` SET `gossip_menu_id`=13060 WHERE `entry`=54943;
DELETE FROM `npc_text` WHERE `ID`=53245;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`) VALUES
(53245, 'I came to ask the famous Aysa Cloudsinger for help, but I just can\'t bring myself to intrude on her exercises. She\'s so poised, so practiced... so beautiful.', '');
DELETE FROM `gossip_menu` WHERE `entry`=13060;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(13060, 53245);

UPDATE `creature_template` SET `npcflag`=1, `gossip_menu_id`=14617 WHERE  `entry`=64939;
DELETE FROM `npc_text` WHERE `ID`=65910;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`) VALUES
(65910, 'It is said, "Turn your face towards the sun, and the shadows will fall behind you."$b$bI hope you find many bright days ahead of you, $n!', '');
DELETE FROM `gossip_menu` WHERE `entry`=14617;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(14617, 65910);

UPDATE `creature_template` SET `gossip_menu_id`=12634 WHERE `entry`=42553;
DELETE FROM `npc_text` WHERE `ID`=50616;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`) VALUES
(50616, 'Have you seen my latest invention? It\'s going to be a hit... if it doesn\'t explode.', '');
DELETE FROM `gossip_menu` WHERE `entry`=12634;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(12634, 50616);

UPDATE `creature_template` SET `gossip_menu_id`=11614 WHERE `entry`=42317;
DELETE FROM `npc_text` WHERE `ID`=42230;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`) VALUES
(42230, 'Operation: Gnomeregan was a success. Sure, that dastardly Thermaplugg had an unforeseen trick up his sleeve, but we have him on the run! It\'s only a matter of time before Gnomeregan is cleaned up and in our hands again!', '');
DELETE FROM `gossip_menu` WHERE `entry`=11614;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(11614, 42230);
UPDATE `creature_template` SET `gossip_menu_id`=14672 WHERE `entry`=54567;
DELETE FROM `npc_text` WHERE `ID`=66068;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`) VALUES
(66068, '', '<Aysa appears to be deeply lost in her excercises.>');
DELETE FROM `gossip_menu` WHERE `entry`=14672;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(14672, 66068);

UPDATE `creature_template` SET `gossip_menu_id`=14616 WHERE `entry`=64929;
DELETE FROM `npc_text` WHERE `ID`=65901;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`) VALUES
(65901, 'Whoever said they\'d rather light a candle than curse the darkness...$b$bWell, they probably haven\'t lit as many candles as me.', '');
DELETE FROM `gossip_menu` WHERE `entry`=14616;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(14616, 65901);
UPDATE `creature_template` SET `npcflag`=1 WHERE `entry`=64929;

UPDATE `quest_template` SET `PrevQuestId`=29419 WHERE `Id`=29414;

UPDATE `creature_template` SET `npcflag`=1|2 WHERE `entry`=64929;
UPDATE `creature_template` SET `ScriptName`='npc_master_shang_xi_temple' WHERE `entry`=54786;
DELETE FROM `creature_text` WHERE `entry` IN (54567, 54943);
INSERT INTO `creature_text` (`entry`, `text`, `type`, `probability`) VALUES
(54567, 'A challenge in compassion and humility. By helping Merchant Lorvo, it would seem you\'ve passed, $c. Master Shang would be pleased.', 12, 100),
(54943, 'Shhhh! Come quietly. She\'s practicing.', 12, 100);

DELETE FROM `areatrigger_scripts` WHERE `entry`=7748;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(7748, 'at_fus_pond');
SET @CGUID := 224620;
DELETE FROM `creature` WHERE `guid`=@CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES 
(@CGUID-0, 54608, 860, 1168.2552, 3442.3281, 104.86952, 3.450217962265014648);

DELETE FROM `creature` WHERE `guid`=224621;
DELETE FROM `creature_text` WHERE `entry`=54568;
INSERT INTO `creature_text` (`entry`, `text`, `type`, `probability`) VALUES
(54568, 'Take that monkeybutt!', 12, 100);

DELETE FROM `areatrigger_scripts` WHERE `entry`=7749;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(7749, 'at_wu_song_village');
DELETE FROM `areatrigger_scripts` WHERE `entry`=7750;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(7750, 'at_dawning_valley');

DELETE FROM `creature_text` WHERE `entry`=60248;
INSERT INTO `creature_text` (`entry`, `text`, `type`, `probability`) VALUES
(60248, 'Is that... is that Huo?', 12, 100);

DELETE FROM `creature_text` WHERE `entry`=60253;
INSERT INTO `creature_text` (`entry`, `text`, `type`, `probability`) VALUES
(60253, 'It is! Well done, $p!', 12, 100);
DELETE FROM `areatrigger_scripts` WHERE `entry`=7747;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(7747, 'at_dawning_valley2');

DELETE FROM `creature_text` WHERE `entry`=60244;
INSERT INTO `creature_text` (`entry`, `text`, `type`, `probability`) VALUES
(60244, 'You\'re departing so soon?  I am envious of your prowess, $c. Good luck to you.', 12, 100);
DELETE FROM `achievement_reward` WHERE `entry`=5860;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`, `item`, `spell`, `sender`, `subject`, `text`) VALUES
(5860, 0, 0, 69838, 0, 40578, 'Please... Just Take It', 'Ever since his brother was taken away. This little creature has been nothing short of a handful. The guardians and I have done everything we can to contain his energy, but our efforts have fallen depressingly short. It is now clear.$b$bHe wants to be with his brother.$b$bIt took us nearly an hour to get him contained. Please, take him with you. Perhaps some time outside of this cavern will do him good.$b$b-Farden Talonshrike');
DELETE FROM `achievement_reward` WHERE `entry`=5859;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`, `item`, `spell`, `sender`, `subject`, `text`) VALUES
(5859, 0, 0, 69854, 0, 52825, 'Left Behind', 'I found this locket in the Igneous Depths, near the lava\'s edge. It is not mine, and I know that it does not belong to either of the Arch Druids. Perhaps it belonged to one of the fire druids.$b$bNo matter who its owner, I have no use for it. Please take it as a small token of our appreciation for all you\'ve done for us.');
DELETE FROM `achievement_reward` WHERE `entry`=5866;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`, `item`, `spell`, `sender`, `subject`, `text`) VALUES
(5866, 0, 0, 69213, 0, 52135, 'A Gift From the Arch Druid', 'Never again will Hyjal shudder beneath the endless forces of flame. Never again shall the mortal races of the world tremble before the minions of Ragnaros.$b$bNow, thanks to your efforts, they will tremble before us.$b$bYour heroism on the Molten Front has been instrumental in the fight against the Firelord. You have gathered allies from all over Kalimdor and turned them into an army. You have fought the minions of Ragnaros... And prevailed. And for that, you have our deepest thanks.');
DELETE FROM `achievement_reward` WHERE `entry`=5449;
INSERT INTO `achievement_reward` (`entry`, `title_A`, `title_H`, `item`, `spell`, `sender`, `subject`, `text`) VALUES
(5449, 0, 0, 60869, 0, 34337, 'Recovered Item', 'We recovered a lost item in the Twisting Nether and noted that it was yours.$b$bPlease find said object enclosed.');
