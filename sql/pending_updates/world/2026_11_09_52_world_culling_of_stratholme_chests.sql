-- The Culling of Stratholme: spawn each Dark Runed Chest in its own difficulty.
-- The normal chest was spawned in both difficulties and the heroic chest only in normal, so heroic
-- runs offered the normal loot and normal runs had both chests on top of each other.
UPDATE `gameobject` SET `spawnMask`=2 WHERE `guid`=67603 AND `id`=190663;
UPDATE `gameobject` SET `spawnMask`=4 WHERE `guid`=150376 AND `id`=193597;
