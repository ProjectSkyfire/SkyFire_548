DELETE FROM `spell_script_names` WHERE `spell_id` IN (103069, 103070, 103077);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(103069, 'spell_gen_rock_jump_a'),
(103070, 'spell_gen_rock_jump_b'),
(103077, 'spell_gen_rock_jump_c');

UPDATE `gameobject_template` SET `data2`=5, `data3`=103071 WHERE `entry`=209575;
UPDATE `gameobject_template` SET `data2`=5, `data3`=103072 WHERE `entry`=209576;
UPDATE `gameobject_template` SET `data2`=5, `data3`=103078 WHERE `entry`=209577;

DELETE FROM `areatrigger_scripts` WHERE `entry`=7783;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(7783, 'at_pool_of_reflection');

DELETE FROM `spell_area` WHERE `spell`=116571;
INSERT INTO `spell_area` (`spell`, `area`, `autocast`) VALUES
(116571, 5862, 1);

SET @OGUID := 100017;
DELETE FROM `gameobject` WHERE `guid`=@OGUID-0;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@OGUID-0, 250001, 860, 1118.11, 2870.63, 92.1829, 0);
