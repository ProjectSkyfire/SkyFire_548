-- achievement_criteria_data rules of the 5.4.8 world DB (alexkulya/pandaria_5.4.8 release 2026_08_16) for
-- known titles (type 23), game events (type 24) and holidays (type 16). Rerunnable: rows are deleted by key and rewritten.

DELETE FROM `achievement_criteria_data` WHERE (`criteria_id`,`type`) IN ((5597,23),(5599,23),(5600,23),(6789,23),(7408,23),(7412,23),(9058,24),(9719,23),(9720,23),(10881,23),(10898,23),(11238,23),(11402,23),(12562,24),(12912,23),(16082,24),(17804,16),(18500,16));
INSERT INTO `achievement_criteria_data` (`criteria_id`,`type`,`value1`,`value2`,`ScriptName`) VALUES
(5597,23,62,0,''),
(5599,23,71,0,''),
(5600,23,80,0,''),
(6789,23,53,0,''),
(7408,23,45,0,''),
(7412,23,42,0,''),
(9058,24,65,0,''),
(9719,23,44,0,''),
(9720,23,43,0,''),
(10881,23,157,0,''),
(10898,23,167,0,''),
(11238,23,167,0,''),
(11402,23,169,0,''),
(12562,24,66,0,''),
(12912,23,177,0,''),
(16082,24,67,0,''),
(17804,16,467,0,''),
(18500,16,484,0,'');
