-- Ancient Teleport: Dalaran (and the Ancient Portal that casts it) put the caster in the air over the Dalaran
-- crater in the Eastern Kingdoms. Like Teleport: Dalaran (53140) it teleports to Dalaran in Northrend.
DELETE FROM `spell_target_position` WHERE `id` = 120145 AND `effIndex` = 0;
INSERT INTO `spell_target_position` (`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(120145, 0, 571, 5807.75, 588.347, 661.505, 1.663);
