-- Coldridge Valley (Dwarf and Gnome starting zone): quest scripting fix from the zone audit (finding the quest could not be completed). No new spawns, so no new guids.

-- ---------------------------------------------------------------------------------------------------------------
-- "Trolling for Information" (24489) asks the player to stand near each of the three Frostmane Soothsayers
-- (Shi'kala 37108, Rikkari 37173, Mirim'koa 37174). The data had the 4.3.4 way: each soothsayer reacted to the spell hit 69995
-- (a dummy spell, area target type with no target conditions anywhere) and then cast a credit spell. The aura that casts 69995
-- (69991, spell_area for this quest) has autocast 0 here and in 5.4.8, so nothing ever cast it and the quest could not be
-- completed. The 5.4.8 data gives the credit directly: every second, each soothsayer credits the player within 10 yards
-- with the matching credit bunny (37109, 37110, 37111, the objectives of the quest). The old spell-hit rows and their three
-- action lists (speech and credit spell, never reached) are replaced by these rows.
-- ---------------------------------------------------------------------------------------------------------------
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (37108, 37173, 37174) AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (3710800, 3717300, 3717400) AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(37108, 0, 0, 0, 1, 0, 100, 0, 0, 0, 1000, 1000, 0, 33, 37109, 0, 0, 0, 0, 0, 17, 0, 10, 0, 0, 0, 0, 0, 'Soothsayer Shi''kala Scouted (players within 10 yards)'),
(37173, 0, 0, 0, 1, 0, 100, 0, 0, 0, 1000, 1000, 0, 33, 37110, 0, 0, 0, 0, 0, 17, 0, 10, 0, 0, 0, 0, 0, 'Soothsayer Rikkari Scouted (players within 10 yards)'),
(37174, 0, 0, 0, 1, 0, 100, 0, 0, 0, 1000, 1000, 0, 33, 37111, 0, 0, 0, 0, 0, 17, 0, 10, 0, 0, 0, 0, 0, 'Soothsayer Mirim''koa Scouted (players within 10 yards)');
