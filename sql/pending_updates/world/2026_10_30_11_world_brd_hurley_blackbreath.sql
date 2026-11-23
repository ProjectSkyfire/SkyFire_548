-- Blackrock Depths: Hurley Blackbreath and his three cronies wait unseen in the Grim Guzzler until the third
-- Thunderbrew Lager Keg is broken (quest 4134, Lost Thunderbrew Recipe). Hurley's own SmartAI and texts are
-- already in the database.
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (-8310001, -8310002, -8310003);
DELETE FROM `creature` WHERE `guid` BETWEEN 8310000 AND 8310003;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(8310000, 9537, 230, 2, 1, 878.121, -153.074, -49.76, 0.527783, 300, 0, 0),
(8310001, 9541, 230, 2, 1, 879.018, -155.273, -49.7591, 0.57177, 300, 0, 0),
(8310002, 9541, 230, 2, 1, 876.961, -151.796, -49.7591, 0.499513, 300, 0, 0),
(8310003, 9541, 230, 2, 1, 876.535, -154.334, -49.7586, 0.339292, 300, 0, 0);

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 9541 AND `ScriptName` = '';
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-8310001, 0, 0, 1, 11, 0, 100, 0, 0, 0, 0, 0, 0, 18, 320, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - On Spawn - Set Unit Flags'),
(-8310001, 0, 1, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 47, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - Linked with Previous Event - Set Invisible'),
(-8310001, 0, 2, 4, 38, 0, 100, 0, 1, 1, 0, 0, 0, 47, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - On Data Set - Set visible'),
(-8310001, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - On spawn- react passif'),
(-8310001, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 53, 0, 9541, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - Linked with Previous Event - Start WP'),
(-8310001, 0, 5, 6, 40, 0, 100, 0, 3, 0, 0, 0, 0, 101, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - On Reached WP 3 - Set Home Position'),
(-8310001, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 0, 19, 320, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - Linked with Previous Event - Set Unit Flags'),
(-8310001, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 89, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - Linked with Previous Event - Set Random Movement'),
(-8310001, 0, 8, 0, 0, 0, 100, 0, 1000, 3000, 4000, 7000, 0, 11, 15581, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - IC - Cast Sinister Strike'),
(-8310001, 0, 9, 0, 0, 0, 100, 0, 2000, 2000, 11000, 12000, 0, 11, 15583, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - IC - Cast Rupture'),
(-8310001, 0, 10, 11, 4, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 2, 0, 0, 0, 0, 11, 9541, 50, 0, 0, 0, 0, 0, 'Blackbreath Crony - On Agro - Set Set Data'),
(-8310001, 0, 11, 12, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 2, 0, 0, 0, 0, 11, 9537, 50, 0, 0, 0, 0, 0, 'Blackbreath Crony - On Agro - Set Set Data'),
(-8310001, 0, 12, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - Linked with Previous Event - Attack'),
(-8310001, 0, 13, 0, 38, 0, 100, 0, 1, 2, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 50, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - On Data Set - Attack'),
(-8310002, 0, 0, 1, 11, 0, 100, 0, 0, 0, 0, 0, 0, 18, 320, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - On Spawn - Set Unit Flags'),
(-8310002, 0, 1, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 47, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - Linked with Previous Event - Set Invisible'),
(-8310002, 0, 2, 4, 38, 0, 100, 0, 1, 1, 0, 0, 0, 47, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - On Data Set - Set visible'),
(-8310002, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - On spawn - react passif'),
(-8310002, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 53, 0, 9541000, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - Linked with Previous Event - Start WP'),
(-8310002, 0, 5, 6, 40, 0, 100, 0, 3, 0, 0, 0, 0, 101, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - On Reached WP 3 - Set Home Position'),
(-8310002, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 0, 19, 320, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - Linked with Previous Event - Set Unit Flags'),
(-8310002, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 89, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - Linked with Previous Event - Set Random Movement'),
(-8310002, 0, 8, 0, 0, 0, 100, 0, 1000, 3000, 4000, 7000, 0, 11, 15581, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - IC - Cast Sinister Strike'),
(-8310002, 0, 9, 0, 0, 0, 100, 0, 2000, 2000, 11000, 12000, 0, 11, 15583, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - IC - Cast Rupture'),
(-8310002, 0, 10, 11, 4, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 2, 0, 0, 0, 0, 11, 9541, 50, 0, 0, 0, 0, 0, 'Blackbreath Crony - On Agro - Set Set Data'),
(-8310002, 0, 11, 12, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 2, 0, 0, 0, 0, 11, 9537, 50, 0, 0, 0, 0, 0, 'Blackbreath Crony - On Agro - Set Set Data'),
(-8310002, 0, 12, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - Linked with Previous Event - Attack'),
(-8310002, 0, 13, 0, 38, 0, 100, 0, 1, 2, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 50, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - On Data Set - Attack'),
(-8310003, 0, 0, 1, 11, 0, 100, 0, 0, 0, 0, 0, 0, 18, 320, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - On Spawn - Set Unit Flags'),
(-8310003, 0, 1, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 47, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - Linked with Previous Event - Set Invisible'),
(-8310003, 0, 2, 4, 38, 0, 100, 0, 1, 1, 0, 0, 0, 47, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - On Data Set - Set visible'),
(-8310003, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - On spawn - react passif'),
(-8310003, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 53, 0, 9541001, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - Linked with Previous Event - Start WP'),
(-8310003, 0, 5, 6, 40, 0, 100, 0, 3, 0, 0, 0, 0, 101, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - On Reached WP 3 - Set Home Position'),
(-8310003, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 0, 19, 320, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - Linked with Previous Event - Set Unit Flags'),
(-8310003, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 89, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - Linked with Previous Event - Set Random Movement'),
(-8310003, 0, 8, 0, 0, 0, 100, 0, 2000, 2000, 11000, 12000, 0, 11, 15581, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - IC - Cast Sinister Strike'),
(-8310003, 0, 9, 0, 0, 0, 100, 0, 3000, 5000, 15000, 20000, 0, 11, 15583, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - IC - Cast Rupture'),
(-8310003, 0, 10, 11, 4, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 2, 0, 0, 0, 0, 11, 9541, 50, 0, 0, 0, 0, 0, 'Blackbreath Crony - On Agro - Set Set Data'),
(-8310003, 0, 11, 12, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 2, 0, 0, 0, 0, 11, 9537, 50, 0, 0, 0, 0, 0, 'Blackbreath Crony - On Agro - Set Set Data'),
(-8310003, 0, 12, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - Linked with Previous Event - Attack'),
(-8310003, 0, 13, 0, 38, 0, 100, 0, 1, 2, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 50, 0, 0, 0, 0, 0, 0, 'Blackbreath Crony - On Data Set - Attack');

DELETE FROM `waypoints` WHERE `entry` IN (9541, 9541000, 9541001);
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(9541, 1, 885.926, -145.908, -49.76, 'Blackbreath Crony'),
(9541, 2, 891.983, -138.614, -49.7543, 'Blackbreath Crony'),
(9541, 3, 896.412, -144.16, -49.7574, 'Blackbreath Crony'),
(9541000, 1, 888.984, -152.599, -49.76, 'Blackbreath Crony'),
(9541000, 2, 895.56, -134.14, -49.7485, 'Blackbreath Crony'),
(9541000, 3, 897.199, -139.513, -49.7554, 'Blackbreath Crony'),
(9541001, 1, 886.917, -149.176, -49.76, 'Blackbreath Crony'),
(9541001, 2, 899.913, -133.535, -49.7479, 'Blackbreath Crony'),
(9541001, 3, 901.009, -139.604, -49.7551, 'Blackbreath Crony');

-- Each broken keg advances the others; the third one wakes Hurley.
UPDATE `gameobject_template` SET `AIName` = 'SmartGameObjectAI' WHERE `entry` = 164911 AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 1 AND `entryorguid` = 164911;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(164911, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 15, 164911, 50, 0, 0, 0, 0, 0, 'Thunderbrew Lager Keg - On State Changed - Set Data'),
(164911, 1, 1, 0, 70, 2, 100, 0, 2, 0, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 9537, 50, 0, 0, 0, 0, 0, 'Thunderbrew Lager Keg - On State Changed - Set Data'),
(164911, 1, 2, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 23, 1, 0, 0, 0, 0, 0, 11, 9537, 50, 0, 0, 0, 0, 0, 'Thunderbrew Lager Keg - On Data Set - Increment Phase');
