DELETE FROM `creature_text` WHERE `entry` IN (54567, 54943);
INSERT INTO `creature_text` (`entry`, `text`, `type`, `probability`) VALUES
(54567, 'A challenge in compassion and humility. By helping Merchant Lorvo, it would seem you\'ve passed, $c. Master Shang would be pleased.', 12, 100),
(54943, 'Shhhh! Come quietly. She\'s practicing.', 12, 100);

DELETE FROM `areatrigger_scripts` WHERE `entry`=7748;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(7748, 'at_fus_pond');
