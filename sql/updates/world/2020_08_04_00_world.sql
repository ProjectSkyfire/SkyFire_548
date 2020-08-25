SET @CGUID := 100053;

UPDATE `creature_template` SET `trainer_type`=2, `npcflag`=1|2|16 WHERE `entry` IN (63596, 63626);

DELETE FROM `creature` WHERE `id` IN (63596, 63626);
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID-1 AND @CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID-0, 63596, 0, 1, 0, 0, 0, 0, -8287.43, 515.474, 120.471, 1.47402, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-1, 63626, 1, 1, 0, 0, 0, 0, 1800.2, -4418.77, 102.744, 1.54004, 300, 0, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `npc_trainer` WHERE `entry` IN (63596, 63626);
INSERT INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`) VALUES
(63596, 119467, 1000000, 0, 0, 5),
(63626, 119467, 1000000, 0, 0, 5);

DELETE FROM `spell_learn_spell` WHERE `entry`=119467;
INSERT INTO `spell_learn_spell` (`entry`, `SpellID`) VALUES
(119467, 122026);
