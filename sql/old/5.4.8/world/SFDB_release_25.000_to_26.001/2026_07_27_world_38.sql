-- Fix spellcaster gameobjects that reference spell IDs not present in the 5.4.8 Spell.dbc.

UPDATE `gameobject_template`
SET `data0` = 81349
WHERE `entry` = 204422
  AND `type` = 22
  AND `data0` = 81358;

UPDATE `gameobject_template`
SET `data0` = 86855
WHERE `entry` = 206195
  AND `type` = 22
  AND `data0` = 86761;

UPDATE `gameobject_template`
SET `data0` = 100268
WHERE `entry` IN (207690, 207691)
  AND `type` = 22
  AND `data0` IN (90244, 90245);

-- These templates have no valid replacement spell in client data. Reclassify them
-- as generic objects so they no longer load as broken spellcasters.
UPDATE `gameobject_template`
SET
    `type` = 5,
    `data0` = 1,
    `data1` = 1,
    `data2` = 0,
    `data3` = 0,
    `data4` = 0,
    `data5` = 0
WHERE `entry` IN (205876, 205877, 207073, 207078, 208325)
  AND `type` = 22
  AND `data0` IN (85891, 85892, 89685, 89691, 96364);
