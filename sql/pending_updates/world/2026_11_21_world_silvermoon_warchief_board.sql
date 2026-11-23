-- Silvermoon City: the Warchief's Command board (object 207325) that starts the Horde zone quests for Ghostlands,
-- Hillsbrad Foothills, Arathi Highlands, The Hinterlands, Western and Eastern Plaguelands, Badlands, Searing Gorge, Burning
-- Steppes, Swamp of Sorrows, Stranglethorn Vale, Blasted Lands and Northrend was not spawned. Based on Legends of
-- Azeroth 5.4.8 and the Cataclysm Preservation Project 4.3.4, which place it at the same spot.
DELETE FROM `gameobject` WHERE `guid`=1068201;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(1068201, 207325, 530, 1, 0, 0, 9665.52, -7153.06, 14.2512, -0.802851, 0, 0, -0.390731, 0.920505, 300, 100, 1);
