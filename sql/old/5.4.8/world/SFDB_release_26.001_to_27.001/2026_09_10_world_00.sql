-- Darkmoon Faire runs from 00:00 on the first Sunday of each month until
-- 00:00 on the following Sunday. The core uses holiday 479 for the
-- calendar-aware monthly recurrence; occurence remains a safe four-week
-- fallback for older binaries.
UPDATE `game_event`
SET `start_time` = '2026-09-06 00:00:00',
    `end_time` = '2037-12-31 23:59:59',
    `occurence` = 40320,
    `length` = 10080
WHERE `eventEntry` = 75
  AND `holiday` = 479;
