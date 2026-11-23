-- It's Never Over (26616): the player takes Keeshan's Riverboat from the Lakeshire dock to Camp Everstill. Using the boat at
-- the dock summons a second boat (a vehicle) next to the player, the player boards it, the boat follows its 32 waypoints
-- along the lake and the quest is credited when it reaches the last one; the boat is removed when the player leaves it. The
-- boats had no gossip, no vehicle data, no script, no waypoints and no spawn, so the quest could not be finished by boat.
-- Template values, spawns, waypoints and rows are as in Legends of Azeroth 5.4.8. The source names the AI of the vehicle
-- SmartVehicleAI, which this core does not have: SmartAI handles it.

UPDATE `creature_template` SET `minlevel` = 20, `maxlevel` = 20, `faction_A` = 1802, `faction_H` = 1802, `npcflag` = 1, `AIName` = 'SmartAI' WHERE `entry` = 43443 AND `ScriptName` = '';
UPDATE `creature_template` SET `unit_flags` = 32768, `VehicleId` = 964, `AIName` = 'SmartAI' WHERE `entry` = 43450 AND `ScriptName` = '';

DELETE FROM `creature` WHERE `guid` IN (9310804, 9310805);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9310804, 43443, 0, 1, 0, 0, 0, 0, -9345, -2369.39, 55.5233, 3.21957, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(9310805, 43443, 0, 1, 0, 0, 0, 0, -9299.93, -2351.42, 55.3257, 4.0835, 300, 0, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `waypoints` WHERE `entry` = 43450;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(43450, 1, -9303.79, -2358.54, 56.1623, ''),
(43450, 2, -9314.12, -2373.78, 56.1628, ''),
(43450, 3, -9323.39, -2390.75, 56.1635, ''),
(43450, 4, -9328.69, -2408.3, 56.1631, ''),
(43450, 5, -9333.65, -2424.76, 56.164, ''),
(43450, 6, -9337.97, -2439.06, 56.1641, ''),
(43450, 7, -9341.92, -2452.16, 56.1642, ''),
(43450, 8, -9347.2, -2469.64, 56.163, ''),
(43450, 9, -9352.82, -2488.27, 56.1631, ''),
(43450, 10, -9358.44, -2506.89, 56.163, ''),
(43450, 11, -9364.39, -2526.6, 56.1627, ''),
(43450, 12, -9370.34, -2546.31, 56.1642, ''),
(43450, 13, -9375.62, -2563.79, 56.1629, ''),
(43450, 14, -9380.93, -2581.4, 56.1633, ''),
(43450, 15, -9386.22, -2598.95, 56.1615, ''),
(43450, 16, -9391.16, -2615.32, 56.164, ''),
(43450, 17, -9397.78, -2637.26, 56.1645, ''),
(43450, 18, -9402.26, -2652.56, 56.1623, ''),
(43450, 19, -9406.5, -2667.88, 56.1651, ''),
(43450, 20, -9410.03, -2682.18, 56.1642, ''),
(43450, 21, -9412.48, -2693.2, 56.1638, ''),
(43450, 22, -9415.65, -2707.48, 56.1623, ''),
(43450, 23, -9417.69, -2722.01, 56.1635, ''),
(43450, 24, -9416.21, -2735.35, 56.1636, ''),
(43450, 25, -9414.42, -2749.78, 56.164, ''),
(43450, 26, -9412.11, -2768.77, 56.1633, ''),
(43450, 27, -9410.52, -2782.12, 56.1635, ''),
(43450, 28, -9409.06, -2795.57, 56.162, ''),
(43450, 29, -9407.65, -2810.25, 56.1639, ''),
(43450, 30, -9408.22, -2820.1, 56.164, ''),
(43450, 31, -9412.94, -2831, 56.1638, ''),
(43450, 32, -9421.76, -2838.7, 56.163, '');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (43443, 43450);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(43443, 0, 0, 1, 64, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Keeshan''s Riverboat'),
(43443, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 43450, 8, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Keeshan''s Riverboat'),
(43450, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 21, 5, 0, 0, 0, 0, 0, 0, 'Keeshan''s Riverboat'),
(43450, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4345000, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Keeshan''s Riverboat'),
(43450, 0, 2, 0, 28, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Keeshan''s Riverboat'),
(43450, 0, 3, 4, 40, 0, 100, 0, 32, 43450, 0, 0, 0, 15, 26616, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Keeshan''s Riverboat'),
(43450, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 28, 46598, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Keeshan''s Riverboat');

DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` IN (4345000);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4345000, 9, 0, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 86, 46598, 0, 12, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Keeshan''s Riverboat'),
(4345000, 9, 1, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 53, 1, 43450, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Keeshan''s Riverboat'),
(4345000, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 18, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Keeshan''s Riverboat');
