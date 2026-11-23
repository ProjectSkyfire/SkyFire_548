-- Razorfen Downs: spawn the rare Ragglesnout and Lady Falther'ess (in 5.4.8 they are in the dungeon, here they
-- had templates but no spawns) and give Ragglesnout his 5.4.8 level.
-- Idempotent: the spawns are deleted by guid before they are inserted.
UPDATE `creature_template` SET `minlevel`=45, `maxlevel`=45 WHERE `entry`=7354;

DELETE FROM `creature` WHERE `guid` IN (9250001, 9250002);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9250001, 7354, 129, 2, 0, 0, 0, 0, 2364.75, 904.524, 28.7671, 0.304753, 43200, 5, 0, 19410, 3801, 1, 0, 0, 0),
(9250002, 14686, 129, 2, 0, 0, 0, 0, 2554.84, 1002.23, 52.181, 0.768614, 7200, 0, 0, 500, 10506, 0, 0, 0, 0);
