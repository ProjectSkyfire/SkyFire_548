-- Forlorn Spirit
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=43923;

DELETE FROM `smart_scripts` WHERE `entryorguid`=43923;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(43923, 0, 0, 0, 0, 0, 100, 0, 4000, 6000, 9000, 11000, 0, 11, 81193, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Forlorn Spirit - In Combat - Cast Unrelenting Anguish'),
(43923, 0, 1, 0, 8, 0, 100, 1, 82198, 0, 0, 0, 0, 80, 4392300, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Forlorn Spirit - On Spellhit - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=4392300;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4392300, 9, 0, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 37, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Forlorn Spirit - On Script - Die');

-- Soothing Incense Cloud
DELETE FROM `creature_template_addon` WHERE `entry`=43925;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(43925, 0, 0, 0, 0, 0, '82199');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=43925;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=43925;

DELETE FROM `smart_scripts` WHERE `entryorguid`=43925;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(43925, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4392500, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Soothing Incense Cloud - On Summon - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=4392500;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4392500, 9, 0, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 33, 43930, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Soothing Incense Cloud - On Script - Give Credit'),
(4392500, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Soothing Incense Cloud - On Script - Despawn');
