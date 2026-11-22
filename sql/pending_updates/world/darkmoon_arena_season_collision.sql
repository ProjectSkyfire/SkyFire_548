-- Event 75 was repurposed for Darkmoon Faire, but season 15 still started it
-- unconditionally at world startup. Give the arena season its own event.
-- Preserve all Darkmoon spawn links and the first-Sunday calendar schedule.
INSERT INTO `game_event` (`eventEntry`, `start_time`, `end_time`, `occurence`, `length`, `holiday`, `description`, `world_event`, `announce`)
SELECT 84, '2000-01-01 00:00:00', '2000-01-01 00:00:00', 5184000, 2592000, 0, 'Arena Season 15', 0, 2
WHERE NOT EXISTS (SELECT 1 FROM `game_event` WHERE `eventEntry` = 84);

-- Do not replace an unrelated event on databases with custom event IDs.
UPDATE `game_event_arena_seasons` AS seasons
JOIN `game_event` AS faire ON faire.`eventEntry` = seasons.`eventEntry`
JOIN `game_event` AS arena ON arena.`eventEntry` = 84
SET seasons.`eventEntry` = arena.`eventEntry`
WHERE seasons.`season` = 15 AND faire.`holiday` = 479
  AND arena.`description` = 'Arena Season 15' AND arena.`holiday` = 0;
