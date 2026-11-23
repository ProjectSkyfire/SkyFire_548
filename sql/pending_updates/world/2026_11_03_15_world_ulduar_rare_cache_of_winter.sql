-- Ulduar, Hodir: the Rare Cache of Winter had no spawn, so there was no reward for the hard mode. It stands hidden
-- (not selectable) until the boss script opens it after a fast kill. Positions and respawn time as in TrinityCore 3.3.5.
-- The 25-player chest's template lacked the not-selectable flag the 10-player one has.
UPDATE `gameobject_template` SET `flags` = 16 WHERE `entry` = 194201;
DELETE FROM `gameobject` WHERE `guid` IN (910804, 910805);
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(910804, 194200, 603, 8, 0, 0, 2036.17, -201.843, 432.687, 3.14159, 0, 0, -1, 0, 604800, 255, 1),
(910805, 194201, 603, 16, 0, 0, 2036.17, -201.843, 432.687, 3.14159, 0, 0, -1, 0, 604800, 255, 1);
