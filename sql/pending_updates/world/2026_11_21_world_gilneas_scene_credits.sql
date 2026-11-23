-- Gilneas: the scene quests The Battle for Gilneas City and Endgame. Based on Legends of Azeroth 5.4.8.

-- The Battle for Gilneas City and Endgame are scene quests: the credit comes when the quest is accepted from
--    Lorna Crowley (37783, 43727), and the first one moves the player to the city. Without the rows both objectives
--    stay at 0 and the chain stops at 24904.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (37783, 43727);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (37783, 43727);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(37783, 0, 0, 1, 19, 0, 100, 0, 24904, 0, 0, 0, 0, 33, 38854, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Lorna Crowley - On quest accepted (The Battle for Gilneas City) - Give kill credit'),
(37783, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 62, 654, 0, 0, 0, 0, 0, 7, 0, 0, 0, -1681.73, 1619.83, 20.5728, 0, 'Lorna Crowley - Linked - Teleport invoker'),
(43727, 0, 0, 0, 19, 0, 100, 0, 26706, 0, 0, 0, 0, 33, 43729, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Lorna Crowley - On quest accepted (Endgame) - Give kill credit');
