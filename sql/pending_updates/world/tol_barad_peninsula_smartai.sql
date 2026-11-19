-- Tol Barad Peninsula: eleven creatures of the dailies have no AI in the world: the Darkwood Lurker, Shipwrecked Sailor, Tank,
-- Foreman Wellson, the three ghosts of Largo's Overlook and the four villagers of Rustberg. The Cataclysm Preservation
-- Project 4.3.4 data has their combat spells (18 rows) and the three lines of Foreman Wellson, and the 5.4.8 data has none.
-- The two equipment changes of the Rustberg Fisherman are left out: its equipment rows here are not the ones of 4.3.4.
-- Ported from the Cataclysm Preservation Project 4.3.4.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (46508, 46605, 46608, 46648, 47181, 47182, 47183, 47627, 47642, 47657, 47659);

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (46508, 46605, 46608, 46648, 47181, 47182, 47183, 47627, 47642, 47657, 47659);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(46508, 0, 0, 0, 0, 0, 100, 1, 4000, 5000, 0, 0, 0, 11, 12023, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'When in combat and timer at the begining between 4000 and 5000 ms (and later repeats every 0 and 0 ms) - Self: Cast spell 12023 on Victim // '),
(46605, 0, 0, 0, 0, 0, 100, 0, 7000, 8000, 9000, 11000, 0, 11, 75388, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'When in combat and timer at the begining between 7000 and 8000 ms (and later repeats every 9000 and 11000 ms) - Self: Cast spell Rusty Cut (75388) on Victim // '),
(46608, 0, 0, 0, 0, 0, 100, 0, 6000, 6000, 30000, 30000, 0, 11, 81173, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'When in combat and timer at the begining between 6000 and 6000 ms (and later repeats every 30000 and 30000 ms) - Self: Cast spell Frenzy (81173) on Self // '),
(46608, 0, 1, 0, 0, 0, 100, 0, 12000, 12000, 16000, 16000, 0, 11, 69203, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'When in combat and timer at the begining between 12000 and 12000 ms (and later repeats every 16000 and 16000 ms) - Self: Cast spell Vicious Bite (69203) on Victim // '),
(46648, 0, 0, 0, 1, 0, 100, 0, 30000, 60000, 30000, 60000, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'When out of combat and timer at the begining between 30000 and 60000 ms (and later repeats every 30000 and 60000 ms) - Self: Talk 0 // '),
(46648, 0, 1, 0, 0, 0, 100, 0, 4500, 5000, 8000, 11000, 0, 11, 80146, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'When in combat and timer at the begining between 4500 and 5000 ms (and later repeats every 8000 and 11000 ms) - Self: Cast spell Bonk (80146) on Victim // '),
(47181, 0, 0, 0, 0, 0, 100, 0, 0, 0, 3400, 3400, 0, 11, 77721, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'When in combat and timer at the begining between 0 and 0 ms (and later repeats every 3400 and 3400 ms) - Self: Cast spell 77721 on Victim // '),
(47181, 0, 1, 0, 0, 0, 100, 0, 3000, 3000, 7000, 7000, 0, 11, 31976, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'When in combat and timer at the begining between 3000 and 3000 ms (and later repeats every 7000 and 7000 ms) - Self: Cast spell 31976 on Self // '),
(47182, 0, 0, 0, 0, 0, 100, 0, 4500, 5000, 16000, 21000, 0, 11, 79113, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'When in combat and timer at the begining between 4500 and 5000 ms (and later repeats every 16000 and 21000 ms) - Self: Cast spell Ancient Darkness (79113) on Victim // '),
(47182, 0, 1, 0, 0, 0, 100, 0, 10500, 11000, 19000, 21000, 0, 11, 88883, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'When in combat and timer at the begining between 10500 and 11000 ms (and later repeats every 19000 and 21000 ms) - Self: Cast spell Pact of Darkness (88883) on Self // '),
(47183, 0, 0, 0, 0, 0, 100, 0, 5000, 6000, 8000, 9000, 0, 11, 86085, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'When in combat and timer at the begining between 5000 and 6000 ms (and later repeats every 8000 and 9000 ms) - Self: Cast spell 86085 on Victim // '),
(47627, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 11, 66060, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On aggro - Self: Cast spell Sprint (66060) on Self // '),
(47642, 0, 0, 0, 0, 0, 100, 0, 4500, 5000, 9000, 11000, 0, 11, 32011, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'When in combat and timer at the begining between 4500 and 5000 ms (and later repeats every 9000 and 11000 ms) - Self: Cast spell Water Bolt (32011) on Victim // '),
(47642, 0, 1, 0, 0, 0, 100, 0, 8500, 9000, 33000, 34000, 0, 11, 78799, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'When in combat and timer at the begining between 8500 and 9000 ms (and later repeats every 33000 and 34000 ms) - Self: Cast spell Wave Crash (78799) on Victim // '),
(47642, 0, 2, 0, 1, 0, 100, 0, 14000, 19000, 14000, 19000, 0, 5, 380, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'When out of combat and timer at the begining between 14000 and 19000 ms (and later repeats every 14000 and 19000 ms) - Self: Play emote ONESHOT_FISHING (380) // '),
(47657, 0, 0, 0, 0, 0, 100, 0, 5000, 5000, 20000, 26000, 0, 11, 79878, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'When in combat and timer at the begining between 5000 and 5000 ms (and later repeats every 20000 and 26000 ms) - Self: Cast spell Bloodthirst (79878) on Victim // '),
(47657, 0, 1, 0, 0, 0, 100, 0, 8000, 9000, 9000, 11000, 0, 11, 15496, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'When in combat and timer at the begining between 8000 and 9000 ms (and later repeats every 9000 and 11000 ms) - Self: Cast spell Cleave (15496) on Victim // '),
(47659, 0, 0, 0, 0, 0, 100, 1, 100, 100, 0, 0, 0, 11, 78894, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'When in combat and timer at the begining between 100 and 100 ms (and later repeats every 0 and 0 ms) - Self: Cast spell 78894 on Victim //');

DELETE FROM `creature_text` WHERE `entry` = 46648;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(46648, 0, 0, 'Let''s get back to work, people!', 14, 0, 100, 0, 0, 0, 'Foreman Wellson'),
(46648, 0, 1, 'These ships are not going to build themselves!', 14, 0, 100, 0, 0, 0, 'Foreman Wellson'),
(46648, 0, 2, 'I expect the highest quality craftmanship from my workforce.  Double your efforts!', 14, 0, 100, 0, 0, 0, 'Foreman Wellson');
