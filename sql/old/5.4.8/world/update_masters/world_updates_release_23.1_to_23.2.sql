-- Singing Moth SAI
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=59785;
DELETE FROM `smart_scripts` WHERE `entryorguid`=59785;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(59785, 0, 1, 0, 0, 0, 100, 0, 4500, 5000, 14500, 15000, 11, 116034, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Singing Moth - In Combat - Cast Lullaby');
-- Thunderfist Gorilla SAI
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=59742;
DELETE FROM `smart_scripts` WHERE `entryorguid`=59785;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(59742, 0, 1, 0, 0, 0, 100, 0, 4000, 5000, 8000, 9000, 11, 116013, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Thunderfist Gorilla - In Combat - Cast Thunderfist Rage'),
(59742, 0, 2, 0, 0, 0, 100, 0, 8000, 9000, 12000, 13000, 11, 116007, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Thunderfist Gorilla - In Combat - Cast Thunderfist');
-- Jadeglow Wasp SAI
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=59768;
DELETE FROM `smart_scripts` WHERE `entryorguid`=59768;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(59768, 0, 1, 0, 0, 0, 100, 0, 3000, 3500, 13000, 13500, 11, 116010, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Jadeglow Wasp - In Combat - Cast Jadeglow Poison');
-- Bamboo Python SAI
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=59769;
DELETE FROM `smart_scripts` WHERE `entryorguid`=59769;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(59769, 0, 1, 0, 0, 0, 100, 0, 4000, 4500, 10000, 10500, 11, 116026, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Bamboo Python - In Combat - Cast Swamp Fever');
UPDATE `creature_template` SET `ScriptName`='npc_darkspear_jailor' WHERE `entry`=39062;
-- The Basics: Hitting Things (Monk)
UPDATE `quest_template` SET `RequiredClasses`=512, `PrevQuestId`=31159 WHERE `Id`=31158;

-- A Rough Start (Monk)
UPDATE `quest_template` SET `RequiredClasses`=512, `PrevQuestId`=31158 WHERE `Id`=31160;

-- Proving Pit (Monk)
UPDATE `quest_template` SET `RequiredClasses`=512, `PrevQuestId`=31160, `NextQuestIdChain`=31162 WHERE `Id`=31161;

-- The Arts of a Monk
UPDATE `quest_template` SET `ZoneOrSort`=368, `Type`=0, `RequiredClasses`=512, `PrevQuestId`=31161, `NextQuestIdChain`=31163 WHERE `Id`=31162;
DELETE FROM `creature_queststarter` WHERE `quest`=31162;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(63310, 31162);

-- More then Expected (Monk)
UPDATE `quest_template` SET `RequiredClasses`=512, `PrevQuestId`=31162 WHERE `Id`=31163;
DELETE FROM `playercreateinfo_spell` WHERE `Spell`=100780;
INSERT INTO `playercreateinfo_spell` (`racemask`, `classmask`, `Spell`, `Note`) VALUES
(0, 512, 100780, 'Jab');

DELETE FROM `playercreateinfo_action` WHERE `action`=100780;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES 
(1, 10, 0, 100780, 0),
(2, 10, 0, 100780, 0),
(3, 10, 0, 100780, 0),
(4, 10, 0, 100780, 0),
(5, 10, 0, 100780, 0),
(6, 10, 0, 100780, 0),
(7, 10, 0, 100780, 0),
(10, 10, 0, 100780, 0),
(11, 10, 0, 100780, 0),
(24, 10, 0, 100780, 0),
(1, 10, 72, 100780, 0),
(2, 10, 72, 100780, 0),
(3, 10, 72, 100780, 0),
(4, 10, 72, 100780, 0),
(5, 10, 72, 100780, 0),
(6, 10, 72, 100780, 0),
(7, 10, 72, 100780, 0),
(10, 10, 72, 100780, 0),
(11, 10, 72, 100780, 0),
(24, 10, 72, 100780, 0);
DELETE FROM `playercreateinfo_action` WHERE `class`=10 AND `button` IN (9, 81);
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES 
(1, 10, 9, 59752, 0), -- Every Man for Himself (Racial)
(2, 10, 9, 20572, 0), -- Blood Fury (Racial) 
(3, 10, 9, 20594, 0), -- Stoneform (Racial)
(4, 10, 9, 58984, 0), -- Shadowmeld (Racial)
(5, 10, 9, 20577, 0), -- Cannibalize (Racial)
(6, 10, 9, 20549, 0), -- War Stomp (Racial)
(7, 10, 9, 20589, 0), -- Escape Artist (Racial)
(10, 10, 9, 28730, 0), -- Arcane Torrent (Racial)
(11, 10, 9, 59542, 0), -- Gift of the Naaru (Racial)
(24, 10, 9, 107079, 0), -- Quaking Palm (Racial)
(1, 10, 81, 59752, 0), -- Every Man for Himself (Racial)
(2, 10, 81, 20572, 0), -- Blood Fury (Racial) 
(3, 10, 81, 20594, 0), -- Stoneform (Racial)
(4, 10, 81, 58984, 0), -- Shadowmeld (Racial)
(5, 10, 81, 20577, 0), -- Cannibalize (Racial)
(6, 10, 81, 20549, 0), -- War Stomp (Racial)
(7, 10, 81, 20589, 0), -- Escape Artist (Racial)
(10, 10, 81, 28730, 0), -- Arcane Torrent (Racial)
(11, 10, 81, 59542, 0), -- Gift of the Naaru (Racial)
(24, 10, 81, 107079, 0); -- Quaking Palm (Racial)

-- Gnome Escape Artist
DELETE FROM `playercreateinfo_action` WHERE `action`=20589;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES 
(7, 1, 9, 20589, 0),
(7, 1, 81, 20589, 0),
(7, 4, 9, 20589, 0),
(7, 5, 9, 20589, 0),
(7, 6, 9, 20589, 0),
(7, 8, 9, 20589, 0),
(7, 9, 9, 20589, 0),
(7, 10, 9, 20589, 0),
(7, 10, 81, 20589, 0);

-- Warrior Auto Cast Battle Stance 
DELETE FROM `playercreateinfo_spell_cast` WHERE `racemask`=0 AND `classmask`=1 AND `Spell`=2457;
INSERT INTO `playercreateinfo_spell_cast` (`racemask`, `classmask`, `Spell`, `Note`) VALUES
(0, 1, 2457, 'Warrior - Battle Stance');

-- Warrior Auto Attack
DELETE FROM `playercreateinfo_action` WHERE  `class`=1 AND `action`=6603;
DELETE FROM `playercreateinfo_action` WHERE  `class`=1 AND `action`=88163;
DELETE FROM `playercreateinfo_action` WHERE  `class`=1 AND `button` IN (108, 96, 84);
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(1, 1, 0, 88163, 0),
(2, 1, 0, 88163, 0),
(3, 1, 0, 88163, 0),
(4, 1, 0, 88163, 0),
(5, 1, 0, 88163, 0),
(6, 1, 0, 88163, 0),
(7, 1, 0, 88163, 0),
(8, 1, 0, 88163, 0),
(9, 1, 0, 88163, 0),
(10, 1, 0, 88163, 0),
(11, 1, 0, 88163, 0),
(22, 1, 0, 88163, 0),
(24, 1, 0, 88163, 0),
(1, 1, 72, 88163, 0),
(2, 1, 72, 88163, 0),
(3, 1, 72, 88163, 0),
(4, 1, 72, 88163, 0),
(5, 1, 72, 88163, 0),
(6, 1, 72, 88163, 0),
(7, 1, 72, 88163, 0),
(8, 1, 72, 88163, 0),
(9, 1, 72, 88163, 0),
(10, 1, 72, 88163, 0),
(11, 1, 72, 88163, 0),
(22, 1, 72, 88163, 0),
(24, 1, 72, 88163, 0);

-- Warrior Heroic Strike
DELETE FROM `playercreateinfo_spell` WHERE `Spell`=78;
INSERT INTO `playercreateinfo_spell` (`classmask`, `Spell`, `Note`) VALUES
(1, 78, 'Heroic Strike');

DELETE FROM `playercreateinfo_action` WHERE `action`=88161;
DELETE FROM `playercreateinfo_action` WHERE `action`=78;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
 (1, 1, 1, 78, 0),
 (2, 1, 1, 78, 0),
 (3, 1, 1, 78, 0),
 (4, 1, 1, 78, 0),
 (5, 1, 1, 78, 0),
 (6, 1, 1, 78, 0),
 (7, 1, 1, 78, 0),
 (8, 1, 1, 78, 0),
 (9, 1, 1, 78, 0),
 (10, 1, 1, 78, 0),
 (11, 1, 1, 78, 0),
 (22, 1, 1, 78, 0),
 (1, 1, 73, 78, 0),
 (2, 1, 73, 78, 0),
 (3, 1, 73, 78, 0),
 (4, 1, 73, 78, 0),
 (5, 1, 73, 78, 0),
 (6, 1, 73, 78, 0),
 (7, 1, 73, 78, 0),
 (8, 1, 73, 78, 0),
 (9, 1, 73, 78, 0),
 (10, 1, 73, 78, 0),
 (11, 1, 73, 78, 0),
 (22, 1, 73, 78, 0);

-- Warrior Racials
DELETE FROM `playercreateinfo_action` WHERE `class`=1 AND `button` IN (9, 81);
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES 
(1, 1, 9, 59752, 0), -- Every Man for Himself (Racial)
(2, 1, 9, 20572, 0), -- Blood Fury (Racial) 
(3, 1, 9, 20594, 0), -- Stoneform (Racial)
(4, 1, 9, 58984, 0), -- Shadowmeld (Racial)
(5, 1, 9, 20577, 0), -- Cannibalize (Racial)
(6, 1, 9, 20549, 0), -- War Stomp (Racial)
(7, 1, 9, 20589, 0), -- Escape Artist (Racial)
(8, 1, 9, 26297, 0), -- Berserking (Racial)
(9, 1, 9, 69070, 0), -- Rocket Jump (Racial)
(10, 1, 9, 28730, 0), -- Arcane Torrent (Racial)
(11, 1, 9, 59542, 0), -- Gift of the Naaru (Racial)
(22, 1, 9, 68992, 0), -- Darkflight (Racial)
(24, 1, 9, 107079, 0), -- Quaking Palm (Racial)
(1, 1, 81, 59752, 0), -- Every Man for Himself (Racial)
(2, 1, 81, 20572, 0), -- Blood Fury (Racial) 
(3, 1, 81, 20594, 0), -- Stoneform (Racial)
(4, 1, 81, 58984, 0), -- Shadowmeld (Racial)
(5, 1, 81, 20577, 0), -- Cannibalize (Racial)
(6, 1, 81, 20549, 0), -- War Stomp (Racial)
(7, 1, 81, 20589, 0), -- Escape Artist (Racial)
(8, 1, 81, 26297, 0), -- Berserking (Racial)
(9, 1, 81, 69070, 0), -- Rocket Jump (Racial)
(10, 1, 81, 28730, 0), -- Arcane Torrent (Racial)
(11, 1, 81, 59542, 0), -- Gift of the Naaru (Racial)
(22, 1, 81, 68992, 0), -- Darkflight (Racial)
(24, 1, 81, 107079, 0); -- Quaking Palm (Racial)

-- The Lesson of the Iron Bough - RequestItemsText
UPDATE `quest_template` SET `RequestItemsText`='Picking up your first weapon is also a symbolic gesture. Every $c that leaves these training grounds must be ready to take up arms in defense of their fellow $r.' WHERE  `Id` IN (30027, 30033, 30034, 30035, 30036, 30037, 30038);
-- The Lesson of the Sandy Fist - RequestItemsText
UPDATE `quest_template` SET `RequestItemsText`='The training targets are just down the hill. Come back to me once you\'re finished.' WHERE `Id`=29406;
-- The Lesson of the Sandy Fist - OfferRewardText
UPDATE `quest_template` SET `OfferRewardText`='Not bad, $n. You may indeed have chosen the proper path.' WHERE `Id`=29406;
-- The Lesson of Stifled Pride - RequestItemsText
UPDATE`quest_template` SET `RequestItemsText`='There is a time for meditation and contemplation, and there is a time for action. Diving the balance between the two is one of the greatest wisdoms we seek.' WHERE `Id`=29524;

-- The Lesson of Stifled Pride - OfferRewardText
UPDATE `quest_template` SET `OfferRewardText`='Intriguing, my young pupil.\r\n\r\nMost of the other trainees have been here for quite some time, but you are able to match them even within this first hour.\r\n\r\nThis speaks well of you, but there are yet other lessons I would see you learn.' WHERE `Id`=29524;

-- The Lesson of the Burning Scroll - RequestItemsText
UPDATE `quest_template` SET `RequestItemsText`='There will come time when you must trust your instincts and simply act.' WHERE `Id`=29408;

-- The Lesson of the Burning Scroll - OfferRewardText
UPDATE `quest_template` SET `OfferRewardText`='The fact that you were able to snatch the flame so easily is no small feat.\r\n\r\nThe Edict of Temperance is a scroll of wisdom passed down from my elders\' elders. Wisdom from a more peaceful time.\r\n\r\nEvery lesson has its time and place, and with darkness on the horizon, the time for this particular wisdom has passed.\r\n\r\nThe burning of the scroll is an acceptance of tidings to come and a promise to action. Your hand carried the flame, and i suspect that it will continue to do so in the future.' WHERE `Id`=29408;
UPDATE `quest_template` SET `SourceItemId`=0 WHERE `Id`=29408;
UPDATE `gameobject_template` SET `unkInt32`=0 WHERE `entry`=210986;

DELETE FROM `smart_scripts` WHERE `entryorguid`=53566 AND `id`=9;
INSERT INTO `smart_scripts` (`entryorguid`, `id`, `event_type`, `event_param1`, `action_type`, `action_param1`, `target_type`, `comment`) VALUES
(53566, 9, 19, 29408, 11, 114611, 7, 'Master Shang Xi: On quest accept 29408 - cast spell 114611');
-- The Disciple's Challenge - RequestItemsText
UPDATE `quest_template` SET `RequestItemsText`='Our greatest glory is not in never falling, but in getting up every time we do.\r\nJaomin Ro knows this. He finds strength in every defeat.\r\n\r\nYou may yet learn that lesson.' WHERE `Id`=29409;

-- The Disciple's Challenge - OfferRewardText
UPDATE `quest_template` SET `OfferRewardText`='You truly impress, $n. It seems certain that the path of the $c is indeed the path for you.' WHERE `Id`=29409;
-- The Rise of the Darkspear - Monk RequestItemsText
UPDATE `quest_template` SET `OfferRewardText`='Anotha newblood, eh?\r\n\r\nYa actually look ta be in shape though.\r\nSteady hands. Fierce eyes. I maybe able ta turn ya into a read $c.\r\n\r\nLets not waste anymore time \'den.\r\n' WHERE `Id`=31159;
-- The Basics: Hitting Things - Monk RequestItemsText
UPDATE `quest_template` SET `RequestItemsText`='Strength isn\'t da end all of defeating ya enemy, and it most certainly isn\'t tha end all of stayin\' alive. Fightin\' is about balance. Everytin has its time and place.' WHERE `Id`=31158;

-- The Basics: Hitting Things - Monk OfferRewardText
UPDATE `quest_template` SET `OfferRewardText`='Not bad, mon. Ya have a natural flow to ya punches - you may have a talent for dis.\r\n\r\nSome rough edges ta be sure, but we\'ll get dose ironed out here and I\'ll teach ya a few new tings as we go.' WHERE `Id`=31158;
-- The Basics: Hitting Things - Monk RequestItemsText
UPDATE `quest_template` SET `RequestItemsText`='I doubt dey too much of a challenge, mon, but they\'ll still claw up ya ankles if ya don\'t stay quick footed.' WHERE  `Id`=31160;

-- The Basics: Hitting Things - Monk OfferRewardText
UPDATE `quest_template` SET `OfferRewardText`='Dese are some fine pelts, $G boy:girl;. We could make some hefty cloaks outta dese.' WHERE `Id`=31160;

-- Proving Pit - Monk RequestItemsText
UPDATE `quest_template` SET `RequestItemsText`='All $rs gotta prove demselves before we let them move past da training grounds.' WHERE  `Id`=31161;

-- Proving Pit - Monk OfferRewardText
UPDATE `quest_template` SET `OfferRewardText`='Ya handle yerself quite well. Ya gonna be quite powerful some day.' WHERE  `Id`=31161;
-- The Lesson of the Iron Bough (Mage)
UPDATE `quest_template` SET `RequiredSourceItemId2`=76392 WHERE `Id`=30033;
UPDATE `quest_template` SET `RewardItemId2`=76392 WHERE `Id`=30033;
-- More Than Expected (Monk) Questender
DELETE FROM `creature_questender` WHERE `quest`=31163;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(38966, 31163);
-- More Than Expected - Monk OfferRewardText
UPDATE `quest_template` SET `OfferRewardText`='Ya tha new recruit everyone\'s talkin\' about, eh? Welcome ta tha Darkspear.' WHERE `Id`=31163;
UPDATE `quest_template` SET `NextQuestIdChain`=25064 WHERE `Id`=31163;
DELETE FROM `playercreateinfo_action` WHERE `race`=8 AND `class`=10 AND `button`=72;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(8, 10, 72, 100780, 0);
DELETE FROM `playercreateinfo_action` WHERE `race`=8 AND `class`=10 AND `button`=0;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(8, 10, 0, 100780, 0);
UPDATE `quest_template` SET `OfferRewardText`='Anotha newblood, eh?\r\n\r\nYa actually look ta be in shape though.\r\nSteady hands. Fierce eyes. I maybe able ta turn ya into a real $c.\r\n\r\nLets not waste anymore time \'den.\r\n' WHERE `Id`=24607;
UPDATE `quest_template` SET `OfferRewardText`='Anotha newblood, eh?\r\n\r\nYa actually look ta be in shape though.\r\nSteady hands. Fierce eyes. I maybe able ta turn ya into a real $c.\r\n\r\nLets not waste anymore time \'den.\r\n' WHERE `Id`=24750;
UPDATE `quest_template` SET `OfferRewardText`='Anotha newblood, eh?\r\n\r\nYa actually look ta be in shape though.\r\nSteady hands. Fierce eyes. I maybe able ta turn ya into a real $c.\r\n\r\nLets not waste anymore time \'den.\r\n' WHERE `Id`=24758;
UPDATE `quest_template` SET `OfferRewardText`='Anotha newblood, eh?\r\n\r\nYa actually look ta be in shape though.\r\nSteady hands. Fierce eyes. I maybe able ta turn ya into a real $c.\r\n\r\nLets not waste anymore time \'den.\r\n' WHERE `Id`=24764;
UPDATE `quest_template` SET `OfferRewardText`='Anotha newblood, eh?\r\n\r\nYa actually look ta be in shape though.\r\nSteady hands. Fierce eyes. I maybe able ta turn ya into a real $c.\r\n\r\nLets not waste anymore time \'den.\r\n' WHERE `Id`=24770;
UPDATE `quest_template` SET `OfferRewardText`='Anotha newblood, eh?\r\n\r\nYa actually look ta be in shape though.\r\nSteady hands. Fierce eyes. I maybe able ta turn ya into a real $c.\r\n\r\nLets not waste anymore time \'den.\r\n' WHERE `Id`=24782;
UPDATE `quest_template` SET `OfferRewardText`='Anotha newblood, eh?\r\n\r\nYa actually look ta be in shape though.\r\nSteady hands. Fierce eyes. I maybe able ta turn ya into a real $c.\r\n\r\nLets not waste anymore time \'den.\r\n' WHERE `Id`=26272;
UPDATE `quest_template` SET `OfferRewardText`='Anotha newblood, eh?\r\n\r\nYa actually look ta be in shape though.\r\nSteady hands. Fierce eyes. I maybe able ta turn ya into a real $c.\r\n\r\nLets not waste anymore time \'den.\r\n' WHERE `Id`=31159;

UPDATE `quest_template` SET `RequestItemsText`='Strength isn\'t da end all of defeating ya enemy, and it most certainly isn\'t tha end all of stayin\' alive. Fightin\' is about balance. Everytin has its time and place.' WHERE `Id`=24639;
UPDATE `quest_template` SET `RequestItemsText`='Strength isn\'t da end all of defeating ya enemy, and it most certainly isn\'t tha end all of stayin\' alive. Fightin\' is about balance. Everytin has its time and place.' WHERE `Id`=24751;
UPDATE `quest_template` SET `RequestItemsText`='Strength isn\'t da end all of defeating ya enemy, and it most certainly isn\'t tha end all of stayin\' alive. Fightin\' is about balance. Everytin has its time and place.' WHERE `Id`=24759;
UPDATE `quest_template` SET `RequestItemsText`='Strength isn\'t da end all of defeating ya enemy, and it most certainly isn\'t tha end all of stayin\' alive. Fightin\' is about balance. Everytin has its time and place.' WHERE `Id`=24765;
UPDATE `quest_template` SET `RequestItemsText`='Strength isn\'t da end all of defeating ya enemy, and it most certainly isn\'t tha end all of stayin\' alive. Fightin\' is about balance. Everytin has its time and place.' WHERE `Id`=24771;
UPDATE `quest_template` SET `RequestItemsText`='Strength isn\'t da end all of defeating ya enemy, and it most certainly isn\'t tha end all of stayin\' alive. Fightin\' is about balance. Everytin has its time and place.' WHERE `Id`=24783;
UPDATE `quest_template` SET `RequestItemsText`='Strength isn\'t da end all of defeating ya enemy, and it most certainly isn\'t tha end all of stayin\' alive. Fightin\' is about balance. Everytin has its time and place.' WHERE `Id`=26273;
UPDATE `quest_template` SET `RequestItemsText`='Strength isn\'t da end all of defeating ya enemy, and it most certainly isn\'t tha end all of stayin\' alive. Fightin\' is about balance. Everytin has its time and place.' WHERE `Id`=24777;

UPDATE `quest_template` SET `OfferRewardText`='Not bad, mon. Ya have a natural flow to ya punches - you may have a talent for dis.\r\n\r\nSome rough edges ta be sure, but we\'ll get dose ironed out here and I\'ll teach ya a few new tings as we go.' WHERE `Id`=24639;
UPDATE `quest_template` SET `OfferRewardText`='Not bad, mon. Ya have a natural flow to ya punches - you may have a talent for dis.\r\n\r\nSome rough edges ta be sure, but we\'ll get dose ironed out here and I\'ll teach ya a few new tings as we go.' WHERE `Id`=24751;
UPDATE `quest_template` SET `OfferRewardText`='Not bad, mon. Ya have a natural flow to ya punches - you may have a talent for dis.\r\n\r\nSome rough edges ta be sure, but we\'ll get dose ironed out here and I\'ll teach ya a few new tings as we go.' WHERE `Id`=24759;
UPDATE `quest_template` SET `OfferRewardText`='Not bad, mon. Ya have a natural flow to ya punches - you may have a talent for dis.\r\n\r\nSome rough edges ta be sure, but we\'ll get dose ironed out here and I\'ll teach ya a few new tings as we go.' WHERE `Id`=24765;
UPDATE `quest_template` SET `OfferRewardText`='Not bad, mon. Ya have a natural flow to ya punches - you may have a talent for dis.\r\n\r\nSome rough edges ta be sure, but we\'ll get dose ironed out here and I\'ll teach ya a few new tings as we go.' WHERE `Id`=24771;
UPDATE `quest_template` SET `OfferRewardText`='Not bad, mon. Ya have a natural flow to ya attacks - you may have a talent for dis.$B$BSome rough edges ta be sure, but we\'ll get dose ironed out here and I\'ll teach ya a few new tings as we go.' WHERE `Id`=24783;
UPDATE `quest_template` SET `OfferRewardText`='Not bad, mon. Ya have a natural flow to ya attacks - you may have a talent for dis.$B$BSome rough edges ta be sure, but we\'ll get dose ironed out here and I\'ll teach ya a few new tings as we go.' WHERE `Id`=26273;

UPDATE `quest_template` SET `OfferRewardText`='Dese are some fine pelts, $G boy:girl;. We could make some hefty cloaks outta dese.' WHERE `Id`=24641;
UPDATE `quest_template` SET `OfferRewardText`='Dese are some fine pelts, $G boy:girl;. We could make some hefty cloaks outta dese.' WHERE `Id`=24753;
UPDATE `quest_template` SET `OfferRewardText`='Dese are some fine pelts, $G boy:girl;. We could make some hefty cloaks outta dese.' WHERE `Id`=24761;
UPDATE `quest_template` SET `OfferRewardText`='Dese are some fine pelts, $G boy:girl;. We could make some hefty cloaks outta dese.' WHERE `Id`=24767;
UPDATE `quest_template` SET `OfferRewardText`='Dese are some fine pelts, $G boy:girl;. We could make some hefty cloaks outta dese.' WHERE `Id`=24773;
UPDATE `quest_template` SET `OfferRewardText`='Dese are some fine pelts, $G boy:girl;. We could make some hefty cloaks outta dese.' WHERE `Id`=24785;
UPDATE `quest_template` SET `OfferRewardText`='Dese are some fine pelts, $G boy:girl;. We could make some hefty cloaks outta dese.' WHERE `Id`=26275;

UPDATE `quest_template` SET `RequestItemsText`='I doubt dey too much of a challenge, mon, but they\'ll still claw up ya ankles if ya don\'t stay quick footed.' WHERE `Id`=24761;
UPDATE `quest_template` SET `RequestItemsText`='I doubt dey too much of a challenge, mon, but they\'ll still claw up ya ankles if ya don\'t stay quick footed.' WHERE `Id`=24773;
UPDATE `quest_template` SET `RequestItemsText`='I doubt dey too much of a challenge, mon, but they\'ll still claw up ya ankles if ya don\'t stay quick footed.' WHERE `Id`=24785;
UPDATE `quest_template` SET `RequestItemsText`='I doubt dey too much of a challenge, mon, but they\'ll still claw up ya ankles if ya don\'t stay quick footed.' WHERE `Id`=26275;

UPDATE `quest_template` SET `RequestItemsText`='All $rs gotta prove demselves before we let them move past da training grounds.' WHERE `Id`=24642;
UPDATE `quest_template` SET `RequestItemsText`='All $rs gotta prove demselves before we let them move past da training grounds.' WHERE `Id`=24780;
UPDATE `quest_template` SET `RequestItemsText`='All $rs gotta prove demselves before we let them move past da training grounds.' WHERE `Id`=24754;
UPDATE `quest_template` SET `RequestItemsText`='All $rs gotta prove demselves before we let them move past da training grounds.' WHERE `Id`=24762;
UPDATE `quest_template` SET `RequestItemsText`='All $rs gotta prove demselves before we let them move past da training grounds.' WHERE `Id`=24768;
UPDATE `quest_template` SET `RequestItemsText`='All $rs gotta prove demselves before we let them move past da training grounds.' WHERE `Id`=24774;
UPDATE `quest_template` SET `RequestItemsText`='All $rs gotta prove demselves before we let them move past da training grounds.' WHERE `Id`=24786;
UPDATE `quest_template` SET `RequestItemsText`='All $rs gotta prove demselves before we let them move past da training grounds.' WHERE `Id`=26276;

UPDATE `quest_template` SET `OfferRewardText`='Ya handle yerself quite well. Ya gonna be quite powerful some day.' WHERE `Id`=24642;
UPDATE `quest_template` SET `OfferRewardText`='Ya handle yerself quite well. Ya gonna be quite powerful some day.' WHERE `Id`=24754;
UPDATE `quest_template` SET `OfferRewardText`='Ya handle yerself quite well. Ya gonna be quite powerful some day.' WHERE `Id`=24762;
UPDATE `quest_template` SET `OfferRewardText`='Ya handle yerself quite well. Ya gonna be quite powerful some day.' WHERE `Id`=24768;
UPDATE `quest_template` SET `OfferRewardText`='Ya handle yerself quite well. Ya gonna be quite powerful some day.' WHERE `Id`=24774;
UPDATE `quest_template` SET `OfferRewardText`='Ya handle yerself quite well. Ya gonna be quite powerful some day.' WHERE `Id`=24786;
UPDATE `quest_template` SET `OfferRewardText`='Ya handle yerself quite well. Ya gonna be quite powerful some day.' WHERE `Id`=26276;

UPDATE `quest_template` SET `OfferRewardText`='Ya tha new recruit everyone\'s talkin\' about, eh? Welcome ta tha Darkspear.' WHERE `Id`=24643;
UPDATE `quest_template` SET `OfferRewardText`='Ya tha new recruit everyone\'s talkin\' about, eh? Welcome ta tha Darkspear.' WHERE `Id`=24755;
UPDATE `quest_template` SET `OfferRewardText`='Ya tha new recruit everyone\'s talkin\' about, eh? Welcome ta tha Darkspear.' WHERE `Id`=24763;
UPDATE `quest_template` SET `OfferRewardText`='Ya tha new recruit everyone\'s talkin\' about, eh? Welcome ta tha Darkspear.' WHERE `Id`=24769;
UPDATE `quest_template` SET `OfferRewardText`='Ya tha new recruit everyone\'s talkin\' about, eh? Welcome ta tha Darkspear.' WHERE `Id`=24775;
UPDATE `quest_template` SET `OfferRewardText`='Ya tha new recruit everyone\'s talkin\' about, eh? Welcome ta tha Darkspear.' WHERE `Id`=24787;
UPDATE `quest_template` SET `OfferRewardText`='Ya tha new recruit everyone\'s talkin\' about, eh? Welcome ta tha Darkspear.' WHERE `Id`=26277;

UPDATE `quest_template` SET `NextQuestIdChain`=25064 WHERE `Id`=24643;
UPDATE `quest_template` SET `NextQuestIdChain`=25064 WHERE `Id`=24755;
UPDATE `quest_template` SET `NextQuestIdChain`=25064 WHERE `Id`=24763;
UPDATE `quest_template` SET `NextQuestIdChain`=25064 WHERE `Id`=24769;
UPDATE `quest_template` SET `NextQuestIdChain`=25064 WHERE `Id`=24775;
UPDATE `quest_template` SET `NextQuestIdChain`=25064 WHERE `Id`=24781;
UPDATE `quest_template` SET `NextQuestIdChain`=25064 WHERE `Id`=24787;
UPDATE `quest_template` SET `NextQuestIdChain`=25064 WHERE `Id`=26277;
DELETE FROM `areatrigger_scripts` WHERE `entry`=7746;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(7746, 'at_the_dawning_valley');
UPDATE `quest_template` SET `OfferRewardText`='Another one of Kaltunk\'s recruits, hmm?$B$BA sorry state of affairs we find ourselves in if this is the best the Horde can produce. No matter. By the time we think you\'re ready to leave the Valley, you\'ll be a proud $C of the Horde.' WHERE `Id`=4641;
UPDATE `quest_template` SET `OfferRewardText`='Another one of Kaltunk\'s recruits, hmm?$b$bA sorry state of affairs we find ourselves in if this is the best the Horde can produce. No matter. By the time we think you\'re ready to leave the Valley, you\'ll be a proud warrior of the Horde.' WHERE `Id`=25152;

UPDATE `quest_template` SET `RequestItemsText`='I hope you haven\'t returned to try and convince me that you have completed your task, $n? No, of course not. I would think better of you.' WHERE `Id`=25126;

UPDATE `quest_template` SET `OfferRewardText`='Hmmm, not bad, $n. But don\'t let it get to your head...you\'ll fight tougher than boars in your career.$b$bNonetheless, you\'ve proven yourself well, and your next trial will be against a considerably more dangerous opponent, so you\'ll need some extra protection.' WHERE  `Id`=25126;

UPDATE `quest_template` SET `RequestItemsText`='Quickly, $n. Dispatch of these humans in our valley.' WHERE `Id`=25172;
UPDATE `quest_template` SET `OfferRewardText`='Executed with quickness and discretion. A job well done.' WHERE `Id`=25172;

UPDATE `quest_template` SET `RequestItemsText`='The carapace of a scorpid isn\'t so thick that the strength of a determined $C will be deterred. Strike strongly and without doubt, and the scorpids should prove easy prey.' WHERE `Id`=25127;
UPDATE `quest_template` SET `OfferRewardText`='There is an important lesson that you must take away from fighting scorpids. The smallest or largest of opponents can still send you to your doom. In fierce combat, any number of things can prove your downfall.$b$bI have no more to teach you, $n. You have done well, and I will watch your progress with interest.' WHERE `Id`=25127;

UPDATE `quest_template` SET `RequestItemsText`='To prove yourself against the Burning Blade, you must first defeat its Vile Familiars. Return to me when you have done this.' WHERE `Id`=25131;

UPDATE `quest_template` SET `RequestItemsText`='No good lazy...$b$b Eh? Do you have my blackjack? Did you catch any peons sleeping on the job?!' WHERE `Id`=25134;

UPDATE `quest_template` SET `OfferRewardText`='<Hana\'zua groans in pain.>' WHERE `Id`=25128;

UPDATE `quest_template` SET `RequestItemsText`='Ahhh... my father allways said I would never amount to much, and here, lying beneath a tree as life seeps away, I\'m afraid it looks like he was right.$b$bAt least I wish to die knowing that my last enemy in life lays dead.' WHERE `Id`=25129;
UPDATE `quest_template` SET `OfferRewardText`='My blow was not enough to kill him, but looking at the damage I inflicted gives me some small measure of pride. That small measure will be all i have to sustain myself if I die, and in that light the short list of my life\'s accomplishments fills me with anger.' WHERE `Id`=25129;

UPDATE `quest_template` SET `OfferRewardText`='From your description of the beast, I believe you must be speaking of Sarkoth! It is no wonder that Hana\'zua was overtaken by it. Aid will be dispatched to him immediately, trouble yourself no more with Hana\'zua\'s plight.$b$bHowever, I must say, I am most impressed to hear that you brought Sarkoth to death. It is a feat to be proud of, $n. And that you would fight for a stranger\'s honor, while other tasks occupied your time, your own honor is heightened.' WHERE `Id`=25130;

UPDATE `quest_template` SET `OfferRewardText`='You have done well, $n.$b$bAlthough the vile familiars were merely pets of the darker powers within the Burning Blade, your success against them foretells greater deeds ahead.' WHERE `Id`=25131;

UPDATE `quest_template` SET `OfferRewardText`='Wonderful, $n! As promised, here is your cactus apple surprise plus a little something extra.' WHERE `Id`=25136;

UPDATE `quest_template` SET `OfferRewardText`='Good. good. Maybe they\'ll think twice before slacking next time! Thanks for the help!' WHERE `Id`=25134;

UPDATE `quest_template` SET `RequestItemsText`='Is your task complete, $n? Do you have the Burning Blade Medallion?' WHERE `Id`=25132;

UPDATE `quest_template` SET `RequestItemsText`='Have you entered the Burning Blade Coven, $n? Did you find my pick?' WHERE `Id`=25135;

UPDATE `quest_template` SET `OfferRewardText`='Great, you got it! Thanks a lot, $n. This is my favorite pick! Now if my peons ever finish chopping down these trees, maybe we can find a nice cave to mine!' WHERE `Id`=25135;

UPDATE `quest_template` SET `OfferRewardText`='You retrieved it! Well done!$b$bYour efforts within the Burning Blade Coven are key in rooting out this cult in the Valley of Trials. But I fear they have further aims in our land.$b$bWe have not seen the end of them.' WHERE `Id`=25132;

UPDATE `quest_template` SET `OfferRewardText`='Hmm... your report comes at a bad time. The Burning Blade is not seen here in Sen\'jin, but their evil has taken seed off the coast, on the Echo Isles.$b$bThe orcs are friends of the Darkspear Trolls. Honorable friends. We want to help the orcs, but... we need help too.$b' WHERE `Id`=805;
UPDATE `quest_template` SET `OfferRewardText`='Hmm... your report comes at a bad time. The Burning Blade is not seen here in Sen\'jin, but their evil has taken seed off the coast, on the Echo Isles.$b$bThe orcs are friends of the Darkspear Trolls. Honorable friends. We want to help the orcs, but... we need help too.$b' WHERE `Id`=25133;

UPDATE `quest_template` SET `RewardSpellCast`=0 WHERE `Id`=29406;
UPDATE `quest_template` SET `RewardSpellCast`=0 WHERE `Id`=29524;
UPDATE `smart_scripts` SET `action_param2`=0 WHERE `entryorguid`=53566 AND `source_type`=0 AND `id`=1 AND `link`=0;
UPDATE `gameobject` SET `spawntimesecs`=900 WHERE `id`=209328; -- Rich Ghost Iron Deposit
UPDATE `gameobject` SET `spawntimesecs`=900 WHERE `id`=209311; -- Ghost Iron Deposit
UPDATE `gameobject` SET `spawntimesecs`=600 WHERE `id`=209353; -- Rain Poppy
UPDATE `gameobject` SET `spawntimesecs`=600 WHERE `id`=209349; -- Green Tea Leaf
DELETE FROM `disables` WHERE `sourceType`=1 AND `entry`=9413;
INSERT INTO `disables` (`sourceType`, `entry`, `comment`) VALUES
(1, 9413, 'Disable Quest 9413: <UNUSED>Crystal Flake Throat Lozenge');

DELETE FROM `disables` WHERE `sourceType`=1 AND `entry`=9414;
INSERT INTO `disables` (`sourceType`, `entry`, `comment`) VALUES
(1, 9414, 'Disable Quest 9414: <UNUSED>Infused Bandages');

DELETE FROM `disables` WHERE `sourceType`=1 AND `entry`=9507;
INSERT INTO `disables` (`sourceType`, `entry`, `comment`) VALUES
(1, 9507, 'Disable Quest 9507: <DEPRECATED>From the Desk of Mogul Razdunk');

DELETE FROM `disables` WHERE `sourceType`=1 AND `entry`=9613;
INSERT INTO `disables` (`sourceType`, `entry`, `comment`) VALUES
(1, 9613, 'Disable Quest 9613: Deprecated: Keanna\'s Will');

DELETE FROM `disables` WHERE `sourceType`=1 AND `entry`=9652;
INSERT INTO `disables` (`sourceType`, `entry`, `comment`) VALUES
(1, 9652, 'Disable Quest 9652: Deprecated: Physical Enhancement');

DELETE FROM `disables` WHERE `sourceType`=1 AND `entry`=9653;
INSERT INTO `disables` (`sourceType`, `entry`, `comment`) VALUES
(1, 9653, 'Disable Quest 9653: Deprecated: Physical Enhancement');

DELETE FROM `disables` WHERE `sourceType`=1 AND `entry`=9658;
INSERT INTO `disables` (`sourceType`, `entry`, `comment`) VALUES
(1, 9658, 'Disable Quest 9658: Deprecated: Offensive Channeling');

DELETE FROM `disables` WHERE `sourceType`=1 AND `entry`=9659;
INSERT INTO `disables` (`sourceType`, `entry`, `comment`) VALUES
(1, 9659, 'Disable Quest 9659: Deprecated: Healing Amplification');

DELETE FROM `disables` WHERE `sourceType`=1 AND `entry`=9661;
INSERT INTO `disables` (`sourceType`, `entry`, `comment`) VALUES
(1, 9661, 'Disable Quest 9661: Deprecated: A Well Rounded Defense');

DELETE FROM `disables` WHERE `sourceType`=1 AND `entry`=9695;
INSERT INTO `disables` (`sourceType`, `entry`, `comment`) VALUES
(1, 9695, 'Disable Quest 9695: The Sun King\'s Command');

DELETE FROM `disables` WHERE `sourceType`=1 AND `entry`=14020;
INSERT INTO `disables` (`sourceType`, `entry`, `comment`) VALUES
(1, 14020, 'Disable Quest 14020: The Curse');
DELETE FROM `game_event_gameobject` WHERE `guid`=39805 AND `eventEntry`=12;
DELETE FROM `game_event_gameobject` WHERE `guid`=39806 AND `eventEntry`=12;
DELETE FROM `game_event_gameobject` WHERE `guid`=39807 AND `eventEntry`=12;
DELETE FROM `game_event_gameobject` WHERE `guid`=39808 AND `eventEntry`=12;
DELETE FROM `game_event_gameobject` WHERE `guid`=39809 AND `eventEntry`=12;
DELETE FROM `game_event_gameobject` WHERE `guid`=39810 AND `eventEntry`=12;
DELETE FROM `game_event_gameobject` WHERE `guid`=39811 AND `eventEntry`=12;
DELETE FROM `game_event_gameobject` WHERE `guid`=39812 AND `eventEntry`=12;
DELETE FROM `game_event_gameobject` WHERE `guid`=39813 AND `eventEntry`=12;
DELETE FROM `game_event_gameobject` WHERE `guid`=39814 AND `eventEntry`=12;
DELETE FROM `game_event_gameobject` WHERE `guid`=54946 AND `eventEntry`=12;
DELETE FROM `game_event_gameobject` WHERE `guid`=54947 AND `eventEntry`=12;
DELETE FROM `game_event_gameobject` WHERE `guid`=54948 AND `eventEntry`=12;
SET @CGUID := 200218;
DELETE FROM `creature` WHERE `id`=66292 AND `map`=0;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID-74 AND @CGUID-1;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-1, 66300, 0, -7885.13, 1315.39, 338.955, 0.0183194),
(@CGUID-2, 55825, 0, -7883.68, 1263.53, 358.572, 2.05512),
(@CGUID-3, 66480, 0, -7872.8, 1304.17, 339.235, 4.73768),
(@CGUID-4, 66292, 0, -7879.8, 1279.52, 358.561, 4.72882),
(@CGUID-5, 66480, 0, -7872.37, 1303.06, 339.279, 2.0365),
(@CGUID-6, 66300, 0, -7885.17, 1317.52, 338.955, 0.0183194),
(@CGUID-7, 66300, 0, -7877.22, 1315.36, 338.947, 3.1612),
(@CGUID-8, 66473, 0, -7866.56, 1306.53, 338.965, 0.826461),
(@CGUID-9, 66300, 0, -7872.06, 1261.82, 358.562, 3.19396),
(@CGUID-10, 66300, 0, -7877.35, 1319.44, 338.943, 3.1612),
(@CGUID-11, 55819, 0, -7886.45, 1278.3, 358.559, 5.19431),
(@CGUID-12, 55823, 0, -7884.51, 1266.43, 358.568, 5.01145),
(@CGUID-13, 66300, 0, -7877.3, 1317.38, 338.945, 3.1612),
(@CGUID-14, 64860, 0, -7871.98, 1286.14, 341.75, 2.80739),
(@CGUID-15, 66300, 0, -7888.03, 1261.61, 358.562, 0.0889011),
(@CGUID-16, 66480, 0, -7873.55, 1287.45, 341.766, 1.13404),
(@CGUID-17, 55830, 0, -7885.57, 1276.63, 358.561, 2.30356),
(@CGUID-18, 66460, 0, -7875.78, 1328.08, 338.941, 2.9861),
(@CGUID-19, 66300, 0, -7883.45, 1317.52, 338.952, 0.0183194),
(@CGUID-20, 66300, 0, -7883.42, 1315.39, 338.952, 0.0183194),
(@CGUID-21, 64866, 0, -7878.92, 1280.26, 358.56, 4.7333),
(@CGUID-22, 66300, 0, -7891.98, 1272.92, 358.791, 0.00259898),
(@CGUID-23, 64858, 0, -7877.59, 1329.17, 338.941, 2.97156),
(@CGUID-24, 66473, 0, -7867.07, 1328.11, 338.965, 0.826461),
(@CGUID-25, 64861, 0, -7887.71, 1286.34, 341.643, 1.76604),
(@CGUID-26, 64863, 0, -7880.92, 1280.24, 358.56, 4.73861),
(@CGUID-27, 66473, 0, -7866.8, 1318.09, 338.965, 0.826461),
(@CGUID-28, 66300, 0, -7887.37, 1282.55, 358.556, 6.19046),
(@CGUID-29, 66300, 0, -7875.5, 1315.35, 338.945, 3.1612),
(@CGUID-30, 66300, 0, -7835.6, 1277, 343.51, 3.14393),
(@CGUID-31, 55832, 0, -7882.62, 1265.51, 358.571, 3.66331),
(@CGUID-32, 66300, 0, -7885.15, 1319.56, 338.954, 0.0183194),
(@CGUID-33, 66300, 0, -7883.43, 1319.55, 338.952, 0.0183194),
(@CGUID-34, 66300, 0, -7872.69, 1282.88, 358.556, 3.08068),
(@CGUID-35, 66473, 0, -7893.86, 1306.31, 338.965, 2.20691),
(@CGUID-36, 66460, 0, -7875.73, 1330.12, 338.94, 3.06502),
(@CGUID-37, 40789, 0, -7843.48, 1265.74, 308.835, 5.26435),
(@CGUID-38, 66300, 0, -7875.64, 1319.44, 338.941, 3.1612),
(@CGUID-39, 66300, 0, -7875.58, 1317.38, 338.943, 3.1612),
(@CGUID-40, 40789, 0, -7915.59, 1261.06, 316.726, 2.03287),
(@CGUID-41, 66300, 0, -7924.33, 1276.03, 343.419, 3.14876),
(@CGUID-42, 66473, 0, -7894.15, 1317.89, 338.965, 2.20691),
(@CGUID-43, 66480, 0, -7898.16, 1318.71, 338.807, 4.70694),
(@CGUID-44, 66300, 0, -7925.91, 1276, 343.511, 0.126102),
(@CGUID-45, 66460, 0, -7885.12, 1339.75, 338.942, 6.27965),
(@CGUID-46, 64864, 0, -7877.44, 1341.37, 338.939, 3.08458),
(@CGUID-47, 66473, 0, -7894.04, 1341.65, 338.965, 2.20691),
(@CGUID-48, 66460, 0, -7875.72, 1340.41, 338.939, 3.05843),
(@CGUID-49, 66460, 0, -7885.26, 1342.18, 338.944, 0.0716081),
(@CGUID-50, 64862, 0, -7883.45, 1340.96, 338.942, 0.0849101),
(@CGUID-51, 66460, 0, -7885.55, 1330.13, 338.942, 0.188897),
(@CGUID-52, 66300, 0, -7883.48, 1347.02, 338.945, 0.0183194),
(@CGUID-53, 66473, 0, -7894.15, 1327.89, 338.965, 2.20691),
(@CGUID-54, 66300, 0, -7875.69, 1351.07, 338.94, 3.1612),
(@CGUID-55, 40789, 0, -7880.45, 1331.93, 340.028, 1.56033),
(@CGUID-56, 66300, 0, -7885.19, 1347.03, 338.947, 0.0183194),
(@CGUID-57, 66460, 0, -7875.82, 1342.45, 338.939, 3.13735),
(@CGUID-58, 66300, 0, -7877.41, 1351.08, 338.941, 3.1612),
(@CGUID-59, 66300, 0, -7883.61, 1351.11, 338.944, 0.0183194),
(@CGUID-60, 66300, 0, -7883.53, 1349.09, 338.945, 0.0183194),
(@CGUID-61, 66480, 0, -7893.82, 1352.99, 338.915, 1.61052),
(@CGUID-62, 66473, 0, -7867.33, 1341.94, 338.965, 0.826461),
(@CGUID-63, 64859, 0, -7880.63, 1355.88, 338.849, 4.71383),
(@CGUID-64, 66480, 0, -7892.85, 1356.23, 338.886, 3.19125),
(@CGUID-65, 66300, 0, -7877.39, 1346.91, 338.941, 3.1612),
(@CGUID-66, 55370, 0, -7881.21, 1328.65, 350.847, 0),
(@CGUID-67, 66300, 0, -7877.38, 1348.94, 338.941, 3.1612),
(@CGUID-68, 64865, 0, -7884.01, 1329.34, 338.943, 0.207194),
(@CGUID-69, 66300, 0, -7885.33, 1351.11, 338.945, 0.0183194),
(@CGUID-70, 66460, 0, -7885.14, 1327.73, 338.945, 0.113754),
(@CGUID-71, 66300, 0, -7885.25, 1349.09, 338.946, 0.0183194),
(@CGUID-72, 66300, 0, -7875.65, 1348.94, 338.94, 3.1612),
(@CGUID-73, 66300, 0, -7875.67, 1346.91, 338.94, 3.1612),
(@CGUID-74, 66480, 0, -7868.27, 1358.91, 339.204, 5.11884);

SET @OGUID := 200005;
DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID-5 AND @OGUID-0;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@OGUID-0, 215648, 0, -7892.52, 1354.90, 338.81, 0),
(@OGUID-1, 215648, 0, -7894.48, 1354.68, 339.89, 0),
(@OGUID-2, 215648, 0, -7895.23, 1354.65, 338.83, 0),
(@OGUID-3, 215648, 0, -7893.98, 1355.55, 338.81, 0),
(@OGUID-4, 215648, 0, -7893.88, 1354.17, 338.81, 0),
(@OGUID-5, 215648, 0, -7892.50, 1353.51, 338.82, 0);

UPDATE `creature_template` SET `unit_flags2`=2048 WHERE `entry`=66460;

UPDATE `gossip_menu_option` SET `action_menu_id`=15111 WHERE `menu_id`=14971 AND `id`=1;
DELETE FROM `gossip_menu_option` WHERE `menu_id`=15111;
INSERT INTO `gossip_menu_option` (`menu_id`, `option_text`, `option_id`, `npc_option_npcflag`) VALUES
(15111, 'I am ready to depart.', 1, 1);
UPDATE `quest_template` SET `OfferRewardText`='That\'s a whole Horde base down there!$BWhere was our intel!$B$BWell $c, it\'s time to see what this upgraded gunship is capable of.' WHERE `Id`=29548;

UPDATE `quest_template` SET `RequestItemsText`='What are you waiting for?! Scorch the ground beneath them!' WHERE `Id`=31732;

UPDATE `quest_template` SET `OfferRewardText`='A proper welcome for the Horde to this new land!' WHERE `Id`=31732;

UPDATE `quest_template` SET `OfferRewardText`='\'Bout time you showed up, lassie! Let\'s do some damage!' WHERE `Id`=31733;

UPDATE `quest_template` SET `OfferRewardText`='Great work! If you ever decide to give up your life of adventure, the SI:7 could use more talent like you...' WHERE `Id`=30069;

UPDATE `quest_template` SET `RequestItemsText`='Did "The Pickle" send you with a mission?' WHERE `Id`=31734;

UPDATE `quest_template` SET `OfferRewardText`='With those wagons our of commission, we can turn our attention to the commander.' WHERE `Id`=31734;

UPDATE `quest_template` SET `RequestItemsText`='We will complete our mission, but right now our priority is that warlock!' WHERE `Id`=31735;

UPDATE `quest_template` SET `OfferRewardText`='We\'ll catch up with him aggain. I\'m sure of it.' WHERE `Id`=31735;

UPDATE `quest_template` SET `OfferRewardText`='I am glad you took responsibility for your actions. Left unchecked, negative emotions can quickly overwhelm an individual here, just as they started to overwhelm the one you call "Rell."' WHERE `Id`=31737;

UPDATE `quest_template` SET `OfferRewardText`='When the mist fell, we knew it would only be a matter of time before visitors arrived.$B$BWe could not have predicted it would be like this.' WHERE `Id`=31736;

UPDATE `quest_template` SET `RequestItemsText`='Have the $r been dealt with?' WHERE `Id`=14093;

UPDATE `quest_template` SET `RequestItemsText`='Have you succeeded, $n?' WHERE `Id`=14098;

UPDATE `creature_template` SET `unit_flags2`=1|2048 WHERE `entry`=35912;

UPDATE `quest_template` SET `RequestItemsText`='What is it then, $n?' WHERE `Id`=24930;

UPDATE `quest_template` SET `RequestItemsText`='How many of \'em did ya get?' WHERE `Id`=24469;

UPDATE `quest_template` SET `RequestItemsText`='Well, what\'re ye waitin\' for, $c?' WHERE `Id`=24470;

UPDATE `quest_template` SET `RequestItemsText`='Thanks for yer help, $n.' WHERE `Id`=24471;
DELETE FROM `spell_area` WHERE `spell`=92237 AND `area`=257;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(92237, 257, 0, 0, 0, 0, 2, 1, 64, 11);
UPDATE `quest_template` SET `OfferRewardText`='Ilthalaine sent you? He was wise to do so. I am indeed in need of help.' WHERE `Id`=28734;
DELETE FROM `creature_queststarter` WHERE `id`=2077 AND `quest`=31168;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(2077, 31168);

UPDATE `quest_template` SET `RequiredClasses`=512, `PrevQuestId`=28715 WHERE `Id`=31168;

SET @CGUID := 200109;
DELETE FROM `creature` WHERE `guid`=@CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 63331, 1, 10524.5, 786.045, 1329.6, 3.05581);

DELETE FROM `creature_queststarter` WHERE `id`=63331 AND `quest`=28723;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(63331, 28723);
UPDATE `creature_template` SET `ScriptName`='npc_tarindrella' WHERE `entry`=49480;
UPDATE `creature_template` SET `ScriptName`='npc_githyiss' WHERE `entry`=1994;
UPDATE `quest_template` SET `PrevQuestId`=30039 WHERE `Id`=30027;
UPDATE `quest_template` SET `PrevQuestId`=30040 WHERE `Id`=30033;
UPDATE `quest_template` SET `PrevQuestId`=30041 WHERE `Id`=30034;
UPDATE `quest_template` SET `PrevQuestId`=30042 WHERE `Id`=30035;
UPDATE `quest_template` SET `PrevQuestId`=30043 WHERE `Id`=30036;
UPDATE `quest_template` SET `PrevQuestId`=30044 WHERE `Id`=30037;
UPDATE `quest_template` SET `PrevQuestId`=30045 WHERE `Id`=30038;
UPDATE `quest_template` SET `PrevQuestId`=29404 WHERE `Id`=29405;

-- Much to Learn (Warrior)
UPDATE `quest_template` SET `OfferRewardText`='Today is the day when your fate will be decided. Do you have the strength and hardiness to be a $c? Or are you destined to a life working the fields?$B$BEach has its place, but i can sense that you aspire to greatness.$B$BLet us begin our training, my newest pupil.' WHERE  `Id`=30045;
-- Much to Learn (Hunter)
UPDATE `quest_template` SET `OfferRewardText`='Today is the day when your fate will be decided. Do you have the compassion and keen eye of a $c? Or are you destined to a life tending to the livestock?$B$BEach has its place, but I can sense that you aspire to greatness.$B$BLet us begin our training, my newest pupil.' WHERE  `Id`=30041;
-- Much to Learn (Priest)
UPDATE `quest_template` SET `OfferRewardText`='Today is the day when your fate will be decided. Do you have the awareness and wisdom necessary to be a $c? Or are you destined to a life scribing in the libraries?$B$BEach has its place, but I can sense that you aspire to greatness.$B$BLet us begin our training, my newest pupil.' WHERE  `Id`=30042;
-- Much to Learn (Shaman)
UPDATE `quest_template` SET `OfferRewardText`='Today is the day when your fate will be decided. Do you have the strength of will to be a $c? Or are you destined to a life of tending to the shrines?$B$BEach has its place, but i can sense that you aspire to greatness.$B$BLet us begin our training, my newest pupil.' WHERE  `Id`=30044;
-- Much to Learn (Mage)
UPDATE `quest_template` SET `OfferRewardText`='Today is the day when your fate will be decided. Do you have the intellect and force of will necessary to be a $c? Or are you destined to a life scribing in the dusty libraries?$B$BEach has its place, but i can sense that you aspire to greatness.$B$BLet us begin our training, my newest pupil.' WHERE  `Id`=30040;
-- Much to Learn (Monk)
UPDATE `quest_template` SET `OfferRewardText`='Today is the day when your fate will be decided. Do you have the strength and cunning to be a $c? Or are you destined to a life working in the fields?$B$BEach has its place, but I can sense that you aspire to greatness.$B$BLet us begin our training, my newest pupil.' WHERE  `Id`=30039;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=57748;
SET @NPC := 224302;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=1249.1267,`position_y`=3454.8315,`position_z`=103.5636 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_flag`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,1249.1267,3454.8315,103.5636,0,0,0,0,100,0),
(@PATH,2,1256.8594,3455.244,104.49251,0,0,0,0,100,0),
(@PATH,3,1264.5087,3458.4124,105.21208,0,0,0,0,100,0),
(@PATH,4,1272.0192,3462.1946,105.42941,0,0,0,0,100,0),
(@PATH,5,1278.9323,3467.9253,104.81073,0,0,0,0,100,0),
(@PATH,6,1281.5209,3473.0095,104.007385,0,0,0,0,100,0),
(@PATH,7,1281.6406,3478.4792,103.14691,0,0,0,0,100,0),
(@PATH,8,1279.9913,3489.9055,101.09776,0,0,0,0,100,0),
(@PATH,9,1277.7778,3500.2605,98.54746,0,0,0,0,100,0),
(@PATH,10,1273.7188,3503.5876,97.83821,0,0,0,0,100,0),
(@PATH,11,1276.9584,3516.6797,95.24937,0,0,0,0,100,0),
(@PATH,12,1276.5555,3522.3645,94.46181,0,0,0,0,100,0),
(@PATH,13,1277.5173,3529.9358,93.66951,0,0,0,0,100,0),
(@PATH,14,1280.0817,3538.131,93.59839,0,0,0,0,100,0),
(@PATH,15,1281.1598,3542.9229,93.77478,0,0,0,0,100,0),
(@PATH,16,1283.1649,3546.5955,93.68326,0,0,0,0,100,0),
(@PATH,17,1280.9341,3554.8213,92.895874,0,0,0,0,100,0),
(@PATH,18,1283.9584,3561.626,92.69742,0,0,0,0,100,0),
(@PATH,19,1288.5764,3567.8342,91.98248,0,0,0,0,100,0),
(@PATH,20,1303.8247,3596.184,89.39445,0,0,0,0,100,0),
(@PATH,21,1300.5625,3586.429,89.904564,0,0,0,0,100,0),
(@PATH,22,1295.7848,3580.4722,90.57019,0,0,0,0,100,0),
(@PATH,23,1287.3629,3569.2588,92.00115,0,0,0,0,100,0),
(@PATH,24,1283.5312,3561.12,92.727165,0,0,0,0,100,0),
(@PATH,25,1279.9323,3553.4072,93.06555,0,0,0,0,100,0),
(@PATH,26,1278.6997,3544.6094,93.59578,0,0,0,0,100,0),
(@PATH,27,1277.158,3533.5754,93.645706,0,0,0,0,100,0),
(@PATH,28,1275.0608,3523.3767,94.23459,0,0,0,0,100,0),
(@PATH,29,1276.441,3510.941,95.96466,0,0,0,0,100,0),
(@PATH,30,1278.5052,3498.1885,99.11742,0,0,0,0,100,0),
(@PATH,31,1280.5452,3488.4626,101.41778,0,0,0,0,100,0),
(@PATH,32,1281.0747,3477.0903,103.34571,0,0,0,0,100,0),
(@PATH,33,1281.3646,3467.7708,105.13789,0,0,0,0,100,0),
(@PATH,34,1284.5625,3461.1858,107.256775,0,0,0,0,100,0),
(@PATH,35,1291.941,3453.1736,110.699615,0,0,0,0,100,0),
(@PATH,36,1299.0605,3447.6719,113.270004,0,0,0,0,100,0),
(@PATH,37,1306.1598,3444.2205,115.04013,0,0,0,0,100,0),
(@PATH,38,1313.5,3435.5713,117.749374,0,0,0,0,100,0),
(@PATH,39,1320.3055,3426.3525,120.39638,0,0,0,0,100,0),
(@PATH,40,1325.8524,3419.4124,121.76149,0,0,0,0,100,0),
(@PATH,41,1334.4427,3413.7258,123.29486,0,0,0,0,100,0),
(@PATH,42,1340.349,3403.408,124.41885,0,0,0,0,100,0),
(@PATH,43,1343.7014,3387.7395,125.12563,0,0,0,0,100,0),
(@PATH,44,1349.908,3377.5217,125.491905,0,0,0,0,100,0),
(@PATH,45,1352.2466,3365.3074,126.178986,0,0,0,0,100,0),
(@PATH,46,1356.8055,3355.382,126.95244,0,0,0,0,100,0),
(@PATH,47,1360.2952,3342.948,127.78494,0,0,0,0,100,0),
(@PATH,48,1360.4427,3327.9192,129.23535,0,0,0,0,100,0),
(@PATH,49,1357.8021,3315.5312,129.77429,0,0,0,0,100,0),
(@PATH,50,1354.3298,3306.1536,130.23294,0,0,0,0,100,0),
(@PATH,51,1353.3038,3295.2795,130.81131,0,0,0,0,100,0),
(@PATH,52,1354.6927,3287.2292,131.22624,0,0,0,0,100,0),
(@PATH,53,1361.3611,3277.0217,131.36896,0,0,0,0,100,0),
(@PATH,54,1368.481,3274.2024,131.63034,0,0,0,0,100,0),
(@PATH,55,1375.9584,3271.8152,132.19711,0,0,0,0,100,0),
(@PATH,56,1370.8455,3273.6086,131.78532,0,0,0,0,100,0),
(@PATH,57,1366.5122,3276.0557,131.38477,0,0,0,0,100,0),
(@PATH,58,1360.3368,3280.2708,131.34384,0,0,0,0,100,0),
(@PATH,59,1356.106,3285.4097,131.36252,0,0,0,0,100,0),
(@PATH,60,1355.467,3292.2239,131.13074,0,0,0,0,100,0),
(@PATH,61,1357.0851,3297.611,130.74484,0,0,0,0,100,0),
(@PATH,62,1353.7778,3304.5173,130.29346,0,0,0,0,100,0),
(@PATH,63,1355.3368,3310.5635,130.09756,0,0,0,0,100,0),
(@PATH,64,1357.9827,3317.8298,129.68593,0,0,0,0,100,0),
(@PATH,65,1360.3038,3324.4792,129.50514,0,0,0,0,100,0),
(@PATH,66,1361.1924,3331.4248,129.04236,0,0,0,0,100,0),
(@PATH,67,1360.5817,3339.856,128.11148,0,0,0,0,100,0),
(@PATH,68,1357.2413,3342.7432,127.887474,0,0,0,0,100,0),
(@PATH,69,1360.5348,3351.9827,127.244705,0,0,0,0,100,0),
(@PATH,70,1357.1077,3358.5017,126.76606,0,0,0,0,100,0),
(@PATH,71,1353.6875,3365.5903,126.25423,0,0,0,0,100,0),
(@PATH,72,1351.6602,3371.2383,125.84179,0,0,0,0,100,0),
(@PATH,73,1350.6702,3378.942,125.37667,0,0,0,0,100,0),
(@PATH,74,1349.8646,3385.2961,125.16545,0,0,0,0,100,0),
(@PATH,75,1342.1979,3395.269,124.7548,0,0,0,0,100,0),
(@PATH,76,1339.6318,3403.9004,124.39438,0,0,0,0,100,0),
(@PATH,77,1335.1459,3413.6494,123.36618,0,0,0,0,100,0),
(@PATH,78,1329.7535,3418.934,122.194305,0,0,0,0,100,0),
(@PATH,79,1323.1198,3423.3638,120.95212,0,0,0,0,100,0),
(@PATH,80,1318.5139,3425.0876,120.443596,0,0,0,0,100,0),
(@PATH,81,1313.9375,3426.073,120.44553,0,0,0,0,100,0),
(@PATH,82,1310.6094,3438.9036,116.71936,0,0,0,0,100,0),
(@PATH,83,1304.125,3446.7153,114.252655,0,0,0,0,100,0),
(@PATH,84,1298.8073,3449.3003,112.81301,0,0,0,0,100,0),
(@PATH,85,1293.2432,3453.546,110.76933,0,0,0,0,100,0),
(@PATH,86,1289.1406,3459.1067,108.67095,0,0,0,0,100,0),
(@PATH,87,1287.5352,3464.8896,106.93947,0,0,0,0,100,0),
(@PATH,88,1271.9497,3460.9844,105.68841,0,0,0,0,100,0),
(@PATH,89,1264.5764,3456.448,105.35658,0,0,0,0,100,0),
(@PATH,90,1254.8646,3454.894,104.28882,0,0,0,0,100,0),
(@PATH,91,1248.1459,3456.3489,103.448,0,0,0,0,100,0),
(@PATH,92,1240.3108,3458.1633,102.72408,0,0,0,0,100,0);
DELETE FROM `creature_addon` WHERE `guid`=224522;
INSERT INTO `creature_addon` (`guid`, `auras`) VALUES
(224522, '80797');

DELETE FROM `creature_addon` WHERE `guid`=224519; 
INSERT INTO `creature_addon` (`guid`, `auras`) VALUES
(224519, '80797');

DELETE FROM `creature_addon` WHERE `guid`=224593;
INSERT INTO `creature_addon` (`guid`, `auras`) VALUES
(224593, '82358');

DELETE FROM `creature_addon` WHERE `guid`=224592; 
INSERT INTO `creature_addon` (`guid`, `auras`) VALUES
(224592, '82358');

DELETE FROM `spell_area` WHERE `spell`=102393;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(102393, 5846, 0, 29419, 0, 0, 2, 1, 64, 9),
(102393, 5825, 0, 29419, 0, 0, 2, 1, 64, 9);

DELETE FROM `spell_area` WHERE `spell`=102395;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(102395, 5846, 0, 29419, 0, 0, 2, 1, 64, 66),
(102395, 5825, 0, 29419, 0, 0, 2, 1, 64, 66);
ALTER TABLE `smart_scripts` ADD COLUMN `event_param5` INT(10) UNSIGNED DEFAULT 0 NOT NULL AFTER `event_param4`;
UPDATE `creature_template` SET `ScriptName`='npc_jinthala' WHERE `entry`=37951;
UPDATE `creature_template` SET `ScriptName`='npc_zuni' WHERE `entry`=37988;

DELETE FROM `playercreateinfo_spell_cast` WHERE `Spell`=71033;
INSERT INTO `playercreateinfo_spell_cast` (`racemask`, `Spell`, `Note`) VALUES
(128, 71033, 'Troll - Calm of the Novice');

DELETE FROM `creature_addon` WHERE `guid`=80046; -- 右 (right)
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (80046, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=80047; -- 左 (Left)
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (80047, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=80091; -- 右
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (80091, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=80092; -- 左
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (80092, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=80360; -- 右
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (80360, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=80361; -- 左
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (80361, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=80512; -- 右
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (80512, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=80513; -- 左
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (80513, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=120679; -- 左
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (120679, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=120680; -- 右
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (120680, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=120683; -- 右
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (120683, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=120684; -- 左
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (120684, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=150001; -- 右
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (150001, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=150002; -- 左
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (150002, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=224431; -- 右
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (224431, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=224434; -- 左
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (224434, 0, 0, 0, 0, 0, NULL);



DELETE FROM `smart_scripts` WHERE `entryorguid`=-80046;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-80046, 0, 2, 0, 52, 0, 100, 0, 6, 53565, 0, 0, 0, 1, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 6 Over - Say Text Line 7'),
(-80046, 0, 1, 0, 52, 0, 100, 0, 5, 53565, 0, 0, 0, 1, 6, 15000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 5 Over - Say Text Line 6'),
(-80046, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 1, 5, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 5');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-80047;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-80047, 0, 4, 0, 52, 0, 100, 0, 4, 53565, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 53565, 3, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 4 Over - Set Data 1 1'),
(-80047, 0, 3, 0, 52, 0, 100, 0, 3, 53565, 0, 0, 0, 1, 4, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 3 Over - Say Text Line 4'),
(-80047, 0, 2, 0, 52, 0, 100, 0, 2, 53565, 0, 0, 0, 1, 3, 11000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 2 Over - Say Text Line 3'),
(-80047, 0, 1, 0, 52, 0, 100, 0, 1, 53565, 0, 0, 0, 1, 2, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 2'),
(-80047, 0, 0, 0, 1, 0, 100, 0, 20000, 20000, 120000, 120000, 0, 1, 1, 16000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - OOC - Say Text Line 1');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-80091;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-80091, 0, 2, 0, 52, 0, 100, 0, 6, 53565, 0, 0, 0, 1, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 6 Over - Say Text Line 7'),
(-80091, 0, 1, 0, 52, 0, 100, 0, 5, 53565, 0, 0, 0, 1, 6, 15000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 5 Over - Say Text Line 6'),
(-80091, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 1, 5, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 5');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-80092;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-80092, 0, 4, 0, 52, 0, 100, 0, 4, 53565, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 53565, 3, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 4 Over - Set Data 1 1'),
(-80092, 0, 3, 0, 52, 0, 100, 0, 3, 53565, 0, 0, 0, 1, 4, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 3 Over - Say Text Line 4'),
(-80092, 0, 2, 0, 52, 0, 100, 0, 2, 53565, 0, 0, 0, 1, 3, 11000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 2 Over - Say Text Line 3'),
(-80092, 0, 1, 0, 52, 0, 100, 0, 1, 53565, 0, 0, 0, 1, 2, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 2'),
(-80092, 0, 0, 0, 1, 0, 100, 0, 20000, 20000, 120000, 120000, 0, 1, 1, 16000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - OOC - Say Text Line 1');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-80360;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-80360, 0, 2, 0, 52, 0, 100, 0, 6, 53565, 0, 0, 0, 1, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 6 Over - Say Text Line 7'),
(-80360, 0, 1, 0, 52, 0, 100, 0, 5, 53565, 0, 0, 0, 1, 6, 15000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 5 Over - Say Text Line 6'),
(-80360, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 1, 5, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 5');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-80361;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-80361, 0, 4, 0, 52, 0, 100, 0, 4, 53565, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 53565, 3, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 4 Over - Set Data 1 1'),
(-80361, 0, 3, 0, 52, 0, 100, 0, 3, 53565, 0, 0, 0, 1, 4, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 3 Over - Say Text Line 4'),
(-80361, 0, 2, 0, 52, 0, 100, 0, 2, 53565, 0, 0, 0, 1, 3, 11000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 2 Over - Say Text Line 3'),
(-80361, 0, 1, 0, 52, 0, 100, 0, 1, 53565, 0, 0, 0, 1, 2, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 2'),
(-80361, 0, 0, 0, 1, 0, 100, 0, 20000, 20000, 120000, 120000, 0, 1, 1, 16000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - OOC - Say Text Line 1');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-80512;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-80512, 0, 2, 0, 52, 0, 100, 0, 6, 53565, 0, 0, 0, 1, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 6 Over - Say Text Line 7'),
(-80512, 0, 1, 0, 52, 0, 100, 0, 5, 53565, 0, 0, 0, 1, 6, 15000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 5 Over - Say Text Line 6'),
(-80512, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 1, 5, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 5');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-80513;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-80513, 0, 4, 0, 52, 0, 100, 0, 4, 53565, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 53565, 3, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 4 Over - Set Data 1 1'),
(-80513, 0, 3, 0, 52, 0, 100, 0, 3, 53565, 0, 0, 0, 1, 4, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 3 Over - Say Text Line 4'),
(-80513, 0, 2, 0, 52, 0, 100, 0, 2, 53565, 0, 0, 0, 1, 3, 11000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 2 Over - Say Text Line 3'),
(-80513, 0, 1, 0, 52, 0, 100, 0, 1, 53565, 0, 0, 0, 1, 2, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 2'),
(-80513, 0, 0, 0, 1, 0, 100, 0, 20000, 20000, 120000, 120000, 0, 1, 1, 16000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - OOC - Say Text Line 1');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-120683;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-120683, 0, 2, 0, 52, 0, 100, 0, 6, 53565, 0, 0, 0, 1, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 6 Over - Say Text Line 7'),
(-120683, 0, 1, 0, 52, 0, 100, 0, 5, 53565, 0, 0, 0, 1, 6, 15000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 5 Over - Say Text Line 6'),
(-120683, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 1, 5, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 5');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-120684;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-120684, 0, 4, 0, 52, 0, 100, 0, 4, 53565, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 53565, 3, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 4 Over - Set Data 1 1'),
(-120684, 0, 3, 0, 52, 0, 100, 0, 3, 53565, 0, 0, 0, 1, 4, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 3 Over - Say Text Line 4'),
(-120684, 0, 2, 0, 52, 0, 100, 0, 2, 53565, 0, 0, 0, 1, 3, 11000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 2 Over - Say Text Line 3'),
(-120684, 0, 1, 0, 52, 0, 100, 0, 1, 53565, 0, 0, 0, 1, 2, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 2'),
(-120684, 0, 0, 0, 1, 0, 100, 0, 20000, 20000, 120000, 120000, 0, 1, 1, 16000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - OOC - Say Text Line 1');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-150001;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-150001, 0, 2, 0, 52, 0, 100, 0, 6, 53565, 0, 0, 0, 1, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 6 Over - Say Text Line 7'),
(-150001, 0, 1, 0, 52, 0, 100, 0, 5, 53565, 0, 0, 0, 1, 6, 15000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 5 Over - Say Text Line 6'),
(-150001, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 1, 5, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 5');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-150002;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-150002, 0, 4, 0, 52, 0, 100, 0, 4, 53565, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 53565, 3, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 4 Over - Set Data 1 1'),
(-150002, 0, 3, 0, 52, 0, 100, 0, 3, 53565, 0, 0, 0, 1, 4, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 3 Over - Say Text Line 4'),
(-150002, 0, 2, 0, 52, 0, 100, 0, 2, 53565, 0, 0, 0, 1, 3, 11000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 2 Over - Say Text Line 3'),
(-150002, 0, 1, 0, 52, 0, 100, 0, 1, 53565, 0, 0, 0, 1, 2, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 2'),
(-150002, 0, 0, 0, 1, 0, 100, 0, 20000, 20000, 120000, 120000, 0, 1, 1, 16000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - OOC - Say Text Line 1');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-224431;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-224431, 0, 2, 0, 52, 0, 100, 0, 6, 53565, 0, 0, 0, 1, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 6 Over - Say Text Line 7'),
(-224431, 0, 1, 0, 52, 0, 100, 0, 5, 53565, 0, 0, 0, 1, 6, 15000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 5 Over - Say Text Line 6'),
(-224431, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 1, 5, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 5');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-224434;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-224434, 0, 4, 0, 52, 0, 100, 0, 4, 53565, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 53565, 3, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 4 Over - Set Data 1 1'),
(-224434, 0, 3, 0, 52, 0, 100, 0, 3, 53565, 0, 0, 0, 1, 4, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 3 Over - Say Text Line 4'),
(-224434, 0, 2, 0, 52, 0, 100, 0, 2, 53565, 0, 0, 0, 1, 3, 11000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 2 Over - Say Text Line 3'),
(-224434, 0, 1, 0, 52, 0, 100, 0, 1, 53565, 0, 0, 0, 1, 2, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 2'),
(-224434, 0, 0, 0, 1, 0, 100, 0, 20000, 20000, 120000, 120000, 0, 1, 1, 16000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - OOC - Say Text Line 1');SET @CGUID := 120768;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID-1 AND @CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID-0, 53714, 860, 1, 594, 0, 0, 0, 1493.68, 3418.64, 171.051, 2.77507, 30, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-1, 57873, 860, 1, 594, 0, 0, 0, 1467.26, 3424.37, 171.087, 5.68977, 30, 0, 0, 0, 0, 0, 0, 0, 0);
UPDATE `creature_template` SET `ScriptName`='npc_aysa_cloudsinger' WHERE `entry`=54567;
DELETE FROM `spell_area` WHERE `spell`=116421;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(116421, 5848, 29414, 0, 0, 0, 2, 1, 8, 11);
UPDATE `creature_template` SET `ScriptName`='npc_aysa_meditation' WHERE `entry`=59642;
DELETE FROM `creature` WHERE `guid` IN (224600, 224603, 224604, 224605, 224606, 224607, 224608, 224609, 224610, 224611, 224612, 224613, 224614, 224615, 224616, 224617, 224618, 224619);
UPDATE `creature_template` SET `ScriptName`='npc_li_fei' WHERE `entry`=54856;
UPDATE `quest_template` SET `OfferRewardText`='Your path is set before you. You will be the one to rekindle the spirit of fire and bring it to the safety of the temple.$B$BThis will not be the only time you and Aysa work together. She is strong and wise. You can trust in her.' WHERE `Id`=29414;
UPDATE `quest_template` SET `OfferRewardText`='Hello! You look poisoned and confident... I like that!$B$BI\'ve heard some rather impressive whispers about you from the training grounds. If you\'re half as capable as they say, I think you and I are going to be good friends!' WHERE `Id`=29522;
UPDATE `quest_template` SET `OfferRewardText`='Well done, $n. My men can take it from here. Let\'s focus on getting the other survivors out now.' WHERE `Id`=27670;

UPDATE `quest_template` SET `NextQuestIdChain`=28167 WHERE `Id`=27670;

UPDATE `quest_template` SET `OfferRewardText`='Did Nevin send you ahead? That means he\'s getting ready to end the mission, but there are still survivors to be rescued. We have to help them!' WHERE `Id`=28167;

UPDATE `quest_template` SET `OfferRewardText`='I know Nevin will be pleased with all the survivors you managed to help. I still can\'t believe you managed to stay so strong down here in the radiation. You\'ve seen the shape the other survivors are in.$B$BLet\'s get you to the loading room for decontamination.' WHERE `Id`=27671;

UPDATE `quest_template` SET `NextQuestIdChain`=28169 WHERE `Id`=27671;

UPDATE `quest_template` SET `OfferRewardText`='It\'s good to meet you, $c. We rarely come across survivors as strong and capable as you are. I can help get you decontaminated and on your way out of here.' WHERE `Id`=28169;

UPDATE `quest_template` SET `NextQuestIdChain`=27635 WHERE `Id`=28169;

UPDATE `quest_template` SET `OfferRewardText`='There, now you\'re fit to head off to the surface and start your new life. The High Tinker will be delighted to hear of your arrival.' WHERE  `Id`=27635;

UPDATE `quest_template` SET `OfferRewardText`='It\'s good to see you again, $n. Without your help, we wouldn\'t have been able to get so many survivors out of Gnomeregan this time. Everyone is going to be thrilled to meet you and hear your story.' WHERE `Id`=27674;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=56739;

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry`=56739;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`) VALUES
(56739, 46598, 0);

DELETE FROM `vehicle_template_accessory` WHERE `entry`=56739;
INSERT INTO `vehicle_template_accessory` (`entry`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
(56739, 54131, 0, 0, 'Hozen Hanging bunny - Fe Feng Hozen', 6, 30000);

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=54131;
DELETE FROM `smart_scripts` WHERE `entryorguid`=54131;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(54131, 0, 0, 0, 1, 0, 100, 1, 500, 500, 500, 500, 0, 11, 46598, 3, 0, 0, 0, 0, 19, 56739, 5, 0, 0, 0, 0, 0, 'Fe Feng Hozen - OOC once - Cast Ride Vehicle Hardcoded on Hozen Hanging bunny'),
(54131, 0, 1, 0, 0, 0, 100, 1, 500, 500, 500, 500, 0, 11, 46598, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Fe Feng Hozen - IC once - Cast Ride Vehicle Hardcoded on self');

DELETE FROM `creature` WHERE `guid` IN (224774, 224554, 224553, 224574, 224570, 224666, 224658, 224673, 224660, 224688, 224580, 224548, 224542, 224715, 224714, 224692, 224675, 224707, 224705, 224783, 224784, 224779, 224786, 224718);

UPDATE `creature` SET `unit_flags`=256|512|262144|536870912 WHERE `guid` IN
(224557, 224564, 224562, 224560, 224638, 224563, 224566);
DELETE FROM `spell_script_names` WHERE `spell_id`=106299;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(106299, 'spell_gen_summon_living_air');
DELETE FROM `creature_addon` WHERE `guid`=224620;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(224620, 0, 0, 0, 0, 0, '85096 126160');
DELETE FROM `creature` WHERE `guid` IN (224620, 224294, 224602);
UPDATE `gameobject_template` SET `name`='Brazier of the Flickering Flame' WHERE `entry`=209369;
UPDATE `gameobject_template` SET `name`='Brazier of the Violet Flame' WHERE `entry`=209803;
UPDATE `gameobject_template` SET `name`='Brazier of the Blue Flame' WHERE `entry`=209802;
UPDATE `gameobject_template` SET `name`='Brazier of the Red Flame' WHERE `entry`=209801;

UPDATE `gameobject_template` SET `name`='Brazier of the Red Flame' WHERE `entry`=209374;
UPDATE `gameobject_template` SET `name`='Brazier of the Violet Flame' WHERE `entry`=209372;
UPDATE `gameobject_template` SET `name`='Brazier of the Blue Flame' WHERE `entry`=209373;

UPDATE `gameobject_template` SET `type`=22, `data0`=105151, `data5`=13004 WHERE `entry`=209369;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceGroup`=1 AND `SourceEntry`=108967 AND `SourceId`=0 AND `ElseGroup`=1 AND `ConditionTypeOrReference`=31 AND `ConditionTarget`=0 AND `ConditionValue1`=3 AND `ConditionValue2`=57873 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES (13, 1, 108967, 0, 1, 31, 0, 3, 57873, 0, 0, 0, 0, '', 'Jab target only Training Target');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceGroup`=1 AND `SourceEntry`=108967 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=31 AND `ConditionTarget`=0 AND `ConditionValue1`=3 AND `ConditionValue2`=53714 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES (13, 1, 108967, 0, 0, 31, 0, 3, 53714, 0, 0, 0, 0, '', 'Jab target only Training Target');

SET @ENTRY := -120675;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
UPDATE `creature_template` SET `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` = 53565;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -120676;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -120677;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
UPDATE `creature_template` SET `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` = 53565;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');
UPDATE `creature` SET `position_x`=1485.11, `position_y`=3427.23, `position_z`=171.161, `orientation`=6.16406, `spawndist`=0, `MovementType`=2 WHERE `guid`=120674;
DELETE FROM `creature_template_addon` WHERE `entry`=57754;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`) VALUES (57754, 120674, 0, 0, 1, 0);

DELETE FROM `waypoint_data` WHERE `id`=120674;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) VALUES 
(120674, 16, 1463.1, 3405.83, 171.412, 0, 0, 0, 0, 100, 0),
(120674, 15, 1466.85, 3413.33, 171.412, 1.15775, 10000, 0, 0, 100, 0),
(120674, 14, 1473.35, 3414.33, 171.412, 1.86002, 10000, 0, 0, 100, 0),
(120674, 13, 1482.6, 3413.83, 171.412, 0, 12000, 0, 0, 100, 0),
(120674, 12, 1486.1, 3423.83, 171.412, 0.320132, 15000, 0, 0, 100, 0),
(120674, 11, 1487.6, 3437.33, 171.412, 1.28008, 10000, 0, 0, 100, 0),
(120674, 10, 1480.35, 3439.33, 171.412, 0.400882, 10000, 0, 0, 100, 0),
(120674, 9, 1473.35, 3433.08, 171.412, 4.30989, 15000, 0, 0, 100, 0),
(120674, 8, 1465.6, 3434.08, 171.412, 2.50569, 10000, 0, 0, 100, 0),
(120674, 7, 1452.85, 3438.33, 171.412, 0.898095, 12000, 0, 0, 100, 0),
(120674, 6, 1442.6, 3439.58, 171.412, 2.51673, 28000, 0, 0, 100, 0),
(120674, 5, 1439.1, 3431.83, 171.412, 0, 0, 0, 0, 100, 0),
(120674, 4, 1442.1, 3421.83, 171.412, 4.62134, 15000, 0, 0, 100, 0),
(120674, 3, 1451.85, 3416.08, 171.412, 0, 0, 0, 0, 100, 0),
(120674, 2, 1449.6, 3404.08, 171.162, 2.18658, 10000, 0, 0, 100, 0),
(120674, 1, 1456.1, 3400.33, 171.162, 0, 30000, 0, 0, 100, 0);
SET @ENTRY := -224457;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
UPDATE `creature_template` SET `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` = 53565;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -224455;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -224470;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
UPDATE `creature_template` SET `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` = 53565;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -224439;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
UPDATE `creature_template` SET `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` = 53565;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -224449;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
UPDATE `creature_template` SET `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` = 53565;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');
SET @ENTRY := -120673;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -120670;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');
SET @ENTRY := -80101;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -80098;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -80096;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -80094;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -80095;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');
SET @ENTRY := -120693;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -120690;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -120688;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -120686;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -120687;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');
SET @ENTRY := -80056;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -80053;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -80051;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -80050;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -80049;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');
SET @ENTRY := -80370;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -80367;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -80365;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -80364;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -80363;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');
SET @ENTRY := -80519;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -80522;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -80517;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -80516;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -80515;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');
SET @ENTRY := -150011;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -150008;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -150006;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -150005;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');

SET @ENTRY := -150004;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 1, 1, 0, 100, 0, 1000, 5000, 6000, 7000, 10, 509, 543, 511, 507, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random emote: 509, 543, 511, 507,'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33646, 33643, 33645, 33646, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Play random direct sound one of: 33646, 33643, 33645, 33646 of all'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 108967, 0, 0, 0, 0, 0, 9, 57873, 0, 5, 0, 0, 0, 0, 'Every 6 - 7 seconds (1 - 5s initially) (OOC) - Self: Cast spell 108967 on Creature Training Target (57873) in 0 - 5 yards');
UPDATE `creature` SET `position_x`=1485.11, `position_y`=3427.23, `position_z`=171.161, `orientation`=6.16406, `spawndist`=0, `MovementType`=2 WHERE `id`=57754;
UPDATE `creature_template` SET `mindmg`=0, `maxdmg`=0, `unit_flags`=4, `unit_flags2`=0, `flags_extra`=2359380 WHERE `entry`=53714;
UPDATE `creature_template` SET `mindmg`=0, `maxdmg`=0, `unit_flags`=4, `flags_extra`=2359296 WHERE `entry`=57873;UPDATE `gameobject_template` SET `unk1`='', `faction`=33 WHERE `entry`=214409;
DELETE FROM `page_text` WHERE `entry`=4605;
INSERT INTO `page_text` (`entry`, `text`, `next_page`, `WDBVerified`) VALUES
(4605, 'Nodding, the General withdrew a coin from his pocket. "Let us see how the winds blow!" he said, tossing the coin into the air. "If it is heads, our defense will hold. If it is tails, the wall will be overrun."$B$BBy now, many of his men had gathered to see the outcome, and a crowd of soldiers eagerly pressed forward to watch the coin land. It bounced, spun, and came to rest. Heads! Cheers erupted.$B$BThe next day battle was fought. The mantid swarmed and the defenders prevailed. Outnumbered thirty to one, the defenders were victorious.', 0, 1);
DELETE FROM `playercreateinfo_action` WHERE `race`=1 AND `class`=10 AND `button`=81;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES 
(1, 10, 81, 59752, 0);

DELETE FROM `playercreateinfo_action` WHERE `race`=2 AND `class`=10 AND `button`=81;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) 
VALUES (2, 10, 81, 33702, 0);

DELETE FROM `playercreateinfo_action` WHERE `race`=3 AND `class`=10 AND `button`=81;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(3, 10, 81, 20594, 0);

DELETE FROM `playercreateinfo_action` WHERE `race`=4 AND `class`=10 AND `button`=81;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(4, 10, 81, 58984, 0);

DELETE FROM `playercreateinfo_action` WHERE `race`=5 AND `class`=10 AND `button`=81;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(5, 10, 81, 20577, 0);

DELETE FROM `playercreateinfo_action` WHERE `race`=6 AND `class`=10 AND `button`=81;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(6, 10, 81, 20549, 0);

DELETE FROM `playercreateinfo_action` WHERE `race`=7 AND `class`=10 AND `button`=81;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(7, 10, 81, 20589, 0);

DELETE FROM `playercreateinfo_action` WHERE `race`=8 AND `class`=10 AND `button`=81;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(8, 10, 81, 26297, 0);

DELETE FROM `playercreateinfo_action` WHERE `race`=9 AND `class`=10 AND `button`=81;
DELETE FROM `playercreateinfo_action` WHERE `race`=9 AND `class`=10 AND `button`=82;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(9, 10, 81, 69070, 0),
(9, 10, 82, 69041, 0);

DELETE FROM `playercreateinfo_action` WHERE `race`=10 AND `class`=10 AND `button`=81;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(10, 10, 81, 28730, 0);

DELETE FROM `playercreateinfo_action` WHERE `race`=11 AND `class`=10 AND `button`=81;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(11, 10, 81, 59548, 0);

DELETE FROM `playercreateinfo_action` WHERE `race`=24 AND `class`=10 AND `button`=81;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(24, 10, 81, 107079, 0);
DELETE FROM `playercreateinfo_spell_cast` WHERE `classmask`=512 AND `Spell`=103985;
INSERT INTO `playercreateinfo_spell_cast` (`racemask`, `classmask`, `Spell`, `Note`) VALUES (0, 512, 103985, 'Monk - Stance of the Fierce Tiger');
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE  `entry`=54586;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE  `entry`=54587;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE  `entry`=65470;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE  `entry`=65471;

DELETE FROM `spell_area` WHERE `spell`=102403;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(102403, 5825, 29524, 29524, 0, 0, 2, 1, 74, 11),
(102403, 5834, 29524, 29524, 0, 0, 2, 1, 74, 11),
(102403, 5843, 29524, 29524, 0, 0, 2, 1, 74, 11);

DELETE FROM `smart_scripts` WHERE `entryorguid`=54586;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(54586, 0, 0, 1, 11, 0, 100, 0, 0, 0, 0, 0, 42, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - On Respawn - Set Invincibility Hp 1'),
(54586, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 0 - Set React State Defensive'),
(54586, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 1 - Set Phase 0'),
(54586, 0, 3, 0, 1, 0, 100, 0, 2000, 4000, 2000, 4000, 87, 5458601, 5458602, 5458603, 5458604, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - OOC - Run Random Script'),
(54586, 0, 4, 0, 4, 0, 100, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - On Aggro - Set Phase 1'),
(54586, 0, 5, 6, 2, 1, 100, 1, 0, 1, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Between 0-1% Health - Combat Stop'),
(54586, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 5 - Set Faction 35'),
(54586, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 6 - Set Emote State 0'),
(54586, 0, 8, 9, 61, 0, 100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 7 - Say Line Random'),
(54586, 0, 9, 10, 61, 0, 100, 0, 0, 0, 0, 0, 33, 54586, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 8 - Quest Credit'),
(54586, 0, 10, 0, 61, 0, 100, 0, 0, 0, 0, 0, 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 9 - Despawn after 3 sec');

DELETE FROM `smart_scripts` WHERE `entryorguid`=54587;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(54587, 0, 0, 1, 11, 0, 100, 0, 0, 0, 0, 0, 42, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Respawn - Set Invincibility Hp 1'),
(54587, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 0 - Set React State Defensive'),
(54587, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 1 - Set Phase 0'),
(54587, 0, 3, 0, 38, 0, 100, 0, 0, 1, 0, 0, 5, 507, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Data Set 0 1 - Play Emote 507'),
(54587, 0, 4, 0, 38, 0, 100, 0, 0, 2, 0, 0, 5, 508, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Data Set 0 2 - Play Emote 508'),
(54587, 0, 5, 0, 38, 0, 100, 0, 0, 3, 0, 0, 5, 509, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Data Set 0 3 - Play Emote 509'),
(54587, 0, 6, 0, 4, 0, 100, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Aggro - Set Phase 1'),
(54587, 0, 7, 8, 2, 1, 100, 1, 0, 1, 0, 0, 27, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Between 0-1% Health - Combat Stop'),
(54587, 0, 8, 9, 61, 1, 100, 0, 0, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 7 - Set Faction 35'),
(54587, 0, 9, 10, 61, 1, 100, 0, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 8 - Set Emote State 0'),
(54587, 0, 10, 11, 61, 1, 100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 9 - Say Line Random'),
(54587, 0, 11, 12, 61, 1, 100, 0, 0, 0, 0, 0, 33, 54586, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 10 - Quest Credit'),
(54587, 0, 12, 0, 61, 1, 100, 0, 0, 0, 0, 0, 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 11 - Despawn after 3 sec');

DELETE FROM `smart_scripts` WHERE `entryorguid`=65470;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(65470, 0, 0, 1, 11, 0, 100, 0, 0, 0, 0, 0, 42, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - On Respawn - Set Invincibility Hp 1'),
(65470, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 0 - Set React State Defensive'),
(65470, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 1 - Set Phase 0'),
(65470, 0, 3, 0, 1, 0, 100, 0, 2000, 4000, 2000, 4000, 87, 5458601, 5458602, 5458603, 5458604, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - OOC - Run Random Script'),
(65470, 0, 4, 0, 4, 0, 100, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - On Aggro - Set Phase 1'),
(65470, 0, 5, 6, 2, 1, 100, 1, 0, 1, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Between 0-1% Health - Combat Stop'),
(65470, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 5 - Set Faction 35'),
(65470, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 6 - Set Emote State 0'),
(65470, 0, 8, 9, 61, 0, 100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 7 - Say Line Random'),
(65470, 0, 9, 10, 61, 0, 100, 0, 0, 0, 0, 0, 33, 54586, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 8 - Quest Credit'),
(65470, 0, 10, 0, 61, 0, 100, 0, 0, 0, 0, 0, 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 9 - Despawn after 3 sec');

DELETE FROM `smart_scripts` WHERE `entryorguid`=65471;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(65471, 0, 0, 1, 11, 0, 100, 0, 0, 0, 0, 0, 42, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Respawn - Set Invincibility Hp 1'),
(65471, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 0 - Set React State Defensive'),
(65471, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 1 - Set Phase 0'),
(65471, 0, 3, 0, 38, 0, 100, 0, 0, 1, 0, 0, 5, 507, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Data Set 0 1 - Play Emote 507'),
(65471, 0, 4, 0, 38, 0, 100, 0, 0, 2, 0, 0, 5, 508, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Data Set 0 2 - Play Emote 508'),
(65471, 0, 5, 0, 38, 0, 100, 0, 0, 3, 0, 0, 5, 509, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Data Set 0 3 - Play Emote 509'),
(65471, 0, 6, 0, 4, 0, 100, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Aggro - Set Phase 1'),
(65471, 0, 7, 8, 2, 1, 100, 1, 0, 1, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Between 0-1% Health - Combat Stop'),
(65471, 0, 8, 9, 61, 1, 100, 0, 0, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 7 - Set Faction 35'),
(65471, 0, 9, 10, 61, 1, 100, 0, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 8 - Set Emote State 0'),
(65471, 0, 10, 11, 61, 1, 100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 9 - Say Line Random'),
(65471, 0, 11, 12, 61, 1, 100, 0, 0, 0, 0, 0, 33, 54586, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 10 - Quest Credit'),
(65471, 0, 12, 0, 61, 1, 100, 0, 0, 0, 0, 0, 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 11 - Despawn after 3 sec');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5458601;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5458601, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 5, 507, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - On Script - Play Emote 507');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5458602;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5458602, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 5, 509, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - On Script - Play Emote 509');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5458603;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5458603, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 5, 511, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - On Script - Play Emote 511');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5458604;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5458604, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 5, 543, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - On Script - Play Emote 543');

UPDATE `creature_template` SET `unit_flags2`=2048 WHERE `entry`=54586;
UPDATE `creature_template` SET `unit_flags2`=2048 WHERE `entry`=54587;
UPDATE `creature_template` SET `unit_flags2`=2048 WHERE `entry`=65470;
UPDATE `creature_template` SET `unit_flags2`=2048 WHERE `entry`=65471;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE  `entry`=61411;

DELETE FROM `smart_scripts` WHERE `entryorguid`=61411;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(61411, 0, 0, 0, 1, 0, 100, 0, 5000, 5000, 5000, 5000, 87, 6141101, 6141102, 6141103, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Zhi - OOC - Run Random Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=6141101;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(6141101, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 5, 507, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Zhi - Play Emote 507'),
(6141101, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 45, 0, 1, 0, 0, 0, 0, 11, 54587, 10, 0, 0, 0, 0, 0, 'Instructor Zhi - Set Data 0 1'),
(6141101, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 11, 65471, 10, 0, 0, 0, 0, 0, 'Instructor Zhi - Set Data 0 1');

DELETE FROM `smart_scripts` WHERE `entryorguid`=6141102;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(6141102, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 5, 508, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Zhi - Play Emote 508'),
(6141102, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 45, 0, 2, 0, 0, 0, 0, 11, 54587, 10, 0, 0, 0, 0, 0, 'Instructor Zhi - Set Data 0 2'),
(6141102, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 45, 0, 2, 0, 0, 0, 0, 11, 65471, 10, 0, 0, 0, 0, 0, 'Instructor Zhi - Set Data 0 2');

DELETE FROM `smart_scripts` WHERE `entryorguid`=6141103;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(6141103, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 5, 509, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Zhi - Play Emote 509'),
(6141103, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 45, 0, 3, 0, 0, 0, 0, 11, 54587, 10, 0, 0, 0, 0, 0, 'Instructor Zhi - Set Data 0 3'),
(6141103, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 45, 0, 3, 0, 0, 0, 0, 11, 65471, 10, 0, 0, 0, 0, 0, 'Instructor Zhi - Set Data 0 3');
DELETE FROM `playercreateinfo_spell` WHERE `racemask`=8388608 AND `classmask`=4 AND `Spell`=982;
INSERT INTO `playercreateinfo_spell` (`racemask`, `classmask`, `Spell`, `Note`) VALUES
(8388608, 4, 982, 'Revive Pet');

DELETE FROM `playercreateinfo_spell` WHERE `racemask`=8388608 AND `classmask`=4 AND `Spell`=3044;
INSERT INTO `playercreateinfo_spell` (`racemask`, `classmask`, `Spell`, `Note`) VALUES
(8388608, 4, 3044, 'Arcane Shot');

DELETE FROM `playercreateinfo_action` WHERE `race`=24 AND `class`=3;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(24, 3, 11, 982, 0),
(24, 3, 10, 9, 48),
(24, 3, 9, 107079, 0),
(24, 3, 0, 3044, 0);
DELETE FROM `playercreateinfo_action` WHERE `race`=24 AND `class`=1;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(24, 1, 0, 6603, 0),
(24, 1, 9, 107079, 0);
DELETE FROM `playercreateinfo_spell` WHERE `racemask`=8388608 AND `classmask`=8 AND `Spell`=1752;
INSERT INTO `playercreateinfo_spell` (`racemask`, `classmask`, `Spell`, `Note`) VALUES
(8388608, 8, 1752, 'Sinister Strike');

DELETE FROM `playercreateinfo_action` WHERE `race`=24 AND `class`=4;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(24, 4, 9, 107079, 0),
(24, 4, 0, 1752, 0);
DELETE FROM `playercreateinfo_spell` WHERE `racemask`=8388608 AND `classmask`=16 AND `Spell`=585;
INSERT INTO `playercreateinfo_spell` (`racemask`, `classmask`, `Spell`, `Note`) VALUES
(8388608, 16, 585, 'Smite');

DELETE FROM `playercreateinfo_action` WHERE `race`=24 AND `class`=5;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(24, 5, 0, 585, 0),
(24, 5, 9, 107079, 0);
DELETE FROM `playercreateinfo_spell` WHERE `racemask`=8388608 AND `classmask`=64 AND `Spell`=403;
INSERT INTO `playercreateinfo_spell` (`racemask`, `classmask`, `Spell`, `Note`) VALUES
(8388608, 64, 403, 'Lightning Bolt');

DELETE FROM `playercreateinfo_action` WHERE `race`=24 AND `class`=7;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(24, 7, 9, 107079, 0),
(24, 7, 0, 403, 0);
DELETE FROM `playercreateinfo_action` WHERE `race`=24 AND `class`=8 AND `button`=9;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(24, 8, 9, 107079, 0);
DELETE FROM `creature_text` WHERE `entry`=60183;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(60183, 0, 0, 'I hope you\'re ready, $p. Jaomin Ro awaits you just over the bridge.', 12, 0, 100, 113, 0, 0, 'Trainee Nim to Player');
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_mage_mirror_image';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(55342, 'spell_mage_mirror_image');
DELETE FROM `spell_script_names` WHERE `spell_id`=46968;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(46968, 'spell_warr_shockwave');
DELETE FROM `page_text` WHERE `entry`=4610;
INSERT INTO `page_text` (`entry`, `text`, `next_page`, `WDBVerified`) VALUES
(4610, 'At that moment, thunder and lightning tore open the sky as if it were a sheet, and a great rain poured down. The little boat soon became swamped and flipped over. Casting off his overcoat, the Angler began swimming for the shore.$B$B"Help us!" the monks called out, as the swells began to crash around them. "We cannot swim!"The Angler answered: "It would seem to me that you have wasted your lives!"', 0, 18414);
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE  `entry`=53565;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE  `entry`=65469;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE  `entry`=57753;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE  `entry`=57752;

UPDATE `creature` SET `MovementType`=2 WHERE `guid`=224456;

DELETE FROM `creature_addon` WHERE `guid`=120680;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (120680, 0, 0, 1, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=120679;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (120679, 0, 0, 1, 0, 0, NULL);

DELETE FROM `smart_scripts` WHERE `entryorguid`=-120680;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(-120680, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 1, 5, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 5'),
(-120680, 0, 1, 0, 52, 0, 100, 0, 5, 53565, 0, 0, 1, 6, 15000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 5 Over - Say Text Line 6'),
(-120680, 0, 2, 0, 52, 0, 100, 0, 6, 53565, 0, 0, 1, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 6 Over - Say Text Line 7');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-120679;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(-120679, 0, 0, 0, 1, 0, 100, 0, 20000, 20000, 120000, 120000, 1, 1, 16000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - OOC - Say Text Line 1'),
(-120679, 0, 1, 0, 52, 0, 100, 0, 1, 53565, 0, 0, 1, 2, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 2'),
(-120679, 0, 2, 0, 52, 0, 100, 0, 2, 53565, 0, 0, 1, 3, 11000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 2 Over - Say Text Line 3'),
(-120679, 0, 3, 0, 52, 0, 100, 0, 3, 53565, 0, 0, 1, 4, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 3 Over - Say Text Line 4'),
(-120679, 0, 4, 0, 52, 0, 100, 0, 4, 53565, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 53565, 3, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 4 Over - Set Data 1 1');

DELETE FROM `smart_scripts` WHERE `entryorguid`=53565;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(53565, 0, 0, 1, 38, 0, 100, 0, 1, 1, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 1 1 - Set Phase 1'),
(53565, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 17, 510, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 1 1 (Link) - Set Emote State (510)'),
(53565, 0, 2, 3, 60, 1, 100, 0, 1000, 1000, 5000, 5000, 11, 108967, 0, 0, 0, 0, 0, 11, 0, 4, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Update - Cast Spell \'Jab\''),
(53565, 0, 3, 4, 61, 1, 100, 0, 0, 0, 0, 0, 10, 507, 509, 511, 543, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Update (Link) - Play Random Emote (507, 509, 511, 543)'),
(53565, 0, 4, 0, 61, 1, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Update (Link) - Play Random Sound (33643, 33645, 33646)'),
(53565, 0, 5, 6, 38, 0, 100, 0, 2, 2, 0, 0, 22, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 2 2 - Set Phase 2'),
(53565, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 17, 30, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 2 2 (Link) - Clear Emote'),
(53565, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 2 2 (Link) - Say Text Line 0'),
(53565, 0, 8, 0, 38, 0, 100, 0, 3, 1, 0, 0, 80, 5356500, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 3 1 - Start Script'),
(53565, 0, 9, 0, 38, 0, 100, 0, 3, 2, 0, 0, 80, 5356502, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 3 2 - Start Script'),
(53565, 0, 10, 0, 38, 0, 100, 0, 3, 3, 0, 0, 80, 5356503, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 3 3 - Start Script'),
(53565, 0, 11, 0, 38, 0, 100, 0, 3, 4, 0, 0, 80, 5356504, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 3 4 - Start Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=65469;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(65469, 0, 0, 1, 38, 0, 100, 0, 1, 1, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 1 1 - Set Phase 1'),
(65469, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 17, 510, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 1 1 (Link) - Set Emote State (510)'),
(65469, 0, 2, 3, 60, 1, 100, 0, 1000, 1000, 5000, 5000, 11, 108967, 0, 0, 0, 0, 0, 11, 0, 4, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Update - Cast Spell \'Jab\''),
(65469, 0, 3, 4, 61, 1, 100, 0, 0, 0, 0, 0, 10, 507, 509, 511, 543, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Update (Link) - Play Random Emote (507, 509, 511, 543)'),
(65469, 0, 4, 0, 61, 1, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Update (Link) - Play Random Sound (33643, 33645, 33646)'),
(65469, 0, 5, 6, 38, 0, 100, 0, 2, 2, 0, 0, 22, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 2 2 - Set Phase 2'),
(65469, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 17, 30, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 2 2 (Link) - Clear Emote'),
(65469, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 2 2 (Link) - Say Text Line 0'),
(65469, 0, 8, 0, 38, 0, 100, 0, 3, 1, 0, 0, 80, 5356500, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 3 1 - Start Script'),
(65469, 0, 9, 0, 38, 0, 100, 0, 3, 2, 0, 0, 80, 5356502, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 3 2 - Start Script'),
(65469, 0, 10, 0, 38, 0, 100, 0, 3, 3, 0, 0, 80, 5356503, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 3 3 - Start Script'),
(65469, 0, 11, 0, 38, 0, 100, 0, 3, 4, 0, 0, 80, 5356504, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 3 4 - Start Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5356500;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(5356500, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 5, 507, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - Play Emote (507)'),
(5356500, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - Play Random Sound (33643, 33645, 33646)');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5356501;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(5356501, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 5, 508, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - Play Emote (508)'),
(5356501, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - Play Random Sound (33643, 33645, 33646)');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5356502;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(5356502, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 5, 509, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - Play Emote (509)'),
(5356502, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - Play Random Sound (33643, 33645, 33646)');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5356503;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(5356503, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 5, 511, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - Play Emote (511)'),
(5356503, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - Play Random Sound (33643, 33645, 33646)');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5356504;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(5356504, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 5, 543, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - Play Emote (543)'),
(5356504, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - Play Random Sound (33643, 33645, 33646)');

DELETE FROM `smart_scripts` WHERE `entryorguid`=57748;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(57748, 0, 0, 0, 1, 0, 100, 0, 0, 0, 5000, 5000, 87, 5774800, 5774801, 5774802, 5774803, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Qun - OOC - Start Random Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5774800;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(5774800, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 10, 507, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Qun - Play Emote (507)'),
(5774800, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Qun - Play Random Sound (33643, 33645, 33646)'),
(5774800, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 45, 3, 1, 0, 0, 0, 0, 11, 0, 15, 0, 0, 0, 0, 0, 'Instructor Qun - Set Data 3 1');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5774801;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(5774801, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 10, 509, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Qun - Play Emote'),
(5774801, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Qun - Play Random Sound (33643, 33645, 33646)'),
(5774801, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 45, 3, 2, 0, 0, 0, 0, 11, 0, 15, 0, 0, 0, 0, 0, 'Instructor Qun - Set Data 3 2');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5774802;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(5774802, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 10, 511, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Qun - Play Emote'),
(5774802, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Qun - Play Random Sound (33643, 33645, 33646)'),
(5774802, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 45, 3, 3, 0, 0, 0, 0, 11, 0, 15, 0, 0, 0, 0, 0, 'Instructor Qun - Set Data 3 3');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5774803;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(5774803, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 10, 543, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Qun - Play Emote'),
(5774803, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Qun - Play Random Sound (33643, 33645, 33646)'),
(5774803, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 45, 3, 4, 0, 0, 0, 0, 11, 0, 15, 0, 0, 0, 0, 0, 'Instructor Qun - Set Data 3 4');

DELETE FROM `smart_scripts` WHERE `entryorguid`=57752;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(57752, 0, 0, 1, 1, 0, 100, 0, 0, 0, 5000, 5000, 10, 507, 509, 511, 543, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Quiet Lam - OOC - Play Random Emote (507, 509, 511, 543)'),
(57752, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 'Quiet Lam - Play Random Sound (33643, 33645, 33646)');

DELETE FROM `smart_scripts` WHERE `entryorguid`=57753;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(57753, 0, 0, 1, 1, 0, 100, 0, 0, 0, 5000, 5000, 10, 507, 509, 511, 543, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ironfist Zhou - OOC - Play Random Emote (507, 509, 511, 543)'),
(57753, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 'Ironfist Zhou - Play Random Sound (33643, 33645, 33646)');

DELETE FROM `creature_text` WHERE `entry`=53565;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES 
(53565, 0, 0, 'Such skill for a new trainee.', 12, 0, 100, 511, 0, 0, 'Aspiring Trainee'),
(53565, 0, 1, 'That target didn\'t stand a chance.', 12, 0, 100, 511, 0, 0, 'Aspiring Trainee'),
(53565, 0, 2, 'One day you will have to teach me your secrets.', 12, 0, 100, 509, 0, 0, 'Aspiring Trainee'),
(53565, 0, 3, 'You are bound for a great things, trainee.', 12, 0, 100, 508, 0, 0, 'Aspiring Trainee'),
(53565, 0, 4, 'I must train harder so I can be like you.', 12, 0, 100, 1, 0, 0, 'Aspiring Trainee'),
(53565, 0, 5, 'I hope the instructors saw that!', 12, 0, 100, 1, 0, 0, 'Aspiring Trainee'),
(53565, 0, 6, 'I must work as hard as you.', 12, 0, 100, 1, 0, 0, 'Aspiring Trainee'),
(53565, 0, 7, 'Your form is impeccable.', 12, 0, 100, 1, 0, 0, 'Aspiring Trainee'),
(53565, 1, 0, 'My money\'s on Lam. Did you see him take down Chu during training last week?', 12, 0, 100, 1, 0, 0, 'Aspiring Trainee'),
(53565, 2, 0, 'His stamina is nothing against Lam\'s strength.', 12, 0, 100, 0, 0, 0, 'Aspiring Trainee'),
(53565, 3, 0, 'He has the legs of an ox! Have you ever been kicked by him in training?', 12, 0, 100, 1, 0, 0, 'Aspiring Trainee'),
(53565, 4, 0, 'Raise you a keg of Stormshout Gold. It\'ll be your loss.', 12, 0, 100, 1, 0, 0, 'Aspiring Trainee'),
(53565, 5, 0, 'That\'s where you\'re mistaken, friend. Zhou is as sturdy as an oak.', 12, 0, 100, 1, 0, 0, 'Aspiring Trainee'),
(53565, 6, 0, 'Zhou has the stamina of three men, so he can out-stay most opponents.', 12, 0, 100, 0, 0, 0, 'Aspiring Trainee'),
(53565, 7, 0, 'Four coppers on Zhou.', 12, 0, 100, 0, 0, 0, 'Aspiring Trainee');

DELETE FROM `creature_text` WHERE `entry`=65469;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES 
(65469, 0, 0, 'Such skill for a new trainee.', 12, 0, 100, 511, 0, 0, 'Aspiring Trainee to Training Target'),
(65469, 0, 1, 'That target didn\'t stand a chance.', 12, 0, 100, 509, 0, 0, 'Aspiring Trainee to Training Target'),
(65469, 0, 2, 'One day you will have to teach me your secrets.', 12, 0, 100, 509, 0, 0, 'Aspiring Trainee to Training Target'),
(65469, 0, 3, 'You are bound for a great things, trainee.', 12, 0, 100, 508, 0, 0, 'Aspiring Trainee to Training Target');
UPDATE `smart_scripts` SET `action_param5`=5774804 WHERE `entryorguid`=57748;

INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5774804, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 10, 508, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Qun - Play Emote 508'),
(5774804, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 115, 33643, 33645, 33646, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Qun - Play Random Sound (33643, 33645, 33646)'),
(5774804, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 3, 5, 0, 0, 0, 0, 11, 0, 15, 0, 0, 0, 0, 0, 'Instructor Qun - Set Data 3 5');

DELETE FROM `smart_scripts` WHERE `entryorguid`=53565 AND `source_type`=0 AND `id`=12;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(53565, 0, 12, 0, 38, 0, 100, 0, 3, 5, 0, 0, 0, 80, 5356501, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 3 5 - Start Script');
DELETE FROM `smart_scripts` WHERE `entryorguid`=65469 AND `source_type`=0 AND `id`=12;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(65469, 0, 12, 0, 38, 0, 100, 0, 3, 5, 0, 0, 0, 80, 5356501, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Data Set 3 5 - Start Script');
UPDATE `creature` SET `id`=54587 WHERE `guid`=224359;
UPDATE `creature` SET `id`=54587 WHERE `guid`=224374;

DELETE FROM `smart_scripts` WHERE `entryorguid`=-224377;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-224377, 0, 0, 0, 1, 0, 100, 0, 5000, 5000, 5000, 5000, 87, 5458701, 5458702, 5458703, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - OOC - Run Random Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5458701;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5458701, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 5, 507, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Script - Play Emote 507'),
(5458701, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 11, 54587, 10, 0, 0, 0, 0, 0, 'Tushui Trainee - On Script - Set Data 0 1');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5458702;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5458702, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 5, 508, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Script - Play Emote 508'),
(5458702, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 2, 0, 0, 0, 0, 11, 54587, 10, 0, 0, 0, 0, 0, 'Tushui Trainee - On Script - Set Data 0 2');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5458703;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5458703, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 5, 509, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Script - Play Emote 509'),
(5458703, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 3, 0, 0, 0, 0, 11, 54587, 10, 0, 0, 0, 0, 0, 'Tushui Trainee - On Script - Set Data 0 3');
UPDATE `creature_template` SET `faction_A`=2101, `faction_H`=2101 WHERE `entry`=54587;
UPDATE `creature_template` SET `faction_A`=2101, `faction_H`=2101 WHERE `entry`=65471;
DELETE FROM `creature_text` WHERE `entry`=59642;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(59642, 0, 0, 'Keep those creatures at bay while I meditate. We\'ll soon have the answers we seek.', 12, 0, 100, 0, 0, 27398, 'Aysa Cloudsinger to Player'),
(59642, 1, 0, 'And so our path lays before us. Speak to Master Shang Xi, he will tell you what comes next.', 12, 0, 100, 0, 0, 27399, 'Aysa Cloudsinger to Player');
DELETE FROM `quest_poi` WHERE `questId`=29522;
INSERT INTO `quest_poi` (`questId`, `id`, `objIndex`, `mapid`, `WorldMapAreaId`, `FloorId`, `unk3`, `unk4`) VALUES
(29522, 0, -1, 860, 808, 0, 0, 1);

DELETE FROM `quest_poi_points` WHERE `questId`=29522;
INSERT INTO `quest_poi_points` (`questId`, `id`, `idx`, `x`, `y`) VALUES
(29522, 0, 0, 1407, 3637);
UPDATE `creature_addon` SET `auras`='126968' WHERE `guid`=224302;
DELETE FROM `creature` WHERE `guid` IN(224616, 224614, 224613, 224606, 224619, 224611, 224609, 224618, 224615, 224607, 224612, 224605, 224610, 224617, 224608, 224604);
