-- Remove old instance templates for scarlet halls and scholomance
DELETE FROM instance_template WHERE map IN (289, 189);

-- Remove creatures from old scarlet halls and scholomance
DELETE FROM creature WHERE map IN (189, 289);

-- Remove gameobjects from old scarlet halls and scholomance
DELETE FROM gameobject WHERE map IN (189, 289);

-- Remove access_requirement from old scarlet halls and scholomance
DELETE FROM access_requirement WHERE mapId IN (189, 289);

DELETE FROM instance_template WHERE map IN (1001, 1004, 1007);
INSERT INTO instance_template (map, parent) VALUES
(1001, 0), -- Scarlet Halls
(1004, 0), -- Scarlet Monastery
(1007, 0); -- Scholomance

DELETE FROM areatrigger_teleport WHERE id IN (45, 614, 2567, 7801, 7802, 7539);
INSERT INTO areatrigger_teleport VALUES 
-- ENTER INSTANCE
(614, 'Scarlet Halls - Entrance Target', 1001, 820.743, 607.812, 13.6389, 0), -- Scarlet Halls
(45, 'Scarlet Monastery - Entrance Target', 1004, 1124.64, 512.467, 0.989549, 1.5708), -- Scarlet Monastery
(2567, 'Scholomance - Entrance Target', 1007, 199.876, 125.346, 138.43, 4.67748), -- Scholomance

-- EXIT INSTANCE
(7801, 'Scarlet Halls - Exit Target', 0, 2869.32, -820.818, 160.333, 0.349066), -- Scarlet Halls
(7802, 'Scarlet Monastery - Exit Target', 0, 2915.34, -801.58, 160.333, 3.49066), -- Scarlet Monastery
(7539, 'Scholomance - Exit Target', 0, 1273.91, -2553.09, 90.6353, 3.57792); -- Scholomance

DELETE FROM access_requirement WHERE mapId IN (961, 994, 959, 962, 1011, 960, 1001, 1004, 1007);
INSERT INTO `access_requirement` (`mapId`, `difficulty`, `level_min`, `level_max`, `item`, `item2`, `quest_done_A`, `quest_done_H`, `completed_achievement`, `quest_failed_text`, `comment`) VALUES
(961, 0, 85, 0, 0, 0, 0, 0, 0, NULL, 'Stormstout Brewery - Entrance'),
(961, 1, 90, 0, 0, 0, 0, 0, 0, NULL, 'Stormstout Brewery (H) - Entrance'),
(994, 0, 87, 0, 0, 0, 0, 0, 0, NULL, 'Mogushan Palace - Entrance'),
(994, 1, 90, 0, 0, 0, 0, 0, 0, NULL, 'Mogushan Palace (H) - Entrance'),
(959, 0, 87, 0, 0, 0, 30752, 30752, 0, NULL, 'Shado-Pan Monastery - Entrance'),
(959, 1, 90, 0, 0, 0, 0, 0, 0, NULL, 'Shado-Pan Monastery (H) - Entrance'),
(962, 0, 88, 0, 0, 0, 0, 0, 0, NULL, 'Gate of the Setting Sun - Entrance'),
(962, 1, 90, 0, 0, 0, 0, 0, 0, NULL, 'Gate of the Setting Sun (H) - Entrance'),
(1011, 0, 90, 0, 0, 0, 0, 0, 0, NULL, 'Siege of Niuzao Temple - Entrance'),
(1011, 1, 90, 0, 0, 0, 0, 0, 0, NULL, 'Siege of Niuzao Temple (H) - Entrance'),
(960, 0, 85, 0, 0, 0, 0, 0, 0, NULL, 'Temple of the Jade Serpent - Entrance'),
(960, 1, 90, 0, 0, 0, 0, 0, 0, NULL, 'Temple of the Jade Serpent (H) - Entrance'),
(1001, 0, 21, 0, 0, 0, 0, 0, 0, NULL, 'Scarlet Halls - Entrance'),
(1001, 1, 90, 0, 0, 0, 0, 0, 0, NULL, 'Scarlet Halls (H) - Entrance'),
(1004, 0, 23, 0, 0, 0, 0, 0, 0, NULL, 'Scarlet Monastery - Entrance'),
(1004, 1, 90, 0, 0, 0, 0, 0, 0, NULL, 'Scarlet Monastery (H) - Entrance'),
(1007, 0, 33, 0, 0, 0, 0, 0, 0, NULL, 'Scholomance - Entrance'),
(1007, 1, 90, 0, 0, 0, 0, 0, 0, NULL, 'Scholomance (H) - Entrance');