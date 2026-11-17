-- Basic Chemistry (13279, 13295) and Neutralizing the Plague (13281, 13297): the plague cauldron never gave credit. One
-- of the three scripts the Summoned Plague Cauldron Bunny picks from (3244501, the one with the Kill Credit cast) was
-- missing; an upstream cleanup of 2017 deleted it with other Cataclysm leftovers.
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` = 3244501;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(3244501, 9, 0, 0, 0, 0, 100, 0, 7000, 7000, 7000, 7000, 0, 11, 59876, 0, 0, 0, 0, 0, 21, 50, 0, 0, 0, 0, 0, 0, 'Summoned Plague Cauldron Bunny 02 - On Script - Cast Kill Credit'),
(3244501, 9, 1, 0, 0, 0, 100, 0, 1500, 1500, 1500, 1500, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Summoned Plague Cauldron Bunny 02 - On Script - Say Line 2'),
(3244501, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 19, 32442, 0, 0, 0, 0, 0, 0, 'Summoned Plague Cauldron Bunny 02 - On Script - Set Data 1 1'),
(3244501, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 2, 0, 0, 0, 0, 19, 31773, 0, 0, 0, 0, 0, 0, 'Summoned Plague Cauldron Bunny 02 - On Script - Set Data 1 2'),
(3244501, 9, 4, 0, 0, 0, 100, 0, 1000, 1000, 1000, 1000, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Summoned Plague Cauldron Bunny 02 - On Script - Despawn In 1000 ms');
