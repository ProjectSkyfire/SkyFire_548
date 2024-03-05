UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=38345;
UPDATE `gameobject_template` SET `AIName`='SmartGameObjectAI' WHERE `entry`=202112;
DELETE FROM `smart_scripts` WHERE entryorguid IN (202112, 38345, 3834500);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(202112, 1, 0, 1, 70, 0, 100, 0, 2, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 11, 38345, 5, 0, 0, 0, 0, 0, 'Quilboar Cage - On activate - set data on Goblin Prisoner'),
(202112, 1, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 38345, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Quilboar Cage - On activate - quest credit'),
(38345, 0, 0, 0, 11, 0, 100, 1, 0, 0, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 15, 202112, 5, 0, 0, 0, 0, 0, 'Captured Brave - On respawn - reset cage'),
(38345, 0, 1, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 80, 3834500, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Brave - On dataset - Run Script'),
(3834500, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Brave - Script - set data 0'),
(3834500, 9, 1, 0, 0, 0, 100, 0, 2000, 2000, 2000, 2000, 0, 46, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Brave - Script - move 5 yards'),
(3834500, 9, 2, 0, 0, 0, 100, 0, 1000, 1000, 1000, 1000, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Brave - Script - Say random text'),
(3834500, 9, 3, 0, 0, 0, 100, 0, 4000, 4000, 4000, 4000, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Brave - Script - despawn');

DELETE FROM `creature_text` WHERE `entry`=38345;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(38345, 0, 0, 'I can move again!', 12, 0, 50, 0, 0, 0, 'Captured Brave'),
(38345, 0, 1, 'The quilboar will pay!', 12, 0, 50, 0, 0, 0, 'Captured Brave');
