-- The Ironwall Rampart (13312, 13337): Grimkor the Wicked, the quest's kill target, had no spawn.
DELETE FROM `creature` WHERE `guid` = 8326954;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8326954, 32162, 571, 1, 0, 0, 0, 0, 7505.78, 2600.54, 534.755, 3.45575, 60, 0, 0, 0, 0, 0, 0, 0, 0);
