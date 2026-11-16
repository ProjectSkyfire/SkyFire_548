-- The Underbog: SmartAI and spawns for the Murkblood Healer (17730), Lykul Wasp (17732) and Lykul Stinger
-- (19632), which had no AI in SkyFire and so were left out of the spawn fill, and the Large AOI Underbat
-- (18409, no AI in TrinityCore either). SmartAI rows and texts from TrinityCore 3.3.5 (same columns apart
-- from target_param4); spawns of its sniff (build 14545), capped to its count.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (17730, 17732, 19632) AND `AIName` = '' AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (17730, 17732, 19632) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(17730, 0, 0, 1, 74, 0, 100, 2, 0, 50, 16000, 20000, 40, 11, 29427, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Murkblood Healer - On Friendly Between 0-50% Health - Cast ''Holy Light'' (Normal Dungeon)'),
(17730, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Murkblood Healer - Friendly At 7000 Health - Say Line 0 (Normal Dungeon)'),
(17730, 0, 2, 3, 74, 0, 100, 4, 0, 50, 16000, 20000, 40, 11, 37979, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Murkblood Healer - On Friendly Between 0-50% Health - Cast ''Holy Light'' (Heroic Dungeon)'),
(17730, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Murkblood Healer - Friendly At 14000 Health - Say Line 0 (Heroic Dungeon)'),
(17730, 0, 4, 0, 74, 0, 100, 2, 0, 60, 9000, 9000, 40, 11, 34423, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Murkblood Healer - On Friendly Between 0-60% Health - Cast ''Renew'' (Normal Dungeon)'),
(17730, 0, 5, 0, 74, 0, 100, 4, 0, 60, 9000, 9000, 40, 11, 37978, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Murkblood Healer - On Friendly Between 0-60% Health - Cast ''Renew'' (Heroic Dungeon)'),
(17730, 0, 6, 0, 2, 0, 100, 2, 0, 80, 15000, 20000, 0, 11, 15585, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Murkblood Healer - Between 0-80% Health - Cast ''Prayer of Healing'' (Heroic Dungeon)'),
(17730, 0, 7, 0, 2, 0, 100, 4, 0, 80, 15000, 20000, 0, 11, 35943, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Murkblood Healer - Between 0-80% Health - Cast ''Prayer of Healing'' (Heroic Dungeon)'),
(17732, 0, 0, 0, 0, 0, 100, 0, 0, 0, 2400, 4900, 0, 11, 32330, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Lykul Wasp - In Combat CMC - Cast ''Poison Spit'''),
(17732, 0, 1, 2, 2, 0, 100, 1, 0, 30, 0, 0, 0, 11, 8599, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lykul Wasp - Between 0-30% Health - Cast ''Enrage'' (No Repeat)'),
(17732, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lykul Wasp - On Link - Say Line 0'),
(19632, 0, 0, 1, 2, 0, 100, 6, 0, 50, 14000, 18000, 0, 11, 34392, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lykul Stinger - Between 0-50% Health - Cast ''Stinger Rage'' (Dungeon)'),
(19632, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lykul Stinger - Between 0-50% Health - Say Line 0 (Dungeon)');

DELETE FROM `creature_text` WHERE `entry` IN (17730, 17732, 19632);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(17730, 0, 0, '%s begins a massive heal!', 16, 0, 100, 0, 0, 0, 'Murkblood Healer'),
(17732, 0, 0, '%s goes into a frenzy!', 16, 0, 100, 0, 0, 0, 'Lykul Wasp'),
(19632, 0, 0, '%s becomes enraged!', 16, 0, 100, 0, 0, 0, 'Lykul Stinger');

DELETE FROM `creature` WHERE `guid` BETWEEN 8319440 AND 8319459;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8319440, 17730, 546, 6, 0, 0, 0, 0, 179.03400, -391.33800, 48.15670, 5.183630, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8319441, 17730, 546, 6, 0, 0, 0, 0, 197.16200, -389.32200, 72.46490, 5.096360, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8319442, 17730, 546, 6, 0, 0, 0, 0, 259.42500, -367.67600, 72.38190, 1.221730, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8319443, 17732, 546, 6, 0, 0, 0, 0, 266.32900, -220.27800, 28.77760, 2.482410, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8319444, 17732, 546, 6, 0, 0, 0, 0, 266.32900, -220.27800, 28.77760, 2.482410, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8319445, 17732, 546, 6, 0, 0, 0, 0, 266.11200, -217.97700, 28.86630, 4.031710, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8319446, 17732, 546, 6, 0, 0, 0, 0, 253.95700, -183.70600, 28.18600, 5.515240, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8319447, 17732, 546, 6, 0, 0, 0, 0, 262.64900, -209.12600, 28.48090, 5.099930, 7200, 0, 0, 0, 0, 2, 0, 0, 0),
(8319448, 17732, 546, 6, 0, 0, 0, 0, 269.11800, -194.11300, 28.54380, 5.099930, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8319449, 18409, 546, 6, 0, 0, 0, 0, -79.51650, -129.93300, 6.23128, 6.161010, 7200, 0, 0, 0, 0, 2, 0, 0, 0),
(8319450, 18409, 546, 6, 0, 0, 0, 0, 131.52600, -183.17900, 32.36650, 5.044000, 7200, 0, 0, 0, 0, 2, 0, 0, 0),
(8319451, 18409, 546, 6, 0, 0, 0, 0, 58.87120, -257.56100, -4.44989, 4.782200, 7200, 0, 0, 0, 0, 2, 0, 0, 0),
(8319452, 18409, 546, 6, 0, 0, 0, 0, 97.35210, -228.64800, 60.44420, 1.092120, 7200, 0, 0, 0, 0, 2, 0, 0, 0),
(8319453, 18409, 546, 6, 0, 0, 0, 0, 99.96250, -268.83000, 56.41410, 2.070500, 7200, 0, 0, 0, 0, 2, 0, 0, 0),
(8319454, 18409, 546, 6, 0, 0, 0, 0, -74.26940, -332.65600, 8.65701, 2.631590, 7200, 0, 0, 0, 0, 2, 0, 0, 0),
(8319455, 18409, 546, 6, 0, 0, 0, 0, 54.51790, -230.67000, 14.35030, 5.567600, 7200, 0, 0, 0, 0, 2, 0, 0, 0),
(8319456, 18409, 546, 6, 0, 0, 0, 0, 24.99160, -372.86900, 69.41330, 5.567600, 7200, 0, 0, 0, 0, 2, 0, 0, 0),
(8319457, 19632, 546, 6, 0, 0, 0, 0, 266.32900, -220.27800, 28.77760, 1.729360, 7200, 0, 0, 0, 0, 2, 0, 0, 0),
(8319458, 19632, 546, 6, 0, 0, 0, 0, 261.71300, -197.13900, 28.43210, 5.099930, 7200, 0, 0, 0, 0, 0, 0, 0, 0);
