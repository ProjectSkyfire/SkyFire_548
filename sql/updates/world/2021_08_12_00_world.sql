DELETE FROM `spell_scripts` WHERE `id`=71898;
INSERT INTO `spell_scripts` (`id`, `effIndex`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES
(71898, 1, 0, 8, 38438, 0, 0, 0, 0, 0, 0);

UPDATE `gameobject_template` SET `type`='8', `data0`='1647', `data1`='15' WHERE `entry`=202138;
