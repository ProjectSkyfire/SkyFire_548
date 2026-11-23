-- Showdown at Stonewatch (26713): the two kills are Tharil'zun (486) and Gath'Ilzogg (334) at Stonewatch Keep. Neither
-- had a spawn, so the quest could not be finished. Spawns, positions and the two hour respawn are as in Legends of
-- Azeroth 5.4.8.

DELETE FROM `creature` WHERE `guid` IN (9310806, 9310807);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9310806, 334, 0, 1, 0, 0, 0, 0, -9382.84, -3082.02, 158.155, 1.8326, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(9310807, 486, 0, 1, 0, 0, 0, 0, -9442.19, -3074.29, 136.676, 0.690293, 7200, 0, 0, 0, 0, 0, 0, 0, 0);
