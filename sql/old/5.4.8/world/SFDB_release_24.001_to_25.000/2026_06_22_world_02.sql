-- Pending world cleanup from the 2026-06-22 DBErrors.log audit.
--
-- The loot loader reports these creature loot templates as unused because no
-- creature_template.lootid points at them. Old 5.4.8 SQL history shows the
-- Timeless Isle creatures below should use their own entry as lootid, so this
-- restores the missing links instead of deleting valid loot.

UPDATE `creature_template`
SET `lootid` = `entry`
WHERE `entry` IN (
    71864, 71908, 71919, 72033, 72045, 72048, 72049, 72193, 72245, 72970,
    73160, 73161, 73163, 73166, 73173, 73279, 73281, 73282, 73703, 73704
)
  AND `lootid` = 0;

-- These two rows have no matching creature_template entry in the 5.4.8 world
-- data and are not referenced by any creature lootid.
DELETE FROM `creature_loot_template`
WHERE `entry` IN (68026, 68027)
  AND `item` = 92499
  AND `mincountOrRef` = -100;
