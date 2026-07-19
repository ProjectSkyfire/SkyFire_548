-- Goblin Trike ground-vehicle bounce fix (Lost Isles map 648 and related spawns).
-- LOA/sniff data uses HoverHeight=1 and bytes1 hover flags on entry 49132; the core
-- applies MOVEMENTFLAG_HOVER on ground mobs and bobbles on uneven vmap mesh.
-- Pair with Creature::UpdateMovementFlags idle-ground skip in worldserver.
-- Safe to re-apply (idempotent).

UPDATE `creature_template` SET `HoverHeight` = 0 WHERE `entry` IN (46754, 46755, 49132, 49133); -- Goblin Trike variants

UPDATE `creature_template_addon` SET `bytes1` = 0 WHERE `entry` = 49132 AND `bytes1` = 50331648; -- Goblin Trike (hover stand flags)
