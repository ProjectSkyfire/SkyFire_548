-- Burning Steppes: John J. Keeshan (48346) starts and ends the Alliance quests "Blood Tour" (28314), "Draconic Vanguard"
-- (28315) and "A Deal With a Dragon" (28316), and Ariok (48567) starts and ends the Horde ones (28446 to 28448), but neither
-- was spawned, so those three quests of each faction could not be taken or turned in. Positions are as in Legends of
-- Azeroth 5.4.8, where the second Ariok stands next to the first one (48565).

DELETE FROM `creature` WHERE `guid` IN (9310287, 9310288);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9310287, 48346, 0, 1, 0, 0, 0, 0, -8395.88, -2791.99, 194.796, 0.729943, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(9310288, 48567, 0, 1, 0, 0, 0, 0, -7501.35, -2194.38, 167.572, 1.95169, 300, 0, 0, 0, 0, 0, 0, 0, 0);
