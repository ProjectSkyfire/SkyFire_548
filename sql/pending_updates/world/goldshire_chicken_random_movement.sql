-- Allow the Goldshire chickens to wander near their spawn positions.
-- Spawn-level idle movement overrides the Chicken template's random movement.
UPDATE `creature`
SET `MovementType` = 1, `spawndist` = 5
WHERE `id` = 620 AND `map` = 0
  AND `guid` IN (167568, 167574, 167594, 167596, 168149, 168150)
  AND `MovementType` = 0 AND `spawndist` = 0;
