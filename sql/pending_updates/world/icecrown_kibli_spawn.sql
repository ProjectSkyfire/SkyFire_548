-- Icecrown: Kibli Killohertz (Alliance) stands at the Valley of Lost Hope camp in 3.3.5, AzerothCore and 5.4.8 and
-- had no spawn here. Position from 5.4.8.
DELETE FROM `creature` WHERE `guid` BETWEEN 8320021 AND 8320021;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8320021, 32444, 571, 1, 0, 0, 0, 0, 7629.04, 2060.11, 600.26, 3.55533, 300, 0, 0, 0, 0, 0, 0, 0, 0);
