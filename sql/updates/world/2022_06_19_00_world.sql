-- Drakkari Frostweaver SAI
UPDATE creature_template SET AIName='SmartAI' WHERE entry=66707;
DELETE FROM `smart_scripts` WHERE `entryorguid`=66707;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(66707, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 11, 131559, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Drakkari Frostweaver - On Aggro - Cast Ice Column'),
(66707, 0, 1, 0, 9, 0, 100, 0, 0, 40, 3300, 6600, 11, 9672, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Drakkari Frostweaver - At 0 - 40 Range - Cast Frostbolt'),
(66707, 0, 2, 0, 9, 0, 100, 0, 0, 8, 11000, 16000, 11, 11831, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Drakkari Frostweaver - At 0 - 8 Range - Cast Frost Nova');
