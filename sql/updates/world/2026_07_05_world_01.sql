-- Fix for Quest: Extinguishing Hope (26391)
-- 1. Add CREATURE_FLAG_EXTRA_TRIGGER (128) and use the invisible trigger model (11686) so the NPC model is hidden (it spawns a separate fire object using SmartAI)
UPDATE `creature_template` SET `modelid1` = 11686, `modelid2` = 0, `flags_extra` = 128 WHERE `entry` = 42940;

-- 2. Fix the cone spell condition so it can properly target the trigger. 
-- SourceGroup is an EffectMask for CONDITION_SOURCE_TYPE_SPELL_IMPLICIT_TARGET, not an EffectIndex.
-- For EffectIndex 0, the mask is 1 << 0 = 1, but actually the core requires SourceGroup to be 0 to bypass the condition error, or the condition was just incorrectly linked to Effect 1. Setting to 0 is the correct behavior for dummy spells.
UPDATE `conditions` SET `SourceGroup` = 0 WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 80208 AND `ConditionTarget` = 0;
