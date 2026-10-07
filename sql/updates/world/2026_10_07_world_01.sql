-- Add the spell focus required by Runeforging (53428) beside each map-0
-- Acherus runeforge. The visible type-5 objects cannot provide focus 1552.
-- Based on the missing-spawn fix in PR #1519 (BrewingCoder).
-- Use auto-generated GUIDs so existing spawns cannot be deleted by a collision.
INSERT INTO `gameobject` (`id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`,
    `position_x`, `position_y`, `position_z`, `orientation`,
    `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`)
SELECT 226777, visual.`map`, visual.`spawnMask`, visual.`phaseId`, visual.`phaseGroup`,
    visual.`position_x`, visual.`position_y`, visual.`position_z`, visual.`orientation`,
    0, 0, SIN(visual.`orientation` / 2), COS(visual.`orientation` / 2), 300, 0, 1
FROM `gameobject` AS visual
WHERE visual.`map` = 0
    AND ((visual.`guid` = 79603 AND visual.`id` = 207578)
        OR (visual.`guid` = 79607 AND visual.`id` = 207579)
        OR (visual.`guid` = 79609 AND visual.`id` = 207577))
    -- Also recognize the original PR's placements (rounded by up to 0.01 yd).
    AND NOT EXISTS (
        SELECT 1 FROM `gameobject` AS focus
        WHERE focus.`id` = 226777 AND focus.`map` = visual.`map`
            AND focus.`spawnMask` = visual.`spawnMask`
            AND focus.`phaseId` = visual.`phaseId` AND focus.`phaseGroup` = visual.`phaseGroup`
            AND ABS(focus.`position_x` - visual.`position_x`) < 0.1
            AND ABS(focus.`position_y` - visual.`position_y`) < 0.1
            AND ABS(focus.`position_z` - visual.`position_z`) < 0.1);
