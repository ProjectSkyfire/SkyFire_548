-- =============================================================================
-- DUSKWOOD: Raven Hold Inn duplicate GOs + Cooking Fire (818) client crash
-- =============================================================================
-- Symptom:  Stack overflow relog at Raven Hold Inn; Cooking Fire (818) crashes
--           client (~12.5 GB alloc in WowClientDB2.cpp).
-- Cause:    Duplicate inn GOs; CREATED_BY on SPELL_FOCUS GO 29784 (CREATE_OBJECT2);
--           linked trap 31442 also sent as CREATE_OBJECT2 without Object.cpp guard.
-- Fix:      Delete duplicate inn spawns. Keep linked trap (data2=31442, data6=-1).
--           Requires full C++ fix: EffectTransmitted + TrackGameObject + Object.cpp.
-- Map/NPCs:  Raven Hold Inn map 0 (~ -10517, -1162, 28)
-- Spells:    818 Cooking Fire, 7353 Cozy Fire
-- GOs:       29784 Basic Campfire, 31442 linked trap
-- Apply:     rebuild worldserver, apply SQL, .reload gameobject_template
-- =============================================================================
-- ROLLBACK (world-main 2026-07-03):
--   gameobject: re-insert duplicate rows guid 5532-5568 (see SFDB positions)
--   gameobject_template 31442: data6=0
-- =============================================================================

DELETE FROM `gameobject`
WHERE `guid` IN (
    5532, 5533, 5538, 5540, 5541, 5543,
    5548, 5549, 5550, 5555, 5556, 5558,
    5566, 5567, 5568
)
  AND `map` = 0;

UPDATE `gameobject_template`
SET `data2` = 31442,
    `AIName` = ''
WHERE `entry` = 29784;

UPDATE `gameobject_template`
SET `data6` = -1
WHERE `entry` = 31442;

DELETE FROM `smart_scripts`
WHERE `entryorguid` = 29784 AND `source_type` = 1;
