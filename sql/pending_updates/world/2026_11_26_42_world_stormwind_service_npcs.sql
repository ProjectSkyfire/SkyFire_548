-- Stormwind City: two NPCs with templates, gossip and a vendor list but no spawn.
--
-- Nayura, the Dockmaster (28650), stands at the Stormwind Harbor docks, at the same spot in Legends of Azeroth 5.4.8
-- and in the Cataclysm Preservation Project 4.3.4.
-- Lil Timmy (8666), the boy with kittens, sells the item in his vendor list. Both sources have him; the position is the
-- 5.4.8 one (he walks a path there, kept stationary here).

DELETE FROM `creature` WHERE `guid` IN (9311800, 9311801);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9311800, 28650, 0, 1, 0, 0, 0, 0, -8641.18, 1331.08, 5.31558, 4.95674, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(9311801, 8666, 0, 1, 0, 0, 0, 0, -8514.46, 659.027, 101.393, 2.19626, 300, 0, 0, 0, 0, 0, 0, 0, 0);
