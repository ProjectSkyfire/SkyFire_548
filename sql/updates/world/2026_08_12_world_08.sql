-- Frostmane Troll Whelp
DELETE FROM `creature_text` WHERE `entry`=706;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(706, 0, 0, 'Killing you be easy.', 12, 0, 80, 0, 0, 0, 'SAY_AGGRO1'),
(706, 0, 1, 'You be dead soon!', 12, 0, 80, 0, 0, 0, 'SAY_AGGRO2'),
(706, 0, 2, 'My weapon be thirsty!', 12, 0, 80, 0, 0, 0, 'SAY_AGGRO3'),
(706, 0, 3, 'I gonna make you into mojo!', 12, 0, 80, 0, 0, 0, 'SAY_AGGRO4');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=706;

DELETE FROM `smart_scripts` WHERE `entryorguid`=706;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(706, 0, 0, 0, 4, 0, 50, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frostmane Troll Whelp - On Aggro - Say 0');