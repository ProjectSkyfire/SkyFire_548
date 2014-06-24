-- Innkeeper Lei Lan (SPAWN) -- GUID GAPS TAKEN FROM LATEST SKYFIREDB
-- Gina Mudclaw <Tillers Quartermaster> (SPAWN)
-- Farmer Yoon (SPAWN)
-- Boatbuilder Shu (SPAWN)
-- Anthea Ironpaw <Master of the Wok> (SPAWN)
-- Yun Applebarrel <Apples> (SPAWN)
-- Hopsmaster Chang (SPAWN)
-- Yan Ironpaw <Master of the Steamer> (SPAWN)
DELETE FROM creature WHERE guid IN (993, 994, 995, 996, 997, 998, 999, 1000);
INSERT INTO creature (guid, id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, MovementType) VALUES
(993, 59582, 870, 1, 1, -230.1615, 510.6875, 190.9044, 3.745674, 120, 0, 0), -- Innkeeper Lei Lan
(994, 58706, 870, 1, 1, -259.5399, 592.5469, 167.6311, 6.132261, 120, 0, 0), -- Gina Mudclaw <Tillers Quartermaster>
(995, 58646, 870, 1, 1, -180.8438, 628.3577, 165.4926, 1.854483, 120, 0, 0), -- Farmer Yoon
(996, 64452, 870, 1, 1, -177.9913, 435.1059, 167.7279, 4.900331, 120, 0, 0), -- Boatbuilder Shu
(997, 58713, 870, 1, 1, -264.7535, 611.3941, 167.6311, 5.253441, 120, 0, 0), -- Anthea Ironpaw <Master of the Wok>
(998, 64451, 870, 1, 1, -177.2691, 394.9879, 162.189, 4.611999, 120, 0, 0), -- Yun Applebarrel <Apples>
(999, 64448, 870, 1, 1, -259.6754, 479.901, 170.4586, 4.615591, 120, 0, 0), -- Hopsmaster Chang
(1000, 58715, 870, 1, 1, -258.3629, 617.0035, 167.6311, 5.09427, 120, 0, 0); -- Yan Ironpaw <Master of the Steamer>