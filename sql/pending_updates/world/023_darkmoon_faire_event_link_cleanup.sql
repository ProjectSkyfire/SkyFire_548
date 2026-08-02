-- Darkmoon Faire event 4 link cleanup.

SET @DARKMOON_EVENT_ENTRY := 75;
SET @OLD_DARKMOON_EVENT_ENTRY := 4;

INSERT IGNORE INTO `game_event_gameobject` (`eventEntry`, `guid`)
SELECT @DARKMOON_EVENT_ENTRY, `guid`
FROM `game_event_gameobject`
WHERE `eventEntry` = @OLD_DARKMOON_EVENT_ENTRY;

DELETE FROM `game_event_gameobject`
WHERE `eventEntry` = @OLD_DARKMOON_EVENT_ENTRY;

INSERT IGNORE INTO `game_event_creature` (`eventEntry`, `guid`)
SELECT @DARKMOON_EVENT_ENTRY, `guid`
FROM `game_event_creature`
WHERE `eventEntry` = @OLD_DARKMOON_EVENT_ENTRY;

DELETE FROM `game_event_creature`
WHERE `eventEntry` = @OLD_DARKMOON_EVENT_ENTRY;
