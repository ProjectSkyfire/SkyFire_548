UPDATE `creature_template` SET `ScriptName`='boss_galgann_firehammer' WHERE `entry`=7291;

DELETE FROM `creature_text` WHERE `entry`=7291;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(7291, 0, 0, 'By Thaurissan\'s beard! Slay them!', 14, 0, 100, 0, 0, 5852, 'Galgann Firehammer SAY_AGGRO');
