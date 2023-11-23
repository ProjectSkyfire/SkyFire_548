DELETE FROM `areatrigger_scripts` WHERE `entry`=7747;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(7747, 'at_dawning_valley2');

DELETE FROM `creature_text` WHERE `entry`=60244
INSERT INTO `creature_text` (`entry`, `text`, `type`, `probability`) VALUES
(60244, 'You\'re departing so soon?  I am envious of your prowess, $c. Good luck to you.', 12, 100);
