UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_clattering_scorpid' WHERE `entry`=3125;

DELETE FROM `creature_text` WHERE `entry`=3125 AND `groupid`=0 AND `id`=0;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(3125, 0, 0, '$n is casting Envenom! Place your totem down now.', 41, 0, 100, 0, 0, 37666, 'Clattering Scorpid envenom broadcast');

