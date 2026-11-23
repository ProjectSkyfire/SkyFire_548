-- Ruins of Gilneas: "The F.C.D." (27345) had no giver and no ender, and the camp Deathstalker Commander Belmont
-- (45474), who ends "A Man Named Godfrey" (27406) and "Resistance is Futile" (27423), was not spawned, so neither
-- quest could be turned in. Forward Commander Onslaught gives 27345 like its sister quest "Losing Ground" (27333): both
-- are offered by him, lead to 27349, and the text of 27345 is in his voice ("get back to me").
-- Based on Legends of Azeroth 5.4.8 (relations and the spawn next to Onslaught).
DELETE FROM `creature_queststarter` WHERE `id`=45315 AND `quest`=27345;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES (45315, 27345);
DELETE FROM `creature_questender` WHERE `id`=45315 AND `quest`=27345;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES (45315, 27345);

DELETE FROM `creature` WHERE `guid`=9311603;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9311603, 45474, 0, 1, 0, 0, 0, 0, -918.496, 1629.96, 68.3621, 0.0275731, 300, 0, 0, 0, 0, 0, 0, 0, 0);
