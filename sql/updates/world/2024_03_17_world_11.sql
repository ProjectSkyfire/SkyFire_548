UPDATE `creature_template` SET `ScriptName`='npc_jaomin_ro' WHERE `entry`=54611;
DELETE FROM `creature_text` WHERE `entry`=54611;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(54611, 0, 0, 'A Challenger approaches', 12, 0, 100, 0, 0, 0, 'Jaomin Ro SAY_0'),
(54611, 1, 0, 'Well fought. Master Shang will be pleased.', 12, 0, 100, 0, 0, 0, 'Jaomin Ro SAY_1');

