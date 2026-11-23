-- The portal to the Maelstrom (spell 84464, the portal that Farseer Krogar and Naraat the Earthspeaker conjure for "The
-- Maelstrom") sent players to map 646 (Deepholm) at the Maelstrom coordinates, so they arrived in the void under Deepholm
-- instead of on the Maelstrom (map 730) next to Thrall. Destination from the 5.4.8 data.
-- Based on Legends of Azeroth 5.4.8.
DELETE FROM `spell_target_position` WHERE `id` = 84464 AND `effIndex` = 0;
INSERT INTO `spell_target_position` (`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(84464, 0, 730, 851.307, 1067.76, -10.0183, 4.50656);
