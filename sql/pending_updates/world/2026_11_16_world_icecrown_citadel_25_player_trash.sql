-- Icecrown Citadel: spawn masks of the heroic-only and 25 player-only rows, and the 25 player trash that was missing
-- Ported from TrinityCore 3.3.5. TrinityCore's spawn bits 1, 2, 4 and 8 (10N, 25N, 10H, 25H) are moved to this core's bits 8, 16, 32 and 64.

-- The 10 and 25 player heroic Deathbringer's Caches carried the 10 player normal bit, so all three stood in the 10 player run
UPDATE `gameobject` SET `spawnMask` = 32 WHERE `guid` = 151167 AND `id` = 202238;
UPDATE `gameobject` SET `spawnMask` = 64 WHERE `guid` = 151168 AND `id` = 202241;

-- Wicked Spirit (23) and Nightmare Cloud (9) are heroic-only spawns (TrinityCore mask 12)
UPDATE `creature` SET `spawnMask` = 96 WHERE `id` = 39190 AND `map` = 631 AND `spawnMask` = 8 AND `guid` BETWEEN 88615 AND 88637;
UPDATE `creature` SET `spawnMask` = 96 WHERE `id` = 38421 AND `map` = 631 AND `spawnMask` = 8 AND `guid` BETWEEN 137808 AND 137816;

-- Rows that TrinityCore spawns in both normal sizes (mask 15) or in 25 player only (mask 10) but stood in 10 player only
UPDATE `creature` SET `spawnMask` = 24 WHERE `guid` IN (201080, 200966, 201066, 201107, 201146) AND `spawnMask` = 8;
UPDATE `creature` SET `spawnMask` = 16 WHERE `guid` = 201090 AND `spawnMask` = 8;

-- 25 player trash that was missing (TrinityCore guids in the comments); positions are TrinityCore's
DELETE FROM `creature` WHERE `guid` BETWEEN 8357000 AND 8357024;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8357000, 36724, 631, 16, 0, 0, 0, 0, -212.28, 2222.43, 35.2335, 6.20612, 86400, 0, 0, 378000, 99850, 0, 0, 0, 0), -- Servant of the Throne, TrinityCore guid 86248
(8357001, 36724, 631, 16, 0, 0, 0, 0, -213.399, 2199.63, 35.234, 6.1472, 86400, 0, 0, 378000, 99850, 0, 0, 0, 0), -- Servant of the Throne, TrinityCore guid 86249
(8357002, 36724, 631, 16, 0, 0, 0, 0, -240.447, 2207.22, 42.5645, 0.00512781, 86400, 0, 0, 378000, 99850, 0, 0, 0, 0), -- Servant of the Throne, TrinityCore guid 86250
(8357003, 37011, 631, 24, 0, 0, 0, 0, -118.939, 2191.25, 32.522, 5.48788, 86400, 0, 0, 478800, 0, 0, 0, 0, 0), -- The Damned, TrinityCore guid 86240
(8357004, 37011, 631, 24, 0, 0, 0, 0, -103.527, 2211.8, 31.0923, 6.22616, 86400, 0, 0, 478800, 0, 0, 0, 0, 0), -- The Damned, TrinityCore guid 86241
(8357005, 37011, 631, 24, 0, 0, 0, 0, -117.35, 2230.88, 32.3868, 0.894935, 86400, 0, 0, 478800, 0, 0, 0, 0, 0), -- The Damned, TrinityCore guid 86242
(8357006, 37011, 631, 24, 0, 0, 0, 0, -172.569, 2217.53, 35.2335, 6.24856, 86400, 0, 0, 478800, 0, 0, 0, 0, 0), -- The Damned, TrinityCore guid 86243
(8357007, 37011, 631, 16, 0, 0, 0, 0, -167.622, 2166.53, 37.9851, 2.00976, 86400, 0, 0, 478800, 0, 0, 0, 0, 0), -- The Damned, TrinityCore guid 86244
(8357008, 37011, 631, 16, 0, 0, 0, 0, -161.425, 2256.88, 37.9853, 4.01646, 86400, 0, 0, 478800, 0, 0, 0, 0, 0), -- The Damned, TrinityCore guid 86245
(8357009, 37012, 631, 16, 0, 0, 0, 1, -198.303, 2215.24, 35.2334, 0.0689005, 86400, 0, 0, 315000, 0, 0, 0, 0, 0), -- Ancient Skeletal Soldier, TrinityCore guid 86246
(8357010, 37012, 631, 16, 0, 0, 0, 1, -197.742, 2207.1, 35.2334, 0.0689005, 86400, 0, 0, 315000, 0, 0, 0, 0, 0), -- Ancient Skeletal Soldier, TrinityCore guid 86247
(8357011, 37012, 631, 16, 0, 0, 0, 1, -303.076, 2225.2, 41.9796, 6.12287, 86400, 0, 0, 315000, 0, 0, 0, 0, 0), -- Ancient Skeletal Soldier, TrinityCore guid 86251
(8357012, 37012, 631, 16, 0, 0, 0, 1, -302.821, 2198.51, 41.9806, 6.26345, 86400, 0, 0, 315000, 0, 0, 0, 0, 0), -- Ancient Skeletal Soldier, TrinityCore guid 86252
(8357013, 37595, 631, 16, 0, 0, 0, 1, 4517.7, 2761.09, 351.101, 0.973106, 86400, 0, 0, 431360, 91600, 0, 0, 0, 0), -- Darkfallen Blood Knight, TrinityCore guid 91573
(8357014, 37595, 631, 16, 0, 0, 0, 1, 4624.9, 2779.31, 361.163, 2.14492, 86400, 0, 0, 431360, 91600, 0, 0, 0, 0), -- Darkfallen Blood Knight, TrinityCore guid 91575
(8357015, 37595, 631, 16, 0, 0, 0, 1, 4615.84, 2742.08, 361.165, 0.90562, 86400, 0, 0, 431360, 91600, 0, 0, 0, 0), -- Darkfallen Blood Knight, TrinityCore guid 91577
(8357016, 37595, 631, 16, 0, 0, 0, 1, 4625.39, 2754.32, 361.165, 4.14067, 86400, 0, 0, 431360, 91600, 0, 0, 0, 0), -- Darkfallen Blood Knight, TrinityCore guid 91578
(8357017, 37664, 631, 16, 0, 0, 0, 0, 4517.5, 2777.68, 351.101, 5.28887, 86400, 0, 0, 431360, 91600, 0, 0, 0, 0), -- Darkfallen Archmage, TrinityCore guid 91574
(8357018, 37666, 631, 16, 0, 0, 0, 1, 4614.96, 2792.25, 361.163, 5.36904, 86400, 0, 0, 647040, 9160, 0, 0, 0, 0), -- Darkfallen Tactician, TrinityCore guid 91576
(8357019, 37666, 631, 16, 0, 0, 0, 1, 4573.82, 2865.49, 384.686, 4.73412, 86400, 0, 0, 647040, 9160, 0, 0, 0, 0), -- Darkfallen Tactician, TrinityCore guid 91582
(8357020, 37666, 631, 16, 0, 0, 0, 1, 4582.47, 2850.06, 384.686, 2.50044, 86400, 0, 0, 647040, 9160, 0, 0, 0, 0), -- Darkfallen Tactician, TrinityCore guid 91583
(8357021, 37666, 631, 16, 0, 0, 0, 1, 4581.6, 2688.63, 384.684, 3.81672, 86400, 0, 0, 647040, 9160, 0, 0, 0, 0), -- Darkfallen Tactician, TrinityCore guid 91584
(8357022, 37666, 631, 16, 0, 0, 0, 1, 4573.08, 2674.56, 384.684, 1.65374, 86400, 0, 0, 647040, 9160, 0, 0, 0, 0), -- Darkfallen Tactician, TrinityCore guid 91585
(8357023, 37666, 631, 16, 0, 0, 0, 1, 4531.93, 2769.49, 403.983, 3.15507, 86400, 0, 0, 647040, 9160, 0, 0, 0, 0), -- Darkfallen Tactician, TrinityCore guid 91586
(8357024, 37666, 631, 16, 0, 0, 0, 1, 4513, 2769.32, 403.983, 6.2581, 86400, 0, 0, 647040, 9160, 0, 0, 0, 0); -- Darkfallen Tactician, TrinityCore guid 91587
