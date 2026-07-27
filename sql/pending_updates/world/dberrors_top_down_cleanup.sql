-- Top-down DBErrors cleanup for loader-verified data issues.

-- Linked trap targets exist but were left as placeholder generic objects.
UPDATE `gameobject_template`
SET `type` = 6
WHERE `entry` IN (4, 129, 12653, 176592, 176750, 179530, 179531, 181831, 186471, 202463)
  AND `type` = 0;

-- Creature templates with missing Horde faction mirror their valid Alliance faction.
UPDATE `creature_template`
SET `faction_H` = `faction_A`
WHERE `entry` IN (
    69154, 71864, 71908, 71919, 72033, 72045, 72048,
    72049, 72193, 72245, 72970, 73160, 73161, 73163,
    73166, 73173, 73279, 73281, 73282, 73703, 73704
)
  AND `faction_H` = 0
  AND `faction_A` <> 0;

-- The loader already clamps ModLevel creatures below level 90; persist that correction.
UPDATE `creature_template`
SET `minlevel` = 90
WHERE `entry` IN (15466, 25499, 56160)
  AND `ModLevel` = 1
  AND `minlevel` < 90;

-- Remove a dynamically assigned flag that is not allowed to be stored in creature_template.
UPDATE `creature_template`
SET `flags_extra` = `flags_extra` - 2097152
WHERE `entry` IN (32666, 53714, 57873)
  AND (`flags_extra` & 2097152) <> 0;
