-- Fix reference_loot_template errors

-- 1. Remove entries with non-existent item 22541
-- Item 22541 does not exist in item_template (MoP DBC issue)
DELETE FROM `reference_loot_template` WHERE `entry` = 24092 AND `item` = 22541;
DELETE FROM `reference_loot_template` WHERE `entry` = 10006 AND `item` = 22541;

-- 2. Remove references to non-existent reference IDs 21929 and 23436
-- These reference IDs don't exist in reference_loot_template but are referenced in gameobject_loot_template entry 27240
-- Removing the invalid references from gameobject_loot_template (mincountOrRef is negative for references)
DELETE FROM `gameobject_loot_template` WHERE `entry` = 27240 AND `item` = 21929 AND `mincountOrRef` = -21929;
DELETE FROM `gameobject_loot_template` WHERE `entry` = 27240 AND `item` = 23436 AND `mincountOrRef` = -23436;

-- 3. Remove unused reference_loot_template entries
-- These entries exist but are not referenced as negative values (reference IDs) anywhere
-- Entry 24122: Not referenced as -24122, can be removed
DELETE FROM `reference_loot_template` WHERE `entry` = 24122;

-- Entry 24123: Not referenced as -24123, can be removed
DELETE FROM `reference_loot_template` WHERE `entry` = 24123;

-- Entry 44012: Not referenced as -44012, can be removed
DELETE FROM `reference_loot_template` WHERE `entry` = 44012;

