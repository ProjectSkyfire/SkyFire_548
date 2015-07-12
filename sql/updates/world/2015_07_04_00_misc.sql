-- Madam Goya
UPDATE creature_template SET npcflag = 1 WHERE entry = 62943;
UPDATE creature_template SET ScriptName = "npc_madam_goya" WHERE entry = 62943;

-- Blackmarker Auction Teleport
DELETE FROM `game_tele` WHERE `id` = 1525;
INSERT INTO `game_tele` (`Id`, `position_x`, `position_y`, `position_z`, `orientation`, `map`, `name`) 
VALUES ('1525', '800.708', '-343.901', '400.784', '2.22925', '870', 'BlackmarketAuction');
