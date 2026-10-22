-- Anzu (23035) has been a permanent Heroic Sethekk Halls boss since Patch 4.0.1:
-- "A druid is no longer required to summon Anzu. He is now a permanent boss in
-- Heroic Sethekk Halls." The world DB still only has the TBC summon path (The
-- Raven's Claw 185554, lock 1732 requiring the removed druid quest item 32449,
-- firing event 14797), so on 5.4.8 Anzu and Reins of the Raven Lord (32768)
-- are unreachable.
-- Spawn him heroic-only (spawnMask 4 = 1 << DIFFICULTY_HEROIC) at the exact point
-- the existing summon event 14797 places him. Auto GUID; skipped if a map-556
-- Anzu spawn already exists, so it is safe to re-apply.
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`,
    `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`,
    `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`)
SELECT es.`datalong`, 556, 4, 0, 0, 0, 0,
    es.`x`, es.`y`, es.`z`, es.`o`, 86400, 0,
    0, 0, 0, 0, 0, 0, 0
FROM `event_scripts` AS es
WHERE es.`id` = 14797 AND es.`command` = 10 AND es.`datalong` = 23035
    AND NOT EXISTS (SELECT 1 FROM `creature` AS c WHERE c.`id` = 23035 AND c.`map` = 556);
