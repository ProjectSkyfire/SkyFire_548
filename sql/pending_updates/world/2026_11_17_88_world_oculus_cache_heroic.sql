-- Heroic fix for The Oculus. The Cache of Eregos of each mode was spawned in the wrong modes: the normal cache in both and
-- the heroic cache in normal only, so the instance script picked the heroic cache on normal and the normal cache on
-- heroic. The normal cache now exists on normal only and the heroic cache on heroic only.

UPDATE `gameobject` SET `spawnMask` = 2 WHERE `guid` = 2853 AND `id` = 191349 AND `map` = 578;
UPDATE `gameobject` SET `spawnMask` = 4 WHERE `guid` = 150 AND `id` = 193603 AND `map` = 578;
