-- Gilneas, chapter two scripts that only exist as data in the original:
--  * Knee-Deep (24678): the Half-Burnt Torch (item spell 70631) clears the Graveyard Rats around the caster. The
--    spell takes its targets by entry, which fails without conditions, so the torch could not be used. Add the
--    condition and the rat script: a rat hit by the torch dies.
--  * They Have Allies, But So Do We (24681): the Orcish War Machines (37921) are targets only and stay passive.
-- Based on Legends of Azeroth 5.4.8. The war machine row uses the reset event instead of the respawn event of the
-- original, so the machines are passive from their first spawn on and not only after the first death.

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 70631;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 70631, 0, 0, 31, 0, 3, 37889, 0, 0, 0, 0, '', 'Half-Burnt Torch - Target Graveyard Rat');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (37889, 37921);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (37889, 37921);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(37889, 0, 0, 0, 8, 0, 100, 0, 70631, 0, 0, 0, 0, 37, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Graveyard Rat - On Spell Hit (Half-Burnt Torch) - Die'),
(37921, 0, 0, 0, 25, 0, 100, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orcish War Machine - On Reset - Set Passive');
