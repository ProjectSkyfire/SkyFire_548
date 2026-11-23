-- Classic 40-man raids: the creature and gameobject spawns on these maps have spawnMask 0, so nothing spawns.
-- MapDifficulty.dbc (5.4.8) gives all three maps only DifficultyID 9 (40-man, 40 players), and spawnMask is one bit
-- per DifficultyID, so the mask is 1 << 9 = 512. ref_loa548 also uses 512 on these maps.
-- Each statement is guarded by map and spawnMask = 0, so the file is safe to run twice.

-- Map 409 Molten Core
UPDATE `creature` SET `spawnMask` = 512 WHERE `map` = 409 AND `spawnMask` = 0;
UPDATE `gameobject` SET `spawnMask` = 512 WHERE `map` = 409 AND `spawnMask` = 0;

-- Map 469 Blackwing Lair
UPDATE `creature` SET `spawnMask` = 512 WHERE `map` = 469 AND `spawnMask` = 0;
UPDATE `gameobject` SET `spawnMask` = 512 WHERE `map` = 469 AND `spawnMask` = 0;

-- Map 531 Temple of Ahn'Qiraj
UPDATE `creature` SET `spawnMask` = 512 WHERE `map` = 531 AND `spawnMask` = 0;
UPDATE `gameobject` SET `spawnMask` = 512 WHERE `map` = 531 AND `spawnMask` = 0;
