-- Azshara, the Bilgewater camp at Southern Rocketway (area 4830): seven NPCs were never spawned, among them the quest
-- givers and enders Foreman Fisk, Malynea Skyreaver, Custer Clubnik, Horzak Zignibble and Private Worcester. Spawns from
-- The Cataclysm Preservation Project 4.3.4, in the default phase like the rest of the zone here (4.3.4 uses phase 169,
-- which this server does not set for that area).
DELETE FROM `creature` WHERE `guid` BETWEEN 8311003 AND 8311009;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8311003, 35085, 1, 1, 0, 0, 0, 0, 2948.38, -4978.83, 126.105, 6.04932, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8311004, 35087, 1, 1, 0, 0, 0, 0, 2922.03, -5009.6, 125.952, 2.00713, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8311005, 35088, 1, 1, 0, 0, 0, 0, 2915.09, -5011.9, 125.745, 0.802851, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8311006, 35091, 1, 1, 0, 0, 0, 0, 2945.52, -4980.34, 126.031, 2.53073, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8311007, 35526, 1, 1, 0, 0, 0, 0, 2909.2, -5032.95, 126.99, 3.42085, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8311008, 36752, 1, 1, 0, 0, 0, 0, 2923.55, -5000.98, 126.595, 1.3439, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8311009, 43708, 1, 1, 0, 0, 0, 0, 2941.93, -4997.47, 166.16, 0, 300, 0, 0, 1, 0, 0, 0, 0, 0);
