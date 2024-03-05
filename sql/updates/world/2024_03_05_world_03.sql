DELETE FROM `spell_scripts` WHERE `id`=71898;
INSERT INTO `spell_scripts` (`id`, `effIndex`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES
(71898, 1, 0, 8, 38438, 0, 0, 0, 0, 0, 0);

UPDATE `gameobject_template` SET `type`='8', `data0`='1647', `data1`='15' WHERE `entry`=202138;

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2981;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (2981, 298100);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2981, 0, 9, 0, 19, 0, 100, 0, 24861, 0, 0, 0, 0, 80, 298100, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Chief Hawkwind - On Quest Accept - Run Script'),
(298100, 9, 0, 0, 0, 0, 100, 0, 2000, 3000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Chief Hawkwind - On Script - Say Line 0'),
(298100, 9, 1, 0, 0, 0, 100, 0, 7000, 8000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Chief Hawkwind - On Script - Say Line 1'),
(298100, 9, 2, 0, 0, 0, 100, 0, 7000, 8000, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Chief Hawkwind - On Script - Say Line 2'),
(298100, 9, 3, 0, 0, 0, 100, 0, 7000, 8000, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Chief Hawkwind - On Script - Say Line 3');
