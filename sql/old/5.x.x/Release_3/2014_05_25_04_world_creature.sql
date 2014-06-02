-- Fang Whitescroll <Inscription Supplies> (SPAWN) -- GUID GAPS TAKEN FROM LATEST SKYFIREDB
-- Victor Pearce <Alchemy Supplies>
-- Derenda Enkleshin <Enchanting Supplies>
-- Edward Hanes
-- Frances Lin
-- Razzie Coalwrench <Mining Supplies>
-- Miss Jadepaw <Jewelcrafting Supplies>
-- Moko Powderrun <Fireworks>
-- Danky <Spirit of Harmony Vendor>
-- Elder Liao <Charm Maker>
-- Mokimo the Strong <Caretaker>
DELETE FROM creature WHERE guid IN (1026, 1027, 1028, 1029, 1030, 1031, 1032, 1033, 1034, 1035, 1036);
INSERT INTO creature (guid, id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, MovementType) VALUES
(1026, 64053, 870, 1, 1, 1712.7, 974.684, 471.0095, 1.86126, 120, 0, 0), -- Fang Whitescroll <Inscription Supplies>
(1027, 64049, 870, 1, 1, 1719.352, 975.7795, 471.0096, 1.7356, 120, 0, 0), -- Victor Pearce <Alchemy Supplies>
(1028, 64050, 870, 1, 1, 1716.389, 975.243, 471.0096, 1.669878, 120, 0, 0), -- Derenda Enkleshin <Enchanting Supplies>
(1029, 64128, 870, 1, 1, 1659.993, 990.4861, 471.0073, 0.5427373, 120, 0, 0), -- Edward Hanes
(1030, 64127, 870, 1, 1, 1661.504, 990.875, 471.0073, 3.646143, 120, 0, 0), -- Frances Lin
(1031, 64059, 870, 1, 1, 1725.115, 980.6007, 471.0098, 2.587013, 120, 0, 0), -- Razzie Coalwrench <Mining Supplies>
(1032, 73422, 870, 1, 1, 1726.724, 982.6563, 471.0099, 2.695751, 120, 0, 0), -- Miss Jadepaw <Jewelcrafting Supplies>
(1033, 64057, 870, 1, 1, 1710.877, 998.2153, 471.0095, 4.825661, 120, 0, 0), -- Moko Powderrun <Fireworks>
(1034, 66685, 870, 1, 1, 1582.142, 918.5226, 473.7804, 0, 120, 0, 0), -- Danky <Spirit of Harmony Vendor>
(1035, 63996, 870, 1, 1, 1600.377, 909.8611, 424.0688, 3.403384, 120, 0, 0), -- Elder Liao <Charm Maker>
(1036, 59959, 870, 1, 1, 1566.382, 929.6233, 473.7804, 4.819012, 120, 0, 0); -- Mokimo the Strong <Caretaker>