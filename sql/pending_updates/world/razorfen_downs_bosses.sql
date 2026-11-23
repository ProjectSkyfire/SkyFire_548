UPDATE `creature_template` SET `ScriptName` = 'boss_mordresh_fire_eye' WHERE `entry` = 7357;
UPDATE `creature_template` SET `ScriptName` = 'boss_glutton' WHERE `entry` = 8567;

DELETE FROM `creature_text` WHERE `entry` IN (7357, 8567) AND `groupid` IN (0, 1, 2, 3);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(7357, 0, 0, 'We will enslave the quilboar!', 12, 0, 100, 1, 0, 5819, 'Mordresh Fire Eye - SAY_OOC_1'),
(7357, 1, 0, 'We will spread across this barren land!', 12, 0, 100, 1, 0, 5820, 'Mordresh Fire Eye - SAY_OOC_2'),
(7357, 2, 0, 'Soon, the Scourge will rule the world!', 12, 0, 100, 22, 0, 5821, 'Mordresh Fire Eye - SAY_OOC_3'),
(7357, 3, 0, 'Slay them, my brethren! For the Scourge!', 14, 0, 100, 0, 0, 5822, 'Mordresh Fire Eye - SAY_AGGRO'),
(8567, 0, 0, 'Me smell stench of the living!', 14, 0, 100, 0, 0, 5823, 'Glutton - SAY_AGGRO'),
(8567, 1, 0, 'Me feast on you all!', 14, 0, 100, 0, 0, 5824, 'Glutton - SAY_SLAY'),
(8567, 2, 0, '%s is getting hungry!', 16, 0, 100, 0, 0, 0, 'Glutton - SAY_HEALTH_50'),
(8567, 3, 0, '%s is getting VERY hungry!', 16, 0, 100, 0, 0, 0, 'Glutton - SAY_HEALTH_15');
