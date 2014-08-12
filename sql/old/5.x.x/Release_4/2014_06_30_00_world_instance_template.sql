-- New 5 Man Instance Template
DELETE FROM instance_template WHERE map IN (961, 994, 959, 962, 1011, 960);
INSERT INTO instance_template (map, parent) VALUES
(961, 870), -- Stormstout Brewery
(994, 870), -- Mogushan Palace
(959, 870), -- Shado-Pan Monastery
(962, 870), -- Gate of the Setting Sun
(1011, 870), -- Siege of Niuzao Temple
(960, 870); -- Temple of the Jade Serpent