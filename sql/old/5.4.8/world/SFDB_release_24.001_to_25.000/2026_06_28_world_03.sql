-- =============================================================================
-- OPTIONAL COMBAT FALLBACK: SmartAI mobs that only have say/flee scripts
-- =============================================================================
-- Symptom:  Mob wanders or stands but does not melee when attacked (after core is rebuilt).
-- Cause:    SmartAI with no combat script relies on core CombatStart/AttackStart.
--            Use only if a specific entry still ignores players after core combat fixes.
-- Requires: worldserver built with combat fixes (UnitCombatState, SmartAI, RandomMG).
-- Prefer:   Fix type_flags first; assign AIName='AggressorAI' for mobs with no SAI at all.
-- =============================================================================

-- Goldtooth (327): retail SAI is say-on-aggro + flee@20% only.
DELETE FROM `smart_scripts` WHERE `entryorguid` = 327 AND `source_type` = 0 AND `id` = 2;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`)
VALUES (327, 0, 2, 0, 32, 0, 100, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Goldtooth - On Damaged - Attack Start');

-- Kurtok (42938): no retail smart_scripts; explicit AggressorAI (core fallback when AIName empty).
UPDATE `creature_template`
SET `AIName` = 'AggressorAI'
WHERE `entry` = 42938 AND (`AIName` IS NULL OR `AIName` = '');
