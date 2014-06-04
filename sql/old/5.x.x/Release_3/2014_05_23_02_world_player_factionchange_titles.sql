-- Win 75 rated battlegrounds.
-- A: <Name>, Veteran of the Alliance
-- H: <Name>, Veteran of the Horde
DELETE FROM player_factionchange_titles WHERE alliance_id=193 OR horde_id=194;
INSERT INTO player_factionchange_titles
(alliance_id, horde_id) VALUES (193, 194);

-- Win 300 rated battlegrounds.
-- A: Warbound <Name>
-- H: Warbringer <Name>
DELETE FROM player_factionchange_titles WHERE alliance_id=260 OR horde_id=259;
INSERT INTO `player_factionchange_titles`
(`alliance_id`, `horde_id`) VALUES (260, 259);

-- Earn a battleground rating of 1100.
-- A: Private <Name>
-- H: Scout <Name>
DELETE FROM player_factionchange_titles WHERE alliance_id=195 OR horde_id=209;
INSERT INTO `player_factionchange_titles`
(`alliance_id`, `horde_id`) VALUES (195, 209);

-- Earn a battleground rating of 1200.
-- A: Corporal <Name>
-- H: Grunt <Name>
DELETE FROM player_factionchange_titles WHERE alliance_id=196 OR horde_id=210;
INSERT INTO `player_factionchange_titles`
(`alliance_id`, `horde_id`) VALUES (196, 210);

-- Earn a battleground rating of 1300.
-- A: Sergeant <Name>
-- H: Sergeant <Name>
DELETE FROM player_factionchange_titles WHERE alliance_id=197 OR horde_id=211;
INSERT INTO `player_factionchange_titles`
(`alliance_id`, `horde_id`) VALUES (197, 211);

-- Earn a battleground rating of 1400.
-- A: Master Sergeant <Name>
-- H: Senior Sergeant <Name>
DELETE FROM player_factionchange_titles WHERE alliance_id=198 OR horde_id=212;
INSERT INTO `player_factionchange_titles`
(`alliance_id`, `horde_id`) VALUES (198, 212);

-- Earn a battleground rating of 1500.
-- A: Sergeant Major <Name>
-- H: First Sergeant <Name>
DELETE FROM player_factionchange_titles WHERE alliance_id=199 OR horde_id=213;
INSERT INTO `player_factionchange_titles` 
(`alliance_id`, `horde_id`) VALUES (199, 213);

-- Earn a battleground rating of 1600.
-- A: Knight <Name>
-- H: Stone Guard <Name>
DELETE FROM player_factionchange_titles WHERE alliance_id=200 OR horde_id=214;
INSERT INTO `player_factionchange_titles`
(`alliance_id`, `horde_id`) VALUES (200, 214);

-- Earn a battleground rating of 1700.
-- A: Knight-Lieutenant <Name>
-- H: Blood Guard <Name>
DELETE FROM player_factionchange_titles WHERE alliance_id=201 OR horde_id=215;
INSERT INTO `player_factionchange_titles`
(`alliance_id`, `horde_id`) VALUES (201, 215);

-- Earn a battleground rating of 1800.
-- A: Knight-Captain <Name>
-- H: Legionnaire <Name>
DELETE FROM player_factionchange_titles WHERE alliance_id=202 OR horde_id=216;
INSERT INTO `player_factionchange_titles`
(`alliance_id`, `horde_id`) VALUES (202, 216);

-- Earn a battleground rating of 1900.
-- A: Knight-Champion <Name>
-- H: Centurion <Name>
DELETE FROM player_factionchange_titles WHERE alliance_id=203 OR horde_id=217;
INSERT INTO `player_factionchange_titles`
(`alliance_id`, `horde_id`) VALUES (203, 217);

-- Earn a battleground rating of 2000.
-- A: Lieutenant Commander <Name>
-- H: Champion <Name>
DELETE FROM player_factionchange_titles WHERE alliance_id=204 OR horde_id=218;
INSERT INTO `player_factionchange_titles`
(`alliance_id`, `horde_id`) VALUES (204, 218);

-- Earn a battleground rating of 2100.
-- A: Commander <Name>
-- H: Lieutenant General <Name>
DELETE FROM player_factionchange_titles WHERE alliance_id=205 OR horde_id=219;
INSERT INTO `player_factionchange_titles`
(`alliance_id`, `horde_id`) VALUES (205, 219);

-- Earn a battleground rating of 2200.
-- A: Marshal <Name>
-- H: General <Name>
DELETE FROM player_factionchange_titles WHERE alliance_id=206 OR horde_id=220;
INSERT INTO `player_factionchange_titles`
(`alliance_id`, `horde_id`) VALUES (206, 220);

-- Earn a battleground rating of 2300.
-- A: Field Marshal <Name>
-- H: Warlord <Name>
DELETE FROM player_factionchange_titles WHERE alliance_id=207 OR horde_id=221;
INSERT INTO `player_factionchange_titles`
(`alliance_id`, `horde_id`) VALUES (207, 221);

-- Earn a battleground rating of 2400.
-- A: Grand Marshal <Name>
-- H: High Warlord <Name>
DELETE FROM player_factionchange_titles WHERE alliance_id=208 OR horde_id=222;
INSERT INTO `player_factionchange_titles`
(`alliance_id`, `horde_id`) VALUES (208, 222);

-- End a PvP season in the top 0.5% of the rated battleground ladder (requires 50 games won in the current season).
-- A: <Name>, Hero of the Alliance
-- H: <Name>, Hero of the Horde
DELETE FROM player_factionchange_titles WHERE alliance_id=223 OR horde_id=224;
INSERT INTO `player_factionchange_titles`
(`alliance_id`, `horde_id`) VALUES (223, 224);
