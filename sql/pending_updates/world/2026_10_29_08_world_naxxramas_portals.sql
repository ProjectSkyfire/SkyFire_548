-- Naxxramas: the Arachnid, Construct and Military wing portals (usable once the wing's last boss is dead).
DELETE FROM `gameobject` WHERE `guid` IN (910702, 910703, 910704);
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(910702, 181575, 533, 24, 3465.18, -3940.4, 308.79, 2.44346, 0.241845, 0.664462, 0.664463, 0.241847, 7200, 255, 1),
(910703, 181576, 533, 24, 3539.02, -2936.82, 302.476, 3.14159, 0, 0, 1, 0.00000126759, 7200, 255, 1),
(910704, 181578, 533, 24, 2493.02, -2921.78, 241.193, 3.14159, 0, 0, 1, 0.00000126759, 7200, 255, 1);
