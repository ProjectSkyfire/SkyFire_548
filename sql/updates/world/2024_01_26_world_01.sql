UPDATE `creature_template` SET `ScriptName`='boss_tinkerer_gizlock' WHERE `entry`=13601;

UPDATE `creature_template` SET `AIName`='' WHERE `entry`=13601;
DELETE FROM `smart_scripts` WHERE `entryorguid`=13601;

DELETE FROM `creature_text` WHERE `entry`=13601;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(13601, 0, 0, 'Mine! Mine! Mine! Gizlock is the ruler of this domain! You shall never reveal my presence!', 12, 0, 100, 0, 0, 0, 'Tinkerer Gizlock SAY_AGGRO');
