UPDATE `creature_template` SET `ScriptName`='boss_grimlok' WHERE `entry`=4854;

DELETE FROM `creature_text` WHERE `entry`=4854;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(4854, 0, 0, 'Me Grimlok, king!', 14, 0, 100, 0, 0, 5853, 'Grimlok SAY_AGGRO'),
(4854, 1, 0, 'Die, die!', 14, 0, 100, 0, 0, 5854, 'Grimlok SAY_KILL');
