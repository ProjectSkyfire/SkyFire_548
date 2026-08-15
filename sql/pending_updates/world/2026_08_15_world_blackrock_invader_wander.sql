-- Keep Blackrock Invaders near their spawn positions instead of wandering 15 yards.
UPDATE `creature`
SET `spawndist`=3, `MovementType`=1
WHERE `id`=42937;
