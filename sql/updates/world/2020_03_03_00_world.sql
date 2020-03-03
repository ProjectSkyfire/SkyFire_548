-- Gate of the Setting Sun
DELETE FROM `areatrigger_teleport` WHERE `id`=8310;
INSERT INTO `areatrigger_teleport` (`id`, `name`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(8310, 'Gate of the Setting Sun - Exit Target', 870, 692.365, 2080.17, 374.694, 3.18706);

-- Siege of Orgrimmar
DELETE FROM `areatrigger_teleport` WHERE `id` IN (9244, 9245);
INSERT INTO `areatrigger_teleport` (`id`, `name`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(9244, 'Siege of Orgrimmar (Exit)', 870, 1238.59, 605.595, 317.472, 0),
(9245, 'Siege of Orgrimmar (Entrance)', 1136, 1440.89, 263.047, 283.558, 1.63);
