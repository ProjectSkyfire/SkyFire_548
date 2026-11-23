-- Isle of Quel'Danas: Kaalif, the Sun's Reach reagent vendor (17 items already in npc_vendor), was not spawned.
-- Based on Legends of Azeroth 5.4.8; the Cataclysm Preservation Project 4.3.4 has the same spot.
DELETE FROM `creature` WHERE `guid`=9311602;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9311602, 25039, 530, 1, 0, 0, 0, 1, 12846.2, -7005.53, 18.5938, 3.40418, 600, 0, 0, 0, 0, 0, 0, 0, 0);
