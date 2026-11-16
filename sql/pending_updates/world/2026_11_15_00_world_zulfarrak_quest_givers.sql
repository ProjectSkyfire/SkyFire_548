-- Zul'Farrak: spawn Chief Engineer Bilgewhizzle and Tran'rek at the entrance next to Mazoga's Spirit.
-- Their quests ("A Fool's Errand", "Breaking and Entering") had a giver and an ender in the data but no spawn, so
-- neither quest could be taken.
DELETE FROM `creature` WHERE `guid` IN (9257500, 9257501);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9257500, 7407, 209, 2, 0, 0, 0, 0, 1229.56, 820.057, 8.9601, 1.0472, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(9257501, 44929, 209, 2, 0, 0, 0, 0, 1233.63, 818.908, 8.9601, 1.55334, 7200, 0, 0, 0, 0, 0, 0, 0, 0);
