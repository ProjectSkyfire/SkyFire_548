-- Teleport to Saurok Creation Pit
DELETE FROM `spell_target_position` WHERE `id` = 140395;
INSERT INTO `spell_target_position` (`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) values
(140395,0,1098,5633.36,4679.78,55.6488,210.507);
