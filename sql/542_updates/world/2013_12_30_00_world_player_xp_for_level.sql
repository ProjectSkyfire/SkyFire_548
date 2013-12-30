-- Update player_xp_for_level 84-89
DELETE FROM player_xp_for_level WHERE lvl IN (85, 86, 87, 88, 89);
INSERT INTO player_xp_for_level (`lvl`, `xp_for_next_level`) VALUES 
('85', '8670000'),
('86', '10050000'),
('87', '12650000'),
('88', '15250000'),
('89', '18370000'); 
