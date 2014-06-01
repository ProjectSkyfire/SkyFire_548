-- Cloud Serpent (SPAWN) -- GUID GAPS TAKEN FROM LATEST SKYFIREDB
-- Brewmaster Linshi
-- Jaluu the Generous <The Golden Lotus Quartermaster>
-- Weng the Merciful <The Golden Lotus>
-- Farmhand Dooka <The Tillers>
-- Skydancer Shun <Flying Trainer>
-- Sage Lotusbloom <The August Celestials Quartermaster>
-- Abrogar Dusthoof
-- Mishi <Lorewalker Cho's Companion>
-- Magenta <Battle Pet>
-- Seamus <Battle Pet>
-- Tungsten <Battle Pet>
-- Master Angler Karu <Anglers>
DELETE FROM creature WHERE guid IN (1037, 1038, 1039, 1040, 1041, 1042, 1043, 1044, 1045, 1046, 1047, 1048, 1049);
INSERT INTO creature (guid, id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, MovementType) VALUES
(1037, 64535, 870, 1, 1, 1584.75, 944.0018, 424.0684, 3.271185, 120, 0, 0), -- Cloud Serpent
(1038, 64019, 870, 1, 1, 1580.592, 961.1163, 424.0684, 4.807791, 120, 0, 0), -- Brewmaster Linshi
(1039, 59908, 870, 1, 1, 1575.398, 879.368, 424.0568, 2.956085, 120, 0, 0), -- Jaluu the Generous <The Golden Lotus Quartermaster>
(1040, 64007, 870, 1, 1, 1571.422, 880.6302, 424.1007, 1.242907, 120, 0, 0), -- Weng the Merciful <The Golden Lotus>
(1041, 64011, 870, 1, 1, 1561.5, 954.5555, 424.0684, 5.347487, 120, 0, 0), -- Farmhand Dooka <The Tillers>
(1042, 60167, 870, 1, 1, 1555.219, 890.882, 478.4328, 0.7505001, 120, 0, 0), -- Skydancer Shun <Flying Trainer>
(1043, 64001, 870, 1, 1, 1553.146, 890.4774, 424.1285, 0.7102981, 120, 0, 0), -- Sage Lotusbloom <The August Celestials Quartermaster>
(1044, 71483, 870, 1, 1, 1541.887, 904.5208, 473.6818, 0.4659754, 120, 0, 0), -- Abrogar Dusthoof
(1045, 65716, 870, 1, 1, 1540.342, 910.0208, 424.0684, 0, 120, 0, 0), -- Mishi <Lorewalker Cho's Companion>
(1046, 64590, 870, 1, 1, 1548.917, 939.6702, 473.6741, 5.877761, 120, 0, 0), -- Magenta <Battle Pet>
(1047, 64591, 870, 1, 1, 1547.519, 938.8351, 473.6741, 4.93681, 120, 0, 0), -- Seamus <Battle Pet>
(1048, 64589, 870, 1, 1, 1549.316, 941.2396, 473.6741, 5.966582, 120, 0, 0), -- Tungsten <Battle Pet>
(1049, 64010, 870, 1, 1, 1547.057, 939.2031, 424.0684, 6.056645, 120, 0, 0); -- Master Angler Karu <Anglers>