-- The Deadmines: the instance script already casts Explode (89769) when a Mining Powder keg dies, so the keg's own
-- SmartAI keeps only the second explosion (90096) and the broken keg model; casting 89769 twice doubled the damage.
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 48284;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(48284, 0, 0, 1, 6, 0, 100, 0, 0, 0, 0, 0, 0, 11, 90096, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mining Powder - On death - Cast Explode'),
(48284, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 3, 0, 11686, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mining Powder - Linked - Morph to the broken keg');
