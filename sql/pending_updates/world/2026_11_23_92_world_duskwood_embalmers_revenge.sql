-- The Embalmer's Revenge (26727): Stitches (43862) stands in front of the Darkshire town hall next to Abercrombie, the
-- Embalmer (43915), with four Night Watch Guards (43903) around them. Stitches was spawned, but Abercrombie and the guards had
-- no spawn and their templates were level 1 placeholders, so the scene was an empty street with a lone Stitches.
-- Spawns (guards and Abercrombie wander 3 yards) and template values (level, faction, civilian flag, run speed of the guards)
-- are as in Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `minlevel` = 22, `maxlevel` = 22, `faction_A` = 974, `faction_H` = 974, `flags_extra` = 2 WHERE `entry` = 43915 AND `ScriptName` = '';
UPDATE `creature_template` SET `minlevel` = 25, `maxlevel` = 25, `faction_A` = 56, `faction_H` = 56, `speed_run` = 1.42857 WHERE `entry` = 43903 AND `ScriptName` = '';

DELETE FROM `creature` WHERE `guid` BETWEEN 9310810 AND 9310814;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9310810, 43915, 0, 1, 0, 0, 0, 0, -10554.7, -1169.61, 32.9705, 1.76278, 300, 3, 0, 0, 0, 1, 0, 0, 0),
(9310811, 43903, 0, 1, 0, 0, 0, 0, -10547.9, -1162.4, 28.044, 4.38078, 300, 3, 0, 0, 0, 1, 0, 0, 0),
(9310812, 43903, 0, 1, 0, 0, 0, 0, -10559.1, -1179.45, 28.0235, 1.11701, 300, 3, 0, 0, 0, 1, 0, 0, 0),
(9310813, 43903, 0, 1, 0, 0, 0, 0, -10557.1, -1163.12, 27.9425, 5.46288, 300, 3, 0, 0, 0, 1, 0, 0, 0),
(9310814, 43903, 0, 1, 0, 0, 0, 0, -10545.5, -1177.14, 28.192, 2.05949, 300, 3, 0, 0, 0, 1, 0, 0, 0);
