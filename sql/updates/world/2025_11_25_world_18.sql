-- Fix condition table errors

-- 1. Remove spell condition entries with incorrect SourceGroup values
-- These spells don't have implicit targets of types _AREA_, _CONE_, _NEARBY_ for the specified effects
-- SourceType 13 = CONDITION_SOURCE_TYPE_SPELL_IMPLICIT_TARGET
-- SourceGroup is an effect mask: 1 = effect 0, 2 = effect 1, 3 = effects 0 and 1

-- Spell 33831 SourceGroup 1 (effect 0) - doesn't have implicit area/cone/nearby targets
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 1 AND `SourceEntry` = 33831;

-- Spell 44885 SourceGroup 2 (effect 1) - doesn't have implicit area/cone/nearby targets
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 2 AND `SourceEntry` = 44885;

-- Spell 45201 SourceGroup 3 (effects 0 and 1) - doesn't have implicit area/cone/nearby targets
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 3 AND `SourceEntry` = 45201;

-- Spell 45203 SourceGroup 3 (effects 0 and 1) - doesn't have implicit area/cone/nearby targets
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 3 AND `SourceEntry` = 45203;

-- Spell 46350 SourceGroup 2 (effect 1) - doesn't have implicit area/cone/nearby targets
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 2 AND `SourceEntry` = 46350;

-- Spell 51395 SourceGroup 3 (effects 0 and 1) - doesn't have implicit area/cone/nearby targets
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 3 AND `SourceEntry` = 51395;

-- Spell 74758 SourceGroup 1 (effect 0) - doesn't have implicit area/cone/nearby targets
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 1 AND `SourceEntry` = 74758;

-- 2. Remove spell condition entry with invalid SourceGroup 0
-- Spell 93072 SourceGroup 0 is invalid (must be > 0 for effect mask)
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 0 AND `SourceEntry` = 93072;

-- 3. Remove condition entries for non-existent spell 40200
-- Spell 40200 does not exist in MoP spell.dbc
-- Remove spell implicit target conditions (SourceType 13)
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 40200;
-- Remove creature_loot_template conditions (SourceType 17) that reference spell 40200
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` = 40200;

