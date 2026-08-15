-- Darkmoon Faire waypoint and text cleanup.

START TRANSACTION;

-- These imported island spawns reference waypoint path ids that were not
-- present in the staged batch. Keep the NPCs spawned, but stop loading
-- missing paths until valid Darkmoon Island movement paths are captured.
UPDATE `creature`
SET `MovementType` = 0,
    `spawndist` = 0,
    `currentwaypoint` = 0
WHERE `guid` IN (8200037, 8200041)
  AND `id` IN (14823, 14860)
  AND `map` = 974;

UPDATE `creature_addon`
SET `path_id` = 0
WHERE `guid` IN (8200037, 8200041)
  AND `path_id` IN (303619, 303637);

DELETE FROM `waypoint_data`
WHERE `id` IN (303619, 303637);

-- Flik's SmartAI has a second timed text event for TextGroup 1.
DELETE FROM `creature_text`
WHERE `entry` = 14860
  AND `groupid` = 1;

INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(14860, 1, 0, 'Frogs and leather balls for sale!', 12, 0, 100, 0, 0, 0, 'Flik');

COMMIT;
