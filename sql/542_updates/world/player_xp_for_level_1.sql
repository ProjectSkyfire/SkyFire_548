-- player_xp_for_level fix officale
UPDATE `player_xp_for_level` SET `xp_for_next_level`=1686300 WHERE `lvl`=80;
UPDATE `player_xp_for_level` SET `xp_for_next_level`=2121500 WHERE `lvl`=81;
UPDATE `player_xp_for_level` SET `xp_for_next_level`=2669000 WHERE `lvl`=82;
UPDATE `player_xp_for_level` SET `xp_for_next_level`=3469000 WHERE `lvl`=83;
UPDATE `player_xp_for_level` SET `xp_for_next_level`=4583000 WHERE `lvl`=84;
-- Update player_xp_for_level 84-89
DELETE FROM player_xp_for_level WHERE lvl IN (85, 86, 87, 88, 89);
INSERT INTO player_xp_for_level (`lvl`, `xp_for_next_level`) VALUES 
('85', '8670000'),
('86', '10050000'),
('87', '12650000'),
('88', '15250000'),
('89', '18370000'); 
