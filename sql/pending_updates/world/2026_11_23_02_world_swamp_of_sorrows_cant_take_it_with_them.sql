-- Can't Take It With Them (27599) asks for 6 Steamwheedle Ditty Bags (61923), which the player takes from the Deceased
-- Bogpaddle Privateers (46014) by talking to them: the privateer gives the bag, closes the window and cannot be talked to
-- for two minutes. The privateer had no gossip flag and no SmartAI, so the bags could not be taken. The scripts, the flag,
-- unit flags and the death aura are as in Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `npcflag` = 1, `unit_flags` = 33536, `AIName` = 'SmartAI' WHERE `entry` = 46014 AND `ScriptName` = '';

UPDATE `creature_template_addon` SET `auras` = '29266' WHERE `entry` = 46014;

DELETE FROM `smart_scripts` WHERE `entryorguid` = 46014 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(46014, 0, 0, 1, 64, 0, 100, 0, 0, 0, 0, 0, 0, 56, 61923, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Deceased Bogpaddle Privateer - On Gossip Hello - Add Item Steamwheedle Ditty Bag'),
(46014, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deceased Bogpaddle Privateer - Linked To Id 0 - Close Gossip'),
(46014, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 83, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deceased Bogpaddle Privateer - Linked To Id 1 - Remove NpcFlag 1'),
(46014, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 67, 46014, 120000, 120000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deceased Bogpaddle Privateer - Linked To Id 2 - Create Timed Event'),
(46014, 0, 4, 0, 59, 0, 100, 0, 46014, 0, 0, 0, 0, 81, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deceased Bogpaddle Privateer - On Timed Event Triggered - Set NpcFlag 1');
