UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=56309;
DELETE FROM `smart_scripts` WHERE `entryorguid`=56309;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(56309, 0, 0, 1, 1, 0, 100, 0, 0, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grookin Wildtail - Out Of Combat - Allow Combat Movement'),
(56309, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grookin Wildtail - Out Of Combat - Set Phase 0'),
(56309, 0, 2, 3, 4, 0, 100, 0, 0, 0, 0, 0, 11, 105762, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Grookin Wildtail - On Aggro - Cast Sling Derk'),
(56309, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 23, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grookin Wildtail - On Aggro - Increment Phase'),
(56309, 0, 4, 0, 9, 0, 100, 0, 0, 40, 3000, 3500, 11, 105762, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Grookin Wildtail - At 0 - 40 Range - Cast Sling Derk'),
(56309, 0, 5, 6, 3, 0, 100, 0, 0, 15, 0, 0, 21, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grookin Wildtail - At 15% Mana - Allow Combat Movement'),
(56309, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 23, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grookin Wildtail - At 15% Mana - Increment Phase'),
(56309, 0, 7, 0, 9, 0, 100, 1, 35, 80, 0, 0, 21, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grookin Wildtail - At 35 - 80 Range - Allow Combat Movement'),
(56309, 0, 8, 0, 9, 0, 100, 1, 5, 15, 0, 0, 21, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grookin Wildtail - At 5 - 15 Range - Allow Combat Movement'),
(56309, 0, 9, 0, 9, 0, 100, 1, 0, 5, 0, 0, 21, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grookin Wildtail - At 0 - 5 Range - Allow Combat Movement'),
(56309, 0, 10, 0, 3, 0, 100, 0, 25, 100, 100, 100, 23, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grookin Wildtail - At 100% Mana - Increment Phase');
