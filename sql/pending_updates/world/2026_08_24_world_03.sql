-- Mound of Loose Dirt
UPDATE `gameobject_template` SET `data3`=30000 WHERE `entry`=204777;

UPDATE `gameobject_template` SET `AIName`='SmartGameObjectAI' WHERE `entry`=204777;

DELETE FROM `smart_scripts` WHERE `entryorguid`=204777;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(204777, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 0, 12, 43799, 6, 0, 1, 0, 0, 8, 0, 0, 0, -11127.5, -518.424, 35.2569, 0.436332, 'Mound of Loose Dirt - On Activate - Summon Lurking Worgen');

-- Lurking Worgen
DELETE FROM `creature` WHERE `guid`=192110;

DELETE FROM `creature_template_addon` WHERE `entry`=43799;

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=43799;

DELETE FROM `smart_scripts` WHERE `entryorguid`=43799;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(43799, 0, 0, 0, 0, 0, 100, 1, 0, 0, 0, 0, 0, 11, 81957, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Lurking Worgen - IC - Cast Stunning Pounce');
