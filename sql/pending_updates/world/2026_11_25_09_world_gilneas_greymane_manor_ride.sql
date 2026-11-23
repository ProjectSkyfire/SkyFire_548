-- Gilneas, To Greymane Manor (14465): Gwen Armstead lends the player a horse for the ride up to Greymane Manor.
-- Accepting the quest makes the player cast Summon Horse (72772), a vehicle summon that seats the caster on the Stout
-- Mountain Horse (38765, vehicle 542). The horse was a level 1 placeholder without a vehicle, so the spell did nothing.
-- It now runs the 15 waypoints of the original once the rider is aboard, opens the two Duskhaven gates on the way
-- (Gate 196863 at waypoint 3, gate 196864 at waypoint 13) and drops the rider at the manor (waypoint 15).
-- The gate guids are the ones in use (90232 and 90231, phase 183), the original has 166783 and 166784.
-- Based on Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `minlevel` = 10, `maxlevel` = 11, `speed_walk` = 1.6, `speed_run` = 1.85714, `VehicleId` = 542, `AIName` = 'SmartAI' WHERE `entry` = 38765;
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 36452;

DELETE FROM `waypoints` WHERE `entry` = 38765;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(38765, 1, -1870.36, 2282.73, 42.32, 'Stout Mountain Horse'),
(38765, 2, -1852.05, 2302.51, 41.83, 'Stout Mountain Horse'),
(38765, 3, -1829.29, 2323.74, 37.55, 'Stout Mountain Horse'),
(38765, 4, -1818.32, 2332.8, 36.37, 'Stout Mountain Horse'),
(38765, 5, -1801.58, 2344.71, 35.77, 'Stout Mountain Horse'),
(38765, 6, -1792.48, 2352.44, 36.52, 'Stout Mountain Horse'),
(38765, 7, -1787.74, 2359.6, 38, 'Stout Mountain Horse'),
(38765, 8, -1781.81, 2383.18, 43.27, 'Stout Mountain Horse'),
(38765, 9, -1777.55, 2423.56, 55.68, 'Stout Mountain Horse'),
(38765, 10, -1770.01, 2447.47, 62.25, 'Stout Mountain Horse'),
(38765, 11, -1754.47, 2464.13, 69.36, 'Stout Mountain Horse'),
(38765, 12, -1710.82, 2466.15, 82.81, 'Stout Mountain Horse'),
(38765, 13, -1691.46, 2493.66, 95.2, 'Stout Mountain Horse'),
(38765, 14, -1682.62, 2507.22, 97.79, 'Stout Mountain Horse'),
(38765, 15, -1669.61, 2519.56, 97.89, 'Stout Mountain Horse');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (36452, 38765);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38765, 0, 0, 0, 27, 0, 100, 0, 0, 0, 0, 0, 0, 53, 1, 38765, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Stout Mountain Horse - On Passenger Boarded - Start Waypoint'),
(38765, 0, 1, 0, 40, 0, 100, 0, 15, 0, 0, 0, 0, 28, 46598, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Stout Mountain Horse - On Waypoint 15 Reached - Remove Passenger'),
(38765, 0, 2, 0, 40, 0, 100, 0, 3, 0, 0, 0, 0, 54, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Stout Mountain Horse - On Waypoint 3 Reached - Pause Waypoint'),
(38765, 0, 3, 0, 40, 0, 100, 0, 3, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 14, 90232, 196863, 0, 0, 0, 0, 0, 'Stout Mountain Horse - On Waypoint 3 Reached - Open Gate'),
(38765, 0, 4, 0, 40, 0, 100, 0, 13, 0, 0, 0, 0, 54, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Stout Mountain Horse - On Waypoint 13 Reached - Pause Waypoint'),
(38765, 0, 5, 0, 40, 0, 100, 0, 13, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 14, 90231, 196864, 0, 0, 0, 0, 0, 'Stout Mountain Horse - On Waypoint 13 Reached - Open Gate'),
(36452, 0, 0, 0, 19, 0, 100, 0, 14465, 0, 0, 0, 0, 85, 72772, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Gwen Armstead - On Quest Accepted (To Greymane Manor) - Invoker Casts Summon Horse');
