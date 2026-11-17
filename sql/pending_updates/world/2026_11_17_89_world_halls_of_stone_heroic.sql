-- Heroic fix for Halls of Stone. The Tribunal Chest of each mode was spawned in the wrong modes: the normal chest in both
-- and the heroic chest in normal only, so the instance script picked the heroic chest on normal and the normal chest on
-- heroic. The normal chest now exists on normal only and the heroic chest on heroic only.

UPDATE `gameobject` SET `spawnMask` = 2 WHERE `guid` = 65564 AND `id` = 190586 AND `map` = 599;
UPDATE `gameobject` SET `spawnMask` = 4 WHERE `guid` = 151790 AND `id` = 193996 AND `map` = 599;
