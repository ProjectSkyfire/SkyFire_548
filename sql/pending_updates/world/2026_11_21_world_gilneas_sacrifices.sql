-- Gilneas: Sacrifices (14212). The quest is a ride on Crowley's Horse and needs a vehicle, its spell click, the
-- accessory Crowley, the route, the Throw Torch target conditions and the credit on the Bloodfang Stalkers.
-- Based on Legends of Azeroth 5.4.8 (vehicle, spell click, accessory, target conditions, torch credit, spawn) and
-- ported from The Cataclysm Preservation Project 4.3.4 (route waypoints, horse script npc_crowleys_horse).
-- The horse stands in phase 172, the window of the quest. Safe to run twice: DELETE then INSERT on the same keys.

-- 1. Crowley's Horse: vehicle 463 (player seat 0, Crowley seat 1), clickable, driven by npc_crowleys_horse.
UPDATE `creature_template` SET `minlevel` = 5, `maxlevel` = 5, `speed_run` = 1.28571, `npcflag` = 16777216, `VehicleId` = 463, `ScriptName` = 'npc_crowleys_horse' WHERE `entry` = 35231;
UPDATE `creature_template` SET `minlevel` = 10, `maxlevel` = 10 WHERE `entry` = 35230;

-- 2. Spell click: Ride Vehicle Hardcoded for the players on the quest, and for Crowley when the horse installs him.
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 35231;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(35231, 46598, 1, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 35231 AND `SourceEntry` = 46598;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 35231, 46598, 0, 0, 9, 0, 14212, 0, 0, 0, 0, 0, '', 'Crowley''s Horse - Required quest active for spellclick'),
(18, 35231, 46598, 0, 1, 31, 0, 3, 35230, 0, 0, 0, 0, '', 'Crowley''s Horse - Lord Darius Crowley rides along');

-- 3. Lord Darius Crowley rides in the second seat and leaves with the horse.
DELETE FROM `vehicle_template_accessory` WHERE `entry` = 35231 AND `accessory_entry` = 35230;
INSERT INTO `vehicle_template_accessory` (`entry`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
(35231, 35230, 1, 1, 'Crowley''s Horse - Lord Darius Crowley', 8, 0);

-- 4. The route: the horse jumps the first barricade before path 352310 and the second one before path 352311.
DELETE FROM `waypoint_data` WHERE `id` IN (352310, 352311);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) VALUES
(352310, 1, -1686.12, 1655.64, 20.6106, 0, 0, 1, 0, 100, 0),
(352310, 2, -1668.16, 1639.43, 20.6106, 0, 0, 1, 0, 100, 0),
(352310, 3, -1666.63, 1628.86, 20.5327, 0, 0, 1, 0, 100, 0),
(352310, 4, -1670.76, 1617.43, 20.6106, 0, 0, 1, 0, 100, 0),
(352310, 5, -1693.65, 1592.64, 20.6106, 0, 0, 1, 0, 100, 0),
(352310, 6, -1708.02, 1590.55, 20.6104, 0, 0, 1, 0, 100, 0),
(352310, 7, -1719.37, 1595.65, 20.6104, 0, 0, 1, 0, 100, 0),
(352310, 8, -1720.57, 1610.14, 20.6074, 0, 0, 1, 0, 100, 0),
(352310, 9, -1711.9, 1629.7, 20.6074, 0, 0, 1, 0, 100, 0),
(352310, 10, -1698.15, 1656.48, 20.6106, 0, 0, 1, 0, 100, 0),
(352310, 11, -1696.42, 1682.01, 20.5262, 0, 0, 1, 0, 100, 0),
(352310, 12, -1684.68, 1700.12, 20.4648, 0, 0, 1, 0, 100, 0),
(352310, 13, -1658.31, 1710.31, 20.5898, 0, 0, 1, 0, 100, 0),
(352310, 14, -1625.48, 1712.3, 21.8051, 0, 0, 1, 0, 100, 0),
(352310, 15, -1592.09, 1710.71, 20.5899, 0, 0, 1, 0, 100, 0),
(352311, 1, -1545.08, 1684.23, 20.6099, 0, 0, 1, 0, 100, 0),
(352311, 2, -1542.79, 1656.08, 20.6099, 0, 0, 1, 0, 100, 0),
(352311, 3, -1543, 1640.54, 20.6099, 0, 0, 1, 0, 100, 0),
(352311, 4, -1520.73, 1618.1, 20.6105, 0, 0, 1, 0, 100, 0),
(352311, 5, -1498.79, 1621.74, 20.6105, 0, 0, 1, 0, 100, 0),
(352311, 6, -1468.36, 1630.78, 20.6105, 0, 0, 1, 0, 100, 0),
(352311, 7, -1441.65, 1627.67, 20.6105, 0, 0, 1, 0, 100, 0),
(352311, 8, -1429.84, 1616.63, 20.5898, 0, 0, 1, 0, 100, 0),
(352311, 9, -1417.99, 1586.55, 20.5898, 0, 0, 1, 0, 100, 0),
(352311, 10, -1429.18, 1548.78, 20.8485, 0, 0, 1, 0, 100, 0),
(352311, 11, -1447.42, 1532.79, 20.601, 0, 0, 1, 0, 100, 0),
(352311, 12, -1463.1, 1535.03, 20.6105, 0, 0, 1, 0, 100, 0),
(352311, 13, -1483.04, 1544.15, 20.6105, 0, 0, 1, 0, 100, 0),
(352311, 14, -1505.16, 1568.61, 20.6105, 0, 0, 1, 0, 100, 0),
(352311, 15, -1531.6, 1581.87, 26.5387, 0, 0, 1, 0, 100, 0),
(352311, 16, -1540.59, 1574.43, 29.2067, 0, 0, 1, 0, 100, 0);

-- 5. The horse waits next to Lord Darius Crowley (35552) in phase 172. It respawns a short time after each ride.
DELETE FROM `creature` WHERE `guid` = 9390001;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9390001, 35231, 654, 1, 172, 0, 0, 0, -1735.78, 1658.31, 20.6732, 0.038924, 15, 0, 0, 0, 0, 0, 0, 0, 0);

-- 6. Throw Torch (67063) hits the Bloodfang Stalkers around the target point, and each stalker gives the credit
--    to the riders of the vehicle that threw it. The cooldown stops one stalker from counting again while it burns.
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 7 AND `SourceEntry` = 67063;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 7, 67063, 0, 0, 31, 0, 3, 35229, 0, 0, 0, 0, '', 'Throw Torch - Target Bloodfang Stalker');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 35229;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 35229;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(35229, 0, 0, 0, 8, 0, 100, 0, 67063, 0, 30000, 30000, 0, 33, 35582, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bloodfang Stalker - On spell hit (Throw Torch) - Give kill credit');
