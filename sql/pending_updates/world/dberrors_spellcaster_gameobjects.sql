-- Align spellcaster gameobjects that reference spell IDs no longer present in Spell.dbc.

UPDATE `gameobject_template`
SET `data0` = 7001
WHERE `entry` IN (181105, 181106, 181165)
  AND `type` = 22
  AND `name` = 'Lightwell'
  AND `data0` IN (27873, 27874, 28276);

UPDATE `gameobject_template`
SET `data0` = 34130,
    `ScriptName` = 'go_soulwell'
WHERE `entry` IN (183510, 183511, 193169, 193170, 193171)
  AND `type` = 22
  AND `name` = 'Soulwell'
  AND `data0` IN (34149, 34150, 58890, 58896, 58898);

UPDATE `gameobject_template`
SET `data0` = 43988
WHERE `entry` = 193061
  AND `type` = 22
  AND `name` = 'Refreshment Table'
  AND `data0` = 58660;

UPDATE `gameobject_template`
SET `data0` = 44089
WHERE `entry` IN (183322, 191008)
  AND `type` = 22
  AND `name` LIKE '%Portal to Ironforge'
  AND `data0` = 17607;

UPDATE `gameobject_template`
SET `data0` = 17334
WHERE `entry` IN (205272, 209080)
  AND `type` = 22
  AND `name` = 'Portal to Stormwind'
  AND `data0` = 84505;

UPDATE `gameobject_template`
SET `data0` = 17609
WHERE `entry` IN (205273, 209081)
  AND `type` = 22
  AND `name` = 'Portal to Orgrimmar'
  AND `data0` = 84506;
