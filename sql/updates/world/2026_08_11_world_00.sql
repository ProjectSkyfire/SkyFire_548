-- Quest 28728 Signs of Things to Come: teleport destination for Tarindrella's Nature Teleport (92420).
-- On accept, Tarindrella casts 92420 and the player arrives in front of Dentaria Silverglade (49479).

DELETE FROM `spell_target_position` WHERE `id`=92420;
INSERT INTO `spell_target_position` (`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(92420, 0, 1, 10551.8, 877.247, 1309.94, 3.47321);
