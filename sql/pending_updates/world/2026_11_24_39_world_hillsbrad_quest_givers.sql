-- Hillsbrad Foothills, the Southshore quest chain: two quest givers and enders were not spawned.
-- Spawns (as in Legends of Azeroth 5.4.8, new guids 9311200 and 9311201): Master Apothecary Lydon (48020), who gives and
-- ends Protocol (28230), Burnside Must Fall (28235) and A Blight Upon the Land (28237), and Johnny Awesome (48021), who ends
-- Do it for Twinkles (28231); both were missing so these quests could not be handed in.

DELETE FROM `creature` WHERE `guid` IN (9311200, 9311201);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9311200, 48020, 0, 1, 0, 0, 0, 0, -440.834, -99.0001, 54.0614, 1.47672, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(9311201, 48021, 0, 1, 0, 0, 0, 0, -336.245, 3.98194, 53.2928, 3.86003, 120, 0, 0, 0, 0, 0, 0, 0, 0);
