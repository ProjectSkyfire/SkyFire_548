-- =============================================================================
-- FLYING FIX: drakes hovering in air but falling when approached
-- =============================================================================
-- Symptom:  NPC spawned high (e.g. Stone Drake) stands in air, then drops to ground
--           when a player enters aggro range.
-- Cause:    InhabitType = 3 (ground + water only). Core treats them as ground mobs;
--           UpdateMovementFlags enables gravity once in combat. HoverHeight alone
--           does not set CanFly() — that requires INHABIT_AIR (4).
-- Fix:      Set InhabitType to 4 (air), matching other flying drakes (e.g. 23689).
-- Core:     Creature::SetFlying + SetAnimTier for idle fly/hover combat animations.
-- =============================================================================

UPDATE `creature_template`
SET `InhabitType` = 4
WHERE `entry` IN (42522, 44077); -- Stone Drake (Deepholm / shared template)
