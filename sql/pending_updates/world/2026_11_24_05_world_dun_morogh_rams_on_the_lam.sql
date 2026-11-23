-- Rams on the Lam (25905): the Stolen Rams (41539) of the Tundrid Hills return to Amberstill Ranch when a player standing next to
-- one whistles at it. The rams had no script, so the whistle did nothing (upstream issue 1414). The script is as in Legends of
-- Azeroth 5.4.8: on the whistle emote (104) the player gets one Stolen Ram Recovery credit (43064) and the ram goes away
-- after two seconds.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 41539 AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (41539);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41539, 0, 0, 1, 22, 0, 100, 0, 104, 2000, 2000, 0, 0, 33, 43064, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Stolen Ram - On Receive Emote Whistle - Give Credit'),
(41539, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Stolen Ram - Linked To Id 0 - Despawn');
