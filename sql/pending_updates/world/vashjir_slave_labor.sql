-- Vashj'ir, "Slave Labor" (25907): save eight Enslaved Pearl Miners with Duarn's Rope. The rope (item 56178, spell 77664 "Throw
-- Rope") applies a dummy aura to a unit near the target (target type 38, which needs a condition naming the unit), and the
-- objective is the credit creature 41555. Nothing named the miners or credited the quest. The Alliance and Horde miners (41494
-- and 41495) are the targets; a miner that is roped gives the player the credit and swims off.

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 77664;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 77664, 0, 0, 31, 0, 3, 41494, 0, 0, 0, 0, '', 'Throw Rope - Enslaved Alliance Pearl Miner'),
(13, 1, 77664, 0, 1, 31, 0, 3, 41495, 0, 0, 0, 0, '', 'Throw Rope - Enslaved Horde Pearl Miner');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (41494, 41495);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (41494, 41495);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41494, 0, 0, 1, 8, 0, 100, 0, 77664, 0, 0, 0, 0, 33, 41555, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Spellhit (Throw Rope) - Quest Credit Pearl Miner Rescue Credit'),
(41494, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On Link - Despawn'),
(41495, 0, 0, 1, 8, 0, 100, 0, 77664, 0, 0, 0, 0, 33, 41555, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Spellhit (Throw Rope) - Quest Credit Pearl Miner Rescue Credit'),
(41495, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On Link - Despawn');
