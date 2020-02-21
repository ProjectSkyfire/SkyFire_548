-- Call to Arms:
DELETE FROM `game_event` WHERE `eventEntry` IN (65,66,70,71,76);
INSERT INTO `game_event` (`eventEntry`, `start_time`, `end_time`, `occurence`, `length`, `holiday`, `description`, `world_event`, `announce`) VALUES
-- Cataclysm
(65, '2019-05-14 15:00:00', '2020-12-31 17:00:00', 60480, 6240, 435, 'Call to Arms: The Battle for Gilneas!', 0, 2),
(66, '2019-05-21 15:00:00', '2020-12-31 17:00:00', 60480, 6240, 436, 'Call to Arms: Twin Peaks!', 0, 2),
-- Pandaria
(70, '2019-05-28 15:00:00', '2020-12-31 17:00:00', 60480, 6240, 488, 'Call to Arms: Silvershard Mines!', 0, 2),
(71, '2019-06-04 15:00:00', '2020-12-31 17:00:00', 60480, 6240, 489, 'Call to Arms: Temple of Kotmogu!', 0, 2),
(76, '2019-06-11 15:00:00', '2020-12-31 17:00:00', 60480, 6240, 515, 'Call to Arms: Deepwind Gorge!', 0, 2);

-- Arena Seasons
DELETE FROM `game_event` WHERE `eventEntry` IN (67,68,69,72,73,74,75);
INSERT INTO `game_event` (`eventEntry`, `description`) VALUES
-- Cataclysm
(67, 'Arena Season 9'),
(68, 'Arena Season 10'),
(69, 'Arena Season 11'),
-- Pandaria
(72, 'Arena Season 12'),
(73, 'Arena Season 13'),
(74, 'Arena Season 14'),
(75, 'Arena Season 15');

DELETE FROM `game_event_arena_seasons` WHERE `eventEntry` IN (67,68,69,72,73,74,75);
INSERT INTO `game_event_arena_seasons` (`eventEntry`, `season`) VALUES
-- Cataclysm
(67, 9),
(68, 10),
(69, 11),
-- Pandaria
(72, 12),
(73, 13),
(74, 14),
(75, 15);
