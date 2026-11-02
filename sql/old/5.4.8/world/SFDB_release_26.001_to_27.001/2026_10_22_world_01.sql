-- Creatures with MovementType 2 (waypoint) but no path_id spam
-- "WaypointMovementGenerator::LoadPath: ... doesn't have waypoint path id: 0".

-- Waypoints already exist in waypoint_data as guid*10; link them.
DELETE FROM `creature_addon` WHERE `guid` IN (75, 1901, 1905, 1912, 1920, 1934, 1942, 1967, 203183);
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(75, 750, 0, 0, 0, 0, NULL),         -- Orb Rotation Focus
(1901, 19010, 0, 0, 0, 0, NULL),     -- Serpent-Touched Berserker
(1905, 19050, 0, 0, 0, 0, NULL),     -- Serpent-Touched Berserker
(1912, 19120, 0, 0, 0, 0, NULL),     -- Serpent-Touched Berserker
(1920, 19200, 0, 0, 0, 0, NULL),     -- Serpent-Touched Berserker
(1934, 19340, 0, 0, 0, 0, NULL),     -- Serpent-Touched Berserker
(1942, 19420, 0, 0, 0, 0, NULL),     -- Serpent-Touched Berserker
(1967, 19670, 0, 0, 0, 0, NULL),     -- Serpent-Touched Berserker
(203183, 2031830, 0, 0, 0, 0, NULL); -- Howling Cyclone

-- No waypoint data anywhere: wander where a spawn radius is set, otherwise stand.
UPDATE `creature` SET `MovementType` = 1 WHERE `guid` IN (113, 114);
UPDATE `creature` SET `MovementType` = 0 WHERE `guid` IN (3560, 3581, 3584, 3589, 3591, 3592, 203187);
