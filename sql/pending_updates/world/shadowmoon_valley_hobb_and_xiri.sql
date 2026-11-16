-- Shadowmoon Valley: Commander Hobb and Xi'ri were not spawned, same spots in both 3.3.5 databases.

-- Commander Hobb, Xi'ri and the Spirit of Ar'tor, same spots in both 3.3.5 databases
DELETE FROM `creature` WHERE `guid` IN (8317008,8317009);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8317008, 23434, 530, 1, 0, 0, 0, 0, -4098.37000, 1118.43000, 42.75070, 5.358160, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8317009, 18528, 530, 1, 0, 0, 0, 0, -3559.68000, 637.63300, 4.14156, 1.601020, 300, 0, 0, 0, 0, 0, 0, 0, 0);
