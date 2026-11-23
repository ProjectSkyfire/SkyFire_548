-- Serpentshrine Cavern: the Lady Vashj bridge console (184568), the three bridge parts (184203 to
-- 184205) and the two steam doodads (184569, 184570) were not spawned, although the instance script and
-- go_bridge_console exist and wait for them. Rows from the TrinityCore 3.3.5 sniff (build 15595), which
-- AzerothCore confirms for the console and the bridge. Raid spawn mask 16 like the other objects of the map.
DELETE FROM `gameobject` WHERE `guid` BETWEEN 1062220 AND 1062225;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(1062220, 184203, 548, 16, 0, 0, 26.32850, -654.49500, 15.47420, 3.141590, 0, 0, -1, 0, 7200, 255, 1),
(1062221, 184204, 548, 16, 0, 0, 26.32850, -654.49500, 15.47420, 3.141590, 0, 0, -1, 0, 7200, 255, 1),
(1062222, 184205, 548, 16, 0, 0, 26.32850, -654.49500, 15.47420, 3.141590, 0, 0, -1, 0, 7200, 255, 1),
(1062223, 184568, 548, 16, 0, 0, 47.75670, -581.82900, 4.65559, 3.141590, 0, 0, -1, 0, 7200, 255, 1),
(1062224, 184569, 548, 16, 0, 0, -57.28420, -370.51400, 29.93850, 0.000000, -1, 0, 0, 0, 7200, 255, 1),
(1062225, 184570, 548, 16, 0, 0, 42.01680, -304.07300, 29.93850, 0.000000, -1, 0, 0, 0, 7200, 255, 1);
