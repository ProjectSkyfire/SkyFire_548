-- =============================================================================
-- DUSKWOOD: quest NPC invisibility detection (Master/Oliver Harris)
-- Duskwood chain part 1/3 (apply 2026-07-01_world_00 -> _01 -> _02 in order)
-- =============================================================================
-- Symptom:  Master Harris (288) and Oliver Harris (43730) at Raven Hill are
--           invisible without .gm on. Both use creature_template_addon aura
--           49414 (Generic Quest Invisibility 1).
-- Cause:    Missing spell_area rows for player aura 49416 (Generic Quest
--           Invisibility Detection 1) during the Duskwood quest chain.
-- Fix:      Autocast 49416 for all races (racemask 0) in Duskwood areas while
--           quest 26717 is active/rewarded through quest 26721 incomplete.
-- NPCs:     288 Master Harris, 43730 Oliver Harris (aura 49414)
-- Quests:    26717 The Yorgen Worgen -> 26719 Delivery to Master Harris ->
--            26720 A Curse We Cannot Lift -> 26721 (chain continues)
-- Areas:     10 Duskwood zone (covers all subzones), 94 Raven Hill, 4815 Yorgen Stead
-- Not added: spell_phase / phase_area - 49414 is MOD_INVISIBILITY (not SPELL_AURA_PHASE);
--            LoadSpellPhaseInfo is commented out in ObjectMgr.cpp anyway.
-- quest_start_status 64 = quest 26717 must be REWARDED (1 << QUEST_STATUS_REWARDED).
-- quest_end_status 9 = aura stays while quest 26721 is NONE or INCOMPLETE; removed when
--           26721 is REWARDED. (64 was wrong - it only allowed the aura after 26721 done.)
-- Jitters:   43731 uses aura 48168 (Inner Fire in DBC, not 49414); 49416 will
--            not detect that aura - fix separately if Jitters visibility fails.
-- Apply:     .reload spell_area (or restart worldserver), then retest without GM.
-- =============================================================================

DELETE FROM `spell_area`
WHERE `spell` = 49416
  AND `area` IN (10, 42, 94, 4815)
  AND `quest_start` = 26717
  AND `aura_spell` = 0;

INSERT INTO `spell_area`
    (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`)
VALUES
    -- Duskwood zone (matches via zone id for any subzone)
    (49416, 10, 26717, 26721, 0, 0, 2, 1, 64, 9),
    -- Raven Hill (Oliver Harris / Master Harris spawns at ~-10754, 338)
    (49416, 94, 26717, 26721, 0, 0, 2, 1, 64, 9),
    -- Yorgen Stead (chain start at 26717)
    (49416, 4815, 26717, 26721, 0, 0, 2, 1, 64, 9);
