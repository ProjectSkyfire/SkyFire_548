-- Milo's Gyro
DELETE FROM `vehicle_template_accessory` WHERE `entry`=37198;
INSERT INTO `vehicle_template_accessory` (`entry`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
(37198, 37518, 1, 1, 'Milo\'s Gyro', 6, 30000);

UPDATE `creature_template` SET `speed_run`=4 WHERE `entry`=37198;
UPDATE `creature_template` SET `VehicleId`=581 WHERE `entry`=37198;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=37198;
UPDATE `creature_template` SET `InhabitType`=4 WHERE `entry`=37198;

DELETE FROM `smart_scripts` WHERE `entryorguid`=37198;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(37198, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 53, 1, 37198, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Milo\'s Gyro - On Spawn - Start Waypoint Movement'),
(37198, 0, 1, 0, 40, 0, 100, 0, 24, 37198, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Milo\'s Gyro - On Reach WP - Despawn');

-- Milo Geartwinge
DELETE FROM `creature_text` WHERE `entry`=37518;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(37518, 0, 0, '<Static>$bIs this thing on?  Hello?', 12, 0, 100, 0, 0, 0, 'Milo Geartwinge - SAY_0'),
(37518, 1, 0, 'Ahh, there you are.  Hello!  I figured that, while you\'re flying to Kharanos, I\'d give you some data about what\'s going on outside of Coldridge Valley.', 12, 0, 100, 0, 0, 0, 'Milo Geartwinge - SAY_1'),
(37518, 2, 0, 'It seems you Coldridge folks are dreadfully under-informed about the events of the last few days.', 12, 0, 100, 0, 0, 0, 'Milo Geartwinge - SAY_2'),
(37518, 3, 0, 'If you thought you had problems with trolls in Coldridge, wait \'til you get to Kharanos!  That big cataclysm got the trolls all stirred up out here.', 12, 0, 100, 0, 0, 0, 'Milo Geartwinge - SAY_3'),
(37518, 4, 0, 'And if you\'ll look to your left, you\'ll see that the gnomes have retaken Gnomeregan.  Hooray for us!', 12, 0, 100, 0, 0, 0, 'Milo Geartwinge - SAY_4'),
(37518, 5, 0, 'There\'s plenty else going on out here, too.  We\'ve opened up the path to the airport, there\'s rumors of Dark Irons in the east... oh, and I think something weird is going on with that dwarf king, Magni.  Might want to check it out.', 12, 0, 100, 0, 0, 0, 'Milo Geartwinge - SAY_5'),
(37518, 6, 0, 'And here we are!  Welcome to Kharanos!  Watch your step on the way out.', 12, 0, 100, 0, 0, 0, 'Milo Geartwinge - SAY_6');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=37518;

DELETE FROM `smart_scripts` WHERE `entryorguid`=37518;
DELETE FROM `smart_scripts` WHERE `entryorguid`=3751800;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(37518, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 80, 3751800, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Milo Geartwinge - On Spawn - Run Script'),
(3751800, 9, 0, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Milo Geartwinge - On Script - SAY_0'),
(3751800, 9, 1, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Milo Geartwinge - On Script - SAY_1'),
(3751800, 9, 2, 0, 0, 0, 100, 0, 8000, 8000, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Milo Geartwinge - On Script - SAY_2'),
(3751800, 9, 3, 0, 0, 0, 100, 0, 8000, 8000, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Milo Geartwinge - On Script - SAY_3'),
(3751800, 9, 4, 0, 0, 0, 100, 0, 11000, 11000, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Milo Geartwinge - On Script - SAY_4'),
(3751800, 9, 5, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Milo Geartwinge - On Script - SAY_5'),
(3751800, 9, 6, 0, 0, 0, 100, 0, 8000, 8000, 0, 0, 0, 1, 6, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Milo Geartwinge - On Script - SAY_6');
