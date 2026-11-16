-- Sholazar Basin: quest givers and enders that were listed for their quests but had no spawn here.
-- 28106 Shaman Jakjek (ends 12703, 12759, 12760), 28214 Goregek the Gorilla Hunter (gives 12530) and Pilot Vic,
-- who ends 13052 "Aerial Surveillance" (5.4.8).
DELETE FROM `creature` WHERE `guid` IN (8320003,8320005);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8320003, 28106, 571, 1, 0, 0, 0, 0, 4891.31, 5905.05, -40.6086, 3.66024, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8320005, 28214, 571, 1, 0, 0, 0, 0, 5231.3, 4495.74, -83.2173, 0.352819, 120, 0, 0, 0, 0, 0, 0, 0, 0);

-- 13052 "Aerial Surveillance": Pilot Vic ends it (5.4.8).
DELETE FROM `creature_questender` WHERE (`id`, `quest`) IN ((28746, 13052));
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(28746, 13052);
