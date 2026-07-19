-- =============================================================================
-- DISPLAY FIX: skull level, missing nameplate name, broken creature query cache
-- =============================================================================
-- Symptom:  Skull/?? level, grey name with no mob name, wrong difficulty color.
-- Cause:    creature_template.type_flags = 2147483648 (0x80000000, CREATURE_TYPEFLAGS_UNK31).
--            This value is sent in SMSG_CREATURE_QUERY_RESPONSE and breaks 5.4.8 client
--            nameplate/level handling. Working starter mobs (e.g. Kobold Vermin) use 0.
-- Fix:      Set type_flags to 0. Players must /reload creature_template or restart
--            worldserver; clear client cache once if names/levels stay wrong.
-- NOT fixed:  Combat, movement, or vmap floor clipping (those are core issues).
-- =============================================================================

UPDATE `creature_template`
SET `type_flags` = 0
WHERE `type_flags` = 2147483648;

-- Spot-check entries from starter-NPC investigation:
--   327   Goldtooth
--   42938 Kurtok the Slayer
--   300   Zzarc' Vul (level 23 skull vs low player is still correct after this)
