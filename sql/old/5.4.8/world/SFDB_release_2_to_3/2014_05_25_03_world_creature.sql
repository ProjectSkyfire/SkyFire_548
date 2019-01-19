-- Tu'aho Pathcutter <Heroic Vendor> (SPAWN) -- GUID GAPS TAKEN FROM LATEST SKYFIREDB
-- Blizzix Sparkshiv <Raid Finder Vendor>
-- Jontan Dum'okk
-- Andrew Vestal
-- Andy
-- Uncle Gus
-- Kat
-- Evangelia
-- Mr. Creasey
-- Ala'thinel <First Aid>
DELETE FROM creature WHERE guid IN (1016, 1017, 1018, 1019, 1020, 1021, 1022, 1023, 1024, 1025);
INSERT INTO creature (guid, id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, MovementType) VALUES
(1016, 74019, 870, 1, 1, 1668.498, 948.0608, 489.5732, 1.248165, 120, 0, 0), -- Tu'aho Pathcutter <Heroic Vendor>
(1017, 73674, 870, 1, 1, 1682.056, 949.9028, 489.5803, 2.618294, 120, 0, 0), -- Blizzix Sparkshiv <Raid Finder Vendor>
(1018, 64134, 870, 1, 1, 1656.668, 942.7864, 471.0411, 3.901165, 120, 0, 0), -- Jontan Dum'okk
(1019, 64122, 870, 1, 1, 1655.208, 941.9271, 471.0386, 0.1832798, 120, 0, 0), -- Andrew Vestal
(1020, 67133, 870, 1, 1, 1635.899, 894.2309, 470.6865, 3.337983, 120, 0, 0), -- Andy
(1021, 64043, 870, 1, 1, 1716.13, 879.0417, 471.0092, 2.662929, 120, 0, 0), -- Uncle Gus
(1022, 67134, 870, 1, 1, 1633.313, 896.5278, 470.5966, 4.96013, 120, 0, 0), -- Kat
(1023, 64131, 870, 1, 1, 1631.566, 895.3802, 470.6861, 5.495202, 120, 0, 0), -- Evangelia
(1024, 66731, 870, 1, 1, 1634.04, 891.9739, 470.5998, 1.784811, 120, 0, 0), -- Mr. Creasey
(1025, 65862, 870, 1, 1, 1659.988, 979.1893, 471.0073, 0.4268483, 120, 0, 0); -- Ala'thinel <First Aid>