-- Uldum, Schnottz's Landing: the "Gobbles!" ritual (quest 27950). Talking to Commander Schnottz with the quest taken
-- starts a scene in phase 312 (Phase.dbc "Gobbles Completion Event"): his double and Belloc Brightblade are summoned,
-- Gobbles walks in, the double reads his notes, fires on Gobbles and counts only eight idols. The player gets the kill
-- credit of 47249 (the quest objective) and the scene ends. The script side is in zone_uldum.cpp.
--
-- Scene actors are summons and get phase 312 only (npc_gobbles_ritual_actor); the rune and the idols are spawns of
-- phase 312. The player keeps the default phase 169 and gains 312 from aura 88111 while chapter 1 (313) is off.
-- 88111 is limited to the area, so leaving it ends the scene; the cleanup spell 88143 ends it when it is over.
-- Ported from The Cataclysm Preservation Project 4.3.4; the 5.4.8 creature flags are from Legends of Azeroth.
--
-- New spawns: gameobject guids 1031434 to 1031442.

-- Templates: Schnottz, his double, Belloc and Gobbles run SmartAI; the three scene actors share the phase script
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 47159;
UPDATE `creature_template` SET `AIName` = 'SmartAI', `ScriptName` = 'npc_gobbles_ritual_actor' WHERE `entry` IN (47249, 47251, 47255);
UPDATE `creature_template` SET `unit_flags` = 768 WHERE `entry` = 47251;
UPDATE `creature_template` SET `unit_flags` = 256 WHERE `entry` = 47255;

-- The ritual option is shown while the quest is taken and the ritual is not running; the menu text follows the quest
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 12213 AND `SourceEntry` = 0 AND `SourceId` = 0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 14 AND `SourceGroup` = 12213 AND `SourceEntry` IN (17153, 17192) AND `SourceId` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 12213, 0, 0, 0, 9, 0, 27950, 0, 0, 0, 0, 0, '', 'Show gossip menu 12213 option id 0 if quest Gobbles! is taken'),
(15, 12213, 0, 0, 0, 1, 0, 88111, 0, 0, 1, 0, 0, '', 'and the ritual is not running'),
(14, 12213, 17153, 0, 0, 8, 0, 27950, 0, 0, 1, 0, 0, '', 'Show gossip menu 12213 text id 17153 if quest Gobbles! has not been rewarded'),
(14, 12213, 17192, 0, 0, 8, 0, 27950, 0, 0, 0, 0, 0, '', 'Show gossip menu 12213 text id 17192 if quest Gobbles! has been rewarded');

-- Spells: Summon Belloc and Summon Gobbles land at fixed places, Phase Player works only at the Landing
DELETE FROM `spell_target_position` WHERE `id` IN (88109, 88117);
INSERT INTO `spell_target_position` (`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(88109, 0, 1, -10694.4, 932.092, 26.74553, 5.427974),
(88117, 0, 1, -10677.1, 942.766, 25.2108, 3.73246);

DELETE FROM `spell_area` WHERE `spell` = 88111 AND `area` = 5602;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(88111, 5602, 0, 0, 0, 0, 2, 0, 64, 64);

DELETE FROM `spell_script_names` WHERE `spell_id` IN (88107, 88108, 88111);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(88107, 'spell_gobbles_initialize'),
(88108, 'spell_summon_schnottz'),
(88111, 'spell_gobbles_phase_player');

-- Creator Summon Gobbles: the player summons Gobbles. Clean-up: the player loses Phase Player and the detection of the
-- real Gobbles, which the quest credit has just turned
DELETE FROM `spell_scripts` WHERE `id` IN (88116, 88143);
INSERT INTO `spell_scripts` (`id`, `effIndex`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES
(88116, 0, 0, 15, 88117, 2, 1, 0, 0, 0, 0),
(88143, 0, 0, 14, 88144, 0, 0, 0, 0, 0, 0),
(88143, 0, 0, 14, 88111, 0, 0, 0, 0, 0, 0);

-- Ritual rune and the eight idols, phase 312
DELETE FROM `gameobject` WHERE `guid` BETWEEN 1031434 AND 1031442;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(1031434, 206572, 1, 1, 312, 0, -10690.65, 924.967, 26.63605, 3.351047, 0, 0, -0.9945211, 0.1045355, 120, 255, 1),
(1031435, 206571, 1, 1, 312, 0, -10689.2, 926.821, 26.4897, 5.759588, 0, 0, -0.2588186, 0.9659259, 120, 255, 1),
(1031436, 206571, 1, 1, 312, 0, -10688.24, 925.1094, 26.46245, 4.852017, 0, 0, -0.6560583, 0.7547102, 120, 255, 1),
(1031437, 206571, 1, 1, 312, 0, -10691, 927.444, 26.5564, 0.05235888, 0, 0, 0.02617645, 0.9996573, 120, 255, 1),
(1031438, 206571, 1, 1, 312, 0, -10688.82, 923.2153, 26.49851, 3.996807, 0, 0, -0.9099607, 0.4146944, 120, 255, 1),
(1031439, 206571, 1, 1, 312, 0, -10692.7, 926.51, 26.6309, 0.8901166, 0, 0, 0.4305105, 0.9025856, 120, 255, 1),
(1031440, 206571, 1, 1, 312, 0, -10690.58, 922.3333, 26.57419, 3.054327, 0, 0, 0.9990482, 0.04361926, 120, 255, 1),
(1031441, 206571, 1, 1, 312, 0, -10693.21, 924.6493, 26.66352, 1.588249, 0, 0, 0.7132502, 0.7009096, 120, 255, 1),
(1031442, 206571, 1, 1, 312, 0, -10692.2, 922.896, 26.6352, 2.426008, 0, 0, 0.9366722, 0.3502074, 120, 255, 1);

-- Paths: Schnottz's double in seven steps, Gobbles in one
DELETE FROM `waypoints` WHERE `entry` IN (47255, 472490, 472491, 472492, 472493, 472494, 472495, 472496);
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(472490, 1, -10684, 930.243, 26.2597, 'Commander Schnottz'),
(472490, 2, -10684.4, 932.252, 26.26217, 'Commander Schnottz'),
(472491, 1, -10690.6, 925.205, 26.55584, 'Commander Schnottz'),
(472492, 1, -10692.4, 930.745, 26.59044, 'Commander Schnottz'),
(472493, 1, -10691, 925.96, 26.56671, 'Commander Schnottz'),
(472494, 1, -10684.4, 929.488, 26.28065, 'Commander Schnottz'),
(472494, 2, -10684.4, 932.471, 26.26071, 'Commander Schnottz'),
(472495, 1, -10687.5, 927.516, 26.41711, 'Commander Schnottz'),
(472496, 1, -10676.04, 927.8622, 26.31276, 'Commander Schnottz'),
(47255, 1, -10676.5, 940.067, 25.52802, 'Gobbles'),
(47255, 2, -10676, 938.067, 26.52802, 'Gobbles'),
(47255, 3, -10676, 937.567, 26.52802, 'Gobbles'),
(47255, 4, -10675, 933.067, 26.52802, 'Gobbles'),
(47255, 5, -10674.75, 932.067, 26.52802, 'Gobbles'),
(47255, 6, -10681, 925.317, 26.27802, 'Gobbles'),
(47255, 7, -10690.7, 924.821, 26.5624, 'Gobbles');

-- Texts of Schnottz's double
DELETE FROM `creature_text` WHERE `entry` = 47249;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(47249, 0, 0, 'Very vell, zhen. Let me just check my notes vone lasht time...', 12, 0, 100, 396, 0, 0, 'Commander Schnottz'),
(47249, 1, 0, 'Mmm-hmmm, yes, yes... simple enough...', 12, 0, 100, 396, 0, 0, 'Commander Schnottz'),
(47249, 2, 0, 'Oh, Gobbles! Ve''re ready to begin! Come, come!', 14, 0, 100, 22, 0, 0, 'Commander Schnottz'),
(47249, 3, 0, 'Text: Hold on, Gobbles! Shtand back everyvone!', 12, 0, 100, 22, 0, 0, 'Commander Schnottz'),
(47249, 4, 0, 'VHAT!?', 14, 0, 100, 388, 0, 0, 'Commander Schnottz'),
(47249, 5, 0, 'My poor Gobbles...', 12, 0, 100, 0, 0, 0, 'Commander Schnottz'),
(47249, 6, 0, 'Vhat could haf possibly gone vrong? I followed every lasht detail...', 14, 0, 100, 22, 0, 0, 'Commander Schnottz'),
(47249, 7, 0, 'Vait a shekkont! Vhat''s zhis!?', 12, 0, 100, 6, 0, 0, 'Commander Schnottz'),
(47249, 8, 0, 'You only brought me eight idols!', 14, 0, 100, 397, 0, 0, 'Commander Schnottz'),
(47249, 9, 0, 'Zhe ritual shpecifically calls for NINE!!', 14, 0, 100, 0, 0, 0, 'Commander Schnottz'),
(47249, 10, 0, 'NINE! NINE! NINE! NINE! NINE!!!', 14, 0, 100, 5, 0, 0, 'Commander Schnottz'),
(47249, 11, 0, 'Out of my sight, vizh you! Auch...', 14, 0, 100, 18, 0, 0, 'Commander Schnottz');

-- SmartAI: Schnottz starts the spell, his double runs the seven lists, Gobbles walks in and falls
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (47159, 47249, 47255);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` IN (472490, 472491, 472492, 472493, 472494, 472495, 472496, 472550);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(47159, 0, 0, 1, 62, 0, 100, 0, 12213, 0, 0, 0, 0, 11, 88107, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz - On Gossip Option 0 Selected - Cast ''Gobbles Initialize'''),
(47159, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz - On Gossip Option 0 Selected - Close Gossip'),
(47249, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - On Just Summoned - Face Summoner'),
(47249, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 472490, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - On Just Summoned - Run Script'),
(47249, 0, 2, 0, 58, 0, 100, 0, 2, 472490, 0, 0, 0, 80, 472491, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - On Path 472490 Ended - Run Script'),
(47249, 0, 3, 0, 58, 0, 100, 0, 1, 472491, 0, 0, 0, 80, 472492, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - On Path 472491 Ended - Run Script'),
(47249, 0, 4, 0, 58, 0, 100, 0, 1, 472492, 0, 0, 0, 80, 472493, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - On Path 472492 Ended - Run Script'),
(47249, 0, 5, 0, 58, 0, 100, 0, 1, 472493, 0, 0, 0, 80, 472494, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - On Path 472493 Ended - Run Script'),
(47249, 0, 6, 0, 58, 0, 100, 0, 2, 472494, 0, 0, 0, 80, 472495, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - On Path 472494 Ended - Run Script'),
(47249, 0, 7, 0, 58, 0, 100, 0, 1, 472495, 0, 0, 0, 80, 472496, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - On Path 472495 Ended - Run Script'),
(47249, 0, 8, 9, 58, 0, 100, 0, 1, 472496, 0, 0, 0, 11, 88143, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - On Path 472496 Ended - Cast ''Clean-up'''),
(47249, 0, 9, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - On Path 472496 Ended - Despawn'),
(472490, 9, 0, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Say 0'),
(472490, 9, 1, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 53, 0, 472490, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Start Path 472490'),
(472491, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 5, 432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Play Emote 432'),
(472491, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Say 1'),
(472491, 9, 2, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 53, 0, 472491, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Start Path 472491'),
(472492, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Say 2'),
(472492, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 90, 8, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Kneel'),
(472492, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 17, 133, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Set Emote State 133'),
(472492, 9, 3, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 5, 21, 0, 0, 0, 0, 0, 11, 47251, 10, 1, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Belloc Play Emote 21'),
(472492, 9, 4, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 91, 8, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Stand'),
(472492, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Set Emote State 0'),
(472492, 9, 6, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 88116, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Cast ''Creator Summon Gobbles'''),
(472492, 9, 7, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 53, 0, 472492, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Start Path 472492'),
(472493, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 4.869469, 'Commander Schnottz (ritual) - Script - Face 4.869469'),
(472493, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Say 3'),
(472493, 9, 2, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, 11, 88118, 2, 0, 0, 0, 0, 11, 47255, 10, 1, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Cast ''Fire LPAC at Gobbles'''),
(472493, 9, 3, 0, 0, 0, 100, 0, 8000, 8000, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Say 4'),
(472493, 9, 4, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 1, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Say 5'),
(472493, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 53, 0, 472493, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Start Path 472493'),
(472494, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 90, 8, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Kneel'),
(472494, 9, 1, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, 1, 6, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Say 6'),
(472494, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 91, 8, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Stand'),
(472494, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 53, 0, 472494, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Start Path 472494'),
(472495, 9, 0, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 1, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Say 7'),
(472495, 9, 1, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 53, 1, 472495, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Start Path 472495'),
(472496, 9, 0, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 1, 8, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Say 8'),
(472496, 9, 1, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, 1, 9, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Say 9'),
(472496, 9, 2, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, 1, 10, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Say 10'),
(472496, 9, 3, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 1, 11, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Say 11'),
(472496, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 88127, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Cast ''Kill Credit'''),
(472496, 9, 5, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 53, 0, 472496, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Commander Schnottz (ritual) - Script - Start Path 472496'),
(47255, 0, 0, 0, 1, 0, 100, 1, 0, 0, 0, 0, 0, 53, 1, 47255, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gobbles (ritual) - Out of Combat - Start Path 47255'),
(47255, 0, 1, 0, 58, 0, 100, 0, 7, 47255, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1.710423, 'Gobbles (ritual) - On Path 47255 Ended - Face 1.710423'),
(47255, 0, 2, 0, 8, 0, 100, 0, 88119, 0, 0, 0, 0, 80, 472550, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gobbles (ritual) - On Spell Hit ''Gobbles Ping'' - Run Script'),
(472550, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 11, 35426, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gobbles (ritual) - Script - Cast ''Arcane Explosion Visual'''),
(472550, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 29266, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gobbles (ritual) - Script - Cast ''Permanent Feign Death''');
