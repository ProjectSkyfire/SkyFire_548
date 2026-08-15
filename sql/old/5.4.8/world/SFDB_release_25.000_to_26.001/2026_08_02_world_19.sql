-- Darkmoon Faire Burth movement cleanup.
-- The imported event spawn keeps Burth present, but disables the mismatched
-- waypoint path that can send an invalid movement update while the faire streams in.

START TRANSACTION;

UPDATE `creature`
SET `MovementType` = 0,
    `spawndist` = 0,
    `currentwaypoint` = 0
WHERE `guid` = 8200038
  AND `id` = 14827
  AND `map` = 974;

UPDATE `creature_addon`
SET `path_id` = 0
WHERE `guid` = 8200038
  AND `path_id` = 303620;

DELETE FROM `waypoint_data` WHERE `id` = 303620;

COMMIT;
