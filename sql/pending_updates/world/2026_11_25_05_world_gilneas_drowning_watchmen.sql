-- Gilneas, Gasping for Breath (14395): the Drowning Watchman has its spell click row (the player casts Rescue
-- Drowning Watchman, 68735, vehicle 520 on the player) but no script, so Drowning Watchman Credit (36450) was never
-- given. The watchman now rides the player with Ride Vehicle Hardcoded (46598), five seconds later the player gets the
-- credit, the rescue and ride auras end and the watchman despawns.
-- The player is stored in target list 1 before the timed list starts, because the list does not get the invoker.
-- Based on Legends of Azeroth 5.4.8 (scripts).

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 36440;
DELETE FROM `smart_scripts` WHERE (`source_type` = 0 AND `entryorguid` = 36440) OR (`source_type` = 9 AND `entryorguid` = 3644000);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(36440, 0, 0, 1, 73, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Drowning Watchman - On Spell Click - Store Target List'),
(36440, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 3644000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Drowning Watchman - Linked - Run Script'),
(3644000, 9, 0, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 11, 46598, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Drowning Watchman - Script - Ride Vehicle Hardcoded'),
(3644000, 9, 1, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 33, 36450, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Drowning Watchman - Script - Give Quest Credit'),
(3644000, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 28, 46598, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Drowning Watchman - Script - Remove Ride Vehicle Hardcoded'),
(3644000, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 28, 68735, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Drowning Watchman - Script - Remove Rescue Drowning Watchman'),
(3644000, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Drowning Watchman - Script - Despawn');
