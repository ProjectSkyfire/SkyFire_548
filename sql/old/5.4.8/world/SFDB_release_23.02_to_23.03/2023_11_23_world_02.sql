DELETE FROM `creature_text` WHERE `entry`=54568;
INSERT INTO `creature_text` (`entry`, `text`, `type`, `probability`) VALUES
(54568, 'Take that monkeybutt!', 12, 100);

DELETE FROM `areatrigger_scripts` WHERE `entry`=7749;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(7749, 'at_wu_song_village');
