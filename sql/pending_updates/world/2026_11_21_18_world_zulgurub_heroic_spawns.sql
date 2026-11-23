-- Zul'Gurub is a heroic only dungeon (finder entry 334), but its spawns were flagged for normal mode, so nothing stood
-- in it. Heroic is bit 4 of the spawn mask.
UPDATE `creature` SET `spawnMask` = 4 WHERE `map` = 859 AND `spawnMask` = 2;
UPDATE `gameobject` SET `spawnMask` = 4 WHERE `map` = 859 AND `spawnMask` = 2;
