-- Throne of the Four Winds (map 754): the gameobjects and creatures were only spawned for 10 player normal (mask 8).
-- Spawn them in the 25 player and both heroic modes too (8 + 16 + 32 + 64 = 120). The stacked Hurricane (46419) and
-- Relentless Storm Initial Vehicle (47806) placeholders are left alone, the boss scripts summon those themselves.
-- Safe to run twice.
UPDATE `gameobject` SET `spawnMask` = 120 WHERE `map` = 754 AND `spawnMask` = 8;
UPDATE `creature` SET `spawnMask` = 120 WHERE `map` = 754 AND `spawnMask` = 8 AND `id` NOT IN (46419, 47806);
