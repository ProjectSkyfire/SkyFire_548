-- Zul'Aman is a heroic only dungeon (finder entry 340), but its spawns were flagged for normal mode, so nothing
-- stood in it. Heroic is bit 4 of the spawn mask and of the SmartAI difficulty flags here.
UPDATE `creature` SET `spawnMask` = 4 WHERE `map` = 568 AND `spawnMask` = 2;
UPDATE `gameobject` SET `spawnMask` = 4 WHERE `map` = 568 AND `spawnMask` = 2;
UPDATE `smart_scripts` SET `event_flags` = (`event_flags` & ~2) | 4 WHERE `source_type` = 0 AND (`event_flags` & 2) AND `entryorguid` IN (SELECT DISTINCT `id` FROM `creature` WHERE `map` = 568);
