-- Fix GameObject missing spell errors
-- These gameobjects (GoType 22 = SPELL_CASTER) reference spells that don't exist in MoP DBC
-- Setting data0 to 0 removes the invalid spell reference
UPDATE `gameobject_template` SET `data0` = 0 WHERE `entry` IN (
181105, 181106, 181165, 183322, 183510, 183511, 191008, 193061, 193169, 193170, 193171,
204422, 205272, 205273, 205876, 205877, 206195, 207073, 207078, 207690, 207691, 208325, 209080, 209081
);

