UPDATE `creature_template` SET `ScriptName` = 'boss_grand_vizier_ertan' WHERE `entry` = 43878;

DELETE FROM `creature_text` WHERE `entry` = 43878 AND `groupid` IN (0, 1);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(43878, 0, 0, 'Filthy beasts! Your presence in Skywall will not be tolerated!', 14, 0, 100, 0, 0, 20876, 'Grand Vizier Ertan - Aggro'),
(43878, 1, 0, '%s retracts her cyclone shield!', 41, 0, 100, 0, 0, 0, 'Grand Vizier Ertan - Cyclone Shield');
