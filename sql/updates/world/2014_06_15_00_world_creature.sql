-- Jinho the Wind Breaker (SPAWN) -- GUID GAPS TAKEN FROM LATEST SKYFIREDB
-- Mifan <Food and Drink>
-- Nadina Stargem <Flexible Vendor>
-- Ki'agnuu <Raid Vendor>
-- Erin
-- Sadi
-- Armorer Relna
-- Dirki Tanboshi
-- Wounded Defender
-- Warpweaver Shafiee <Transmogrifier>
-- Thaumaturge Anjool <Arcane Reforger>
-- Vaultkeeper Meelad <Void Storage>
-- Voidbinder Shadzor <Item Upgrade>
DELETE FROM creature WHERE guid IN (1225, 1226, 1293, 1294, 1295, 1296, 1342, 1344, 1345, 1346, 1347, 1360, 1361, 1362, 1363, 1372);
INSERT INTO creature (guid, id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, MovementType) VALUES
(1225, 66998, 870, 1, 1, 1673.726, 943.642, 471.1779, 0.4085974, 120, 0, 0), -- Jinho the Wind Breaker
(1226, 64041, 870, 1, 1, 1713.882, 896.3594, 471.0092, 4.778744, 120, 0, 0), -- Mifan <Food and Drink>
(1293, 74010, 870, 1, 1, 1677.979, 948.7813, 489.5686, 1.898429, 120, 0, 0), -- Nadina Stargem <Flexible Vendor>
(1294, 74012, 870, 1, 1, 1673.382, 948.5573, 489.5636, 1.686976, 120, 0, 0), -- Ki'agnuu <Raid Vendor>
(1295, 64106, 870, 1, 1, 1679.936, 868.1858, 471.0073, 2.01043, 120, 0, 0), -- Erin
(1296, 64107, 870, 1, 1, 1677.481, 867.7882, 471.0073, 0, 120, 0, 0), -- Sadi
(1342, 64588, 870, 1, 1, 1683.776, 953.2205, 489.5803, 3.500753, 120, 0, 0), -- Armorer Relna
(1344, 64119, 870, 1, 1, 1666.602, 967.9636, 489.438, 3.174507, 120, 0, 0), -- Dirki Tanboshi
(1345, 64118, 870, 1, 1, 1664.686, 967.8108, 489.4384, 6.139359, 120, 0, 0), -- Erni Tanboshi
(1346, 63753, 870, 1, 1, 1662.311, 976.3195, 471.0073, 4.523802, 120, 0, 0), -- Wounded Defender
(1347, 63753, 870, 1, 1, 1668.399, 973.0191, 471.0073, 5.395439, 120, 0, 0), -- Wounded Defender
(1360, 63753, 870, 1, 1, 1661.023, 972.8333, 471.0073, 2.463727, 120, 0, 0), -- Wounded Defender
(1361, 67014, 870, 1, 1, 1777.021, 910.5746, 489.2955, 3.286788, 120, 0, 0), -- Warpweaver Shafiee <Transmogrifier>
(1362, 64044, 870, 1, 1, 1778.918, 897.6563, 487.9105, 3.353462, 120, 0, 0), -- Thaumaturge Anjool <Arcane Reforger>
(1363, 67013, 870, 1, 1, 1777.833, 903.6163, 487.331, 3.252547, 120, 0, 0), -- Vaultkeeper Meelad <Void Storage>
(1372, 68981, 870, 1, 1, 1770.705, 917.967, 487.5727, 3.783183, 120, 0, 0); -- Voidbinder Shadzor <Item Upgrade>