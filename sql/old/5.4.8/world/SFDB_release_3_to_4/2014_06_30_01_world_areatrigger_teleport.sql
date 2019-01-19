-- New AreaTrigger Teleport Locations To 5 Man Dungeons.
DELETE FROM areatrigger_teleport WHERE id IN (8134, 7705, 7694, 7726, 8315, 7854, 8135, 7755, 7699, 7725, 8312, 7687);
INSERT INTO areatrigger_teleport (id, name, target_map, target_position_x, target_position_y, target_position_z, target_orientation) VALUES
-- ENTER INSTANCES
(8134, 'Mogushan Palace - Entrance Target', 994, -3969.67, -2542.71, 26.7537, 4.71239), -- Mogushan Palace
(7705, 'Stormstout Brewery - Entrance Target', 961, -732.115, 1266.13, 116.108, 1.81208), -- Stormstout Brewery
(7694, 'Shado-Pan Monastery - Entrance Target', 959, 3657.29, 2551.92, 766.966, 0.436332), -- Shado-Pan Monastery
(7726, 'Gate of the Setting Sun - Entrance Target', 962, 722.097, 2108.08, 402.978, 1.59264), -- Gate of the Setting Sun
(8315, 'Siege of Niuzao Temple - Entrance Target', 1011, 1455.33, 5100.01, 149.415, 0.959931), -- Siege of Niuzao Temple
(7854, 'Temple of the Jade Serpent - Entrance Target', 960, 953.37, -2487.5, 180.431, 4.36908), -- Temple of the Jade Serpent

-- EXIT INSTANCES
(8135, 'Mogushan Palace - Exit Target', 870, 1391.83, 437.694, 478.941, 2.31752), -- Mogushan Palace
(7755, 'Stormstout Brewery - Exit Target', 870, -713.356, 1263.42, 136.223, 0.268626), -- Stormstout Brewery
(7699, 'Shado-Pan Monastery - Exit Target', 870, 3631.29, 2538.41, 769.912, 3.61283), -- Shado-Pan Monastery
(7725, 'Gate of the Setting Sun - Exit Target', 870, 692.365, 2080.17, 374.694, 3.18706), -- Gate of the Setting Sun
(8312, 'Siege of Niuzao Temple - Exit Target', 870, 1433.27, 5085.39, 133.807, 3.57792), -- Temple of the Jade Serpent
(7687, 'Temple of the Jade Serpent - Exit Target', 870, 957.884, -2474.01, 180.505, 1.29244); -- Temple of the Jade Serpent