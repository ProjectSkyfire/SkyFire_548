-- Heroic fix for Hellfire Ramparts. The Reinforced Fel Iron Chest of each mode was spawned in the wrong modes: the normal
-- chest in both and the heroic chest in normal only, so the instance script picked the heroic chest on normal and the
-- normal chest on heroic. The normal chest now exists on normal only and the heroic chest on heroic only.

UPDATE `gameobject` SET `spawnMask` = 2 WHERE `guid` = 20496 AND `id` = 185168 AND `map` = 543;
UPDATE `gameobject` SET `spawnMask` = 4 WHERE `guid` = 20499 AND `id` = 185169 AND `map` = 543;
