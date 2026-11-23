-- Skadi's gauntlet: the Ymirjar of the later waves walk down the hall to its far end before they engage. The
-- boss sends them to point 1, these rows take them to point 2 and into combat.

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (26690, 26691, 26692) AND `id` IN (10, 11);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(26690, 0, 10, 0, 34, 0, 100, 0, 8, 1, 0, 0, 0, 69, 2, 0, 0, 0, 0, 0, 8, 0, 0, 0, 318.177, -503.8898, 104.5326, 0, 'Ymirjar Warrior - On Reached Point 1 - Move to Point 2'),
(26690, 0, 11, 0, 34, 0, 100, 0, 8, 2, 0, 0, 0, 38, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ymirjar Warrior - On Reached Point 2 - Set In Combat With Zone'),
(26691, 0, 10, 0, 34, 0, 100, 0, 8, 1, 0, 0, 0, 69, 2, 0, 0, 0, 0, 0, 8, 0, 0, 0, 318.177, -503.8898, 104.5326, 0, 'Ymirjar Witch Doctor - On Reached Point 1 - Move to Point 2'),
(26691, 0, 11, 0, 34, 0, 100, 0, 8, 2, 0, 0, 0, 38, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ymirjar Witch Doctor - On Reached Point 2 - Set In Combat With Zone'),
(26692, 0, 10, 0, 34, 0, 100, 0, 8, 1, 0, 0, 0, 69, 2, 0, 0, 0, 0, 0, 8, 0, 0, 0, 318.177, -503.8898, 104.5326, 0, 'Ymirjar Harpooner - On Reached Point 1 - Move to Point 2'),
(26692, 0, 11, 0, 34, 0, 100, 0, 8, 2, 0, 0, 0, 38, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ymirjar Harpooner - On Reached Point 2 - Set In Combat With Zone');
