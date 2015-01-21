-- Tivilix Bangalter <Explosive Expeditions> (SPAWN) -- GUID GAPS TAKEN FROM LATEST SKYFIREDB
-- Guyo Crystalgear <Explosive Expeditions>
-- D.E.N.T. <Mechanical Auctioneer>
-- Horde Peacekeeper
DELETE FROM creature WHERE guid IN (1012, 1013, 1014, 1015);
INSERT INTO creature (guid, id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, MovementType) VALUES
(1012, 64925, 870, 1, 1, 1707.613, 907.9549, 471.0124, 1.165972, 120, 0, 0), -- Tivilix Bangalter <Explosive Expeditions>
(1013, 64924, 870, 1, 1, 1709.538, 909.757, 471.0128, 3.97115, 120, 0, 0), -- Guyo Crystalgear <Explosive Expeditions>
(1014, 67130, 870, 1, 1, 1709.651, 915.25, 471.0162, 4.062538, 120, 0, 0), -- D.E.N.T. <Mechanical Auctioneer>
(1015, 64587, 870, 1, 1, 1692.48, 937.7344, 471.2409, 5.398808, 120, 0, 0); -- Horde Peacekeeper