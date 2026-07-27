-- Fix gameobject rows blocked by loader validation.

UPDATE `gameobject`
SET `spawnMask` = 0
WHERE `guid` IN (75155, 75156, 75157)
  AND `id` = 179784
  AND `map` = 469
  AND `spawnMask` = 1;

UPDATE `gameobject_template`
SET `displayId` = 9294
WHERE `entry` = 190191
  AND `type` = 8
  AND `name` = 'Frostmourne Altar'
  AND `displayId` = 1287;
