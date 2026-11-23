-- Blackrock Depths: Plugger Spazzring warns each time a Dark Iron Ale Mug or the Grim Guzzler Boar is taken; the
-- instance script counts the thefts (SkyFire's SmartAI has no counters) and at the third he and the nearby
-- patrons attack.
UPDATE `smart_scripts` SET `link` = 7 WHERE `source_type` = 0 AND `entryorguid` = 9499 AND `id` = 6;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 9499 AND `id` IN (7, 8);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` = 949900;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(9499, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 34, 29, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Plugger Spazzring - Linked - Count the theft in the instance'),
(9499, 0, 8, 0, 38, 0, 100, 0, 2, 1, 0, 0, 0, 80, 949900, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Plugger Spazzring - On data set (third theft) - Start action list'),
(949900, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 9545, 7, 0, 0, 0, 0, 0, 'Plugger Spazzring - Actionlist - Set data'),
(949900, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 9547, 7, 0, 0, 0, 0, 0, 'Plugger Spazzring - Actionlist - Set data'),
(949900, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 35, 0, 0, 0, 0, 0, 0, 'Plugger Spazzring - Actionlist - Start attack');

UPDATE `gameobject_template` SET `AIName` = 'SmartGameObjectAI' WHERE `entry` IN (165738, 165739) AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 1 AND `entryorguid` IN (165738, 165739);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(165738, 1, 0, 0, 70, 0, 100, 0, 1, 0, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 19, 9499, 35, 0, 0, 0, 0, 0, 'Dark Iron Ale Mug - On taken - Set data on Plugger Spazzring'),
(165739, 1, 0, 0, 70, 0, 100, 0, 1, 0, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 19, 9499, 35, 0, 0, 0, 0, 0, 'Grim Guzzler Boar - On taken - Set data on Plugger Spazzring');
