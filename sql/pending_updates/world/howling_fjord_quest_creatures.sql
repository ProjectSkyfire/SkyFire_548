-- Howling Fjord: quest targets that had no spawn here.
-- 24029 Wyrmcaller Vile (quests 11238, 11267), 24277 Garwal (11324, 11326), 24820 Iron Dwarf Relic (11489).
DELETE FROM `creature` WHERE `guid` BETWEEN 8320000 AND 8320002;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8320000, 24029, 571, 1, 0, 0, 0, 0, 2823.65, -3603.02, 245.679, 3.55425, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8320001, 24277, 571, 1, 0, 0, 0, 0, 2764.15, -3097.29, 110.051, 3.31568, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8320002, 24820, 571, 1, 0, 0, 0, 0, 466.789, -5921.3, 309.443, 3.49066, 60, 0, 0, 0, 0, 0, 0, 0, 0);
