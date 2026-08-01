-- Pilfered Supplies
UPDATE `quest_template` SET `PrevQuestId`=25724 WHERE `Id`=25668;

-- Forced to Watch from Afar
UPDATE `quest_template` SET `PrevQuestId`=25724 WHERE `Id`=313;

-- Culling the Wendigos 
UPDATE `quest_template` SET `PrevQuestId`=25724 WHERE `Id`=25667;

-- Young Wendigo
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=40940;

DELETE FROM `smart_scripts` WHERE `entryorguid`=40940;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(40940, 0, 0, 0, 0, 0, 100, 0, 8000, 10000, 10000, 12000, 0, 11, 3131, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Young Wendigo - In Combat - Cast Frost Breath');

-- Wendigo
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=40941;

DELETE FROM `smart_scripts` WHERE `entryorguid`=40941;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(40941, 0, 0, 0, 0, 0, 100, 0, 8000, 10000, 10000, 12000, 0, 11, 3131, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Wendigo - In Combat - Cast Frost Breath');
