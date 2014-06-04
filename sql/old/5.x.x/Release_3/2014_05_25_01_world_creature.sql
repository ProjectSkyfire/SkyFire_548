-- Brewmaster Skye <Innkeeper> (SPAWN) -- GUID GAPS TAKEN FROM LATEST SKYFIREDB
-- Kurong Caskhead <Bartender>
-- Brewmaster Vudia <Food & Drink>
-- Brewmaster Roland
-- Joan Tremblay
-- Madam Vee Luo <Innkeeper>
DELETE FROM creature WHERE guid IN (1006, 1007, 1008, 1009, 1010, 1011);
INSERT INTO creature (guid, id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, MovementType) VALUES
(1006, 63008, 870, 1, 1, 1675.722, 907.3195, 489.5166, 0.181765, 120, 0, 0), -- Brewmaster Skye <Innkeeper>
(1007, 64047, 870, 1, 1, 1675.196, 915.1371, 489.5801, 6.008748, 120, 0, 0), -- Kurong Caskhead <Bartender>
(1008, 64066, 870, 1, 1, 1675.924, 891.6805, 489.3473, 1.322821, 120, 0, 0), -- Brewmaster Vudia <Food & Drink>
(1009, 64065, 870, 1, 1, 1677.958, 879.8542, 489.3861, 1.393569, 120, 0, 0), -- Brewmaster Roland
(1010, 64114, 870, 1, 1, 1700.556, 919.2535, 471.0296, 4.994179, 120, 0, 0), -- Joan Tremblay
(1011, 62996, 870, 1, 1, 1700.946, 893.5833, 471.0085, 3.717179, 120, 0, 0); -- Madam Vee Luo <Innkeeper>