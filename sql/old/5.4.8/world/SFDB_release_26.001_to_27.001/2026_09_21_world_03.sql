-- Quest 10078 "Laying Waste to the Unwanted": the Flaming Torch cannot be cast, and the
-- credit it would give goes nowhere.
--
-- 1. Nothing provides the spell focus.
--
-- Spell 32979 "Flaming Torch" has SpellCastingRequirements 1659, whose RequiresSpellFocus is
-- 1399 "Horde Blade Thrower". The only template of that focus is 183123, and it has no
-- `gameobject` row, so Spell::CheckCast fails with SPELL_FAILED_REQUIRES_SPELL_FOCUS before
-- anything else runs.
--
-- The template would also be discarded if it were spawned: its `displayId` is 1287, which is
-- not a row in GameObjectDisplayInfo.dbc, and ObjectMgr::LoadGameobjects drops any spawn whose
-- template has an invalid displayId ("has an invalid displayId (%u), not loaded"). A spell
-- focus needs no model - 418 other GAMEOBJECT_TYPE_SPELL_FOCUS templates carry displayId 0 and
-- the loader exempts this type from the "doesn't have a displayId" check - so 0 it is, rather
-- than putting a second visible blade thrower on top of the map's own.
--
-- Four spawns, one at the foot of each tower, each within 20 yards of where the torch is
-- thrown from (GameObjectFocusCheck uses spellFocus.dist / 2 = 20).
--
-- 2. The credit is sent to the loot recipient, which these creatures never have.
--
-- All four objectives are SMART_ACTION_CALL_KILLEDMONSTER rows on the Invis Horde Siege Engine
-- bunnies with target_type 1 (SELF). SmartScript routes SMART_TARGET_NONE and SMART_TARGET_SELF
-- through me->GetLootRecipient(). These bunnies are NOT_SELECTABLE | IMMUNE_TO_PC |
-- IMMUNE_TO_NPC and spell 32979 is a single damage-free SPELL_EFFECT_DUMMY, so nothing ever
-- calls SetLootRecipient on them and the branch silently does nothing.
--
-- target_type 7 (ACTION_INVOKER) is the player who cast the torch: row 0 is
-- SMART_EVENT_SPELLHIT on 32979 and row 1 is its linked row, so the invoker is carried down.
-- Row 0 keeps target_type 1, where SELF is correct.

UPDATE `gameobject_template` SET `displayId` = 0 WHERE `entry` = 183123;

SET @OGUID := 901360;

DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID-3 AND @OGUID-0;
DELETE FROM `gameobject` WHERE `id` = 183123;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(@OGUID-0, 183123, 530, 1, -156.984, 2517.59, 43.3923, 3.23083, 0, 0, 0, 1, 300, 0, 1), -- Eastern Thrower, foot of 18818's tower
(@OGUID-1, 183123, 530, 1, -154.539, 2661.95, 44.5689, 2.95437, 0, 0, 0, 1, 300, 0, 1), -- Central Eastern Thrower, foot of 21237's tower
(@OGUID-2, 183123, 530, 1, -172.530, 2773.63, 35.5282, 0.645772, 0, 0, 0, 1, 300, 0, 1), -- Central Western Thrower, foot of 19009's tower
(@OGUID-3, 183123, 530, 1, -172.639, 2818.57, 27.3761, 3.368497, 0, 0, 0, 1, 300, 0, 1); -- Western Thrower, foot of 21236's tower

UPDATE `smart_scripts` SET `target_type` = 7
WHERE `source_type` = 0
  AND `entryorguid` IN (18818, 19009, 21236, 21237)
  AND `id` = 1
  AND `event_type` = 61
  AND `action_type` = 33;
