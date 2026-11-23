-- Heroic Foe Reaper 5000: the Molten Slags rise from the four Goblin Smelting Pots, run at the level of the heroic
-- boss, wear Molten Shield and fixate on a player. The slag summon spells pick the pot through a condition.
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 1 AND `SourceEntry` IN (91839, 91841, 91842, 91843);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 91839, 0, 0, 31, 0, 5, 123207, 0, 0, 0, 0, '', 'Summon Molten Slag - Target Goblin Smelting Pot'),
(13, 1, 91841, 0, 0, 31, 0, 5, 123208, 0, 0, 0, 0, '', 'Summon Molten Slag - Target Goblin Smelting Pot'),
(13, 1, 91842, 0, 0, 31, 0, 5, 123209, 0, 0, 0, 0, '', 'Summon Molten Slag - Target Goblin Smelting Pot'),
(13, 1, 91843, 0, 0, 31, 0, 5, 123210, 0, 0, 0, 0, '', 'Summon Molten Slag - Target Goblin Smelting Pot');

UPDATE `creature_template` SET `minlevel` = 87, `maxlevel` = 87, `mechanic_immune_mask` = 617299967, `spell1` = 91830, `spell2` = 91815, `AIName` = 'SmartAI' WHERE `entry` = 49229 AND `AIName` = '' AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `entryorguid` = 49229 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(49229, 0, 0, 0, 63, 0, 100, 0, 0, 0, 0, 0, 0, 11, 91815, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Molten Slag - On Just Created - Cast Molten Shield'),
(49229, 0, 1, 0, 0, 0, 100, 0, 3000, 3000, 12000, 12000, 0, 86, 91829, 2, 5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Molten Slag - In Combat - Random Enemy Casts Fixate On Self');
