-- Icecrown Citadel: the second pair of Deathspeaker Disciples and Attendants of the 25 player Lower Spire.
-- Ported from TrinityCore 3.3.5. TrinityCore and AzerothCore place them in the 25 player modes only, and
-- Legends of Azeroth 5.4.8 has all four at the same spots.

DELETE FROM `creature` WHERE `guid` BETWEEN 8350300 AND 8350303;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8350300, 36807, 631, 16, 0, 0, 0, 0, -582.7, 2221.8, 49.5, 3.32411, 86400, 0, 0, 0, 0, 0, 0, 0, 0),
(8350301, 36807, 631, 16, 0, 0, 0, 0, -583.9, 2201.0, 49.5, 3.00995, 86400, 0, 0, 0, 0, 0, 0, 0, 0),
(8350302, 36811, 631, 16, 0, 0, 0, 1, -568.1, 2225.8, 49.5, 3.12087, 86400, 0, 0, 0, 0, 0, 0, 0, 0),
(8350303, 36811, 631, 16, 0, 0, 0, 1, -568.9, 2197.5, 49.5, 2.93833, 86400, 0, 0, 0, 0, 0, 0, 0, 0);
