-- Correct quest mob combat and movement data for issue #1221.

UPDATE `creature_template`
SET `npcflag` = 0
WHERE `entry` = 42938;

DELETE FROM `creature_queststarter`
WHERE `id` = 42938 AND `quest` = 26390;

UPDATE `creature`
SET `spawntimesecs` = 300,
    `spawndist` = 3,
    `curhealth` = 1210,
    `MovementType` = 1
WHERE `guid` = 191999 AND `id` = 300;

UPDATE `creature`
SET `position_x` = -9805.8,
    `position_y` = 143.672,
    `position_z` = 52.7457,
    `orientation` = 0.662339,
    `spawntimesecs` = 300,
    `spawndist` = 3,
    `curhealth` = 156,
    `MovementType` = 1
WHERE `guid` = 168119 AND `id` = 327;

UPDATE `creature`
SET `spawntimesecs` = 1,
    `curhealth` = 204
WHERE `guid` = 168411 AND `id` = 42938;

UPDATE `creature`
SET `position_x` = -12610.5,
    `position_y` = -255.319,
    `position_z` = 28.6795,
    `orientation` = 6.14197,
    `spawntimesecs` = 300,
    `spawndist` = 3,
    `curhealth` = 905,
    `curmana` = 100,
    `MovementType` = 1,
    `unit_flags` = 0,
    `dynamicflags` = 0
WHERE `guid` = 217907 AND `id` = 731;
