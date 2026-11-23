-- The Grasp Weakens (25006): Shadow Priestess Malia (39117) has the gossip menu 11157 in the template but that menu had no
-- option, and her script waited for the old menu 11156, so nobody could ask her to begin and the Shadow of Agamand (the
-- kill objective, 38981) never came. Her script, the nine step walk to the altar, her lines and those of the Spirit of
-- Devlin Agamand and of the Shadow are those of Legends of Azeroth 5.4.8: the option shows to players who have the
-- quest, she walks the path, speaks, summons the Spirit for 37 seconds and then the Shadow for 60 seconds. The older
-- script of the 4.3.4 data (two action lists on menu 11156) is replaced.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (38981) AND `ScriptName` = '';

DELETE FROM `gossip_menu_option` WHERE `MenuID` = 11157 AND `OptionID` = 0;
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcflag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(11157, 0, 0, 'I am ready.', 35320, 1, 1, 0, 0, 0, 0, NULL, 0, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 11157 AND `SourceEntry` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 11157, 0, 0, 0, 9, 0, 25006, 0, 0, 0, 0, 0, '', 'Show the gossip option if the player has The Grasp Weakens');

DELETE FROM `waypoints` WHERE `entry` = 39117;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(39117, 1, 2240.83, 232.322, 34.5085, ''),
(39117, 2, 2238.01, 231.717, 36.3729, ''),
(39117, 3, 2235.51, 230.778, 38.2083, ''),
(39117, 4, 2234.68, 228.005, 38.2285, ''),
(39117, 5, 2236.55, 226.433, 38.2023, ''),
(39117, 6, 2238.17, 227.082, 39.2165, ''),
(39117, 7, 2240.67, 228.071, 41.1279, ''),
(39117, 8, 2242.54, 228.779, 41.8125, ''),
(39117, 9, 2244.07, 228.294, 41.8125, '');

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (39117,38981) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38981, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadow of Agamand - On Aggro - Say Line 0'),
(39117, 0, 0, 1, 62, 0, 100, 0, 11157, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Shadow Priestess Malia - On Gossip Select - Store Target'),
(39117, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 83, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadow Priestess Malia - On Gossip Select - Remove Gossip Flag'),
(39117, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 53, 0, 39117, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadow Priestess Malia - On Gossip Select - Start WP'),
(39117, 0, 3, 0, 40, 0, 100, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadow Priestess Malia - On WP Reached 1 - Say Line 0'),
(39117, 0, 4, 0, 40, 0, 100, 0, 9, 0, 0, 0, 0, 80, 3911700, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadow Priestess Malia - On WP Reached 9 - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (3911700, 3911701) AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(3911700, 9, 0, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadow Priestess Malia - Script - Say Line 1'),
(3911700, 9, 1, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadow Priestess Malia - Script - Say Line 2'),
(3911700, 9, 2, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadow Priestess Malia - Script - Say Line 3'),
(3911700, 9, 3, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadow Priestess Malia - Script - Say Line 4'),
(3911700, 9, 4, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 12, 38980, 3, 37000, 0, 0, 0, 8, 0, 0, 0, 2247.13, 228.041, 44.2511, 0, 'Shadow Priestess Malia - Script - Summon Spirit of Devlin Agamand'),
(3911700, 9, 5, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 38980, 50, 0, 0, 0, 0, 0, 'Shadow Priestess Malia - Script - Say Line 0'),
(3911700, 9, 6, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 19, 38980, 50, 0, 0, 0, 0, 0, 'Shadow Priestess Malia - Script - Say Line 1'),
(3911700, 9, 7, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 19, 38980, 50, 0, 0, 0, 0, 0, 'Shadow Priestess Malia - Script - Say Line 2'),
(3911700, 9, 8, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 19, 38980, 50, 0, 0, 0, 0, 0, 'Shadow Priestess Malia - Script - Say Line 3'),
(3911700, 9, 9, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 19, 38980, 50, 0, 0, 0, 0, 0, 'Shadow Priestess Malia - Script - Say Line 4'),
(3911700, 9, 10, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 5, 0, 0, 0, 0, 0, 19, 38980, 50, 0, 0, 0, 0, 0, 'Shadow Priestess Malia - Script - Say Line 5'),
(3911700, 9, 11, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 12, 38983, 3, 1000, 0, 0, 0, 8, 0, 0, 0, 2247.13, 228.041, 44.2511, 0, 'Shadow Priestess Malia - Script - Summon Shadow of Agamand'),
(3911700, 9, 12, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 12, 38981, 1, 60000, 0, 0, 0, 8, 0, 0, 0, 2247.13, 228.041, 44.2511, 0, 'Shadow Priestess Malia - Script - Summon Shadow of Agamand'),
(3911700, 9, 13, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadow Priestess Malia - Script - Despawn');


DELETE FROM `creature_text` WHERE `entry` IN (39117, 38980, 38981);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(38980, 0, 0, 'What''s going on here?', 12, 0, 100, 0, 0, 0, 'Spirit of Devlin Agamand - line 0'),
(38980, 1, 0, 'Wait, am I... home?', 12, 0, 100, 0, 0, 0, 'Spirit of Devlin Agamand - line 1'),
(38980, 2, 0, 'You people... what are you doing here? Why are you in my house?', 12, 0, 100, 0, 0, 0, 'Spirit of Devlin Agamand - line 2'),
(38980, 3, 0, 'Mother... father... Thurman... where are you?', 12, 0, 100, 0, 0, 0, 'Spirit of Devlin Agamand - line 3'),
(38980, 4, 0, 'No... I remember. My family is dead. And so am I.', 12, 0, 100, 0, 0, 0, 'Spirit of Devlin Agamand - line 4'),
(38980, 5, 0, 'And my soul.... my soul belongs to the Scourge!', 12, 0, 100, 0, 0, 0, 'Spirit of Devlin Agamand - line 5'),
(38980, 6, 0, 'DIE, YOU WRETCHES!', 14, 0, 100, 0, 0, 0, 'Spirit of Devlin Agamand - line 6'),
(38981, 0, 0, 'DIE, YOU WRETCHES!', 14, 0, 100, 0, 0, 0, 'Shadow of Agamand - line 0'),
(39117, 0, 0, 'Very well. Follow me.', 12, 0, 100, 0, 0, 0, 'Shadow Priestess Malia - line 0'),
(39117, 1, 0, 'Now it is time for us to begin.', 12, 0, 100, 1, 0, 0, 'Shadow Priestess Malia - line 1'),
(39117, 2, 0, 'Devlin Agamand! Listen to my voice!', 14, 0, 100, 22, 0, 0, 'Shadow Priestess Malia - line 2'),
(39117, 3, 0, 'Your mortal remains have been gathered, here in this place where you spent your childhood!', 14, 0, 100, 22, 0, 0, 'Shadow Priestess Malia - line 3'),
(39117, 4, 0, 'Resist the Lich King''s will, Devlin! Come to us!', 14, 0, 100, 22, 0, 0, 'Shadow Priestess Malia - line 4'),
(39117, 5, 0, 'I had better return to my post. You can find me downstairs if you need anything.', 12, 0, 100, 1, 0, 0, 'Shadow Priestess Malia - line 5');
