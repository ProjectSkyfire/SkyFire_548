-- Quest 31450 "A New Fate": spawn the missing quest enders.
--
-- Problem
--   `creature_questender` points quest 31450 at 60566 (Aysa Cloudsinger) and
--   60570 (Ji Firepaw). Both have a `creature_template` row, but ZERO rows in
--   `creature` -- they have no spawn at all, and never had one anywhere in the
--   SFDB dump history.
--
--   So the final quest of the Pandaren starting zone cannot be handed in. A
--   player who has already picked a faction and earned kill credit 64594 is
--   left holding a completed quest with nobody able to take it.
--
--   Verified against the production world DB on 2026-09-03:
--     entry 60566 -> 0 spawns, entry 60570 -> 0 spawns.
--   The rest of the tail chain is fine (56013, 56418, 57739 have 1 spawn each).
--
-- Placement
--   Put at the landing spots hard-coded in
--   WorldSession::HandleSelectFactionOpcode (MiscHandler.cpp:2944-2963):
--     Horde    -> map 1, 1357.62 -4373.55 26.13 (Orgrimmar, Valley of Strength)
--     Alliance -> map 0, -8960.02 516.10 96.36  (Stormwind, Trade District)
--   Each NPC stands 5 yards in front of that point facing back at it, so the
--   player sees them the moment the faction choice teleports them in.
--
--   These coordinates are FUNCTIONAL, not Blizzard-authentic. Retail places
--   both inside the city behind phasing. To move them to the real spots, only
--   the two VALUES rows below need to change.

SET @GUID_AYSA := 8310001;   -- custom range 8310000+, clear of the heirloom vendor (8300001+)
SET @GUID_JI   := 8310002;

DELETE FROM `creature` WHERE `guid` IN (@GUID_AYSA, @GUID_JI);
DELETE FROM `creature` WHERE `id` IN (60566, 60570);

INSERT INTO `creature`
(`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`,
 `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`,
 `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`)
VALUES
 -- Aysa Cloudsinger -- Stormwind, in front of the Alliance landing spot
 (@GUID_AYSA, 60566, 0, 1, 0, 0, 0, 0, -8956.10,   519.21, 96.36, 3.81159, 120, 0, 0, 1, 0, 0, 0, 0, 0),
 -- Ji Firepaw -- Orgrimmar, in front of the Horde landing spot
 (@GUID_JI,   60570, 1, 1, 0, 0, 0, 0,  1362.58, -4372.90, 26.13, 3.27159, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- The player lands in the city straight out of a phased starting zone.
-- CREATURE_FLAG_EXTRA_ALL_PHASES (0x200000, custom core flag -- see
-- modules/mod-prabowow) keeps the spawns visible in every phase, so they are
-- not invisible if the player still carries a leftover phase. If the landing
-- spots turn out not to be phased, this UPDATE can be dropped with no other
-- effect.
UPDATE `creature_template` SET `flags_extra` = `flags_extra` | 0x200000
WHERE `entry` IN (60566, 60570);
