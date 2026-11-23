-- The Downfall of Marl Wormthorn (25720, 26187): Marl Wormthorn stands entranced and immune (unit flags 768) until the
-- player throws the Demoniac Vessel (item 57185, spell 78985, "Break Marl's Trance") at him, which makes him attackable.
-- He had no row for the spell, so he stayed immune and the quest, which asks for his death, could not be finished.
-- The row is as in Legends of Azeroth 5.4.8; the existing combat rows (ids 0 to 3) are kept.

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 42334 AND `id` = 4;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(42334, 0, 4, 0, 8, 0, 100, 1, 78985, 0, 0, 0, 0, 19, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marl Wormthorn - On SpellHit Break Marl''s Trance - Remove Immunity Flags');
