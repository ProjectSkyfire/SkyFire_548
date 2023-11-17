DELETE FROM `creature_addon` WHERE `guid`=80046; -- 右 (right)
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (80046, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=80047; -- 左 (Left)
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (80047, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=80091; -- 右
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (80091, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=80092; -- 左
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (80092, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=80360; -- 右
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (80360, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=80361; -- 左
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (80361, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=80512; -- 右
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (80512, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=80513; -- 左
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (80513, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=120679; -- 左
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (120679, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=120680; -- 右
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (120680, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=120683; -- 右
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (120683, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=120684; -- 左
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (120684, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=150001; -- 右
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (150001, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=150002; -- 左
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (150002, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=224431; -- 右
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (224431, 0, 0, 0, 0, 0, NULL);
DELETE FROM `creature_addon` WHERE `guid`=224434; -- 左
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (224434, 0, 0, 0, 0, 0, NULL);



DELETE FROM `smart_scripts` WHERE `entryorguid`=-80046;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-80046, 0, 2, 0, 52, 0, 100, 0, 6, 53565, 0, 0, 0, 1, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 6 Over - Say Text Line 7'),
(-80046, 0, 1, 0, 52, 0, 100, 0, 5, 53565, 0, 0, 0, 1, 6, 15000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 5 Over - Say Text Line 6'),
(-80046, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 1, 5, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 5');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-80047;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-80047, 0, 4, 0, 52, 0, 100, 0, 4, 53565, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 53565, 3, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 4 Over - Set Data 1 1'),
(-80047, 0, 3, 0, 52, 0, 100, 0, 3, 53565, 0, 0, 0, 1, 4, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 3 Over - Say Text Line 4'),
(-80047, 0, 2, 0, 52, 0, 100, 0, 2, 53565, 0, 0, 0, 1, 3, 11000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 2 Over - Say Text Line 3'),
(-80047, 0, 1, 0, 52, 0, 100, 0, 1, 53565, 0, 0, 0, 1, 2, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 2'),
(-80047, 0, 0, 0, 1, 0, 100, 0, 20000, 20000, 120000, 120000, 0, 1, 1, 16000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - OOC - Say Text Line 1');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-80091;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-80091, 0, 2, 0, 52, 0, 100, 0, 6, 53565, 0, 0, 0, 1, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 6 Over - Say Text Line 7'),
(-80091, 0, 1, 0, 52, 0, 100, 0, 5, 53565, 0, 0, 0, 1, 6, 15000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 5 Over - Say Text Line 6'),
(-80091, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 1, 5, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 5');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-80092;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-80092, 0, 4, 0, 52, 0, 100, 0, 4, 53565, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 53565, 3, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 4 Over - Set Data 1 1'),
(-80092, 0, 3, 0, 52, 0, 100, 0, 3, 53565, 0, 0, 0, 1, 4, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 3 Over - Say Text Line 4'),
(-80092, 0, 2, 0, 52, 0, 100, 0, 2, 53565, 0, 0, 0, 1, 3, 11000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 2 Over - Say Text Line 3'),
(-80092, 0, 1, 0, 52, 0, 100, 0, 1, 53565, 0, 0, 0, 1, 2, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 2'),
(-80092, 0, 0, 0, 1, 0, 100, 0, 20000, 20000, 120000, 120000, 0, 1, 1, 16000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - OOC - Say Text Line 1');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-80360;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-80360, 0, 2, 0, 52, 0, 100, 0, 6, 53565, 0, 0, 0, 1, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 6 Over - Say Text Line 7'),
(-80360, 0, 1, 0, 52, 0, 100, 0, 5, 53565, 0, 0, 0, 1, 6, 15000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 5 Over - Say Text Line 6'),
(-80360, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 1, 5, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 5');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-80361;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-80361, 0, 4, 0, 52, 0, 100, 0, 4, 53565, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 53565, 3, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 4 Over - Set Data 1 1'),
(-80361, 0, 3, 0, 52, 0, 100, 0, 3, 53565, 0, 0, 0, 1, 4, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 3 Over - Say Text Line 4'),
(-80361, 0, 2, 0, 52, 0, 100, 0, 2, 53565, 0, 0, 0, 1, 3, 11000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 2 Over - Say Text Line 3'),
(-80361, 0, 1, 0, 52, 0, 100, 0, 1, 53565, 0, 0, 0, 1, 2, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 2'),
(-80361, 0, 0, 0, 1, 0, 100, 0, 20000, 20000, 120000, 120000, 0, 1, 1, 16000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - OOC - Say Text Line 1');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-80512;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-80512, 0, 2, 0, 52, 0, 100, 0, 6, 53565, 0, 0, 0, 1, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 6 Over - Say Text Line 7'),
(-80512, 0, 1, 0, 52, 0, 100, 0, 5, 53565, 0, 0, 0, 1, 6, 15000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 5 Over - Say Text Line 6'),
(-80512, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 1, 5, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 5');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-80513;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-80513, 0, 4, 0, 52, 0, 100, 0, 4, 53565, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 53565, 3, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 4 Over - Set Data 1 1'),
(-80513, 0, 3, 0, 52, 0, 100, 0, 3, 53565, 0, 0, 0, 1, 4, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 3 Over - Say Text Line 4'),
(-80513, 0, 2, 0, 52, 0, 100, 0, 2, 53565, 0, 0, 0, 1, 3, 11000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 2 Over - Say Text Line 3'),
(-80513, 0, 1, 0, 52, 0, 100, 0, 1, 53565, 0, 0, 0, 1, 2, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 2'),
(-80513, 0, 0, 0, 1, 0, 100, 0, 20000, 20000, 120000, 120000, 0, 1, 1, 16000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - OOC - Say Text Line 1');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-120683;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-120683, 0, 2, 0, 52, 0, 100, 0, 6, 53565, 0, 0, 0, 1, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 6 Over - Say Text Line 7'),
(-120683, 0, 1, 0, 52, 0, 100, 0, 5, 53565, 0, 0, 0, 1, 6, 15000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 5 Over - Say Text Line 6'),
(-120683, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 1, 5, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 5');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-120684;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-120684, 0, 4, 0, 52, 0, 100, 0, 4, 53565, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 53565, 3, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 4 Over - Set Data 1 1'),
(-120684, 0, 3, 0, 52, 0, 100, 0, 3, 53565, 0, 0, 0, 1, 4, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 3 Over - Say Text Line 4'),
(-120684, 0, 2, 0, 52, 0, 100, 0, 2, 53565, 0, 0, 0, 1, 3, 11000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 2 Over - Say Text Line 3'),
(-120684, 0, 1, 0, 52, 0, 100, 0, 1, 53565, 0, 0, 0, 1, 2, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 2'),
(-120684, 0, 0, 0, 1, 0, 100, 0, 20000, 20000, 120000, 120000, 0, 1, 1, 16000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - OOC - Say Text Line 1');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-150001;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-150001, 0, 2, 0, 52, 0, 100, 0, 6, 53565, 0, 0, 0, 1, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 6 Over - Say Text Line 7'),
(-150001, 0, 1, 0, 52, 0, 100, 0, 5, 53565, 0, 0, 0, 1, 6, 15000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 5 Over - Say Text Line 6'),
(-150001, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 1, 5, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 5');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-150002;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-150002, 0, 4, 0, 52, 0, 100, 0, 4, 53565, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 53565, 3, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 4 Over - Set Data 1 1'),
(-150002, 0, 3, 0, 52, 0, 100, 0, 3, 53565, 0, 0, 0, 1, 4, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 3 Over - Say Text Line 4'),
(-150002, 0, 2, 0, 52, 0, 100, 0, 2, 53565, 0, 0, 0, 1, 3, 11000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 2 Over - Say Text Line 3'),
(-150002, 0, 1, 0, 52, 0, 100, 0, 1, 53565, 0, 0, 0, 1, 2, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 2'),
(-150002, 0, 0, 0, 1, 0, 100, 0, 20000, 20000, 120000, 120000, 0, 1, 1, 16000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - OOC - Say Text Line 1');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-224431;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-224431, 0, 2, 0, 52, 0, 100, 0, 6, 53565, 0, 0, 0, 1, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 6 Over - Say Text Line 7'),
(-224431, 0, 1, 0, 52, 0, 100, 0, 5, 53565, 0, 0, 0, 1, 6, 15000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 5 Over - Say Text Line 6'),
(-224431, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 1, 5, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 5');

DELETE FROM `smart_scripts` WHERE `entryorguid`=-224434;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, 
`event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, 
`action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, 
`target_y`, `target_z`, `target_o`, `comment`) 
VALUES 
(-224434, 0, 4, 0, 52, 0, 100, 0, 4, 53565, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 53565, 3, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 4 Over - Set Data 1 1'),
(-224434, 0, 3, 0, 52, 0, 100, 0, 3, 53565, 0, 0, 0, 1, 4, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 3 Over - Say Text Line 4'),
(-224434, 0, 2, 0, 52, 0, 100, 0, 2, 53565, 0, 0, 0, 1, 3, 11000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 2 Over - Say Text Line 3'),
(-224434, 0, 1, 0, 52, 0, 100, 0, 1, 53565, 0, 0, 0, 1, 2, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - On Text Line 1 Over - Say Text Line 2'),
(-224434, 0, 0, 0, 1, 0, 100, 0, 20000, 20000, 120000, 120000, 0, 1, 1, 16000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Aspiring Trainee - OOC - Say Text Line 1');