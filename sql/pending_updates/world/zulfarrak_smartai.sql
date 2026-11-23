-- Zul'Farrak: SmartAI for the creatures that have it in 4.3.4 but had none in SkyFire
-- (Ukorz Sandscalp, Theka, Gahz'rilla, Executioner, Sezz'ziz, Slave, Drudge, Velratha,
-- Nekrum, Ruuzlu, Antu'sul, Zealot). Rows are copied from 4.3.4 smart_scripts, whose columns
-- are identical to SkyFire's. Changes against the 4.3.4 rows:
--  * event_flags keep the 4.3.4 values (0x2 normal, 0x4 heroic); they match the
--    LoA-style SmartAI difficulty check (1 << spawn mode).
--  * SMART_ACTION_CAST flags: 4.3.4 bit 0x40 (combat move) does not exist in SkyFire, set to 0.
--  * SMART_ACTION_WP_START reactState 0 -> 2 (aggressive) so Gahz'rilla fights after his walk.

-- Enable SmartAI (only where no C++ script is bound).
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (7267, 7272, 7273, 7274, 7275, 7787, 7788, 7795, 7796, 7797, 8127, 8877) AND `ScriptName` = '';

-- Gahz'rilla path used by SMART_ACTION_WP_START (path id = creature entry), from 4.3.4 `waypoints`.
DELETE FROM `waypoints` WHERE `entry` = 7273;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(7273, 1, 1686.55, 1207.18, 9.79578, 'Gahz''rilla'),
(7273, 2, 1700.54, 1220.82, 8.87687, 'Gahz''rilla');

-- Texts used by the scripts above (SkyFire had none), from 4.3.4 creature_text.
DELETE FROM `creature_text` WHERE `entry` IN (7267, 7274, 8127, 8877);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(7267, 0, 0, 'Die, outlander!', 14, 0, 100, 0, 0, 5877, 'combat Text'),
(7267, 1, 0, 'Feel the fury of the sands!', 14, 0, 100, 0, 0, 5879, 'combat Text'),
(7267, 2, 0, 'The Sandfury reign supreme!', 14, 0, 100, 0, 0, 5878, 'combat Text'),
(7267, 3, 0, 'This desert be mine!', 14, 0, 100, 0, 0, 5876, 'combat Text'),
(7274, 0, 0, 'Let the executions begin!', 14, 0, 100, 0, 0, 5874, 'combat Say'),
(7274, 1, 0, 'Justice is done!', 14, 0, 100, 0, 0, 5875, 'combat Say'),
(8127, 0, 0, 'The children of Sul will protect their master. Rise once more Sul''lithuz!', 14, 0, 100, 0, 0, 0, 'Antu''sul'),
(8127, 1, 0, 'Lunch has arrived, my beautiful children. Tear them to pieces!', 14, 0, 100, 0, 0, 0, 'Antu''sul'),
(8127, 2, 0, 'Rise and defend your master!', 14, 0, 100, 0, 0, 0, 'Antu''sul'),
(8877, 0, 0, '%s goes into a frenzy!', 16, 0, 100, 0, 0, 0, 'Sandfury Zealot');

-- SmartAI rows.
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (7267, 7272, 7273, 7274, 7275, 7787, 7788, 7795, 7796, 7797, 8127, 8877);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(7267, 0, 0, 0, 0, 0, 100, 0, 12000, 16000, 7500, 24500, 0, 11, 11837, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Chief Ukorz Sandscalp - In Combat - Cast Wide Slash'),
(7267, 0, 1, 0, 0, 0, 100, 0, 6000, 20000, 8500, 19500, 0, 11, 15496, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Chief Ukorz Sandscalp - In Combat - Cast Cleave'),
(7267, 0, 2, 0, 5, 0, 100, 0, 5000, 5000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Chief Ukorz Sandscalp - On Killed Unit - Say Line 0'),
(7267, 0, 3, 0, 2, 0, 100, 1, 0, 30, 0, 0, 0, 11, 8269, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Chief Ukorz Sandscalp - Between 0-30% Health - Cast Frenzy (No Repeat)'),
(7267, 0, 4, 0, 2, 0, 100, 1, 0, 30, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Chief Ukorz Sandscalp - Between 0-30% Health - Say Line 1 (No Repeat)'),
(7267, 0, 5, 0, 2, 0, 100, 1, 0, 60, 0, 0, 0, 11, 8269, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Chief Ukorz Sandscalp - Between 0-60% Health - Cast Frenzy (No Repeat)'),
(7267, 0, 6, 0, 2, 0, 100, 1, 0, 60, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Chief Ukorz Sandscalp - Between 0-60% Health - Say Line 2 (No Repeat)'),
(7267, 0, 7, 0, 4, 0, 100, 1, 0, 0, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Chief Ukorz Sandscalp - On Aggro - Say Line 3 (No Repeat)'),
(7272, 0, 0, 0, 2, 0, 100, 1, 0, 30, 0, 0, 0, 11, 11089, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Theka the Martyr - Between 0-30% Health - Cast Theka Transform (No Repeat)'),
(7272, 0, 1, 0, 0, 0, 100, 0, 15700, 18800, 10900, 22900, 0, 11, 8600, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Theka the Martyr - In Combat - Cast Fevered Plague'),
(7273, 0, 0, 0, 0, 0, 100, 0, 10000, 21000, 22000, 23000, 0, 11, 11836, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Gahz''rilla - In Combat - Cast Freeze Solid'),
(7273, 0, 1, 0, 0, 0, 100, 0, 27000, 31000, 16000, 28000, 0, 11, 11902, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gahz''rilla - In Combat - Cast Gahz''rilla Slam'),
(7273, 0, 2, 0, 0, 0, 100, 0, 8500, 36000, 8500, 27000, 0, 11, 11131, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Gahz''rilla - In Combat - Cast Icicle'),
(7273, 0, 3, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 18, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gahz''rilla - Just Summoned - Add Unit Flag Immune to PC'),
(7273, 0, 4, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 53, 0, 7273, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 'Gahz''rilla - Just Summoned - Start Waypoint'),
(7273, 0, 5, 0, 40, 0, 100, 0, 2, 7273, 0, 0, 0, 89, 10, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gahz''rilla - On Waypoint 2 Reached - Start Random Movement'),
(7273, 0, 6, 0, 40, 0, 100, 0, 2, 7273, 0, 0, 0, 19, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gahz''rilla - On Waypoint 2 Reached - Remove Unit Flag Immune to PC'),
(7274, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sandfury Executioner - On Aggro - Say Line 0 (No Repeat)'),
(7274, 0, 1, 0, 0, 0, 100, 0, 8000, 13000, 8000, 23000, 0, 11, 15496, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Sandfury Executioner - In Combat - Cast Cleave'),
(7274, 0, 2, 0, 5, 0, 100, 0, 5000, 5000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sandfury Executioner - On Killed Unit - Say Line 1'),
(7275, 0, 0, 0, 0, 0, 100, 0, 0, 0, 3400, 4700, 0, 11, 12739, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Shadowpriest Sezz''ziz - In Combat - Cast Shadow Bolt'),
(7275, 0, 1, 0, 0, 0, 100, 0, 11000, 16000, 22000, 32000, 0, 11, 13704, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadowpriest Sezz''ziz - In Combat - Cast Psychic Scream'),
(7275, 0, 2, 0, 14, 0, 100, 0, 2000, 40, 27800, 34200, 0, 11, 8362, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Shadowpriest Sezz''ziz - Friendly At 2000 Health - Cast Renew'),
(7275, 0, 3, 0, 14, 0, 100, 0, 3000, 40, 14200, 25300, 0, 11, 12039, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Shadowpriest Sezz''ziz - Friendly At 3000 Health - Cast Heal'),
(7275, 0, 4, 0, 2, 0, 100, 1, 0, 30, 0, 0, 0, 11, 8362, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadowpriest Sezz''ziz - Between 0-30% Health - Cast Renew (No Repeat)'),
(7787, 0, 0, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 103, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sandfury Slave - On Just Died - Set Rooted On'),
(7788, 0, 0, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 103, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sandfury Drudge - On Just Died - Set Rooted On'),
(7795, 0, 0, 0, 0, 0, 100, 0, 0, 0, 3400, 4700, 0, 11, 78801, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Hydromancer Velratha - In Combat - Cast Water Bolt'),
(7795, 0, 1, 0, 2, 0, 100, 1, 0, 50, 0, 0, 0, 11, 12491, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hydromancer Velratha - Between 0-50% Health - Cast Healing Wave (No Repeat)'),
(7795, 0, 2, 0, 9, 0, 100, 0, 0, 20, 14000, 16000, 0, 11, 78802, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hydromancer Velratha - Within 0-20 Range - Cast Crashing Wave'),
(7796, 0, 0, 0, 0, 0, 100, 0, 6500, 16500, 16900, 21500, 0, 11, 8600, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Nekrum Gutchewer - In Combat - Cast Fevered Plague'),
(7797, 0, 0, 0, 12, 0, 100, 0, 0, 20, 30000, 30000, 0, 11, 77720, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Ruuzlu - Target Between 20-20% Health - Cast Execute'),
(7797, 0, 1, 0, 0, 0, 100, 0, 8000, 13000, 7500, 18500, 0, 11, 15496, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Ruuzlu - In Combat - Cast Cleave'),
(8127, 0, 0, 1, 0, 0, 50, 4, 11600, 11600, 13300, 21700, 0, 11, 8376, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Antu''sul - In Combat - Cast Earthgrab Totem (Normal Dungeon)'),
(8127, 0, 1, 0, 61, 0, 50, 5, 11600, 11600, 13300, 21700, 0, 11, 11899, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Antu''sul - In Combat - Cast Healing Ward (Normal Dungeon)'),
(8127, 0, 2, 6, 4, 0, 100, 5, 0, 0, 0, 0, 0, 11, 11894, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Antu''sul - On Aggro - Cast Antu''sul''s Minion (No Repeat) (Normal Dungeon)'),
(8127, 0, 3, 0, 2, 0, 100, 5, 0, 20, 0, 0, 0, 11, 11895, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Antu''sul - Between 0-20% Health - Cast Healing Wave of Antu''sul (No Repeat) (Normal Dungeon)'),
(8127, 0, 4, 5, 2, 0, 100, 5, 0, 25, 0, 0, 0, 11, 11894, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Antu''sul - Between 0-25% Health - Cast Antu''sul''s Minion (No Repeat) (Normal Dungeon)'),
(8127, 0, 5, 0, 61, 0, 100, 5, 0, 25, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Antu''sul - Between 0-25% Health - Say Line 0 (No Repeat) (Normal Dungeon)'),
(8127, 0, 6, 0, 61, 0, 100, 5, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Antu''sul - On Aggro - Say Line 1 (No Repeat) (Normal Dungeon)'),
(8127, 0, 7, 8, 2, 0, 100, 5, 0, 75, 0, 0, 0, 11, 11894, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Antu''sul - Between 0-75% Health - Cast Antu''sul''s Minion (No Repeat) (Normal Dungeon)'),
(8127, 0, 8, 0, 61, 0, 100, 5, 0, 75, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Antu''sul - Between 0-75% Health - Say Line 2 (No Repeat) (Normal Dungeon)'),
(8127, 0, 9, 0, 0, 0, 100, 4, 5000, 5000, 12000, 14000, 0, 11, 16006, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Antu''sul - In Combat - Cast Chain Lightning (Normal Dungeon)'),
(8127, 0, 10, 0, 0, 0, 100, 4, 3000, 3000, 9000, 11000, 0, 11, 15501, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Antu''sul - In Combat - Cast Earth Shock (Normal Dungeon)'),
(8127, 0, 11, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 100, 0, 0, 0, 0, 0, 0, 'Antu''sul - On Data Set 1 1 - Start Attacking'),
(8877, 0, 0, 0, 2, 0, 100, 5, 0, 30, 0, 0, 0, 11, 8599, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sandfury Zealot - Between 0-30% Health - Cast Enrage (No Repeat) (Normal Dungeon)'),
(8877, 0, 1, 0, 2, 0, 100, 5, 0, 30, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sandfury Zealot - Between 0-30% Health - Say Line 0 (No Repeat) (Normal Dungeon)'),
(8877, 0, 2, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 103, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sandfury Zealot - On Just Died - Set Rooted On');
