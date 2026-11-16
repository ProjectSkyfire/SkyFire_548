-- Blade's Edge Mountains: the Image of Commander Ameer at Bash'ir Landing, who gives and takes the Stasis
-- Chambers of Bash'ir quests, same spot in both 3.3.5 databases and the 5.4.8 data.

-- Image of Commander Ameer (Bash'ir quests), same spot in both 3.3.5 databases and the 5.4.8 data
DELETE FROM `creature` WHERE `guid` IN (8317007);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8317007, 22919, 530, 1, 0, 0, 0, 0, 3866.55000, 5978.68000, 291.79200, 4.100610, 300, 0, 0, 0, 0, 0, 0, 0, 0);
