-- Added Temporal Anomaly (145389) to the Timeless Isle :)
DELETE FROM spell_area WHERE spell=145389;
INSERT INTO spell_area (spell, area) VALUES
(145389, 6757);
