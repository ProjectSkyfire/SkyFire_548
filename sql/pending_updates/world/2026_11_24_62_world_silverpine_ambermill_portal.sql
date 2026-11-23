-- Transdimensional Warfare (27512, 27513): the Ambermill Dimensional Portal (45752) had no gossip option, no script and
-- the Portal to the Ambermill spell (85360) no destination, so there was no way into Ambermill. Choosing the option
-- now makes the player cast the spell on themselves and closes the gossip; the spell lands inside the keep.
-- Transdimensional Shift (85361) is the screen effect the source applies inside Ambermill (area 233) until the player
-- has finished Transdimensional Warfare: Chapter III (27518).
-- All rows are as in Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 45752 AND `ScriptName` = '';

DELETE FROM `gossip_menu_option` WHERE `MenuID` = 12054 AND `OptionID` = 0;
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcflag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(12054, 0, 0, '<Use the Ambermill Dimensional Portal.>', 45818, 1, 1, 0, 0, 0, 0, NULL, 0, 0);

DELETE FROM `smart_scripts` WHERE `entryorguid` = 45752 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(45752, 0, 0, 1, 62, 0, 100, 0, 12054, 0, 0, 0, 0, 86, 85360, 2, 7, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ambermill Dimensional Portal - On Gossip Select - Cast Portal to the Ambermill'),
(45752, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Ambermill Dimensional Portal - On Gossip Select - Close Gossip');

DELETE FROM `spell_target_position` WHERE `id` = 85360 AND `effIndex` = 0;
INSERT INTO `spell_target_position` (`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(85360, 0, 0, -132.341, 781.067, 67.3826, 0);

DELETE FROM `spell_area` WHERE `spell` = 85361 AND `area` = 233;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(85361, 233, 0, 27518, 0, 0, 2, 1, 64, 9);
