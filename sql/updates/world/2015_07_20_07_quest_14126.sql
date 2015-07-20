DELETE FROM `spell_linked_spell` WHERE spell_trigger=74100;
INSERT INTO `spell_linked_spell` VALUES (91847, 74100, 0, 'Life Savings: Teleport & Bind to the Lost Isles');
DELETE FROM `spell_target_position` WHERE id=74100;
INSERT INTO spell_target_position VALUES (74100, 0, 648, 534.5, 3273.4, 0.23, 4.79);
INSERT INTO spell_target_position VALUES (74100, 1, 648, 534.5, 3273.4, 0.23, 4.79);


DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+0;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(@CGUID+0, 35222, 648, 1, 1, -7854.503, 1847.41, 7.218571, 5.427974, 120, 0, 0);