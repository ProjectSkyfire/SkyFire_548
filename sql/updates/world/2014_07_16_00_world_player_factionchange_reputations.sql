-- A: Baradin's Wardens - H: Hellscream's Reach
DELETE FROM player_factionchange_reputations WHERE alliance_id=1177 OR horde_id=1178;
INSERT INTO player_factionchange_reputations (alliance_id, horde_id) VALUES
(1177, 1178);

-- A: Wildhammer Clan - H: Dragonmaw Clan
DELETE FROM player_factionchange_reputations WHERE alliance_id=1174 OR horde_id=1172;
INSERT INTO player_factionchange_reputations (alliance_id, horde_id) VALUES
(1174, 1172);

-- A: Kirin Tor Offensive - H: Sunreaver Onslaught
DELETE FROM player_factionchange_reputations WHERE alliance_id=1387 OR horde_id=1388;
INSERT INTO player_factionchange_reputations (alliance_id, horde_id) VALUES
(1387, 1388);

-- A: Operation: Shieldwall - H: Dominance Offensive
DELETE FROM player_factionchange_reputations WHERE alliance_id=1376 OR horde_id=1375;
INSERT INTO player_factionchange_reputations (alliance_id, horde_id) VALUES
(1376, 1375);
