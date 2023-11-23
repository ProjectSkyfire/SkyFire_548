DELETE FROM `areatrigger_scripts` WHERE `entry`=7750;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(7750, 'at_dawning_valley');

DELETE FROM `creature_text` WHERE `entry`=60248;
INSERT INTO `creature_text` (`entry`, `text`, `type`, `probability`) VALUES
(60248, 'Is that... is that Huo?', 12, 100);

DELETE FROM `creature_text` WHERE `entry`=60253;
INSERT INTO `creature_text` (`entry`, `text`, `type`, `probability`) VALUES
(60253, 'It is! Well done, $p!', 12, 100);
