-- Gilneas: Save Krennan Aranas (14293). Ported from the Cataclysm Preservation
-- Project 4.3.4 (the horse script and its two routes, the SmartAI rows of Genn and Krennan).
--
-- Accepting the quest from King Genn casts 68232, which summons King Greymane's Horse (35905) with the player aboard. The
-- horse (vehicle 494, script npc_greymanes_horse) runs to the tree and jumps to Krennan; the player rescues him with the
-- vehicle's ability (spell 68219, which credits 35753 and summons the rescued Krennan 35907), and the horse brings both
-- back to the barricade. Krennan boards the horse by casting the ride spell on the player's vehicle when summoned.
UPDATE `creature_template` SET `VehicleId`=494, `speed_run`=1.2857, `minlevel`=5, `maxlevel`=5, `ScriptName`='npc_greymanes_horse' WHERE `entry`=35905;
UPDATE `creature_template` SET `minlevel`=3, `maxlevel`=3, `AIName`='SmartAI' WHERE `entry`=35907;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry` = 35550;

DELETE FROM `waypoint_data` WHERE `id` IN (3590500, 3590501);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) VALUES
(3590500, 1, -1797.425, 1396.941, 20.07336, 0, 0, 1, 0, 100, 0),
(3590500, 2, -1788.675, 1378.441, 20.07336, 0, 0, 1, 0, 100, 0),
(3590500, 3, -1780.925, 1368.941, 20.07336, 0, 0, 1, 0, 100, 0),
(3590500, 4, -1767.425, 1358.191, 19.82336, 0, 0, 1, 0, 100, 0),
(3590500, 5, -1746.425, 1358.691, 20.07336, 0, 0, 1, 0, 100, 0),
(3590500, 6, -1726.175, 1354.191, 19.82336, 0, 0, 1, 0, 100, 0),
(3590500, 7, -1709.064, 1348.535, 19.78232, 0, 0, 1, 0, 100, 0),
(3590501, 1, -1664.807, 1345.011, 15.48499, 0, 0, 1, 0, 100, 0),
(3590501, 2, -1662.807, 1354.511, 15.48499, 0, 0, 1, 0, 100, 0),
(3590501, 3, -1667.307, 1362.511, 15.48499, 0, 0, 1, 0, 100, 0),
(3590501, 4, -1674.307, 1363.761, 15.48499, 0, 0, 1, 0, 100, 0),
(3590501, 5, -1686.057, 1355.011, 15.48499, 0, 0, 1, 0, 100, 0),
(3590501, 6, -1691.057, 1347.261, 15.48499, 0, 0, 1, 0, 100, 0),
(3590501, 7, -1705.807, 1350.011, 19.98499, 0, 0, 1, 0, 100, 0),
(3590501, 8, -1731.307, 1360.011, 19.98499, 0, 0, 1, 0, 100, 0),
(3590501, 9, -1744.807, 1370.511, 20.23499, 0, 0, 1, 0, 100, 0),
(3590501, 10, -1758.807, 1389.511, 19.98499, 0, 0, 1, 0, 100, 0),
(3590501, 11, -1768.307, 1410.011, 19.98499, 0, 0, 1, 0, 100, 0),
(3590501, 12, -1771.557, 1423.011, 19.98499, 0, 0, 1, 0, 100, 0),
(3590501, 13, -1770.955, 1430.332, 19.83506, 0, 0, 1, 0, 100, 0);

DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid` IN (35550, 35907);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(35550, 0, 0, 0, 19, 0, 100, 0, 14293, 0, 0, 0, 0, 11, 68232, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'King Genn Greymane - On Quest Accepted (Save Krennan Aranas) - Cast Summon Greymane Horse On Player'),
(35907, 0, 0, 0, 37, 0, 100, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Krennan Aranas - On AI Initialize - Set Passive'),
(35907, 0, 1, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 11, 46598, 2, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 0, 'Krennan Aranas - On Just Summoned - Ride Invoker Vehicle');
