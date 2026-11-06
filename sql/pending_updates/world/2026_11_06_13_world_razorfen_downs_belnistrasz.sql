-- Razorfen Downs: Belnistrasz walks to the idol, holds the ritual for five minutes against the waves and puts out the
-- idol fires ("Extinguishing the Idol"). The script, his path and his lines come from the 4.3.4 data. Safe to run twice.
UPDATE `creature_template` SET `ScriptName` = 'npc_belnistrasz' WHERE `entry` = 8516;
UPDATE `creature_template` SET `ScriptName` = 'npc_idol_room_spawner' WHERE `entry` = 8611;

DELETE FROM `waypoint_data` WHERE `id` = 871710;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) VALUES
(871710, 1, 2603.31, 724.335, 54.608, 0, 0, 1, 0, 100, 0),
(871710, 2, 2593.38, 726.272, 55.112, 0, 0, 1, 0, 100, 0),
(871710, 3, 2588.5, 733.195, 55.3959, 0, 0, 1, 0, 100, 0),
(871710, 4, 2572.57, 752.516, 54.7182, 0, 0, 1, 0, 100, 0),
(871710, 5, 2558.07, 748.301, 54.3559, 0, 0, 1, 0, 100, 0),
(871710, 6, 2539.68, 777.136, 46.9515, 0, 0, 1, 0, 100, 0),
(871710, 7, 2527.83, 800.84, 44.7471, 0, 0, 1, 0, 100, 0),
(871710, 8, 2496, 785.754, 39.512, 0, 0, 1, 0, 100, 0),
(871710, 9, 2484.36, 814.891, 43.5779, 0, 0, 1, 0, 100, 0),
(871710, 10, 2501.13, 847.961, 47.5574, 0, 0, 1, 0, 100, 0),
(871710, 11, 2537.36, 874.471, 47.678, 0, 0, 1, 0, 100, 0),
(871710, 12, 2548.49, 894.651, 47.6931, 0, 0, 1, 0, 100, 0),
(871710, 13, 2541.48, 910.51, 46.1722, 0, 0, 1, 0, 100, 0),
(871710, 14, 2519.4, 925.633, 46.515, 0, 0, 1, 0, 100, 0),
(871710, 15, 2527.24, 951.461, 49.2807, 0, 0, 1, 0, 100, 0),
(871710, 16, 2541.68, 976.589, 50.4122, 0, 0, 1, 0, 100, 0),
(871710, 17, 2554.08, 973.867, 50.3616, 0, 0, 1, 0, 100, 0),
(871710, 18, 2575.6, 950.138, 52.8459, 0, 0, 1, 0, 100, 0);

DELETE FROM `creature_text` WHERE `entry` = 8516;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(8516, 0, 0, 'All right, stay close.  These fiends will jump right out of the shadows at you if you let your guard down.', 12, 0, 100, 0, 0, 0, 'Belnistrasz'),
(8516, 1, 0, 'Okay, here we go.  It''s going to take about five minutes to shut this thing down through the ritual.  Once I start, keep the vermin off of me or it will be the end of us all!', 12, 0, 100, 0, 0, 0, 'Belnistrasz'),
(8516, 2, 0, 'Three minutes left -- I can feel the energy starting to build!  Keep up the solid defense!', 14, 0, 100, 0, 0, 0, 'Belnistrasz'),
(8516, 3, 0, 'Just two minutes to go!  We''re half way there, but don''t let your guard down!', 14, 0, 100, 0, 0, 0, 'Belnistrasz'),
(8516, 4, 0, 'One more minute!  Hold on now, the ritual is about to take hold!', 14, 0, 100, 0, 0, 0, 'Belnistrasz'),
(8516, 5, 0, 'That''s it -- we made it!  The ritual is set in motion, and idol fires are about to go out for good!  You truly are the heroes I thought you would be!', 14, 0, 100, 4, 0, 0, 'Belnistrasz'),
(8516, 6, 0, 'You''ll rue the day you crossed me, $n.', 12, 0, 100, 0, 0, 0, 'Belnistrasz'),
(8516, 7, 0, 'Watch out for the $n!', 12, 0, 100, 0, 0, 0, 'Belnistrasz');
