-- Ulduar: Brann Bronzebeard's Flame Leviathan menu had no text row, so talking to him showed nothing.
DELETE FROM `gossip_menu` WHERE `MenuID` = 10355 AND `TextID` = 14369;
INSERT INTO `gossip_menu` (`MenuID`, `TextID`) VALUES (10355, 14369);
