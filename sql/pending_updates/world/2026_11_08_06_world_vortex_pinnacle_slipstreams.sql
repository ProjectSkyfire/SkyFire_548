-- Based on Legends of Azeroth 5.4.8.
-- The two entrance shortcuts use the same entry as the platform currents.
UPDATE `creature_template` SET `ScriptName` = 'npc_vortex_slipstream', `AIName` = '' WHERE `entry` = 45455;

DELETE FROM `creature` WHERE `guid` IN (9220127, 9220128) AND `id` = 45455 AND `map` = 657;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9220127, 45455, 657, 6, 0, 0, 0, 0, -313.26, -28.47, 626.23, 2.84, 300, 0, 0, 64496, 0, 0, 0, 0, 0),
(9220128, 45455, 657, 6, 0, 0, 0, 0, -381.66, 38.43, 625.29, 5.02, 300, 0, 0, 64496, 0, 0, 0, 0, 0);
