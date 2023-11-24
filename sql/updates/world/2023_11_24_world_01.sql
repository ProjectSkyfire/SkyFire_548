DELETE FROM `creature_text` WHERE `entry`=60176;
INSERT INTO `creature_text` (`entry`, `text`, `type`, `probability`) VALUES
(60176, 'The fire spirit is upset. It\'s dangerous to enter the shrine now...', 12, 100);

DELETE FROM `areatrigger_scripts` WHERE `entry`=7736;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(7736, 'at_shrine_of_inner_light');
