-- Fix Creature duplicate aura error
-- Creatures with GUID 168283 and 168284 (Entry: 42940) have duplicate aura (spell 80175) in auras field
-- The creature_template_addon entry for entry 42940 has '80175' appearing twice in the auras field (e.g., '80175 80175')
-- This update removes the duplicate by ensuring '80175' only appears once

UPDATE `creature_template_addon` 
SET `auras` = '80175' 
WHERE `entry` = 42940 
  AND `auras` LIKE '%80175%';
