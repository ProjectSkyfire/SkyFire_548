UPDATE `creature_template` SET `ScriptName`='boss_randolph_moloch' WHERE  `entry`=46383;

DELETE FROM `creature_text` WHERE `entry`=46383;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(46383, 0, 0, 'Allow me to introduce myself. O am Randolph Moloch, and i will be killing you all today.', 14, 0, 0, 0, 0, 0, 'Randolph Moloch - SAY_AGGRO'),
(46383, 1, 0, 'My epic schemes, my great plans! Gone!', 14, 0, 0, 0, 0, 0, 'Randolph Moloch - SAY_DEATH');
(46383, 2, 0, 'Randolph Moloch vanishes!', 41, 0, 0, 0, 0, 0, 'Randolph Moloch - SAY_VANISH');
