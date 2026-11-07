-- Ahn'Qiraj: The Fallen Kingdom: Windcaller Yessendra, Warden Haro and Keyl Swiftclaw start and end the 27 reward quests
-- of the Ruins of Ahn'Qiraj (rings, weapons and cloaks), but none of them was spawned at the gates.
DELETE FROM `creature` WHERE `guid` BETWEEN 8311800 AND 8311802;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8311800, 15498, 1, 1, 0, 0, 15477, 0, -8412.72, 1486.41, 26.4685, 0.122173, 300, 0, 0, 1830, 5013, 0, 0, 0, 0),
(8311801, 15499, 1, 1, 0, 0, 15478, 0, -8412.53, 1483.2, 26.0726, 0.261799, 300, 0, 0, 2784, 0, 0, 0, 0, 0),
(8311802, 15500, 1, 1, 0, 0, 15479, 0, -8411.23, 1480.6, 25.6424, 0.907571, 300, 0, 0, 24416, 0, 0, 0, 0, 0);
