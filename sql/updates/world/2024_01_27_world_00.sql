DELETE FROM `areatrigger_scripts` WHERE `entry`=8287;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(8287, 'at_dawning_span');

DELETE FROM `creature_text` WHERE `entry`=64885;
INSERT INTO `creature_text` (`entry`, `text`, `type`, `probability`) VALUES
(64885, 'Hello $n! The Lorewalker is beginning her lesson just down these stairs if you want to listen in.', 12, 100);
