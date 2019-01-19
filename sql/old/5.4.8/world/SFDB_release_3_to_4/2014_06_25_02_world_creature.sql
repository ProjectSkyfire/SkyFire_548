-- Krogo Darkhide <Leatherworking & Skinning Supplies> (SPAWN) -- GUID GAPS TAKEN FROM LATEST SKYFIREDB
-- Stephen Wong <Cooking Supplies>
-- Tixit Brightfuse <Engineering Supplies>
-- Vaultkeeper Goldpenny <Banker>
-- Hafuna Landwalker <Herbalism Supplies>
-- Vaultkeeper Kan <Banker>
-- Vaultkeeper Shifen <Banker>
-- Reeler Uko <Fishing Supplies>
-- Gentle Dari <First Aid Supplies>
-- Esha the Loommaiden <Tailoring Supplies>
-- Challenger Wuli <Challenge Dungeons>
-- Sang-Bo <The Shado-Pan>
-- Lena Stonebrush <The Lorewalkers>
-- Training Dummy
DELETE FROM creature WHERE guid IN (1379, 1380, 1381, 1382, 1383, 1384, 1385, 1386, 1387, 1388, 1389, 1390, 1391, 1392, 1393);
INSERT INTO creature (guid, id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, MovementType) VALUES
(1379, 64054, 870, 1, 1, 1703.592, 973.9566, 471.0091, 1.926135, 120, 0, 0), -- Krogo Darkhide <Leatherworking & Skinning Supplies>
(1380, 64126, 870, 1, 1, 1674.898, 985.6649, 471.0073, 4.84544, 120, 0, 0), -- Stephen Wong <Cooking Supplies>
(1381, 64060, 870, 1, 1, 1723.431, 978.9305, 471.0098, 2.603233, 120, 0, 0), -- Tixit Brightfuse <Engineering Supplies>
(1382, 63965, 870, 1, 1, 1701.424, 1006.589, 489.0875, 4.32208, 120, 0, 0), -- Vaultkeeper Goldpenny <Banker>
(1383, 64056, 870, 1, 1, 1721.516, 996.2864, 471.0098, 4.141436, 120, 0, 0), -- Hafuna Landwalker <Herbalism Supplies>
(1384, 63964, 870, 1, 1, 1714.049, 1005.793, 489.0882, 4.893499, 120, 0, 0), -- Vaultkeeper Kan <Banker>
(1385, 63966, 870, 1, 1, 1725.307, 1009.708, 489.0875, 5.437492, 120, 0, 0), -- Vaultkeeper Shifen <Banker>
(1386, 64585, 870, 1, 1, 1764.873, 978.4774, 487.3397, 1.22025, 120, 0, 0), -- Reeler Uko <Fishing Supplies>
(1387, 64062, 870, 1, 1, 1660.983, 969.7656, 470.9239, 3.896879, 120, 0, 0), -- Gentle Dari <First Aid Supplies>
(1388, 64051, 870, 1, 1, 1692.047, 971.316, 471.0088, 1.641564, 120, 0, 0), -- Esha the Loommaiden <Tailoring Supplies>
(1389, 63994, 870, 1, 1, 1596.967, 933.7761, 424.0688, 3.311165, 120, 0, 0), -- Challenger Wuli <Challenge Dungeons>
(1390, 64002, 870, 1, 1, 1591.049, 879.2604, 424.0684, 1.681486, 120, 0, 0), -- Sang-Bo <The Shado-Pan>
(1391, 64531, 870, 1, 1, 1582.462, 901.6042, 424.0684, 1.758687, 120, 0, 0), -- Lena Stonebrush <The Lorewalkers>
(1392, 67127, 870, 1, 1, 1610.755, 996.9445, 470.7093, 4.849982, 120, 0, 0), -- Training Dummy
(1393, 67127, 870, 1, 1, 1599.734, 995.4358, 470.7093, 4.938851, 120, 0, 0); -- Training Dummy