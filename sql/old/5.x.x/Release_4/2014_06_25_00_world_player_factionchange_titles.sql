-- Defeat Garrosh Hellscream in Siege of Orgrimmar on Normal or Heroic difficulty.
-- A: <Name>, Conqueror of Orgrimmar
-- H: <Name>, Liberator of Orgrimmar
DELETE FROM player_factionchange_titles WHERE alliance_id=382 OR horde_id=383;
INSERT INTO player_factionchange_titles (alliance_id, horde_id) VALUES
(382, 383);