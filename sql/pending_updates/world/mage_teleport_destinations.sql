-- Mage teleports without a destination row finish their cast and leave the caster in place
-- (TARGET_DEST_DB falls back to the caster's own position).
DELETE FROM `spell_target_position` WHERE `effIndex` = 0 AND `id` IN (3562, 3565, 3566, 49359, 88342, 88344, 120145, 132621, 132627);
INSERT INTO `spell_target_position` (`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(3562, 0, 0, -4613.62, -915.38, 501.062, 3.88),        -- Teleport: Ironforge
(3565, 0, 1, 9660.81, 2513.64, 1331.66, 3.06),         -- Teleport: Darnassus
(3566, 0, 1, -964.98, 283.433, 111.187, 3.02),         -- Teleport: Thunder Bluff
(49359, 0, 1, -3730.72, -4422.21, 30.4836, 0.810732),  -- Teleport: Theramore
(88342, 0, 732, -344.6, 1043.8, 21.5, 1.5),            -- Teleport: Tol Barad (Alliance)
(88344, 0, 732, -601.4, 1382.03, 21.9, 1.5),           -- Teleport: Tol Barad (Horde)
(120145, 0, 0, 303.22, 347.21, 125.53, 1.72194),       -- Ancient Teleport: Dalaran (high above the old crater)
(132621, 0, 870, 821.5, 253.2, 503.919, 0.719747),     -- Teleport: Vale of Eternal Blossoms (Alliance)
(132627, 0, 870, 1678.73, 931.493, 471.5, 3.276);      -- Teleport: Vale of Eternal Blossoms (Horde)
