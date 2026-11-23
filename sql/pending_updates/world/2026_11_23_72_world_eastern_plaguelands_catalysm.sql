-- Catalysm (27453) asks for 8 Active Liquid Plague Agents, taken from Blighted Surges and Plague Ravagers with
-- Betina's Flasks (spell 85144). The flask could be used on any target, and the creatures took the sample without
-- any reaction. The spell is now limited to the two creatures, a creature that was sampled turns on the player,
-- shrinks, thanks the player with Flow of Gratitude and is gone after 30 seconds, and it cannot be sampled again
-- while it is shrunk.
-- Rows are as in Legends of Azeroth 5.4.8 (its 'not in phase 1' condition, which this core does not have, is the
-- shrunk aura here; its phase switch is left out). The existing combat rows of both creatures are kept.

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND ((`entryorguid` = 8519 AND `id` BETWEEN 8 AND 11) OR (`entryorguid` = 8520 AND `id` BETWEEN 1 AND 4));
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(8519, 0, 8, 9, 8, 0, 100, 0, 85144, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Blighted Surge - On Spellhit Gather Liquid Sample - Attack Start'),
(8519, 0, 9, 10, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 85178, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blighted Surge - Linked To Id 8 - Cast Shrink'),
(8519, 0, 10, 11, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 85873, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Blighted Surge - Linked To Id 9 - Cast Flow of Gratitude'),
(8519, 0, 11, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 30000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blighted Surge - Linked To Id 10 - Despawn After 30 Seconds'),
(8520, 0, 1, 2, 8, 0, 100, 0, 85144, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Plague Ravager - On Spellhit Gather Liquid Sample - Attack Start'),
(8520, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 85178, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Plague Ravager - Linked To Id 1 - Cast Shrink'),
(8520, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 85873, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Plague Ravager - Linked To Id 2 - Cast Flow of Gratitude'),
(8520, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 30000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Plague Ravager - Linked To Id 3 - Despawn After 30 Seconds');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` = 85144;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 85144, 0, 0, 31, 1, 3, 8519, 0, 0, 0, 0, '', 'Gather Liquid Sample target limit to Blighted Surge'),
(17, 0, 85144, 0, 0, 1, 1, 85178, 0, 0, 1, 0, 0, '', 'Gather Liquid Sample can be cast only if the target is not shrunk'),
(17, 0, 85144, 0, 1, 31, 1, 3, 8520, 0, 0, 0, 0, '', 'Gather Liquid Sample target limit to Plague Ravager'),
(17, 0, 85144, 0, 1, 1, 1, 85178, 0, 0, 1, 0, 0, '', 'Gather Liquid Sample can be cast only if the target is not shrunk');
