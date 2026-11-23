-- Onyxia's Lair (map 249): spawn in 25-man normal as well as 10-man normal.
-- 4.3.4 and 3.3.5 spawn everything with spawnMask 3 (both raid sizes), which was converted to 8 (10N only), so the
-- 25-man lair was empty. MapDifficulty.dbc (5.4.8) has DifficultyID 3 (10 players) and 4 (25 players) for the
-- map, so the mask is 8 | 16 = 24. ref_loa548 also uses 24 (16 creatures, 248 gameobjects).
-- Each statement is guarded by map and spawnMask = 8, so the file is safe to run twice.
UPDATE `creature` SET `spawnMask` = 24 WHERE `map` = 249 AND `spawnMask` = 8;
UPDATE `gameobject` SET `spawnMask` = 24 WHERE `map` = 249 AND `spawnMask` = 8;
