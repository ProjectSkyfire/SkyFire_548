DELETE FROM `areatrigger_teleport` WHERE `id` IN (610, 612);

DELETE FROM `game_tele` WHERE `id` IN (892, 893, 894, 895);
INSERT INTO `game_tele` (`id`, `position_x`, `position_y`, `position_z`, `orientation`, `map`, `name`) VALUES
(895, 820.743, 607.812, 13.6389, 0, 1001, 'SMLib'),
(894, 1124.64, 512.467, 0.989549, 1.5708, 1004, 'SMGY'),
(893, 1124.64, 512.467, 0.989549, 1.5708, 1004, 'SMCath'),
(892, 820.743, 607.812, 13.6389, 0, 1001, 'SMArmory');
