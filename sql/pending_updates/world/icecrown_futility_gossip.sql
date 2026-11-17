-- Futility (13348, 13396) and the Army of the Damned replay: Matthias Lehner's gossip option "show me again" cast
-- nothing. Option 0 of menu 10226 was shown but the script had no row for it.
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 32404 AND `id` IN (2, 3);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(32404, 0, 2, 3, 62, 0, 100, 0, 10226, 0, 0, 0, 0, 11, 58989, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Matthias Lehner - On Gossip Option 0 Selected - Cast Prodigal Leader Force Cast'),
(32404, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Matthias Lehner - On Link - Close Gossip');
